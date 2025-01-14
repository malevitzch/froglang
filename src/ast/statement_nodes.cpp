#include "ast/statement_nodes.hpp"

namespace ast {

  std::string StatementNode::get_name() {
    return "Statement Node";
  }

  void StatementNode::codegen() {
  }

  const std::vector<std::shared_ptr<StatementNode>>& Statements::get() {
    return statements;
  }
  std::string Statements::get_name() {
    return "Statements Node";
  }
  void Statements::add_statement(std::shared_ptr<StatementNode> statement) {
    statements.push_back(statement);
  }
  std::vector<std::shared_ptr<Node>> Statements::get_children() {
    std::vector<std::shared_ptr<Node>> children;
    for(std::shared_ptr<StatementNode> child : statements) children.push_back(child);
    return children;
  }

  Block::Block(std::shared_ptr<Statements> statements) 
  : statements(statements) {}
  void Block::codegen() {
    std::vector<std::string> block_named_values;
    for(std::shared_ptr<StatementNode> statement : statements->get()) {
      if(statement->get_name() == "Declaration Satatement") {
        auto decl_statement = dynamic_pointer_cast<DeclarationStatement>(statement);
        block_named_values.push_back(decl_statement->get_varname());
      }
      else if(statement->get_name() == "Declaration Assignment Statement") {
        auto decl_assignment_statement = dynamic_pointer_cast<DeclarationStatement>(statement);
        block_named_values.push_back(decl_assignment_statement->get_varname());
      }
      statement->codegen();
    }
    for(std::string value_name : block_named_values) {
      CompilerContext::NamedValues->remove_val(value_name);
    }
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
    //TODO: implement
  }
  std::string DeclarationNode::get_name() {
    return "Declaration Node";
  }
  std::vector<std::shared_ptr<Node>> DeclarationNode::get_children() {
    return {};
  }
  std::string DeclarationNode::get_varname() {
    return var_name;
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
  std::string DeclarationStatement::get_varname() {
    return decl->get_varname();
  }

  DeclarationAssignmentStatement::DeclarationAssignmentStatement(std::shared_ptr<DeclarationNode> decl, std::shared_ptr<ExprNode> expr)
  : decl(decl), expr(expr) {}
  void DeclarationAssignmentStatement::codegen() {
  }
  std::string DeclarationAssignmentStatement::get_name() {
    return "Declaration Assignment Statement";
  }
  std::vector<std::shared_ptr<Node>> DeclarationAssignmentStatement::get_children() {
    return {decl, expr};
  }
  std::string DeclarationAssignmentStatement::get_varname() {
    return decl->get_varname();
  }

  ReturnStatement::ReturnStatement()
  : type("void"), val(nullptr) {}
  ReturnStatement::ReturnStatement(std::string type, std::shared_ptr<ExprNode> val)
  : type(type), val(val) {}
  void ReturnStatement::codegen() {
  }
  std::string ReturnStatement::get_name() {
    return "Return Statement";
  }
  std::vector<std::shared_ptr<Node>> ReturnStatement::get_children() {
    if(type == "void") return {};
    return {val};
  }
}
