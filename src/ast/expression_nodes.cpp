#include "ast/expression_nodes.hpp"

namespace ast {

  ExprNode::ExprNode(std::string type) : type(type) {}

  std::string ExprNode::get_name() {
    return "Expression Node";
  }
  std::string ExprNode::get_type() {
    return type;
  }
  llvm::Value* BinaryOperator::codegen() {
    //TODO: implement using builder
  }
  BinaryOperator::BinaryOperator(std::string operator_type) : operator_type(operator_type) {}
  // TODO: This needs complex logic for result type deduction once we want to use more than just integers.
  std::string BinaryOperator::get_type() {
    //FIXME: This is a very bad idea, even when we just introduce if statements (unless we use integers as bools, then it's fine).
    return "int32";
  }
}
