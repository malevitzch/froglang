#pragma once

#include <vector>
#include <unordered_map>
#include <llvm/IR/Value.h>

namespace DSA {
  class ValueHolder {
  private:
    std::unordered_map<std::string, std::vector<llvm::Value*>> values;
  public:
    bool has_val(std::string name);
    llvm::Value* get_val(std::string name);
    void add_val(std::string name, llvm::Value* val);
    void remove_val(std::string name);
  };
}
