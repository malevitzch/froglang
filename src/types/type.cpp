#include "types/type.hpp"

namespace frtype {
  Type::Type(std::string name, llvm::Type* underlying_type)
    : name(name), underlying_type(underlying_type) {}
  std::string Type::get_name() const {
    return name;
  }
  llvm::Type* Type::get_underlying_type() const {
    return underlying_type;
  }
}
