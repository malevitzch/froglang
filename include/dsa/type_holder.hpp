#pragma once

#include <map>
#include <optional>

#include <llvm/IR/Type.h>

namespace DSA {
  class TypeHolder {
  private:
    std::map<std::string, llvm::Type*> types;
  public:
    TypeHolder() = default;
    TypeHolder(std::vector<std::pair<std::string, llvm::Type*>> data);
    // Returns whether or not a type of given name exists
    bool contains(std::string type_name);
    // Adds a type and binds it to its reference from llvm
    bool add_type(std::string type_name, llvm::Type* type);
    // Returns the type of the given name if it exists, otherwise returns nullopts
    std::optional<llvm::Type*> get_type(std::string type_name);
  };
}
