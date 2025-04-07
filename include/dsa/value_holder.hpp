#ifndef DSA_VALUE_HOLDER_HPP
#define DSA_VALUE_HOLDER_HPP

#include <vector>
#include <unordered_map>
#include <llvm/IR/Value.h>

namespace DSA {
  class ValueHolder {
  private:
    std::unordered_map<std::string, std::vector<llvm::Value*>> values;
  public:
    ValueHolder() = default;
    bool has_val(std::string name) const;
    llvm::Value* get_val(std::string name);
    void add_val(std::string name);
    void add_val(std::string name, llvm::Value* val);
    void set_val(std::string name, llvm::Value* val);
    void remove_val(std::string name);
  };
}

#endif
