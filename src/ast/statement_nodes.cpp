#include "ast/statement_nodes.hpp"
#include "ast/globals.hpp"

#include "ast/visitors/tree_visitor.hpp"

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
    for(std::shared_ptr<StatementNode> child : statements)
      children.push_back(child);
    return {statements.begin(), statements.end()};
  }

  Block::Block(std::shared_ptr<Statements> statements) 
  : statements(statements) {}
  void Block::accept_visitor(TreeVisitor& visitor) {
    visitor.visit_block_node(*this);
  }
  std::optional<std::string> Block::codegen() {
    std::vector<std::string> block_named_values;
    for(std::shared_ptr<StatementNode> statement : statements->get()) {
      auto decl_statement = 
        dynamic_pointer_cast<DeclarationStatement>(statement);
      if(decl_statement) {
        block_named_values.push_back(decl_statement->get_varname());
      }
      statement->codegen();
    }
    //TODO: in the future, we will want to do typechecks
    // this should probably be in reverse order or smth
    for(std::string value_name : block_named_values) {
      CompilerContext::NamedValues->remove_val(value_name);
    }
    return std::nullopt;
  }
  std::string Block::get_name() {
    return "Block";
  }
  std::vector<std::shared_ptr<Node>> Block::get_children() {
    return {statements};
  }
  std::shared_ptr<Statements> Block::get_statements() {
    return statements;
  }


  DeclarationNode::DeclarationNode(llvm::Type* var_type, std::string var_name, bool is_var) 
  : var_type(var_type), var_name(var_name), is_var(is_var) {final = true;}
  void DeclarationNode::accept_visitor(TreeVisitor& visitor) {
    visitor.visit_declaration_node(*this);
  }
  std::optional<std::string> DeclarationNode::codegen() {
    CompilerContext::NamedValues->add_val(var_name);
    return std::nullopt;
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
  void ExpressionStatement::accept_visitor(TreeVisitor& visitor) {
    visitor.visit_expression_statement(*this);
  }
  std::optional<std::string> ExpressionStatement::codegen() {
    expr->eval();
    return std::nullopt;
  }
  std::string ExpressionStatement::get_name() {
    return "Expression Statement";
  }
  std::vector<std::shared_ptr<Node>> ExpressionStatement::get_children() {
    return {expr};
  }
  std::shared_ptr<ExprNode> ExpressionStatement::get_expr() {
    return expr;
  }

  DeclarationStatement::DeclarationStatement(
    std::shared_ptr<DeclarationNode> decl)
  : decl(decl) {}
  void DeclarationStatement::accept_visitor(TreeVisitor& visitor) {
    visitor.visit_declaration_statement(*this);
  }
  std::optional<std::string> DeclarationStatement::codegen() {
    CompilerContext::NamedValues->add_val(get_varname());
    return std::nullopt;
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
  std::shared_ptr<DeclarationNode> DeclarationStatement::get_decl() { return decl; }

  DeclarationAssignmentStatement::DeclarationAssignmentStatement(
    std::shared_ptr<DeclarationNode> decl, std::shared_ptr<ExprNode> expr)
  : DeclarationStatement(decl), expr(expr) {}
  void DeclarationAssignmentStatement::accept_visitor(TreeVisitor& visitor) {
    visitor.visit_declaration_assignment_statement(*this);
  }
  std::optional<std::string> DeclarationAssignmentStatement::codegen() {
    CompilerContext::NamedValues->add_val(decl->get_varname(), expr->eval());
    return std::nullopt;
  }
  std::string DeclarationAssignmentStatement::get_name() {
    return "Declaration Assignment Statement";
  }
  std::vector<std::shared_ptr<Node>> 
  DeclarationAssignmentStatement::get_children() {
    return {decl, expr};
  }
  std::string DeclarationAssignmentStatement::get_varname() {
    return decl->get_varname();
  }

  ReturnStatement::ReturnStatement()
  : val(nullptr) {}
  ReturnStatement::ReturnStatement(std::shared_ptr<ExprNode> val)
  : val(val) {}
  void ReturnStatement::accept_visitor(TreeVisitor& visitor) {
    visitor.visit_return_statement(*this);
  }
  std::optional<std::string> ReturnStatement::codegen() {
    // If there is no value, we create a void ret instruction
    if(!val) {
      CompilerContext::Builder->CreateRetVoid();
    }
    else {
      CompilerContext::Builder->CreateRet(val->eval());
    }
    return std::nullopt;
  }
  std::string ReturnStatement::get_name() {
    return "Return Statement";
  }
  std::vector<std::shared_ptr<Node>> ReturnStatement::get_children() {
    if(val) return {val};
    return {};
  }
  bool ReturnStatement::is_void() {
    return val == nullptr;
  }
  std::shared_ptr<ExprNode> ReturnStatement::get_return_val() {
    if(is_void()) {
      //FIXME: throws
    }
    return val;
  }


  IfStatement::IfStatement(
    std::shared_ptr<ExprNode> condition,
    std::shared_ptr<StatementNode> if_body) 
  : condition(condition), if_body(if_body), else_body(nullptr) {}
  IfStatement::IfStatement(
    std::shared_ptr<ExprNode> condition,
    std::shared_ptr<StatementNode> if_body,
    std::shared_ptr<StatementNode> else_body) 
  : condition(condition), if_body(if_body), else_body(else_body) {}
  void IfStatement::accept_visitor(TreeVisitor& visitor) {
    visitor.visit_if_statement(*this);
  }
  std::optional<std::string> IfStatement::codegen() {
    using CompilerContext::Builder;
    using CompilerContext::TheContext;

    llvm::Function* function = Builder->GetInsertBlock()->getParent();

    llvm::BasicBlock *if_body_block =
      llvm::BasicBlock::Create(*TheContext, "then", function);
    llvm::BasicBlock *merge_block =
      llvm::BasicBlock::Create(*TheContext, "ifcont", function);
    llvm::BasicBlock *else_body_block;

    if(else_body) {
      else_body_block = 
        llvm::BasicBlock::Create(*TheContext, "else", function); 
      llvm::Value* condition_val = condition->eval();
      Builder->CreateCondBr(
        Builder->CreateICmpNE(
          condition_val,
          llvm::Constant::getNullValue(condition_val->getType()),
          "cast"),
        if_body_block,
        else_body_block);
    }
    else {
      llvm::Value* condition_val = condition->eval();
      Builder->CreateCondBr(
        Builder->CreateICmpNE(
          condition_val,
          llvm::Constant::getNullValue(condition_val->getType()),
          "cast"), 
        if_body_block,
        merge_block);
    }

    Builder->SetInsertPoint(if_body_block);
    if_body->codegen();
    Builder->CreateBr(merge_block);

    if(else_body) {
      Builder->SetInsertPoint(else_body_block);
      else_body->codegen();
      Builder->CreateBr(merge_block);
    }
    Builder->SetInsertPoint(merge_block);
    return std::nullopt;
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
  std::shared_ptr<ExprNode> IfStatement::get_condition() {
    return condition;
  }
  std::shared_ptr<StatementNode> IfStatement::get_if_body() {
    return if_body;
  }
  std::shared_ptr<StatementNode> IfStatement::get_else_body() {
    return else_body;
  }

  WhileLoop::WhileLoop(
    std::shared_ptr<ExprNode> condition,
    std::shared_ptr<StatementNode> body)
  : condition(condition), body(body) {}
  std::optional<std::string> WhileLoop::codegen() {
    //TODO: implement
    return std::nullopt;
  }
  std::string WhileLoop::get_name() {
    return "While Loop";
  }
  std::vector<std::shared_ptr<Node>> WhileLoop::get_children() {
    return {condition, body};
  }

}
