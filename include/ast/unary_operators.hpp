#ifndef AST_UNARY_OPERATORS_HPP
#define AST_UNARY_OPERATORS_HPP

#include "ast/expression_nodes.hpp"

namespace ast {

  class UnaryMinusOperator : public UnaryOperator {
  private:
    UnaryMinusOperator(std::shared_ptr<ExprNode> operand);
  public:
    virtual llvm::Value* eval() override;
    virtual std::string get_name() override;
  friend class UnaryOperator;
  };

  class NegationOperator : public UnaryOperator {
  private:
    NegationOperator(std::shared_ptr<ExprNode> operand);
  public:
    virtual llvm::Value* eval() override;
    virtual std::string get_name() override;
  friend class UnaryOperator;
  };

}

#endif
