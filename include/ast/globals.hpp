#include <memory>
#include <map>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"

namespace CompilerContext {
  extern std::unique_ptr<llvm::LLVMContext> TheContext;
  extern std::unique_ptr<llvm::Module> TheModule;
  extern std::unique_ptr<llvm::IRBuilder<>> Builder;
  extern std::unique_ptr<std::map<std::string, llvm::Value*>> NamedValues;
}
