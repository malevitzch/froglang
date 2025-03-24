#ifndef AST_BINARY_OPERATORS_HPP
#define AST_BINARY_OPERATORS_HPP

#include "ast/expression_nodes.hpp"

namespace ast {

  class AdditionOperator : BinaryOperator {
  private:
    AdditionOperator(
      std::string operator_type,
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class SubtractionOperator : BinaryOperator {
  private:
    SubtractionOperator(
      std::string operator_type,
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class MultiplicationOperator : BinaryOperator {
  private:
    MultiplicationOperator(
      std::string operator_type,
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class DivisionOperator : BinaryOperator {
  private:
    DivisionOperator(
      std::string operator_type,
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class ModuloOperator : BinaryOperator {
  private:
    ModuloOperator(
      std::string operator_type,
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class LesserComparisonOperator : BinaryOperator {
  private:
    LesserComparisonOperator(
      std::string operator_type,
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class GreaterComparisonOperator : BinaryOperator {
  private:
    GreaterComparisonOperator(
      std::string operator_type,
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class LesserEQComparisonOperator : BinaryOperator {
  private:
    LesserEQComparisonOperator(
      std::string operator_type,
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class GreaterEQComparisonOperator : BinaryOperator {
  private:
    GreaterEQComparisonOperator(
      std::string operator_type,
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class EqualityComparisonOperator : BinaryOperator {
  private:
    EqualityComparisonOperator(
      std::string operator_type,
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class InequalityComparisonOperator : BinaryOperator {
  private:
    InequalityComparisonOperator(
      std::string operator_type,
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class LogicalAndOperator : BinaryOperator {
  private:
    LogicalAndOperator(
      std::string operator_type,
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class LogicalOrOperator : BinaryOperator {
  private:
    LogicalOrOperator(
      std::string operator_type,
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

}

#endif
