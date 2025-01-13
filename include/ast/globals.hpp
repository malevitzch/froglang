#include <memory>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"

#include "dsa/named_values.hpp"

namespace CompilerContext {
  extern std::unique_ptr<llvm::LLVMContext> TheContext;
  extern std::unique_ptr<llvm::Module> TheModule;
  extern std::unique_ptr<llvm::IRBuilder<>> Builder;
  extern std::unique_ptr<DSA::ValueHolder> NamedValues;
}
