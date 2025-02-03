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
    bool contains(std::string type_name);
    bool add_type(std::string type_name, llvm::Type* type);
    std::optional<llvm::Type*> get_type(std::string type_name);
  };
}
