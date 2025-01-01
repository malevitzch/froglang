#include <memory>
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"

namespace CompilerContext {
  std::unique_ptr<llvm::LLVMContext> TheContext;
  std::unique_ptr<llvm::IRBuilder<>> Builder;
  std::unique_ptr<llvm::Module> TheModule;
}
