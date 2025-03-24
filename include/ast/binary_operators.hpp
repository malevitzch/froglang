#ifndef AST_BINARY_OPERATORS_HPP
#define AST_BINARY_OPERATORS_HPP

#include "ast/expression_nodes.hpp"

namespace ast {

  class AdditionOperator : BinaryOperator {
  private:
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class SubtractionOperator : BinaryOperator {
  private:
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class MultiplicationOperator : BinaryOperator {
  private:
  public:
    virtual llvm::Value* eval() override;

  friend class BinaryOperator;
  };

  class DivisionOperator : BinaryOperator {
  private:
  public:
    virtual llvm::Value* eval() override;

    friend class BinaryOperator;
  };

  class ModuloOperator : BinaryOperator {
  private:
  public:
    virtual llvm::Value* eval() override;

    friend class BinaryOperator;
  };

  class LesserComparisonOperator : BinaryOperator {
  private:
  public:
    virtual llvm::Value* eval() override;

    friend class BinaryOperator;
  };

  class GreaterComparisonOperator : BinaryOperator {
  private:
  public:
    virtual llvm::Value* eval() override;

    friend class BinaryOperator;
  };

  class LesserEQComparisonOperator : BinaryOperator {
  private:
  public:
    virtual llvm::Value* eval() override;

    friend class BinaryOperator;
  };

  class GreaterEQComparisonOperator : BinaryOperator {
  private:
  public:
    virtual llvm::Value* eval() override;

    friend class BinaryOperator;
  };

  class EqualityComparisonOperator : BinaryOperator {
  private:
  public:
    virtual llvm::Value* eval() override;

    friend class BinaryOperator;
  };

  class InequalityComparisonOperator : BinaryOperator {
  private:
  public:
    virtual llvm::Value* eval() override;

    friend class BinaryOperator;
  };

  class LogicalAndOperator : BinaryOperator {
  private:
  public:
    virtual llvm::Value* eval() override;

    friend class BinaryOperator;
  };

  class LogicalOrOperator : BinaryOperator {
  private:
  public:
    virtual llvm::Value* eval() override;

    friend class BinaryOperator;
  };

}

#endif
