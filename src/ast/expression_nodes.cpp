#include "ast/expression_nodes.hpp"
#include <cstdlib>

#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"



namespace ast {

  std::string ExprNode::get_name() {
    return "Expression Node";
  }

  UnaryOperator::UnaryOperator(std::string operator_type, std::shared_ptr<ExprNode> operand) 
  : operator_type(operator_type), operand(operand) {}
  llvm::Value* UnaryOperator::eval() {
    if(operator_type == "-") {
      llvm::Value* operand_val = operand->eval();
      llvm::Value* zero = llvm::ConstantInt::get(operand_val->getType(), 0);

      return CompilerContext::Builder->CreateSub(zero, operand_val, "neg");
    }
    throw std::runtime_error("Unimplemented unary operator: \"" + operator_type + "\"");
  }
  std::string UnaryOperator::get_name() {
    return "Unary Operator";
  }
  std::vector<std::shared_ptr<Node>> UnaryOperator::get_children() {
    return {operand};
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
    if(operator_type == "/" ) {
      return CompilerContext::Builder->CreateSDiv(L, R, "sdiv");
    }
    throw std::runtime_error("Unimplemented binary operator: \"" + operator_type + "\"");
  }

  //TODO: the BinaryOperator uses deduce_type() to find out what type it should be. 
  // There will be no implicit conversions because those sound like a very bad idea
  BinaryOperator::BinaryOperator(std::string operator_type, std::shared_ptr<ExprNode> left, std::shared_ptr<ExprNode> right)
  : operator_type(operator_type), left(left), right(right) {}
  // TODO: This needs complex logic for result type deduction once we want to use more than just integers.
  std::string BinaryOperator::get_name() {
    return "Binary Operator";
  }
  std::vector<std::shared_ptr<Node>> BinaryOperator::get_children() {
    return {left, right};
  }

  IntegerConstant::IntegerConstant(std::string data, llvm::Type* type) 
  : value(std::stoi(data)) { final = true; }
  llvm::Value* IntegerConstant::eval() {
    return llvm::ConstantInt::get(*CompilerContext::TheContext, llvm::APInt(32, value));
  }

  std::string IntegerConstant::get_name() {
    return "Integer Constant";
  }

  VariableIdentifier::VariableIdentifier(std::string var_name) 
  : var_name(var_name) { final = true; }
  llvm::Value* VariableIdentifier::eval() {
    if(!CompilerContext::NamedValues->has_val(var_name)) {
      //TODO: some kind of exception?
    }
    return CompilerContext::NamedValues->get_val(var_name);
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
  std::vector<std::shared_ptr<ExprNode>> FunctionCallArgs::get_args() {
    return args;
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
  std::vector<std::shared_ptr<ExprNode>> FunctionCallArglist::get_args() {
    return args->get_args();
  }

  //FIXME: crimes are being committed here
  FunctionCallExpr::FunctionCallExpr(std::string function_name, std::shared_ptr<FunctionCallArglist> args)
  : function_name(function_name), args(args) {}
  llvm::Value* FunctionCallExpr::eval() {
    //TODO: check for availability?
    if(!CompilerContext::Functions->contains(function_name))
      throw std::runtime_error("Unregisterd function \"" + function_name + "\"");
    llvm::Function* to_call = CompilerContext::Functions->at(function_name);
    std::vector<llvm::Value*> call_args;
    for(std::shared_ptr<ExprNode> arg : args->get_args())
      call_args.push_back(arg->eval());
    return CompilerContext::Builder->CreateCall(to_call, call_args);
  }

  std::string FunctionCallExpr::get_name() {
    return "Function Call Expression";
  }
  std::vector<std::shared_ptr<Node>> FunctionCallExpr::get_children() {
    return {args};
  }
}
