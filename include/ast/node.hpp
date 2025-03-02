#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include <llvm/IR/Value.h>

namespace ast {
  // Abstract class
  class Node {
  private:
  protected:
    Node() = default;
    virtual ~Node() = default;
  public:
    bool final = false;
    virtual std::string get_name();
    virtual std::vector<std::shared_ptr<Node>> get_children();
  };
}

#endif
