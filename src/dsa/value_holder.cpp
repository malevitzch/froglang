#include "dsa/value_holder.hpp"
#include <stdexcept>

namespace DSA {
  bool ValueHolder::has_val(std::string name) const {
    return values.contains(name);
  }
  llvm::Value* ValueHolder::get_val(std::string name) {
    if(!has_val(name))
      throw std::runtime_error(
      "Trying to access nonexistent element: \"" + name + "\"\n");
    return values[name].back();
  }
  void ValueHolder::add_val(std::string name) {
    values[name].push_back(nullptr);
  }
  void ValueHolder::add_val(std::string name, llvm::Value* val) {
    values[name].push_back(val);
  }
  void ValueHolder::set_val(std::string name, llvm::Value* val) {
    if(!has_val(name))
      throw std::runtime_error(
      "Trying to set the value of a nonexistent element: \"" + name + "\"\n");
  }
  void ValueHolder::remove_val(std::string name) {
    if(!has_val(name))
      throw std::runtime_error(
      "Trying to remove nonexistent element: \"" + name + "\"\n");
    values[name].pop_back();
    if(values[name].empty())
      values.erase(name);
  }
}
