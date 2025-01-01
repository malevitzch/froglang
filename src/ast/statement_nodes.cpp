#include "ast/statement_nodes.hpp"

namespace ast {
  
  std::string StatementNode::get_name() {
    return "Statement Node";
  }

  std::string Statements::get_name() {
    return "Statements Node";
  }
  llvm::Value* Statements::codegen() {
  }
  void Statements::add_statement(std::shared_ptr<StatementNode> statement) {
    statements.push_back(statement);
  }
  std::vector<std::shared_ptr<Node>> Statements::get_children() {
    return statements;
  }
}
