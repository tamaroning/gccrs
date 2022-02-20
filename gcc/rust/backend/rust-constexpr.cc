// This file is part of GCC.

// GCC is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3, or (at your option) any later
// version.

// GCC is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.

// You should have received a copy of the GNU General Public License
// along with GCC; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#include "rust-constexpr.h"
#include "rust-location.h"
#include "rust-diagnostics.h"

#include "fold-const.h"
#include "realmpfr.h"
#include "convert.h"
#include "print-tree.h"
#include "gimplify.h"
#include "tree-iterator.h"

/* Returns true if NODE is a pointer.  */
#define TYPE_PTR_P(NODE) (TREE_CODE (NODE) == POINTER_TYPE)

/* Returns true if NODE is a reference.  */
#define TYPE_REF_P(NODE) (TREE_CODE (NODE) == REFERENCE_TYPE)

/* Returns true if NODE is a pointer or a reference.  */
#define INDIRECT_TYPE_P(NODE) (TYPE_PTR_P (NODE) || TYPE_REF_P (NODE))

/* Nonzero if this type is const-qualified.  */
#define TYPE_CONST_P(NODE) ((rs_type_quals (NODE) & TYPE_QUAL_CONST) != 0)

/* Nonzero if this type is const-qualified.  */
#define TYPE_VOLATILE_P(NODE) ((rs_type_quals (NODE) & TYPE_QUAL_VOLATILE) != 0)

/* [basic.fundamental]

   Types  bool, char, wchar_t, and the signed and unsigned integer types
   are collectively called integral types.

   Note that INTEGRAL_TYPE_P, as defined in tree.h, allows enumeration
   types as well, which is incorrect in C++.  Keep these checks in
   ascending code order.  */
#define RS_INTEGRAL_TYPE_P(TYPE)                                               \
  (TREE_CODE (TYPE) == BOOLEAN_TYPE || TREE_CODE (TYPE) == INTEGER_TYPE)

/* [basic.fundamental]

   Integral and floating types are collectively called arithmetic
   types.

   As a GNU extension, we also accept complex types.

   Keep these checks in ascending code order.  */
#define ARITHMETIC_TYPE_P(TYPE)                                                \
  (RS_INTEGRAL_TYPE_P (TYPE) || TREE_CODE (TYPE) == REAL_TYPE                  \
   || TREE_CODE (TYPE) == COMPLEX_TYPE)

/* True iff TYPE is cv decltype(nullptr).  */
#define NULLPTR_TYPE_P(TYPE) (TREE_CODE (TYPE) == NULLPTR_TYPE)

/* [basic.types]

   Arithmetic types, enumeration types, pointer types,
   pointer-to-member types, and std::nullptr_t are collectively called
   scalar types.

   Keep these checks in ascending code order.  */
#define SCALAR_TYPE_P(TYPE)                                                    \
  (TREE_CODE (TYPE) == ENUMERAL_TYPE || ARITHMETIC_TYPE_P (TYPE)               \
   || TYPE_PTR_P (TYPE) || NULLPTR_TYPE_P (TYPE))

