#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include <llvm/IR/Value.h>

namespace ast {
  class TreeVisitor;

  // Abstract class
  class Node {
  private:
  protected:
    Node() = default;
    virtual ~Node() = default;
  public:
    virtual void accept_visitor(TreeVisitor& visitor);
    //TODO: does this even make sense? 
    //Doesn't need to be a thing in general, it was a bandaid solution
    //to printing, becomes completely obsolete with visitor
    //also public attribute very bad
    bool final = false;
    // Returns the node name, this is purely cosmetic
    virtual std::string get_name();
    virtual std::vector<std::shared_ptr<Node>> get_children();
    //TODO: add a default print function that relies on the
    //get_children() function
  };
}

#endif
