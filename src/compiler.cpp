#include <iostream>
#include "ast.hpp"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Host.h"
int main() {

  auto TargetTriple = llvm::sys::getDefaultTargetTriple();
  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  std::string Error;
  auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

  llvm::FunctionType *MainFuncType = llvm::FunctionType::get(llvm::Type::getInt32Ty(*CompilerContext::TheContext), false);
  llvm::Function *MainFunc = llvm::Function::Create(MainFuncType, llvm::Function::ExternalLinkage, "main", CompilerContext::TheModule.get());
  
  llvm::BasicBlock *EntryBB = llvm::BasicBlock::Create(*CompilerContext::TheContext, "entry", MainFunc);
  CompilerContext::Builder = std::make_unique<llvm::IRBuilder<>>(*CompilerContext::TheContext);

  auto CPU = "generic";
  auto Features = "";
  llvm::TargetOptions opt;
  auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, llvm::Reloc::PIC_);

  // codegen goes here 
  CompilerContext::Builder->SetInsertPoint(EntryBB);
  llvm::FunctionType *printfType = llvm::FunctionType::get(
  llvm::Type::getInt32Ty(*CompilerContext::TheContext), {llvm::Type::getInt8PtrTy(*CompilerContext::TheContext)}, true);
  llvm::FunctionCallee printfFunc = CompilerContext::TheModule->getOrInsertFunction("printf", printfType);

  llvm::Value *HelloStr = CompilerContext::Builder->CreateGlobalStringPtr("Hello, LLVM!\n");
  CompilerContext::Builder->CreateCall(printfFunc, {HelloStr});
  CompilerContext::Builder->CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(*CompilerContext::TheContext), 0));

  CompilerContext::TheModule->setDataLayout(TargetMachine->createDataLayout());
  CompilerContext::TheModule->setTargetTriple(TargetTriple);

  auto filename = "out.o";
  std::error_code EC;
  llvm::raw_fd_ostream dest(filename, EC, llvm::sys::fs::OF_None);
  llvm::legacy::PassManager pass;
  // This is the version used by modern LLVM
  auto FileType = llvm::CGFT_ObjectFile;
  if(TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
    std::cout<<"BUG";
    return 1;
  }
  pass.run(*CompilerContext::TheModule);
  dest.flush();
  //CompilerContext::TheContext;
  ast::Node* node = new ast::ProgramNode();
  std::cout<<node->get_name()<<"\n";

}
