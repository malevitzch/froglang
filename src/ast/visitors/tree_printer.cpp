#include "ast/visitors/tree_printer.hpp"

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
    line("Unknown Node");
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
    //FIXME: maybe no need for friend, maybe just expose the info
    // in the nodes themselves (here it can be done easily)
    line({"Declaration of function \"", node.function_name, "\""});
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
      arg->accept_visitor(*this);
    }
    unindent();

    unindent();
  }
  void TreePrinter::visit_function_node(FunctionGlobject& node) {
    line({"Function ",  "\"", node.decl->get_varname(), "\""});
    indent();
    visit_node(*(node.decl));
    visit_node(*(node.body));
    unindent();
  }
  void TreePrinter::visit_declaration_node(DeclarationNode& node) {
    line({node.get_varname(), ": ", "UNIMPLEMENTED"});
  }
  void TreePrinter::visit_binary_operator_node(BinaryOperator& node) {
    line({node.get_operator_type()});
    indent();
    line("LHS: ");
    visit_node(node.left);
    line("RHS: ");
    visit_node(node.right);
    unindent();
  }
  void TreePrinter::visit_block_node(Block& node) {
    auto statements = node.get_children();
    if(statements.empty()) {
      line("Empty block");
      return;
    }
    line("Block");
    indent();
    for(auto statement : statements) {
      visit_node(*statement);
    }
    unindent();
  }
}
