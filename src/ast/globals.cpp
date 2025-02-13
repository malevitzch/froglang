#include <memory>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"

#include "ast/globals.hpp"

namespace CompilerContext {
  std::unique_ptr<llvm::LLVMContext> TheContext;
  std::unique_ptr<llvm::Module> TheModule;
  std::unique_ptr<llvm::IRBuilder<>> Builder;
  std::unique_ptr<DSA::ValueHolder> NamedValues;
  std::unique_ptr<std::map<std::string, llvm::Function*>> Functions;
  std::unique_ptr<DSA::TypeHolder> Types;

  const std::vector<std::pair<std::string, llvm::Type*>> get_primitive_types() {
    const std::vector<std::pair<std::string, llvm::Type*>> types = {
      {"i32", llvm::Type::getInt32Ty(*TheContext)},
      {"i1", llvm::Type::getInt1Ty(*TheContext)},
    };
    return types;
  }

  void reset_context() {

    TheModule.reset();
    Builder.reset();
    NamedValues.reset();
    Functions.reset();
    Types.reset();
    TheContext.reset();

    TheContext = std::make_unique<llvm::LLVMContext>();
    TheModule = std::make_unique<llvm::Module>("ProgramModule", *CompilerContext::TheContext);
    Builder = std::make_unique<llvm::IRBuilder<>>(*CompilerContext::TheContext);
    NamedValues = std::make_unique<DSA::ValueHolder>();
    Functions = std::make_unique<std::map<std::string, llvm::Function*>>();
    Types = std::make_unique<DSA::TypeHolder>(get_primitive_types());
  }
}
