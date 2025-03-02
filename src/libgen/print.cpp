#include "libgen/print.hpp"
#include "ast/globals.hpp"

namespace libgen {
  void register_print_i32() {
    using namespace CompilerContext;

    llvm::FunctionType* printf_type = llvm::FunctionType::get(Builder->getInt32Ty(), {Builder->getInt8PtrTy()}, true);
    llvm::FunctionCallee printf = TheModule->getOrInsertFunction("printf", printf_type);

    llvm::FunctionType* print_i32_type = llvm::FunctionType::get(Builder->getVoidTy(), {Builder->getInt32Ty()}, false);
    llvm::Function* print_i32 = llvm::Function::Create(print_i32_type, llvm::Function::ExternalLinkage, "print_i32", TheModule.get());

    llvm::BasicBlock* entry_block = llvm::BasicBlock::Create(*TheContext, "entry", print_i32);
    Builder->SetInsertPoint(entry_block);

    llvm::Value* to_print = print_i32->arg_begin();
    to_print->setName("value");

    llvm::Constant* format_str = Builder->CreateGlobalStringPtr("%d\n", ".str");

    Builder->CreateCall(printf, {format_str, to_print});
    Builder->CreateRetVoid();

    (*Functions)["print_i32"] = print_i32;

  }

  void register_print_i32_decl() {
    using namespace CompilerContext;

    llvm::FunctionType* printf_type = llvm::FunctionType::get(Builder->getInt32Ty(), {Builder->getInt8PtrTy()}, true);
    TheModule->getOrInsertFunction("printf", printf_type);

    llvm::FunctionType* print_i32_type = llvm::FunctionType::get(Builder->getVoidTy(), {Builder->getInt32Ty()}, false);
    llvm::Function* print_i32 = llvm::Function::Create(print_i32_type, llvm::Function::ExternalLinkage, "print_i32", TheModule.get());

    // Store it in the function map
    (*Functions)["print_i32"] = print_i32;
  }
}
