#pragma once

#include <map>
#include <llvm/IR/Type.h>

namespace DSA {
  class TypeHolder {
  private:
    std::map<std::string, llvm::Type*> types;
  public:
    TypeHolder() = default;
    TypeHolder(std::vector<std::pair<std::string, llvm::Type*>> data);
    bool contains(std::string type_name);
    void add_type(std::string type_name, llvm::Type* type);
    llvm::Type* get_type(std::string type_name);
  };
}
