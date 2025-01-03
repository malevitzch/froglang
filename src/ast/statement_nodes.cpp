#include "ast/statement_nodes.hpp"

namespace ast {
  
  std::string StatementNode::get_name() {
    return "Statement Node";
  }

  void StatementNode::codegen() {
  }

  std::string Statements::get_name() {
    return "Statements Node";
  }

  void Statements::codegen() {
  }

  void Statements::add_statement(std::shared_ptr<StatementNode> statement) {
    statements.push_back(statement);
  }

  void Block::codegen() {
  }
  
  std::vector<std::shared_ptr<Node>> Statements::get_children() {
    return statements;
  }
  DeclarationNode::DeclarationNode(std::string var_type, std::string var_name) 
  : var_type(var_type), var_name(var_name) {}
  std::string DeclarationNode::get_name() {
    return "Declaration Node";
  }
  std::vector<std::shared_ptr<Node>> DeclarationNode::get_children() {
    return {};
  }

  DeclarationStatement::DeclarationStatement(std::shared_ptr<DeclarationNode> decl)
  : decl(decl) {}
  void DeclarationStatement::codegen() {
  }
  
  ExpressionStatement::ExpressionStatement(std::shared_ptr<ExprNode> expr) 
  : expr(expr) {}
  void ExpressionStatement::codegen() {
  }

}
