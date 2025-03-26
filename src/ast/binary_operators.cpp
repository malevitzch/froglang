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
  std::string AdditionOperator::get_name() {
    return "Addition Operator";
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
  std::string SubtractionOperator::get_name() {
    return "Subtraction Operator";
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
  std::string MultiplicationOperator::get_name() {
    return "Multiplication Operator";
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
  std::string DivisionOperator::get_name() {
    return "Division Operator";
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
  std::string ModuloOperator::get_name() {
    return "Modulo Operator";
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
  std::string LesserComparisonOperator::get_name() {
    return "Lesser Comparison Operator";
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
  std::string GreaterComparisonOperator::get_name() {
    return "Greater Comparison Operator";
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
  std::string LesserEQComparisonOperator::get_name() {
    return "Lesser Equal Comparison Operator";
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
  std::string GreaterEQComparisonOperator::get_name() {
    return "Greater Equal Comparison Operator";
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
  std::string EqualityComparisonOperator::get_name() {
    return "Equality Comparison Operator";
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
  std::string InequalityComparisonOperator::get_name() {
    return "Inequality Comparison Operator";
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
  std::string LogicalAndOperator::get_name() {
    return "Logical And Operator";
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
  std::string LogicalOrOperator::get_name() {
    return "Logical Or Operator";
  }

}
