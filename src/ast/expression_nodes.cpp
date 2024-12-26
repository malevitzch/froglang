#include "ast/expression_nodes.hpp"

namespace ast {
  
  ExprNode::ExprNode(std::string type) : type(type) {}
  
  std::string ExprNode::get_name() {
    return "Expression Node";
  }

}
