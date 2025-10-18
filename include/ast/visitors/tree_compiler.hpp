#ifndef AST_VISITORS_TREE_COMPILER_HPP
#define AST_VISITORS_TREE_COMPILER_HPP

#include "ast.hpp"

#include "ast/statement_nodes.hpp"
#include "ast/visitors/tree_visitor.hpp"

namespace ast {

  class TreeCompiler : public TreeVisitor {
  private:
    llvm::LLVMContext TheContext;
    llvm::Module TheModule;
    llvm::IRBuilder<> Builder;
    DSA::ValueHolder NamedValues;
    std::map<std::string, llvm::Function*> Functions;
    DSA::TypeHolder Types;
  public:
    TreeCompiler();
    virtual void default_visit(Node& node) override;
    virtual void visit_program_node(ProgramNode& program_node) override;
    virtual void visit_function_declaration_node(FunctionDeclaration& node) override;
    virtual void visit_function_node(FunctionGlobject& node) override;

    virtual void visit_declaration_node(DeclarationNode& node) override;

    virtual void visit_declaration_statement(DeclarationStatement& node) override;
    virtual void visit_declaration_assignment_statement(
      DeclarationAssignmentStatement& statement) override;
    virtual void visit_block_node(Block& node) override;
    virtual void visit_if_statement(IfStatement& node) override;
    virtual void visit_return_statement(ReturnStatement& node) override;
    virtual void visit_expression_statement(ExpressionStatement& node) override;

    virtual void visit_iverson_node(IversonExpr& node) override;
    virtual void visit_unary_operator_node(UnaryOperator& node) override;
    virtual void visit_binary_operator_node(BinaryOperator& node) override;
    virtual void visit_integer_constant(IntegerConstant& node) override;
    virtual void visit_variable_identifier(VariableIdentifier& node) override;
    virtual void visit_function_call(FunctionCallExpr& node) override;
  };
}

#endif
