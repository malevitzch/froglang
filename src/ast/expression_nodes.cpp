#include "ast/expression_nodes.hpp"
#include <cstdlib>
namespace ast {
  ExprNode::ExprNode(std::string type)
  : type(type) {}
  std::shared_ptr<llvm::Value> eval();
  std::string ExprNode::get_name() {
    return "Expression Node";
  }
  std::string ExprNode::get_type() {
    return type;
  }

  void BinaryOperator::codegen() {
    //TODO: implement using builder
  }
  //TODO: the BinaryOperator uses deduce_type() to find out what type it should be. 
  // There will be no implicit conversions because those sound like a very bad idea
  BinaryOperator::BinaryOperator(std::string operator_type, std::shared_ptr<ExprNode> left, std::shared_ptr<ExprNode> right)
  : ExprNode("int32"), operator_type(operator_type), left(left), right(right) {}
  // TODO: This needs complex logic for result type deduction once we want to use more than just integers.
  std::string BinaryOperator::get_type() {
    //FIXME: This is a very bad idea, even when we just introduce if statements (unless we use integers as bools, then it's fine for if statements but bad for everything else).
    return type;
  }
  std::string BinaryOperator::get_name() {
    return "Binary Operator";
  }
  std::vector<std::shared_ptr<Node>> BinaryOperator::get_children() {
    return {left, right};
  }

  IntegerConstant::IntegerConstant(std::string data, std::string type) 
  : ExprNode(type), value(std::stoi(data)) { final = true; }
  void IntegerConstant::codegen() {
  }
  std::string IntegerConstant::get_name() {
    return "Integer Constant";
  }

  VariableIdentifier::VariableIdentifier(std::string var_name, std::string type) 
  : ExprNode(type), var_name(var_name) { final = true; }
  void VariableIdentifier::codegen() {
  }
  std::string VariableIdentifier::get_name() {
    return "Variable Identifier";
  }

  void FunctionCallArgs::add_arg(std::shared_ptr<ExprNode> arg) {
    args.push_back(arg);
  }
  void FunctionCallArgs::codegen() {
  }
  std::string FunctionCallArgs::get_name() {
    return "Function Call Args";
  }
  std::vector<std::shared_ptr<Node>> FunctionCallArgs::get_children() {
    std::vector<std::shared_ptr<Node>> children;
    for(std::shared_ptr<ExprNode> arg : args)
      children.push_back(arg);
    return children;
  }

  FunctionCallArglist::FunctionCallArglist(std::shared_ptr<FunctionCallArgs> args) 
  : args(args) {}
  void FunctionCallArglist::codegen() {
  }
  std::string FunctionCallArglist::get_name() {
    return "Function Call Arglist";
  }
  std::vector<std::shared_ptr<Node>> FunctionCallArglist::get_children() {
    if(args == nullptr) return {};
    return {args};
  }

  //FIXME: crimes are being committed here
  FunctionCallExpr::FunctionCallExpr(std::string function_name, std::shared_ptr<FunctionCallArglist> args)
  : ExprNode("int32"), function_name(function_name), args(args) {}
  void FunctionCallExpr::codegen() {
  }
  std::string FunctionCallExpr::get_name() {
    return "Function Call Expression";
  }
  std::vector<std::shared_ptr<Node>> FunctionCallExpr::get_children() {
    return {args};
  }
}
