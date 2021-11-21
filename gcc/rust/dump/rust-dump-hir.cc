#include "rust-dump-hir.h"
#include <iostream>
#include <fstream>

namespace Rust {
namespace Dump {

DumpHIR::DumpHIR (std::ofstream &out, int ind)
  : m_dumpout (out), m_sawclose (0), m_indent (ind)
{
}

void
DumpHIR::open_tag (std::string tag)
{
  if (m_sawclose)
    m_dumpout << std::string(m_indent * 2, ' ');

  m_dumpout << "(" << tag;
  m_sawclose = false;
}

void
DumpHIR::close_tag (bool new_line)
{
  m_dumpout << std::string(m_indent * 2, ' ') << ")";
  if (new_line)
    {
      m_dumpout << std::endl;
      m_sawclose = true;
    }
}

void
DumpHIR::go (HIR::Crate &crate)
{
  open_tag ("crate");

  for (const auto &attr : crate.inner_attrs)
      m_dumpout << std::string(m_indent * 2, ' ')
                << " \""
                << attr.as_string()
                << "\" ";

  m_dumpout << std::endl;
  m_sawclose = true;

  for (auto it = crate.items.begin (); it != crate.items.end (); it++)
    {
      m_indent += 2;
      it->get()->accept_vis (*this);
      m_indent -= 2;
    }

  close_tag (true);
}

void
DumpHIR::visit (HIR::IdentifierExpr &)
{
  open_tag ("IdentifierExpr");
  close_tag();
}

void
DumpHIR::visit (HIR::Lifetime &)
{
  open_tag ("Lifetime");
  close_tag();
}
void
DumpHIR::visit (HIR::LifetimeParam &)
{
  open_tag ("LifetimeParam");
  close_tag();
}
void
DumpHIR::visit (HIR::PathInExpression &)
{
  open_tag ("PathInExpression");
  close_tag();
}
void
DumpHIR::visit (HIR::TypePathSegment &)
{
  open_tag ("TypePathSegment");
  close_tag();
}
void
DumpHIR::visit (HIR::TypePathSegmentGeneric &)
{
  open_tag ("TypePathSegmentGeneric");
  close_tag();
}
void
DumpHIR::visit (HIR::TypePathSegmentFunction &)
{
  open_tag ("TypePathSegmentFunction");
  close_tag();
}
void
DumpHIR::visit (HIR::TypePath &)
{
  open_tag ("TypePath");
  close_tag();
}
void
DumpHIR::visit (HIR::QualifiedPathInExpression &)
{
  open_tag ("QualifiedPathInExpression");
  close_tag();
}
void
DumpHIR::visit (HIR::QualifiedPathInType &)
{
  open_tag ("QualifiedPathInType");
  close_tag();
}

void
DumpHIR::visit (HIR::LiteralExpr &)
{
  open_tag ("LiteralExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::BorrowExpr &)
{
  open_tag ("BorrowExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::DereferenceExpr &)
{
  open_tag ("DereferenceExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::ErrorPropagationExpr &)
{
  open_tag ("ErrorPropagationExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::NegationExpr &)
{
  open_tag ("NegationExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::ArithmeticOrLogicalExpr &)
{
  open_tag ("ArithmeticOrLogicalExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::ComparisonExpr &)
{
  open_tag ("ComparisonExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::LazyBooleanExpr &)
{
  open_tag ("LazyBooleanExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::TypeCastExpr &)
{
  open_tag ("TypeCastExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::AssignmentExpr &)
{
  open_tag ("AssignmentExpr");
  close_tag();
}

void
DumpHIR::visit (HIR::GroupedExpr &)
{
  open_tag ("GroupedExpr");
  close_tag();
}

void
DumpHIR::visit (HIR::ArrayElemsValues &)
{
  open_tag ("ArrayElemsValues");
  close_tag();
}
void
DumpHIR::visit (HIR::ArrayElemsCopied &)
{
  open_tag ("ArrayElemsCopied");
  close_tag();
}
void
DumpHIR::visit (HIR::ArrayExpr &)
{
  open_tag ("ArrayExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::ArrayIndexExpr &)
{
  open_tag ("ArrayIndexExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::TupleExpr &)
{
  open_tag ("TupleExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::TupleIndexExpr &)
{
  open_tag ("TupleIndexExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::StructExprStruct &)
{
  open_tag ("StructExprStruct");
  close_tag();
}

void
DumpHIR::visit (HIR::StructExprFieldIdentifier &)
{
  open_tag ("StructExprFieldIdentifier");
  close_tag();
}
void
DumpHIR::visit (HIR::StructExprFieldIdentifierValue &)
{
  open_tag ("StructExprFieldIdentifierValue");
  close_tag();
}

void
DumpHIR::visit (HIR::StructExprFieldIndexValue &)
{
  open_tag ("StructExprFieldIndexValue");
  close_tag();
}
void
DumpHIR::visit (HIR::StructExprStructFields &)
{
  open_tag ("StructExprStructFields");
  close_tag();
}
void
DumpHIR::visit (HIR::StructExprStructBase &)
{
  open_tag ("StructExprStructBase");
  close_tag();
}

void
DumpHIR::visit (HIR::CallExpr &)
{
  open_tag ("CallExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::MethodCallExpr &)
{
  open_tag ("MethodCallExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::FieldAccessExpr &)
{
  open_tag ("FieldAccessExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::ClosureExprInner &)
{
  open_tag ("ClosureExprInner");
  close_tag();
}
void
DumpHIR::visit (HIR::BlockExpr &)
{
  open_tag ("BlockExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::ClosureExprInnerTyped &)
{
  open_tag ("ClosureExprInnerTyped");
  close_tag();
}
void
DumpHIR::visit (HIR::ContinueExpr &)
{
  open_tag ("ContinueExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::BreakExpr &)
{
  open_tag ("BreakExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::RangeFromToExpr &)
{
  open_tag ("RangeFromToExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::RangeFromExpr &)
{
  open_tag ("RangeFromExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::RangeToExpr &)
{
  open_tag ("RangeToExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::RangeFullExpr &)
{
  open_tag ("RangeFullExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::RangeFromToInclExpr &)
{
  open_tag ("RangeFromToInclExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::RangeToInclExpr &)
{
  open_tag ("RangeToInclExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::ReturnExpr &)
{
  open_tag ("ReturnExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::UnsafeBlockExpr &)
{
  open_tag ("UnsafeBlockExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::LoopExpr &)
{
  open_tag ("LoopExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::WhileLoopExpr &)
{
  open_tag ("WhileLoopExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::WhileLetLoopExpr &)
{
  open_tag ("WhileLetLoopExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::ForLoopExpr &)
{
  open_tag ("ForLoopExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::IfExpr &)
{
  open_tag ("IfExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::IfExprConseqElse &)
{
  open_tag ("IfExprConseqElse");
  close_tag();
}
void
DumpHIR::visit (HIR::IfExprConseqIf &)
{
  open_tag ("IfExprConseqIf");
  close_tag();
}
void
DumpHIR::visit (HIR::IfExprConseqIfLet &)
{
  open_tag ("IfExprConseqIfLet");
  close_tag();
}
void
DumpHIR::visit (HIR::IfLetExpr &)
{
  open_tag ("IfLetExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::IfLetExprConseqElse &)
{
  open_tag ("IfLetExprConseqElse");
  close_tag();
}
void
DumpHIR::visit (HIR::IfLetExprConseqIf &)
{
  open_tag ("IfLetExprConseqIf");
  close_tag();
}
void
DumpHIR::visit (HIR::IfLetExprConseqIfLet &)
{
  open_tag ("IfLetExprConseqIfLet");
  close_tag();
}

void
DumpHIR::visit (HIR::MatchExpr &)
{
  open_tag ("MatchExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::AwaitExpr &)
{
  open_tag ("AwaitExpr");
  close_tag();
}
void
DumpHIR::visit (HIR::AsyncBlockExpr &)
{
  open_tag ("AsyncBlockExpr");
  close_tag();
}

void
DumpHIR::visit (HIR::TypeParam &)
{
  open_tag ("TypeParam");
  close_tag();
}

void
DumpHIR::visit (HIR::LifetimeWhereClauseItem &)
{
  open_tag ("LifetimeWhereClauseItem");
  close_tag();
}
void
DumpHIR::visit (HIR::TypeBoundWhereClauseItem &)
{
  open_tag ("TypeBoundWhereClauseItem");
  close_tag();
}
void
DumpHIR::visit (HIR::Module &)
{
  open_tag ("Module");
  close_tag();
}
void
DumpHIR::visit (HIR::ExternCrate &)
{


  open_tag ("ExternCrate");
  close_tag();
}

void
DumpHIR::visit (HIR::UseTreeGlob &)
{
  open_tag ("UseTreeGlob");
  close_tag();
}
void
DumpHIR::visit (HIR::UseTreeList &)
{
  open_tag ("UseTreeList");
  close_tag();
}
void
DumpHIR::visit (HIR::UseTreeRebind &)
{
  open_tag ("UseTreeRebind");
  close_tag();
}
void
DumpHIR::visit (HIR::UseDeclaration &)
{
  open_tag ("UseDeclaration");
  close_tag();
}
void
DumpHIR::visit (HIR::Function &f)
{
  std::string tag = "function";

  if (f.has_function_return_type())
    tag = tag + ":";

  open_tag (tag);
  close_tag(true);
}

void
DumpHIR::visit (HIR::TypeAlias &)
{
  open_tag ("TypeAlias");
  close_tag();
}
void
DumpHIR::visit (HIR::StructStruct &)
{
  open_tag ("StructStruct");
  close_tag();
}
void
DumpHIR::visit (HIR::TupleStruct &)
{
  open_tag ("TupleStruct");
  close_tag();
}
void
DumpHIR::visit (HIR::EnumItem &)
{
  open_tag ("EnumItem");
  close_tag();
}
void
DumpHIR::visit (HIR::EnumItemTuple &)
{
  open_tag ("EnumItemTuple");
  close_tag();
}
void
DumpHIR::visit (HIR::EnumItemStruct &)
{
  open_tag ("EnumItemStruct");
  close_tag();
}
void
DumpHIR::visit (HIR::EnumItemDiscriminant &)
{
  open_tag ("EnumItemDiscriminant");
  close_tag();
}
void
DumpHIR::visit (HIR::Enum &)
{
  open_tag ("Enum");
  close_tag();
}
void
DumpHIR::visit (HIR::Union &)
{
  open_tag ("Union");
  close_tag();
}
void
DumpHIR::visit (HIR::ConstantItem &)
{
  open_tag ("ConstantItem");
  close_tag();
}
void
DumpHIR::visit (HIR::StaticItem &)
{
  open_tag ("StaticItem");
  close_tag();
}
void
DumpHIR::visit (HIR::TraitItemFunc &)
{
  open_tag ("TraitItemFunc");
  close_tag();
}
void
DumpHIR::visit (HIR::TraitItemConst &)
{
  open_tag ("TraitItemConst");
  close_tag();
}
void
DumpHIR::visit (HIR::TraitItemType &)
{
  open_tag ("TraitItemType");
  close_tag();
}
void
DumpHIR::visit (HIR::Trait &)
{
  open_tag ("Trait");
  close_tag();
}
void
DumpHIR::visit (HIR::ImplBlock &)
{
  open_tag ("ImplBlock");
  close_tag();
}

void
DumpHIR::visit (HIR::ExternalStaticItem &)
{
  open_tag ("ExternalStaticItem");
  close_tag();
}
void
DumpHIR::visit (HIR::ExternalFunctionItem &)
{
  open_tag ("ExternalFunctionItem");
  close_tag();
}
void
DumpHIR::visit (HIR::ExternBlock &)
{
  open_tag ("ExternBlock");
  close_tag();
}

void
DumpHIR::visit (HIR::LiteralPattern &)
{
  open_tag ("LiteralPattern");
  close_tag();
}
void
DumpHIR::visit (HIR::IdentifierPattern &)
{
  open_tag ("IdentifierPattern");
  close_tag();
}
void
DumpHIR::visit (HIR::WildcardPattern &)
{
  open_tag ("WildcardPattern");
  close_tag();
}

void
DumpHIR::visit (HIR::RangePatternBoundLiteral &)
{
  open_tag ("RangePatternBoundLiteral");
  close_tag();
}
void
DumpHIR::visit (HIR::RangePatternBoundPath &)
{
  open_tag ("RangePatternBoundPath");
  close_tag();
}
void
DumpHIR::visit (HIR::RangePatternBoundQualPath &)
{
  open_tag ("RangePatternBoundQualPath");
  close_tag();
}
void
DumpHIR::visit (HIR::RangePattern &)
{
  open_tag ("RangePattern");
  close_tag();
}
void
DumpHIR::visit (HIR::ReferencePattern &)
{
  open_tag ("ReferencePattern");
  close_tag();
}

void
DumpHIR::visit (HIR::StructPatternFieldTuplePat &)
{
  open_tag ("StructPatternFieldTuplePat");
  close_tag();
}
void
DumpHIR::visit (HIR::StructPatternFieldIdentPat &)
{
  open_tag ("StructPatternFieldIdentPat");
  close_tag();
}
void
DumpHIR::visit (HIR::StructPatternFieldIdent &)
{
  open_tag ("StructPatternFieldIdent");
  close_tag();
}
void
DumpHIR::visit (HIR::StructPattern &)
{
  open_tag ("StructPattern");
  close_tag();
}

void
DumpHIR::visit (HIR::TupleStructItemsNoRange &)
{
  open_tag ("TupleStructItemsNoRange");
  close_tag();
}
void
DumpHIR::visit (HIR::TupleStructItemsRange &)
{
  open_tag ("TupleStructItemsRange");
  close_tag();
}
void
DumpHIR::visit (HIR::TupleStructPattern &)
{
  open_tag ("TupleStructPattern");
  close_tag();
}

void
DumpHIR::visit (HIR::TuplePatternItemsMultiple &)
{
  open_tag ("TuplePatternItemsMultiple");
  close_tag();
}
void
DumpHIR::visit (HIR::TuplePatternItemsRanged &)
{
  open_tag ("TuplePatternItemsRanged");
  close_tag();
}
void
DumpHIR::visit (HIR::TuplePattern &)
{
  open_tag ("TuplePattern");
  close_tag();
}
void
DumpHIR::visit (HIR::GroupedPattern &)
{
  open_tag ("GroupedPattern");
  close_tag();
}
void
DumpHIR::visit (HIR::SlicePattern &)
{
  open_tag ("SlicePattern");
  close_tag();
}

void
DumpHIR::visit (HIR::EmptyStmt &)
{
  open_tag ("EmptyStmt");
  close_tag();
}
void
DumpHIR::visit (HIR::LetStmt &)
{
  open_tag ("LetStmt");
  close_tag();
}
void
DumpHIR::visit (HIR::ExprStmtWithoutBlock &)
{
  open_tag ("ExprStmtWithoutBlock");
  close_tag();
}
void
DumpHIR::visit (HIR::ExprStmtWithBlock &)
{
  open_tag ("ExprStmtWithBlock");
  close_tag();
}

void
DumpHIR::visit (HIR::TraitBound &)
{
  open_tag ("TraitBound");
  close_tag();
}
void
DumpHIR::visit (HIR::ImplTraitType &)
{
  open_tag ("ImplTraitType");
  close_tag();
}
void
DumpHIR::visit (HIR::TraitObjectType &)
{
  open_tag ("TraitObjectType");
  close_tag();
}
void
DumpHIR::visit (HIR::ParenthesisedType &)
{
  open_tag ("ParenthesisedType");
  close_tag();
}
void
DumpHIR::visit (HIR::ImplTraitTypeOneBound &)
{
  open_tag ("ImplTraitTypeOneBound");
  close_tag();
}
void
DumpHIR::visit (HIR::TupleType &)
{
  open_tag ("TupleType");
  close_tag();
}
void
DumpHIR::visit (HIR::NeverType &)
{
  open_tag ("NeverType");
  close_tag();
}
void
DumpHIR::visit (HIR::RawPointerType &)
{
  open_tag ("RawPointerType");
  close_tag();
}
void
DumpHIR::visit (HIR::ReferenceType &)
{
  open_tag ("ReferenceType");
  close_tag();
}
void
DumpHIR::visit (HIR::ArrayType &)
{
  open_tag ("ArrayType");
  close_tag();
}
void
DumpHIR::visit (HIR::SliceType &)
{
  open_tag ("SliceType");
  close_tag();
}
void
DumpHIR::visit (HIR::InferredType &)
{
  open_tag ("InferredType");
  close_tag();
}
void
DumpHIR::visit (HIR::BareFunctionType &)
{
  open_tag ("BareFunctionType");
  close_tag();
}

} // namespace Dump
} // namespace Rust
