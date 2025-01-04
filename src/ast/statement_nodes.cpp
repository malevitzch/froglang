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
  std::vector<std::shared_ptr<Node>> Statements::get_children() {
    return statements;
  }

  Block::Block(std::shared_ptr<Statements> statements) 
  : statements(statements) {}
  void Block::codegen() {
  }
  std::string Block::get_name() {
    return "Block";
  }
  std::vector<std::shared_ptr<Node>> Block::get_children() {
    return {statements};
  }

  DeclarationNode::DeclarationNode(std::string var_type, std::string var_name) 
  : var_type(var_type), var_name(var_name) {final = true;}
  void DeclarationNode::codegen() {

  }
  std::string DeclarationNode::get_name() {
    return "Declaration Node";
  }
  std::vector<std::shared_ptr<Node>> DeclarationNode::get_children() {
    return {};
  }
  
  ExpressionStatement::ExpressionStatement(std::shared_ptr<ExprNode> expr) 
  : expr(expr) {}
  void ExpressionStatement::codegen() {
  }
  std::string ExpressionStatement::get_name() {
    return "Expression Statement";
  }
  std::vector<std::shared_ptr<Node>> ExpressionStatement::get_children() {
    return {expr};
  }

  DeclarationStatement::DeclarationStatement(std::shared_ptr<DeclarationNode> decl)
  : decl(decl) {}
  void DeclarationStatement::codegen() {
  }
  std::string DeclarationStatement::get_name() {
    return "Declaration Statement";
  }
  std::vector<std::shared_ptr<Node>> DeclarationStatement::get_children() {
    return {decl};
  }

  DeclarationAssignmentStatement::DeclarationAssignmentStatement(std::shared_ptr<DeclarationNode> decl, std::shared_ptr<ExprNode> expr)
  : decl(decl), expr(expr) {}
  void DeclarationAssignmentStatement::codegen() {
  }
  std::string DeclarationAssignmentStatement::get_name() {
    return "Declaration Assignment Node";
  }
  std::vector<std::shared_ptr<Node>> DeclarationAssignmentStatement::get_children() {
    return {decl, expr};
  }
}
