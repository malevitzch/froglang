#include <vector>
#include <unordered_map>

#include "llvm/IR/Instructions.h"

namespace DSA {
  class VariableHolder {
    private:
      std::unordered_map<std::string, std::vector<llvm::AllocaInst*>> variables;
    public:
      VariableHolder() = default;
      bool has_var(std::string name);
      llvm::Value* get_val(std::string name);
      void add_var(std::string name);
      void add_var(std::string name, llvm::AllocaInst* val);
      void set_var(std::string name, llvm::AllocaInst* val);
      void remove_var(std::string name);
  };
}
