#include "ast/unary_operators.hpp"
#include "ast/globals.hpp"

namespace ast {

  UnaryMinusOperator::UnaryMinusOperator(std::shared_ptr<ExprNode> operand) 
  : UnaryOperator("-", operand) {}
  llvm::Value* UnaryMinusOperator::eval() {
    return CompilerContext::Builder->CreateNeg(operand->eval(), "uminus");
  }
  std::string UnaryMinusOperator::get_name() {
    return "Unary Minus Operator";
  }

  NegationOperator::NegationOperator(std::shared_ptr<ExprNode> operand)
  : UnaryOperator("~", operand) {}
  llvm::Value* NegationOperator::eval() {
    return CompilerContext::Builder->CreateNot(operand->eval(), "not");
  }
  std::string NegationOperator::get_name() {
    return "Negation Operator";
  }

}
