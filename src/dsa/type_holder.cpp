#include <stdexcept>

#include "dsa/type_holder.hpp"

namespace DSA {

  //TODO: this does not in theory check for conflicts
  //It's not good but since the constructor is only used for primitive types
  //It's not the end of the world for now. Should be changed later though
  TypeHolder::TypeHolder(std::vector<std::pair<std::string, llvm::Type*>> data) 
  : types(data.begin(), data.end()) {}

  bool TypeHolder::contains(std::string type_name) {
    return types.contains(type_name);
  }

  bool TypeHolder::add_type(std::string type_name, llvm::Type* type) {
    if(contains(type_name)) {
      return false;
    }
    types[type_name] = type;
    return true;
  }

  std::optional<llvm::Type*> TypeHolder::get_type(std::string type_name) {
    if(!contains(type_name)) {
      return std::nullopt;
    }
    return types[type_name];
  }

}
