#include <memory>
#include <map>
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"

namespace CompilerContext {
  std::unique_ptr<llvm::LLVMContext> TheContext = std::make_unique<llvm::LLVMContext>();
  std::unique_ptr<llvm::Module> TheModule = std::make_unique<llvm::Module>("MyModule", *CompilerContext::TheContext);
  std::unique_ptr<llvm::IRBuilder<>> Builder = std::make_unique<llvm::IRBuilder<>>(*CompilerContext::TheContext);
  std::unique_ptr<std::map<std::string, llvm::Value*>> NamedValues;
}
