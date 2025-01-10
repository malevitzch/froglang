#include "ast/expression_nodes.hpp"
#include <cstdlib>

#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"



namespace ast {
  ExprNode::ExprNode(std::string type)
  : type(type) {}
  std::string ExprNode::get_name() {
    return "Expression Node";
  }
  std::string ExprNode::get_type() {
    return type;
  }
  
  llvm::Value* BinaryOperator::eval() {
    auto L = left->eval();
    auto R = right->eval();
    if(operator_type == "+") {
      return CompilerContext::Builder->CreateAdd(L, R, "addtmp");
    }
    if(operator_type == "-") {
      return CompilerContext::Builder->CreateSub(L, R, "subtmp");
    }
    if(operator_type == "*") {
      return CompilerContext::Builder->CreateMul(L, R, "multmp");
    }
  //TODO: division is not as simple as the others

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
  llvm::Value* IntegerConstant::eval() {
    return llvm::ConstantInt::get(*CompilerContext::TheContext, llvm::APInt(32, value));
  }

  std::string IntegerConstant::get_name() {
    return "Integer Constant";
  }

  VariableIdentifier::VariableIdentifier(std::string var_name, std::string type) 
  : ExprNode(type), var_name(var_name) { final = true; }
  llvm::Value* VariableIdentifier::eval() {
    //TODO: maybe add some check if there is such a named value already
    return CompilerContext::NamedValues[var_name];
  }

  std::string VariableIdentifier::get_name() {
    return "Variable Identifier";
  }

  void FunctionCallArgs::add_arg(std::shared_ptr<ExprNode> arg) {
    args.push_back(arg);
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
  llvm::Value* FunctionCallExpr::eval() {
    //TODO: make args accessible so that we can make a call
  }

  std::string FunctionCallExpr::get_name() {
    return "Function Call Expression";
  }
  std::vector<std::shared_ptr<Node>> FunctionCallExpr::get_children() {
    return {args};
  }
}
