#include "ast/visitors/tree_printer.hpp"
#include "ast/expression_nodes.hpp"

namespace ast {

  std::string TreePrinter::indentation() {
    return std::string(indent_depth, '\t');
  }
  void TreePrinter::indent() {
    if(indents_to_skip == 0) {
      indent_depth++;
      indents.push(true);
    }
    else {
      // We put a marker on the stack so that when de-indenting
      // we will know where to skip the unindent
      indents.push(false);
      indents_to_skip--;
    }
  }

  //TODO: idk this might crash but it's completely on the programmer
  void TreePrinter::unindent() {
    // Only decrease the indent depth if there is a real indent on the stack
    if(indents.top()) {
      indent_depth--;
    }
    indents.pop();
  }
  void TreePrinter::skip_indent() {
    indents_to_skip++;
  }
  void TreePrinter::write(std::initializer_list<std::string> text) {
    for(std::string s : text) {
      *output_stream << s;
    }
  }
  void TreePrinter::write(std::string text) {
    write({text});
  }
  void TreePrinter::line(std::initializer_list<std::string> text) {
    *output_stream << indentation();
    write(text);
    *output_stream << "\n";
  }
  void TreePrinter::line(std::string text) {
    line({text});
  }
  TreePrinter::TreePrinter(std::ostream* output_stream) : output_stream(output_stream) {}
  void TreePrinter::default_visit(Node& node) {
    line({node.get_name(), "(Unimplemented)"});
  }
  void TreePrinter::visit_program_node(ProgramNode& node) {
    line("Program Node");
    indent();
    for(std::shared_ptr<Node> child : node.get_children()) {
      visit_node(child);
    }
    unindent();
  }
  void TreePrinter::visit_function_declaration_node(FunctionDeclaration& node) {
    line({"Declaration of function \"", node.get_function_name(), "\""});
    indent();
    //FIXME: there is no type wrapper yet unfortunately so it's just raw LLVM types
    line({"Return type: ", "UNIMPLEMENTED"});

    auto arguments = node.get_args();
    if(arguments.empty()) {
      line("No arguments");
      unindent();
      return;
    }

    line("Arguments:");
    indent();
    for(auto& arg : node.get_args()) {
      visit_node(arg);
    }
    unindent();

    unindent();
  }
  void TreePrinter::visit_function_node(FunctionGlobject& node) {
    line({"Function ",  "\"", node.get_decl()->get_function_name(), "\""});
    indent();
    visit_node(node.get_decl());
    visit_node(node.get_body());
    unindent();
  }

  void TreePrinter::visit_declaration_node(DeclarationNode& node) {
    line({node.get_varname(), ": ", "UNIMPLEMENTED"});
  }

  void TreePrinter::visit_declaration_statement(DeclarationStatement& node) {
    line({"Declaration of variable: ", node.get_varname()});
  }
  void TreePrinter::visit_declaration_assignment_statement(
    DeclarationAssignmentStatement& node) {
    line({"Declaration of variable: ", node.get_varname()});
    indent();
    line("Initial value:");
    unindent();
  }
  void TreePrinter::visit_block_node(Block& node) {
    auto statements = node.get_statements()->get_children();
    if(statements.empty()) {
      line("Empty block");
      return;
    }
    line("Block");
    indent();
    for(auto statement : statements) {
      visit_node(statement);
    }
    unindent();
  }
  void TreePrinter::visit_if_statement(IfStatement& node) {
    line("If Statement");
    indent();

    line("Condition:");
    indent();
    visit_node(node.get_condition());
    unindent();

    line("If-Body:");
    indent();
    visit_node(node.get_if_body());
    unindent();

    if(!node.get_else_body()) {
      unindent();
      return;
    }

    line("Else-Body:");
    indent();
    visit_node(node.get_else_body());
    unindent();

    unindent();
  }
  void TreePrinter::visit_return_statement(ReturnStatement& node) {
    line("Return");
    if(node.is_void()) {
      return;
    }
    indent();
    visit_node(node.get_return_val());
    unindent();
  }
  void TreePrinter::visit_expression_statement(ExpressionStatement& node) {
    line("Expression statement:");
    indent();
    visit_node(node.get_expr());
    unindent();
  }

  void TreePrinter::visit_iverson_node(IversonExpr& node) {
    line({"Iverson evaluation:"});
    indent();
    visit_node(node.get_expr());
    unindent();
  }
  void TreePrinter::visit_unary_operator_node(UnaryOperator& node) {
    line({"Operator ", node.get_operator_type()});
    indent();
    visit_node(node.get_operand());
    unindent();
  }
  void TreePrinter::visit_binary_operator_node(BinaryOperator& node) {
    line({"Operator ", node.get_operator_type()});
    indent();
    line("LHS:");
    indent();
    visit_node(node.get_LHS());
    unindent();
    line("RHS:");
    indent();
    visit_node(node.get_RHS());
    unindent();
    unindent();
  }
  void TreePrinter::visit_integer_constant(IntegerConstant& node) {
    line({"Integer constant: ", node.get_data()});
  }
  void TreePrinter::visit_variable_identifier(VariableIdentifier& node) {
    line({"Variable: ", node.get_variable_name()});
  }
  void TreePrinter::visit_function_call(FunctionCallExpr& node) {
    line({"Call of function \"", node.get_function_name(), "\""});
    indent();
    line({"Arguments:"});
    indent();
    for(auto& arg : node.get_children()) {
      visit_node(arg);
    }
    unindent();
    unindent();
  }
}
