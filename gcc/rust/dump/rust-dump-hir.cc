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
DumpHIR::go (HIR::Crate &crate)
{
  m_dumpout << std::string(m_indent * 2, ' ') << "(crate" << std::endl;
  for (const auto &attr : crate.inner_attrs)
    {
      m_dumpout << std::string(m_indent * 2, ' ') << attr.as_string() << std::endl;
    }

  for (auto it = crate.items.begin (); it != crate.items.end (); it++)
    {
      m_indent += 2;
      it->get()->accept_vis (*this);
      m_indent -= 2;
    }
  m_dumpout << std::string(m_indent * 2, ' ') << ")" << std::endl;
}

void
DumpHIR::visit (HIR::IdentifierExpr &)
{}
void
DumpHIR::visit (HIR::Lifetime &)
{}
void
DumpHIR::visit (HIR::LifetimeParam &)
{}
void
DumpHIR::visit (HIR::PathInExpression &)
{}
void
DumpHIR::visit (HIR::TypePathSegment &)
{}
void
DumpHIR::visit (HIR::TypePathSegmentGeneric &)
{}
void
DumpHIR::visit (HIR::TypePathSegmentFunction &)
{}
void
DumpHIR::visit (HIR::TypePath &)
{}
void
DumpHIR::visit (HIR::QualifiedPathInExpression &)
{}
void
DumpHIR::visit (HIR::QualifiedPathInType &)
{}

void
DumpHIR::visit (HIR::LiteralExpr &)
{}
void
DumpHIR::visit (HIR::BorrowExpr &)
{}
void
DumpHIR::visit (HIR::DereferenceExpr &)
{}
void
DumpHIR::visit (HIR::ErrorPropagationExpr &)
{}
void
DumpHIR::visit (HIR::NegationExpr &)
{}
void
DumpHIR::visit (HIR::ArithmeticOrLogicalExpr &)
{}
void
DumpHIR::visit (HIR::ComparisonExpr &)
{}
void
DumpHIR::visit (HIR::LazyBooleanExpr &)
{}
void
DumpHIR::visit (HIR::TypeCastExpr &)
{}
void
DumpHIR::visit (HIR::AssignmentExpr &)
{}

void
DumpHIR::visit (HIR::GroupedExpr &)
{}

void
DumpHIR::visit (HIR::ArrayElemsValues &)
{}
void
DumpHIR::visit (HIR::ArrayElemsCopied &)
{}
void
DumpHIR::visit (HIR::ArrayExpr &)
{}
void
DumpHIR::visit (HIR::ArrayIndexExpr &)
{}
void
DumpHIR::visit (HIR::TupleExpr &)
{}
void
DumpHIR::visit (HIR::TupleIndexExpr &)
{}
void
DumpHIR::visit (HIR::StructExprStruct &)
{}

void
DumpHIR::visit (HIR::StructExprFieldIdentifier &)
{}
void
DumpHIR::visit (HIR::StructExprFieldIdentifierValue &)
{}

void
DumpHIR::visit (HIR::StructExprFieldIndexValue &)
{}
void
DumpHIR::visit (HIR::StructExprStructFields &)
{}
void
DumpHIR::visit (HIR::StructExprStructBase &)
{}

void
DumpHIR::visit (HIR::CallExpr &)
{}
void
DumpHIR::visit (HIR::MethodCallExpr &)
{}
void
DumpHIR::visit (HIR::FieldAccessExpr &)
{}
void
DumpHIR::visit (HIR::ClosureExprInner &)
{}
void
DumpHIR::visit (HIR::BlockExpr &)
{}
void
DumpHIR::visit (HIR::ClosureExprInnerTyped &)
{}
void
DumpHIR::visit (HIR::ContinueExpr &)
{}
void
DumpHIR::visit (HIR::BreakExpr &)
{}
void
DumpHIR::visit (HIR::RangeFromToExpr &)
{}
void
DumpHIR::visit (HIR::RangeFromExpr &)
{}
void
DumpHIR::visit (HIR::RangeToExpr &)
{}
void
DumpHIR::visit (HIR::RangeFullExpr &)
{}
void
DumpHIR::visit (HIR::RangeFromToInclExpr &)
{}
void
DumpHIR::visit (HIR::RangeToInclExpr &)
{}
void
DumpHIR::visit (HIR::ReturnExpr &)
{}
void
DumpHIR::visit (HIR::UnsafeBlockExpr &)
{}
void
DumpHIR::visit (HIR::LoopExpr &)
{}
void
DumpHIR::visit (HIR::WhileLoopExpr &)
{}
void
DumpHIR::visit (HIR::WhileLetLoopExpr &)
{}
void
DumpHIR::visit (HIR::ForLoopExpr &)
{}
void
DumpHIR::visit (HIR::IfExpr &)
{}
void
DumpHIR::visit (HIR::IfExprConseqElse &)
{}
void
DumpHIR::visit (HIR::IfExprConseqIf &)
{}
void
DumpHIR::visit (HIR::IfExprConseqIfLet &)
{}
void
DumpHIR::visit (HIR::IfLetExpr &)
{}
void
DumpHIR::visit (HIR::IfLetExprConseqElse &)
{}
void
DumpHIR::visit (HIR::IfLetExprConseqIf &)
{}
void
DumpHIR::visit (HIR::IfLetExprConseqIfLet &)
{}

