#pragma once
#include <llvm/IR/Value.h>

#include "ast/globals.hpp"

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
