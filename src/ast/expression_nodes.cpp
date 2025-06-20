#include "ast/expression_nodes.hpp"
#include "ast/globals.hpp"
#include "ast/binary_operators.hpp"
#include "ast/unary_operators.hpp"

#include "ast/visitors/tree_visitor.hpp"

#include <cstdlib>

#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"

namespace ast {

  std::string ExprNode::get_name() {
    return "Expression Node";
  }

  IversonExpr::IversonExpr(std::shared_ptr<ExprNode> expr) 
  : expr(expr) {}

  void IversonExpr::accept_visitor(TreeVisitor& visitor) {
    visitor.visit_iverson_node(*this);
  }

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

  std::shared_ptr<ExprNode> IversonExpr::get_expr() {
    return expr;
  }

  UnaryOperator::UnaryOperator(
    std::string operator_type,
    std::shared_ptr<ExprNode> operand) 
    : operator_type(operator_type), operand(operand) {}

  std::shared_ptr<UnaryOperator> UnaryOperator::create(
    std::string operator_type, 
    std::shared_ptr<ExprNode> operand) {
    if(operator_type == "-") {
      return std::shared_ptr<UnaryMinusOperator>(
        new UnaryMinusOperator(operand));
    }
    if(operator_type == "~") {
      return std::shared_ptr<NegationOperator>(
        new NegationOperator(operand));
    }
    throw std::runtime_error(
      "Unimplemented unary operator: \"" + operator_type + "\"");
  }
  void UnaryOperator::accept_visitor(TreeVisitor& visitor) {
    visitor.visit_unary_operator_node(*this);
  }
  std::string UnaryOperator::get_name() {
    return "Unary Operator";
  }
  std::vector<std::shared_ptr<Node>> UnaryOperator::get_children() {
    return {operand};
  }

  std::shared_ptr<ExprNode> UnaryOperator::get_operand() {
    return operand;
  }

  std::string UnaryOperator::get_operator_type() {
    return operator_type;
  }

  std::shared_ptr<BinaryOperator> BinaryOperator::create(
    std::string operator_type,
    std::shared_ptr<ExprNode> left,
    std::shared_ptr<ExprNode> right) {
    if(operator_type == "+") {
      return std::shared_ptr<BinaryOperator>(
        new AdditionOperator(left, right));
    }
    if(operator_type == "-") {
      return std::shared_ptr<BinaryOperator>(
        new SubtractionOperator(left, right));
    }
    if(operator_type == "*") {
      return std::shared_ptr<BinaryOperator>(
        new MultiplicationOperator(left, right));
    }
    if(operator_type == "/" ) {
      return std::shared_ptr<BinaryOperator>(
        new DivisionOperator(left, right));
    }
    if(operator_type == "%") {
      return std::shared_ptr<BinaryOperator>(
        new ModuloOperator(left, right));
    }
    if(operator_type == "<") {
      return std::shared_ptr<BinaryOperator>(
        new LesserComparisonOperator(left, right));
    }
    if(operator_type == ">") {
      return std::shared_ptr<BinaryOperator>(
        new GreaterComparisonOperator(left, right));
    }
    if(operator_type == "<=") {
      return std::shared_ptr<BinaryOperator>(
        new LesserEQComparisonOperator(left, right));
    }
    if(operator_type == ">=") {
      return std::shared_ptr<BinaryOperator>(
        new GreaterEQComparisonOperator(left, right));
    }
    if(operator_type == "==") {
      return std::shared_ptr<BinaryOperator>(
        new EqualityComparisonOperator(left, right));
    }
    if(operator_type == "!=") {
      return std::shared_ptr<BinaryOperator>(
        new InequalityComparisonOperator(left, right));
    }
    if(operator_type == "&&") {
      return std::shared_ptr<BinaryOperator>(
        new LogicalAndOperator(left, right));
    }
    if(operator_type == "||") {
      return std::shared_ptr<BinaryOperator>(
        new LogicalOrOperator(left, right));
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

  void BinaryOperator::accept_visitor(TreeVisitor& visitor) {
    visitor.visit_binary_operator_node(*this);
  }
  std::string BinaryOperator::get_name() {
    return "Binary Operator";
  }
  std::string BinaryOperator::get_operator_type() {
    return operator_type;
  }
  std::vector<std::shared_ptr<Node>> BinaryOperator::get_children() {
    return {left, right};
  }

  IntegerConstant::IntegerConstant(unsigned precision, std::string data) 
  : precision(precision), data(data) { final = true; }

  void IntegerConstant::accept_visitor(TreeVisitor& visitor) {
    visitor.visit_integer_constant(*this);
  }

  llvm::Value* IntegerConstant::eval() {
    //TODO: Precision can't be zero
    return llvm::ConstantInt::get(
      *CompilerContext::TheContext,
      llvm::APInt(precision, data, 10));
  }

  std::string IntegerConstant::get_name() {
    return "Integer Constant";
  }

  std::string IntegerConstant::get_data() {
    return data;
  }

  VariableIdentifier::VariableIdentifier(std::string var_name) 
  : var_name(var_name) { final = true; }

  void VariableIdentifier::accept_visitor(TreeVisitor& visitor) {
    visitor.visit_variable_identifier(*this);
  }

  llvm::Value* VariableIdentifier::eval() {
    if(!CompilerContext::NamedValues->has_val(var_name)) {
      //TODO: Some kind of exception?
    }
    return CompilerContext::NamedValues->get_val(var_name);
  }

  std::string VariableIdentifier::get_name() {
    return "Variable Identifier";
  }

  std::string VariableIdentifier::get_variable_name() {
    return var_name;
  }

  FunctionCallArglist::FunctionCallArglist(
    std::vector<std::shared_ptr<ExprNode>> args) 
  : args(args) {}
  std::string FunctionCallArglist::get_name() {
    return "Function Call Arglist";
  }
  std::vector<std::shared_ptr<Node>> FunctionCallArglist::get_children() {
    std::vector<std::shared_ptr<Node>> children;
    std::ranges::copy(args, std::back_inserter(children));
    return children;
  }
  std::vector<std::shared_ptr<ExprNode>> FunctionCallArglist::get_args() {
    return args;
  }
  void FunctionCallArglist::add_arg(std::shared_ptr<ExprNode> arg) {
    args.push_back(arg);
  }

  FunctionCallExpr::FunctionCallExpr(
    std::string function_name,
    std::shared_ptr<FunctionCallArglist> args)
  : function_name(function_name), args(args) {}
  void FunctionCallExpr::accept_visitor(TreeVisitor& visitor) {
    visitor.visit_function_call(*this);
  }
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
    return args->get_children();
  }

  std::string FunctionCallExpr::get_function_name() {
    return function_name;
  }
}
