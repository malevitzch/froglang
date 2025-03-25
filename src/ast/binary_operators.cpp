#include "ast/binary_operators.hpp"
#include "ast/globals.hpp"

namespace ast {

  AdditionOperator::AdditionOperator(
    std::shared_ptr<ExprNode> left,
    std::shared_ptr<ExprNode> right)
    : BinaryOperator("+", left, right) {}
  llvm::Value* AdditionOperator::eval() {
    auto L = left->eval();
    auto R = right->eval();
    return CompilerContext::Builder->CreateAdd(L, R, "addtmp");
  }

  SubtractionOperator::SubtractionOperator(
    std::shared_ptr<ExprNode> left,
    std::shared_ptr<ExprNode> right)
    : BinaryOperator("-", left, right) {}
  llvm::Value* SubtractionOperator::eval() {
    auto L = left->eval();
    auto R = right->eval();
    return CompilerContext::Builder->CreateSub(L, R, "subtmp");
  }

  MultiplicationOperator::MultiplicationOperator(
    std::shared_ptr<ExprNode> left,
    std::shared_ptr<ExprNode> right)
    : BinaryOperator("*", left, right) {}
  llvm::Value* MultiplicationOperator::eval() {
    auto L = left->eval();
    auto R = right->eval();
    return CompilerContext::Builder->CreateMul(L, R, "multmp");
  }

  DivisionOperator::DivisionOperator(
    std::shared_ptr<ExprNode> left,
    std::shared_ptr<ExprNode> right)
    : BinaryOperator("/", left, right) {}
  llvm::Value* DivisionOperator::eval() {
    auto L = left->eval();
    auto R = right->eval();
    return CompilerContext::Builder->CreateSDiv(L, R, "sdiv"); 
  }

  ModuloOperator::ModuloOperator(
    std::shared_ptr<ExprNode> left,
    std::shared_ptr<ExprNode> right)
    : BinaryOperator("%", left, right) {}
  llvm::Value* ModuloOperator::eval() {
    auto L = left->eval();
    auto R = right->eval();
    return CompilerContext::Builder->CreateSRem(L, R, "lesstmp"); 
  }

  LesserComparisonOperator::LesserComparisonOperator(
    std::shared_ptr<ExprNode> left,
    std::shared_ptr<ExprNode> right)
    : BinaryOperator("<", left, right) {}
  llvm::Value* LesserComparisonOperator::eval() {
    auto L = left->eval();
    auto R = right->eval();
    return CompilerContext::Builder->CreateICmpSLT(L, R, "greatertmp"); 
  }

  GreaterComparisonOperator::GreaterComparisonOperator(
    std::shared_ptr<ExprNode> left,
    std::shared_ptr<ExprNode> right)
    : BinaryOperator(">", left, right) {}
  llvm::Value* GreaterComparisonOperator::eval() {
    auto L = left->eval();
    auto R = right->eval();
    return CompilerContext::Builder->CreateICmpSGT(L, R, "greatertmp"); 
  }

  LesserEQComparisonOperator::LesserEQComparisonOperator (
    std::shared_ptr<ExprNode> left,
    std::shared_ptr<ExprNode> right)
    : BinaryOperator("<=", left, right) {}
  llvm::Value* LesserEQComparisonOperator::eval() {
    auto L = left->eval();
    auto R = right->eval();
    return CompilerContext::Builder->CreateICmpSLE(L, R, "lesseqtmp"); 
  }

  GreaterEQComparisonOperator::GreaterEQComparisonOperator(
    std::shared_ptr<ExprNode> left,
    std::shared_ptr<ExprNode> right)
    : BinaryOperator(">=", left, right) {}
  llvm::Value* GreaterEQComparisonOperator::eval() {
    auto L = left->eval();
    auto R = right->eval();
    return CompilerContext::Builder->CreateICmpSGE(L, R, "greatereqtmp"); 
  }

  EqualityComparisonOperator::EqualityComparisonOperator(
    std::shared_ptr<ExprNode> left,
    std::shared_ptr<ExprNode> right)
    : BinaryOperator("==", left, right) {}
  llvm::Value* EqualityComparisonOperator::eval() {
    auto L = left->eval();
    auto R = right->eval();
    return CompilerContext::Builder->CreateICmpEQ(L, R, "equaltmp"); 
  }

  InequalityComparisonOperator::InequalityComparisonOperator(
    std::shared_ptr<ExprNode> left,
    std::shared_ptr<ExprNode> right)
    : BinaryOperator("!=", left, right) {}
  llvm::Value* InequalityComparisonOperator::eval() {
    auto L = left->eval();
    auto R = right->eval();
    return CompilerContext::Builder->CreateICmpNE(L, R, "nequaltmp"); 
  }

  LogicalAndOperator::LogicalAndOperator(
    std::shared_ptr<ExprNode> left,
    std::shared_ptr<ExprNode> right)
    : BinaryOperator("&&", left, right) {}
  llvm::Value* LogicalAndOperator::eval() {
    auto L = left->eval();
    auto R = right->eval();
    return CompilerContext::Builder->CreateAnd(L, R, "andtmp"); 
  }

  LogicalOrOperator::LogicalOrOperator(
    std::shared_ptr<ExprNode> left,
    std::shared_ptr<ExprNode> right)
    : BinaryOperator("||", left, right) {}
  llvm::Value* LogicalOrOperator::eval() {
    auto L = left->eval();
    auto R = right->eval();
    return CompilerContext::Builder->CreateOr(L, R, "ortmp"); 
  }

}
