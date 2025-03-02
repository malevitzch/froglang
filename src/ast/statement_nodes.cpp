#include "ast/statement_nodes.hpp"
#include "ast/globals.hpp"

#include <llvm/IR/BasicBlock.h>

namespace ast {

  std::string StatementNode::get_name() {
    return "Statement Node";
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
      if(statement->get_name() == "Declaration Statement") {
        auto decl_statement = dynamic_pointer_cast<DeclarationStatement>(statement);
        block_named_values.push_back(decl_statement->get_varname());
      }
      else if(statement->get_name() == "Declaration Assignment Statement") {
        auto decl_assignment_statement = dynamic_pointer_cast<DeclarationAssignmentStatement>(statement);
        block_named_values.push_back(decl_assignment_statement->get_varname());
      }
      statement->codegen();
    }
    //TODO: in the future, when we want to do typechecks, this should probably be in reverse order or smth
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

  DeclarationNode::DeclarationNode(llvm::Type* var_type, std::string var_name) 
  : var_type(var_type), var_name(var_name) {final = true;}
  void DeclarationNode::codegen() {
    CompilerContext::NamedValues->add_val(var_name);
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
  llvm::Type* DeclarationNode::get_var_type() {
    return var_type;
  }

  ExpressionStatement::ExpressionStatement(std::shared_ptr<ExprNode> expr) 
  : expr(expr) {}
  void ExpressionStatement::codegen() {
    expr->eval();
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
    CompilerContext::NamedValues->add_val(get_varname());
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
    CompilerContext::NamedValues->add_val(decl->get_varname(), expr->eval());
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
  : val(nullptr) {}
  ReturnStatement::ReturnStatement(std::shared_ptr<ExprNode> val)
  : val(val) {}
  void ReturnStatement::codegen() {
    // If there is no value, we create a void ret instruction
    if(!val) {
      CompilerContext::Builder->CreateRetVoid();
    }
    else {
      CompilerContext::Builder->CreateRet(val->eval());
    }
  }
  std::string ReturnStatement::get_name() {
    return "Return Statement";
  }
  std::vector<std::shared_ptr<Node>> ReturnStatement::get_children() {
    return {val};
  }
  IfStatement::IfStatement(std::shared_ptr<ExprNode> condition, std::shared_ptr<StatementNode> if_body) 
  : condition(condition), if_body(if_body), else_body(nullptr) {}
  IfStatement::IfStatement(std::shared_ptr<ExprNode> condition, std::shared_ptr<StatementNode> if_body, std::shared_ptr<StatementNode> else_body) 
  : condition(condition), if_body(if_body), else_body(else_body) {}
  void IfStatement::codegen() {
    llvm::Function* function = CompilerContext::Builder->GetInsertBlock()->getParent();

    llvm::BasicBlock *if_body_BB = llvm::BasicBlock::Create(*CompilerContext::TheContext, "then", function);
    llvm::BasicBlock *merge_BB = llvm::BasicBlock::Create(*CompilerContext::TheContext, "ifcont", function);
    llvm::BasicBlock *else_body_BB;

    if(else_body) {
      else_body_BB = llvm::BasicBlock::Create(*CompilerContext::TheContext, "else", function); 
      CompilerContext::Builder->CreateCondBr(condition->eval(), if_body_BB, else_body_BB);
    }
    else {
      CompilerContext::Builder->CreateCondBr(condition->eval(), if_body_BB, merge_BB);
    }

    CompilerContext::Builder->SetInsertPoint(if_body_BB);
    if_body->codegen();
    CompilerContext::Builder->CreateBr(merge_BB);

    if(else_body) {
      CompilerContext::Builder->SetInsertPoint(else_body_BB);
      else_body->codegen();
      CompilerContext::Builder->CreateBr(merge_BB);
    }
    CompilerContext::Builder->SetInsertPoint(merge_BB);
  }
  std::string IfStatement::get_name() {
    return "If Statement";
  }
  std::vector<std::shared_ptr<Node>> IfStatement::get_children() {
    std::vector<std::shared_ptr<Node>> children = {condition, if_body};
    if(else_body) {
      children.push_back(else_body);
    }
    return children;
  }
}