namespace Rust {
namespace Compile {

static tree
constant_value_1 (tree decl, bool strict_p, bool return_aggregate_cst_ok_p,
		  bool unshare_p);
tree
decl_constant_value (tree decl, bool unshare_p);

static void
non_const_var_error (location_t loc, tree r);

static tree
get_function_named_in_call (tree t);

// We have an expression tree T that represents a call, either CALL_EXPR
// or AGGR_INIT_EXPR.  Return the Nth argument.
static inline tree
get_nth_callarg (tree t, int n)
{
  switch (TREE_CODE (t))
    {
    case CALL_EXPR:
      return CALL_EXPR_ARG (t, n);

    default:
      gcc_unreachable ();
      return NULL;
    }
}

// Returns the type qualifiers for this type, including the qualifiers on the
// elements for an array type.
int
rs_type_quals (const_tree type)
{
  int quals;
  /* This CONST_CAST is okay because strip_array_types returns its
     argument unmodified and we assign it to a const_tree.  */
  type = strip_array_types (CONST_CAST_TREE (type));
  if (type == error_mark_node
      /* Quals on a FUNCTION_TYPE are memfn quals.  */
      || TREE_CODE (type) == FUNCTION_TYPE)
    return TYPE_UNQUALIFIED;
  quals = TYPE_QUALS (type);
  /* METHOD and REFERENCE_TYPEs should never have quals.  */
  gcc_assert (
    (TREE_CODE (type) != METHOD_TYPE && !TYPE_REF_P (type))
    || ((quals & (TYPE_QUAL_CONST | TYPE_QUAL_VOLATILE)) == TYPE_UNQUALIFIED));
  return quals;
}

// We are using a reference VAL for its value. Bash that reference all the
// way down to its lowest form.
tree
convert_from_reference (tree val)
{
  if (TREE_TYPE (val) && TYPE_REF_P (TREE_TYPE (val)))
    {
      tree t = TREE_TYPE (TREE_TYPE (val));
      tree ref = build1 (INDIRECT_REF, t, val);

      /* We *must* set TREE_READONLY when dereferencing a pointer to const,
	 so that we get the proper error message if the result is used
	 to assign to.  Also, &* is supposed to be a no-op.  */
      TREE_READONLY (ref) = TYPE_CONST_P (t);
      TREE_THIS_VOLATILE (ref) = TYPE_VOLATILE_P (t);
      TREE_SIDE_EFFECTS (ref)
	= (TREE_THIS_VOLATILE (ref) || TREE_SIDE_EFFECTS (val));
      val = ref;
    }

  return val;
}

/* Fold away simple conversions, but make sure TREE_OVERFLOW is set
   properly and propagate TREE_NO_WARNING if folding EXPR results
   in the same expression code.  */

tree
cp_fold_convert (tree type, tree expr)
{
  tree conv;
  if (TREE_TYPE (expr) == type)
    conv = expr;
  else if (TREE_CODE (expr) == PTRMEM_CST
	   && same_type_p (TYPE_PTRMEM_CLASS_TYPE (type),
			   PTRMEM_CST_CLASS (expr)))
    {
      /* Avoid wrapping a PTRMEM_CST in NOP_EXPR.  */
      conv = copy_node (expr);
      TREE_TYPE (conv) = type;
    }
  else if (TYPE_PTRMEM_P (type))
    {
      conv = convert_ptrmem (type, expr, true, false, tf_warning_or_error);
      conv = cp_fully_fold (conv);
    }
  else
    {
      conv = fold_convert (type, expr);
      conv = ignore_overflows (conv, expr);
    }

  if (TREE_CODE (expr) == TREE_CODE (conv))
    copy_warning (conv, expr);

  return conv;
}

// TEMP is the constant value of a temporary object of type TYPE.  Adjust
// the type of the value to match.
static tree
adjust_temp_type (tree type, tree temp)
{
  /* Avoid wrapping an aggregate value in a NOP_EXPR.  */
  if (TREE_CODE (temp) == CONSTRUCTOR)
    {
      /* build_constructor wouldn't retain various CONSTRUCTOR flags.  */
      tree t = copy_node (temp);
      TREE_TYPE (t) = type;
      return t;
    }

  /* Now we know we're dealing with a scalar, and a prvalue of non-class
     type is cv-unqualified.  */
  return cp_fold_convert (cv_unqualified (type), temp);
}

ConstCtx::ConstCtx () : constexpr_ops_count (0) {}

tree
ConstCtx::fold (tree expr)
{
  tree folded = ConstCtx ().constexpr_expression (expr);
  rust_assert (folded != NULL_TREE);
  return folded;
}

tree
ConstCtx::constexpr_expression (tree t)
{
  location_t loc = EXPR_LOCATION (t);
  if (CONSTANT_CLASS_P (t))
    {
      if (TREE_OVERFLOW (t))
	{
	  rust_error_at (Location (loc), "overflow in constant expression");
	  return t;
	}

      if (TREE_CODE (t) == INTEGER_CST && TYPE_PTR_P (TREE_TYPE (t))
	  && !integer_zerop (t))
	{
	  // FIXME check does this actually work to print out tree types
	  rust_error_at (Location (loc),
			 "value %qE of type %qT is not a constant expression",
			 t, TREE_TYPE (t));
	  return t;
	}

      return t;
    }

  // Avoid excessively long constexpr evaluations
  if (++constexpr_ops_count >= constexpr_ops_limit)
    {
      rust_error_at (
	Location (loc),
	"%<constexpr%> evaluation operation count exceeds limit of "
	"%wd (use %<-fconstexpr-ops-limit=%> to increase the limit)",
	constexpr_ops_limit);

      return t;
    }

  tree r = t;
  tree_code tcode = TREE_CODE (t);
  switch (tcode)
    {
      case CONST_DECL: {
	r = decl_constant_value (t, /*unshare_p=*/false);
	if (TREE_CODE (r) == TARGET_EXPR
	    && TREE_CODE (TARGET_EXPR_INITIAL (r)) == CONSTRUCTOR)
	  r = TARGET_EXPR_INITIAL (r);
	if (DECL_P (r))
	  {
	    non_const_var_error (loc, r);
	    return r;
	  }
      }
      break;

    case POINTER_PLUS_EXPR:
    case POINTER_DIFF_EXPR:
    case PLUS_EXPR:
    case MINUS_EXPR:
    case MULT_EXPR:
    case TRUNC_DIV_EXPR:
    case CEIL_DIV_EXPR:
    case FLOOR_DIV_EXPR:
    case ROUND_DIV_EXPR:
    case TRUNC_MOD_EXPR:
    case CEIL_MOD_EXPR:
    case ROUND_MOD_EXPR:
    case RDIV_EXPR:
    case EXACT_DIV_EXPR:
    case MIN_EXPR:
    case MAX_EXPR:
    case LSHIFT_EXPR:
    case RSHIFT_EXPR:
    case LROTATE_EXPR:
    case RROTATE_EXPR:
    case BIT_IOR_EXPR:
    case BIT_XOR_EXPR:
    case BIT_AND_EXPR:
    case TRUTH_XOR_EXPR:
    case LT_EXPR:
    case LE_EXPR:
    case GT_EXPR:
    case GE_EXPR:
    case EQ_EXPR:
    case NE_EXPR:
    case SPACESHIP_EXPR:
    case UNORDERED_EXPR:
    case ORDERED_EXPR:
    case UNLT_EXPR:
    case UNLE_EXPR:
    case UNGT_EXPR:
    case UNGE_EXPR:
    case UNEQ_EXPR:
    case LTGT_EXPR:
    case RANGE_EXPR:
    case COMPLEX_EXPR:
      r = eval_binary_expression (t);
      break;

    case CALL_EXPR:
      r = eval_call_expression (t);
      break;

    case RETURN_EXPR:
      rust_assert (TREE_OPERAND (t, 0) != NULL_TREE);
      r = constexpr_expression (TREE_OPERAND (t, 0));
      break;

    case MODIFY_EXPR:
      r = eval_store_expression (t);
      break;

    default:
      break;
    }

  return r;
}

tree
ConstCtx::eval_store_expression (tree t)
{
  tree init = TREE_OPERAND (t, 1);
  if (TREE_CLOBBER_P (init))
    /* Just ignore clobbers.  */
    return void_node;

  /* First we figure out where we're storing to.  */
  tree target = TREE_OPERAND (t, 0);

  tree type = TREE_TYPE (target);
  bool preeval = SCALAR_TYPE_P (type) || TREE_CODE (t) == MODIFY_EXPR;
  if (preeval)
    {
      /* Evaluate the value to be stored without knowing what object it will be
	 stored in, so that any side-effects happen first.  */
      init = ConstCtx::fold (init);
    }

  bool evaluated = false;
  tree object = NULL_TREE;
  for (tree probe = target; object == NULL_TREE;)
    {
      switch (TREE_CODE (probe))
	{
	default:
	  if (evaluated)
	    object = probe;
	  else
	    {
	      probe = constexpr_expression (probe);
	      evaluated = true;
	    }
	  break;
	}
    }

  return init;
}

/* Subroutine of cxx_eval_constant_expression.
 Like cxx_eval_unary_expression, except for binary expressions.  */

tree
ConstCtx::eval_binary_expression (tree t)
{
  tree orig_lhs = TREE_OPERAND (t, 0);
  tree orig_rhs = TREE_OPERAND (t, 1);
  tree lhs, rhs;

  lhs = constexpr_expression (orig_lhs);
  rhs = constexpr_expression (orig_rhs);

  location_t loc = EXPR_LOCATION (t);
  enum tree_code code = TREE_CODE (t);
  tree type = TREE_TYPE (t);

  return fold_binary_loc (loc, code, type, lhs, rhs);
}

// Subroutine of cxx_eval_constant_expression.
// Evaluate the call expression tree T in the context of OLD_CALL expression
// evaluation.
tree
ConstCtx::eval_call_expression (tree t)
{
  tree fun = get_function_named_in_call (t);
  return constexpr_fn_retval (DECL_SAVED_TREE (fun));
}

// Subroutine of check_constexpr_fundef.  BODY is the body of a function
// declared to be constexpr, or a sub-statement thereof.  Returns the
// return value if suitable, error_mark_node for a statement not allowed in
// a constexpr function, or NULL_TREE if no return value was found.
tree
ConstCtx::constexpr_fn_retval (tree body)
{
  switch (TREE_CODE (body))
    {
      case STATEMENT_LIST: {
	tree expr = NULL_TREE;
	for (tree stmt : tsi_range (body))
	  {
	    tree s = constexpr_fn_retval (stmt);
	    if (s == error_mark_node)
	      return error_mark_node;
	    else if (s == NULL_TREE)
	      /* Keep iterating.  */;
	    else if (expr)
	      /* Multiple return statements.  */
	      return error_mark_node;
	    else
	      expr = s;
	  }
	return expr;
      }

    case RETURN_EXPR:
      // return break_out_target_exprs (TREE_OPERAND (body, 0));
      return constexpr_expression (body);

      case DECL_EXPR: {
	tree decl = DECL_EXPR_DECL (body);
	if (TREE_CODE (decl) == USING_DECL
	    /* Accept __func__, __FUNCTION__, and __PRETTY_FUNCTION__.  */
	    || DECL_ARTIFICIAL (decl))
	  return NULL_TREE;
	return error_mark_node;
      }

    case CLEANUP_POINT_EXPR:
      return constexpr_fn_retval (TREE_OPERAND (body, 0));

      case BIND_EXPR: {
	tree b = BIND_EXPR_BODY (body);
	return constexpr_fn_retval (b);
      }
      break;

    default:
      return error_mark_node;
    }
  return error_mark_node;
}

// Taken from cp/constexpr.cc
//
// If DECL is a scalar enumeration constant or variable with a
// constant initializer, return the initializer (or, its initializers,
// recursively); otherwise, return DECL.  If STRICT_P, the
// initializer is only returned if DECL is a
// constant-expression.  If RETURN_AGGREGATE_CST_OK_P, it is ok to
// return an aggregate constant.  If UNSHARE_P, return an unshared
// copy of the initializer.
static tree
constant_value_1 (tree decl, bool strict_p, bool return_aggregate_cst_ok_p,
		  bool unshare_p)
{
  while (TREE_CODE (decl) == CONST_DECL)
    {
      tree init;
      /* If DECL is a static data member in a template
	 specialization, we must instantiate it here.  The
	 initializer for the static data member is not processed
	 until needed; we need it now.  */

      init = DECL_INITIAL (decl);
      if (init == error_mark_node)
	{
	  if (TREE_CODE (decl) == CONST_DECL)
	    /* Treat the error as a constant to avoid cascading errors on
	       excessively recursive template instantiation (c++/9335).  */
	    return init;
	  else
	    return decl;
	}

      decl = init;
    }
  return unshare_p ? unshare_expr (decl) : decl;
}

// A more relaxed version of decl_really_constant_value, used by the
// common C/C++ code.
tree
decl_constant_value (tree decl, bool unshare_p)
{
  return constant_value_1 (decl, /*strict_p=*/false,
			   /*return_aggregate_cst_ok_p=*/true,
			   /*unshare_p=*/unshare_p);
}

static void
non_const_var_error (location_t loc, tree r)
{
  error_at (loc,
	    "the value of %qD is not usable in a constant "
	    "expression",
	    r);
  /* Avoid error cascade.  */
  if (DECL_INITIAL (r) == error_mark_node)
    return;

  // more in cp/constexpr.cc
}

static tree
get_callee (tree call)
{
  if (call == NULL_TREE)
    return call;
  else if (TREE_CODE (call) == CALL_EXPR)
    return CALL_EXPR_FN (call);

  return NULL_TREE;
}

// We have an expression tree T that represents a call, either CALL_EXPR
// or AGGR_INIT_EXPR. If the call is lexically to a named function,
// return the _DECL for that function.
static tree
get_function_named_in_call (tree t)
{
  tree fun = get_callee (t);
  if (fun && TREE_CODE (fun) == ADDR_EXPR
      && TREE_CODE (TREE_OPERAND (fun, 0)) == FUNCTION_DECL)
    fun = TREE_OPERAND (fun, 0);
  return fun;
}

// Subroutine of cxx_eval_call_expression.
// We are processing a call expression (either CALL_EXPR or
// AGGR_INIT_EXPR) in the context of CTX.  Evaluate
// all arguments and bind their values to correspondings
// parameters, making up the NEW_CALL context.
tree
ConstCtx::bind_parameters_in_call (tree t, tree fun, bool *non_constant_p,
				   bool *overflow_p, bool *non_constant_args)
{
  const int nargs = call_expr_nargs (t);
  tree parms = DECL_ARGUMENTS (fun);
  int i;
  /* We don't record ellipsis args below.  */
  int nparms = list_length (parms);
  int nbinds = nargs < nparms ? nargs : nparms;
  tree binds = make_tree_vec (nbinds);
  for (i = 0; i < nargs; ++i)
    {
      tree x, arg;
      tree type = parms ? TREE_TYPE (parms) : void_type_node;
      if (parms && DECL_BY_REFERENCE (parms))
	type = TREE_TYPE (type);
      x = get_nth_callarg (t, i);

      if (TREE_ADDRESSABLE (type))
	/* Undo convert_for_arg_passing work here.  */
	x = convert_from_reference (x);

      /* Normally we would strip a TARGET_EXPR in an initialization context
	 such as this, but here we do the elision differently: we keep the
	 TARGET_EXPR, and use its CONSTRUCTOR as the value of the parm.  */
      arg = constexpr_expression (x);

      /* Don't VERIFY_CONSTANT here.  */
      if (*non_constant_p)
	break;

      /* Just discard ellipsis args after checking their constantitude.  */
      if (!parms)
	continue;

      if (!*non_constant_p)
	{
	  /* Make sure the binding has the same type as the parm.  But
	     only for constant args.  */
	  if (!TYPE_REF_P (type))
	    arg = adjust_temp_type (type, arg);

	  if (!TREE_CONSTANT (arg))
	    *non_constant_args = true;

	  /* If arg is or contains address of a heap artificial variable or
	     of a static variable being constructed, avoid caching the
	     function call, as those variables might be modified by the
	     function, or might be modified by the callers in between
	     the cached function and just read by the function.  */
	  else if (!*non_constant_args
		   && cp_walk_tree (&arg, addr_of_non_const_var, ctx->global,
				    NULL))
	    *non_constant_args = true;

	  TREE_VEC_ELT (binds, i) = arg;
	}
      parms = TREE_CHAIN (parms);
    }

  return binds;
}

} // namespace Compile
} // namespace Rust
