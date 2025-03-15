#include "ast/expression_nodes.hpp"
#include "ast/globals.hpp"

#include <cstdlib>

#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"

namespace ast {

  std::string ExprNode::get_name() {
    return "Expression Node";
  }

  IversonExpr::IversonExpr(std::shared_ptr<ExprNode> expr) 
  : expr(expr) {}

  llvm::Value* IversonExpr::eval() {
    llvm::Value* expr_value = expr->eval();
    llvm::Value* zero = llvm::Constant::getNullValue(expr_value->getType());
    llvm::Value* cmp_result = 
      CompilerContext::Builder->CreateICmpNE(expr_value, zero);

    return CompilerContext::Builder->CreateIntCast(
      cmp_result,
      CompilerContext::Builder->getInt32Ty(),
      false);
  }

  std::string IversonExpr::get_name() {
    return "Iverson Expression";
  }

  std::vector<std::shared_ptr<Node>> IversonExpr::get_children() {
    return {expr};
  }

  UnaryOperator::UnaryOperator(
    std::string operator_type,
    std::shared_ptr<ExprNode> operand) 
  : operator_type(operator_type), operand(operand) {}
  llvm::Value* UnaryOperator::eval() {
    //FIXME: replace with operator subclasses
    if(operator_type == "-") {
      return CompilerContext::Builder->CreateNeg(operand->eval(), "uminus");
    }
    if(operator_type == "~") {
      return CompilerContext::Builder->CreateNot(operand->eval(), "not");
    }
    throw std::runtime_error(
      "Unimplemented unary operator: \"" + operator_type + "\"");
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
    //FIXME: replace this with operator subclasses
    //because this is so unbeliveably ugly and stupid
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
    if(operator_type == "<") {
      return CompilerContext::Builder->CreateICmpSLT(L, R, "lesstmp");
    }
    if(operator_type == ">") {
      return CompilerContext::Builder->CreateICmpSGT(L, R, "greatertmp");
    }
    if(operator_type == "<=") {
      return CompilerContext::Builder->CreateICmpSLE(L, R, "lesseqtmp");
    }
    if(operator_type == ">=") {
      return CompilerContext::Builder->CreateICmpSGE(L, R, "greatereqtmp");
    }
    if(operator_type == "==") {
      return CompilerContext::Builder->CreateICmpEQ(L, R, "equaltmp");
    }
    if(operator_type == "!=") {
      return CompilerContext::Builder->CreateICmpNE(L, R, "inequaltmp");
    }
    if(operator_type == "&&") {
      return CompilerContext::Builder->CreateAnd(L, R, "and");
    }
    if(operator_type == "||") {
      return CompilerContext::Builder->CreateOr(L, R, "or");
    }
    throw std::runtime_error(
      "Unimplemented binary operator: \"" + operator_type + "\"");
  }

  //TODO: the BinaryOperator uses deduce_type() to find out 
  // what exactly to do 
  // There will be no implicit conversions
  BinaryOperator::BinaryOperator(
    std::string operator_type,
    std::shared_ptr<ExprNode> left,
    std::shared_ptr<ExprNode> right)
  : operator_type(operator_type), left(left), right(right) {}
  // TODO: This needs complex logic for result type deduction
  //once we want to use more than just integers.
  std::string BinaryOperator::get_name() {
    return "Binary Operator";
  }
  std::vector<std::shared_ptr<Node>> BinaryOperator::get_children() {
    return {left, right};
  }

  IntegerConstant::IntegerConstant(unsigned precision, std::string data) 
  : precision(precision), data(data) { final = true; }

  llvm::Value* IntegerConstant::eval() {
    //TODO: Precision can't be zero
    return llvm::ConstantInt::get(
      *CompilerContext::TheContext,
      llvm::APInt(precision, data, 10));
  }

  std::string IntegerConstant::get_name() {
    return "Integer Constant";
  }

  VariableIdentifier::VariableIdentifier(std::string var_name) 
  : var_name(var_name) { final = true; }
  llvm::Value* VariableIdentifier::eval() {
    if(!CompilerContext::NamedValues->has_val(var_name)) {
      //TODO: Some kind of exception?
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
    return {args.begin(), args.end()};
  }
  std::vector<std::shared_ptr<ExprNode>> FunctionCallArgs::get_args() {
    return args;
  }

  FunctionCallArglist::FunctionCallArglist(
    std::shared_ptr<FunctionCallArgs> args) 
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

  FunctionCallExpr::FunctionCallExpr(
    std::string function_name,
    std::shared_ptr<FunctionCallArglist> args)
  : function_name(function_name), args(args) {}
  llvm::Value* FunctionCallExpr::eval() {
    if(!CompilerContext::Functions->contains(function_name))
      throw std::runtime_error(
        "Unregisterd function \"" + function_name + "\"");
    llvm::Function* to_call = CompilerContext::Functions->at(function_name);
    std::vector<llvm::Value*> call_args;
    std::ranges::transform(
      args->get_args(),
      std::back_inserter(call_args),
      &ExprNode::eval);
    return CompilerContext::Builder->CreateCall(to_call, call_args);
  }

  std::string FunctionCallExpr::get_name() {
    return "Function Call Expression";
  }
  std::vector<std::shared_ptr<Node>> FunctionCallExpr::get_children() {
    return {args};
  }
}
