#ifndef AST_UNARY_OPERATORS_HPP
#define AST_UNARY_OPERATORS_HPP

#include "ast/expression_nodes.hpp"

namespace ast {
  class UnaryMinusOperator : public UnaryOperator {
  
  friend class UnaryOperator;
  };
  class NegationOperator : public UnaryOperator {
  
  friend class UnaryOperator;
  };

}

#endif
