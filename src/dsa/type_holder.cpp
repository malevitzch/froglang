#include <stdexcept>

#include "dsa/type_holder.hpp"

namespace DSA {

  TypeHolder::TypeHolder(std::vector<std::pair<std::string, llvm::Type*>> data) 
  : types(data.begin(), data.end()) {}

  bool TypeHolder::contains(std::string type_name) {
    return types.contains(type_name);
  }

  void TypeHolder::add_type(std::string type_name, llvm::Type* type) {
    if(contains(type_name)) {
      throw std::runtime_error("The type \"" + type_name + "\" is already registered");
    }
    types[type_name] = type;
  }

  llvm::Type* TypeHolder::get_type(std::string type_name) {
    if(!contains(type_name)) {
      throw std::runtime_error("Unknown type: \"" + type_name + "\"");
    }
    return types.at(type_name);
  }

}
