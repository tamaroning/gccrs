#ifndef RUST_HIR_DUMP_H
#define RUST_HIR_DUMP_H

#include "rust-hir-full.h"
#include "rust-hir-visitor.h"

#include "rust-hir-full-decls.h"
#include "rust-hir-map.h"
#include "rust-name-resolver.h"
#include "rust-diagnostics.h"
#include <iostream>
#include <fstream>

namespace Rust {
namespace Dump {

class DumpHIR : public HIR::HIRVisitor
{
public:
  static void dump (HIR::Crate &crate, std::ofstream &out)
  {
    DumpHIR dhir (out, 0);
    dhir.go(crate);
  }
  void go(HIR::Crate &);

  virtual void visit (HIR::IdentifierExpr &) override;
  virtual void visit (HIR::Lifetime &) override;
  virtual void visit (HIR::LifetimeParam &) override;
  virtual void visit (HIR::PathInExpression &) override;
  virtual void visit (HIR::TypePathSegment &) override;
  virtual void visit (HIR::TypePathSegmentGeneric &) override;
  virtual void visit (HIR::TypePathSegmentFunction &) override;
  virtual void visit (HIR::TypePath &) override;
  virtual void visit (HIR::QualifiedPathInExpression &) override;
  virtual void visit (HIR::QualifiedPathInType &) override;

  virtual void visit (HIR::LiteralExpr &) override;
  virtual void visit (HIR::BorrowExpr &) override;
  virtual void visit (HIR::DereferenceExpr &) override;
  virtual void visit (HIR::ErrorPropagationExpr &) override;
  virtual void visit (HIR::NegationExpr &) override;
  virtual void visit (HIR::ArithmeticOrLogicalExpr &) override;
  virtual void visit (HIR::ComparisonExpr &) override;
  virtual void visit (HIR::LazyBooleanExpr &) override;
  virtual void visit (HIR::TypeCastExpr &) override;
  virtual void visit (HIR::AssignmentExpr &) override;

  virtual void visit (HIR::GroupedExpr &) override;

  virtual void visit (HIR::ArrayElemsValues &) override;
  virtual void visit (HIR::ArrayElemsCopied &) override;
  virtual void visit (HIR::ArrayExpr &) override;
  virtual void visit (HIR::ArrayIndexExpr &) override;
  virtual void visit (HIR::TupleExpr &) override;
  virtual void visit (HIR::TupleIndexExpr &) override;
  virtual void visit (HIR::StructExprStruct &) override;

  virtual void visit (HIR::StructExprFieldIdentifier &) override;
  virtual void visit (HIR::StructExprFieldIdentifierValue &) override;

  virtual void visit (HIR::StructExprFieldIndexValue &) override;
  virtual void visit (HIR::StructExprStructFields &) override;
  virtual void visit (HIR::StructExprStructBase &) override;

  virtual void visit (HIR::CallExpr &) override;
  virtual void visit (HIR::MethodCallExpr &) override;
  virtual void visit (HIR::FieldAccessExpr &) override;
  virtual void visit (HIR::ClosureExprInner &) override;
  virtual void visit (HIR::BlockExpr &) override;
  virtual void visit (HIR::ClosureExprInnerTyped &) override;
  virtual void visit (HIR::ContinueExpr &) override;
  virtual void visit (HIR::BreakExpr &) override;
  virtual void visit (HIR::RangeFromToExpr &) override;
  virtual void visit (HIR::RangeFromExpr &) override;
  virtual void visit (HIR::RangeToExpr &) override;
  virtual void visit (HIR::RangeFullExpr &) override;
  virtual void visit (HIR::RangeFromToInclExpr &) override;
  virtual void visit (HIR::RangeToInclExpr &) override;
  virtual void visit (HIR::ReturnExpr &) override;
  virtual void visit (HIR::UnsafeBlockExpr &) override;
  virtual void visit (HIR::LoopExpr &) override;
  virtual void visit (HIR::WhileLoopExpr &) override;
  virtual void visit (HIR::WhileLetLoopExpr &) override;
  virtual void visit (HIR::ForLoopExpr &) override;
  virtual void visit (HIR::IfExpr &) override;
  virtual void visit (HIR::IfExprConseqElse &) override;
  virtual void visit (HIR::IfExprConseqIf &) override;
  virtual void visit (HIR::IfExprConseqIfLet &) override;
  virtual void visit (HIR::IfLetExpr &) override;
  virtual void visit (HIR::IfLetExprConseqElse &) override;
  virtual void visit (HIR::IfLetExprConseqIf &) override;
  virtual void visit (HIR::IfLetExprConseqIfLet &) override;

