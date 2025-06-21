#ifndef AST_VISITORS_TREE_VISITOR_HPP
#define AST_VISITORS_TREE_VISITOR_HPP

#include "ast.hpp"
#include "ast/expression_nodes.hpp"

namespace ast {
  class TreeVisitor {
  private:
  public:
    void visit_node(Node& node);
    void visit_node(Node* node);
    void visit_node(std::shared_ptr<Node> node);
    virtual void default_visit(Node& node) = 0;
    virtual void visit_program_node(ProgramNode& node) = 0;
    virtual void visit_function_declaration_node(FunctionDeclaration& node) = 0;
    virtual void visit_function_node(FunctionGlobject& node) = 0;

    virtual void visit_declaration_node(DeclarationNode& node) = 0;

    virtual void visit_declaration_statement(DeclarationStatement& node) = 0;
    virtual void visit_declaration_assignment_statement(
      DeclarationAssignmentStatement& statement) = 0;
    virtual void visit_block_node(Block& node) = 0;
    virtual void visit_if_statement(IfStatement& node) = 0;
    virtual void visit_return_statement(ReturnStatement& node) = 0;
    virtual void visit_expression_statement(ExpressionStatement& node) = 0;

    virtual void visit_iverson_node(IversonExpr& node) = 0;
    virtual void visit_unary_operator_node(UnaryOperator& node) = 0;
    virtual void visit_binary_operator_node(BinaryOperator& node) = 0;
    virtual void visit_integer_constant(IntegerConstant& node) = 0;
    virtual void visit_variable_identifier(VariableIdentifier& node) = 0;
    virtual void visit_function_call(FunctionCallExpr& node) = 0;
  };
}

#endif
