#ifndef AST_BINARY_OPERATORS_HPP
#define AST_BINARY_OPERATORS_HPP

#include "ast/expression_nodes.hpp"

namespace ast {

  class AdditionOperator : public BinaryOperator {
  private:
  protected:
    AdditionOperator(
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class SubtractionOperator : public BinaryOperator {
  private:
  protected:
    SubtractionOperator(
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class MultiplicationOperator : public BinaryOperator {
  private:
  protected:
    MultiplicationOperator(
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class DivisionOperator : public BinaryOperator {
  private:
  protected:
    DivisionOperator(
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class ModuloOperator : public BinaryOperator {
  private:
  protected:
    ModuloOperator(
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class LesserComparisonOperator : public BinaryOperator {
  private:
  protected:
    LesserComparisonOperator(
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class GreaterComparisonOperator : public BinaryOperator {
  private:
  protected:
    GreaterComparisonOperator(
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class LesserEQComparisonOperator : public BinaryOperator {
  private:
  protected:
    LesserEQComparisonOperator(
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class GreaterEQComparisonOperator : public BinaryOperator {
  private:
  protected:
    GreaterEQComparisonOperator(
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class EqualityComparisonOperator : public BinaryOperator {
  private:
  protected:
    EqualityComparisonOperator(
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class InequalityComparisonOperator : public BinaryOperator {
  private:
  protected:
    InequalityComparisonOperator(
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class LogicalAndOperator : public BinaryOperator {
  private:
  protected:
    LogicalAndOperator(
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class LogicalOrOperator : public BinaryOperator {
  private:
  protected:
    LogicalOrOperator(
      std::shared_ptr<ExprNode> left,
      std::shared_ptr<ExprNode> right);
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

}

#endif