  virtual void visit (HIR::MatchExpr &) override;
  virtual void visit (HIR::AwaitExpr &) override;
  virtual void visit (HIR::AsyncBlockExpr &) override;

  virtual void visit (HIR::TypeParam &) override;

  virtual void visit (HIR::LifetimeWhereClauseItem &) override;
  virtual void visit (HIR::TypeBoundWhereClauseItem &) override;
  virtual void visit (HIR::Module &) override;
  virtual void visit (HIR::ExternCrate &) override;

  virtual void visit (HIR::UseTreeGlob &) override;
  virtual void visit (HIR::UseTreeList &) override;
  virtual void visit (HIR::UseTreeRebind &) override;
  virtual void visit (HIR::UseDeclaration &) override;
  virtual void visit (HIR::Function &) override;
  virtual void visit (HIR::TypeAlias &) override;
  virtual void visit (HIR::StructStruct &) override;
  virtual void visit (HIR::TupleStruct &) override;
  virtual void visit (HIR::EnumItem &) override;
  virtual void visit (HIR::EnumItemTuple &) override;
  virtual void visit (HIR::EnumItemStruct &) override;
  virtual void visit (HIR::EnumItemDiscriminant &) override;
  virtual void visit (HIR::Enum &) override;
  virtual void visit (HIR::Union &) override;
  virtual void visit (HIR::ConstantItem &) override;
  virtual void visit (HIR::StaticItem &) override;
  virtual void visit (HIR::TraitItemFunc &) override;
  virtual void visit (HIR::TraitItemConst &) override;
  virtual void visit (HIR::TraitItemType &) override;
  virtual void visit (HIR::Trait &) override;
  virtual void visit (HIR::ImplBlock &) override;

  virtual void visit (HIR::ExternalStaticItem &) override;
  virtual void visit (HIR::ExternalFunctionItem &) override;
  virtual void visit (HIR::ExternBlock &) override;

  virtual void visit (HIR::LiteralPattern &) override;
  virtual void visit (HIR::IdentifierPattern &) override;
  virtual void visit (HIR::WildcardPattern &) override;

  virtual void visit (HIR::RangePatternBoundLiteral &) override;
  virtual void visit (HIR::RangePatternBoundPath &) override;
  virtual void visit (HIR::RangePatternBoundQualPath &) override;
  virtual void visit (HIR::RangePattern &) override;
  virtual void visit (HIR::ReferencePattern &) override;

  virtual void visit (HIR::StructPatternFieldTuplePat &) override;
  virtual void visit (HIR::StructPatternFieldIdentPat &) override;
  virtual void visit (HIR::StructPatternFieldIdent &) override;
  virtual void visit (HIR::StructPattern &) override;

  virtual void visit (HIR::TupleStructItemsNoRange &) override;
  virtual void visit (HIR::TupleStructItemsRange &) override;
  virtual void visit (HIR::TupleStructPattern &) override;

  virtual void visit (HIR::TuplePatternItemsMultiple &) override;
  virtual void visit (HIR::TuplePatternItemsRanged &) override;
  virtual void visit (HIR::TuplePattern &) override;
  virtual void visit (HIR::GroupedPattern &) override;
  virtual void visit (HIR::SlicePattern &) override;

  virtual void visit (HIR::EmptyStmt &) override;
  virtual void visit (HIR::LetStmt &) override;
  virtual void visit (HIR::ExprStmtWithoutBlock &) override;
  virtual void visit (HIR::ExprStmtWithBlock &) override;

  virtual void visit (HIR::TraitBound &) override;
  virtual void visit (HIR::ImplTraitType &) override;
  virtual void visit (HIR::TraitObjectType &) override;
  virtual void visit (HIR::ParenthesisedType &) override;
  virtual void visit (HIR::ImplTraitTypeOneBound &) override;
  virtual void visit (HIR::TupleType &) override;
  virtual void visit (HIR::NeverType &) override;
  virtual void visit (HIR::RawPointerType &) override;
  virtual void visit (HIR::ReferenceType &) override;
  virtual void visit (HIR::ArrayType &) override;
  virtual void visit (HIR::SliceType &) override;
  virtual void visit (HIR::InferredType &) override;
  virtual void visit (HIR::BareFunctionType &) override;

  virtual ~DumpHIR (){};

protected:

  DumpHIR (std::ofstream &out, int ind);

  void open_tag (std::string tag);
  void close_tag(bool new_line = false);

  void dump_attr (AST::AttrVec &attrs);

  std::ofstream &m_dumpout;
  int m_sawclose;
  int m_indent;
};
} // namespace Dump
} // namespace Rust

#endif // RUST_HIR_DUMP_H
