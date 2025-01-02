#pragma once
#include "node.hpp"
#include "statement_nodes.hpp"

namespace ast {

  // Maybe should be an abstract class
  class GlobjectNode : public Node {
  private:
  protected:
    GlobjectNode() = default;
  public:
    llvm::Value* codegen() override;
    std::string get_name() override;
  };

  class ProgramNode : public Node {
  private:
    std::vector<std::shared_ptr<GlobjectNode>> globjects;
  public:
    ProgramNode();

    llvm::Value* codegen() override;
    std::string get_name() override;
    void add_obj(std::shared_ptr<GlobjectNode> globject);

    std::vector<std::shared_ptr<Node>> get_children() override;
  };

  class FunctionGlobject : public GlobjectNode {
  private:
    std::shared_ptr<FunctionDeclaration> decl;
    std::shared_ptr<Block> body;
  protected:
  public:
    FunctionGlobject(std::shared_ptr<FunctionDeclaration> decl, std::shared_ptr<Block> body);
  };
}