void
DumpHIR::visit (HIR::MatchExpr &)
{}
void
DumpHIR::visit (HIR::AwaitExpr &)
{}
void
DumpHIR::visit (HIR::AsyncBlockExpr &)
{}

void
DumpHIR::visit (HIR::TypeParam &)
{}

void
DumpHIR::visit (HIR::LifetimeWhereClauseItem &)
{}
void
DumpHIR::visit (HIR::TypeBoundWhereClauseItem &)
{}
void
DumpHIR::visit (HIR::Module &)
{}
void
DumpHIR::visit (HIR::ExternCrate &)
{

}

void
DumpHIR::visit (HIR::UseTreeGlob &)
{}
void
DumpHIR::visit (HIR::UseTreeList &)
{}
void
DumpHIR::visit (HIR::UseTreeRebind &)
{}
void
DumpHIR::visit (HIR::UseDeclaration &)
{}
void
DumpHIR::visit (HIR::Function &)
{

  m_dumpout << std::string(m_indent * 2, ' ') << "(function " ;
  m_dumpout << std::string(m_indent * 2, ' ') << ")" << std::endl;
}

void
DumpHIR::visit (HIR::TypeAlias &)
{}
void
DumpHIR::visit (HIR::StructStruct &)
{}
void
DumpHIR::visit (HIR::TupleStruct &)
{}
void
DumpHIR::visit (HIR::EnumItem &)
{}
void
DumpHIR::visit (HIR::EnumItemTuple &)
{}
void
DumpHIR::visit (HIR::EnumItemStruct &)
{}
void
DumpHIR::visit (HIR::EnumItemDiscriminant &)
{}
void
DumpHIR::visit (HIR::Enum &)
{}
void
DumpHIR::visit (HIR::Union &)
{}
void
DumpHIR::visit (HIR::ConstantItem &)
{}
void
DumpHIR::visit (HIR::StaticItem &)
{}
void
DumpHIR::visit (HIR::TraitItemFunc &)
{}
void
DumpHIR::visit (HIR::TraitItemConst &)
{}
void
DumpHIR::visit (HIR::TraitItemType &)
{}
void
DumpHIR::visit (HIR::Trait &)
{}
void
DumpHIR::visit (HIR::ImplBlock &)
{}

void
DumpHIR::visit (HIR::ExternalStaticItem &)
{}
void
DumpHIR::visit (HIR::ExternalFunctionItem &)
{}
void
DumpHIR::visit (HIR::ExternBlock &)
{}

void
DumpHIR::visit (HIR::LiteralPattern &)
{}
void
DumpHIR::visit (HIR::IdentifierPattern &)
{}
void
DumpHIR::visit (HIR::WildcardPattern &)
{}

void
DumpHIR::visit (HIR::RangePatternBoundLiteral &)
{}
void
DumpHIR::visit (HIR::RangePatternBoundPath &)
{}
void
DumpHIR::visit (HIR::RangePatternBoundQualPath &)
{}
void
DumpHIR::visit (HIR::RangePattern &)
{}
void
DumpHIR::visit (HIR::ReferencePattern &)
{}

void
DumpHIR::visit (HIR::StructPatternFieldTuplePat &)
{}
void
DumpHIR::visit (HIR::StructPatternFieldIdentPat &)
{}
void
DumpHIR::visit (HIR::StructPatternFieldIdent &)
{}
void
DumpHIR::visit (HIR::StructPattern &)
{}

void
DumpHIR::visit (HIR::TupleStructItemsNoRange &)
{}
void
DumpHIR::visit (HIR::TupleStructItemsRange &)
{}
void
DumpHIR::visit (HIR::TupleStructPattern &)
{}

void
DumpHIR::visit (HIR::TuplePatternItemsMultiple &)
{}
void
DumpHIR::visit (HIR::TuplePatternItemsRanged &)
{}
void
DumpHIR::visit (HIR::TuplePattern &)
{}
void
DumpHIR::visit (HIR::GroupedPattern &)
{}
void
DumpHIR::visit (HIR::SlicePattern &)
{}

void
DumpHIR::visit (HIR::EmptyStmt &)
{}
void
DumpHIR::visit (HIR::LetStmt &)
{}
void
DumpHIR::visit (HIR::ExprStmtWithoutBlock &)
{}
void
DumpHIR::visit (HIR::ExprStmtWithBlock &)
{}

void
DumpHIR::visit (HIR::TraitBound &)
{}
void
DumpHIR::visit (HIR::ImplTraitType &)
{}
void
DumpHIR::visit (HIR::TraitObjectType &)
{}
void
DumpHIR::visit (HIR::ParenthesisedType &)
{}
void
DumpHIR::visit (HIR::ImplTraitTypeOneBound &)
{}
void
DumpHIR::visit (HIR::TupleType &)
{}
void
DumpHIR::visit (HIR::NeverType &)
{}
void
DumpHIR::visit (HIR::RawPointerType &)
{}
void
DumpHIR::visit (HIR::ReferenceType &)
{}
void
DumpHIR::visit (HIR::ArrayType &)
{}
void
DumpHIR::visit (HIR::SliceType &)
{}
void
DumpHIR::visit (HIR::InferredType &)
{}
void
DumpHIR::visit (HIR::BareFunctionType &)
{}

} // namespace Dump
} // namespace Rust
