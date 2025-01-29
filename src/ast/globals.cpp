#include <memory>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"

#include "ast/globals.hpp"

namespace CompilerContext {
  std::unique_ptr<llvm::LLVMContext> TheContext = std::make_unique<llvm::LLVMContext>();
  std::unique_ptr<llvm::Module> TheModule = std::make_unique<llvm::Module>("MyModule", *CompilerContext::TheContext);
  std::unique_ptr<llvm::IRBuilder<>> Builder = std::make_unique<llvm::IRBuilder<>>(*CompilerContext::TheContext);
  std::unique_ptr<DSA::ValueHolder> NamedValues = std::make_unique<DSA::ValueHolder>();
  std::unique_ptr<std::map<std::string, llvm::Function*>> Functions = std::make_unique<std::map<std::string, llvm::Function*>>();
  std::unique_ptr<DSA::TypeHolder> Types = std::make_unique<DSA::TypeHolder>(std::vector<std::pair<std::string, llvm::Type*>>({
    {"i32", llvm::Type::getInt32Ty(*TheContext)},
    {"bool", llvm::Type::getInt1Ty(*TheContext)},
  }));

  //FIXME: add register_primitive_types or something
  //FIXME: also maybe this should be cleaned up at the end of compilation?
  //FIXME: add init/cleanup to globals
}
