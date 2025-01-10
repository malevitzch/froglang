#include <iostream>
#include <fstream>
#include <FlexLexer.h>
#include "tokens.hpp"
#include "parser.hpp"

#include "llvm/ADT/APFloat.h"
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

std::shared_ptr<ast::Node> ast_root;

void lex_file(std::string filename) {
  std::shared_ptr<std::ifstream> in = std::make_shared<std::ifstream>(std::ifstream(filename));
  if(!in->is_open()) {
    std::cerr << "Cannot open file: " << filename << "\n";
    return;
  }
}


int main(int argc, char** argv) {

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

  auto CPU = "generic";
  auto Features = "";
  llvm::TargetOptions opt;
  auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, llvm::Reloc::PIC_);

  std::istream* in;
  std::ifstream file;
  if(argc > 1) {
    // If a file is provided, open it and set as input stream
    file = std::ifstream(argv[1]);
    if(!file.is_open()) {
      std::cerr << "Cannot open file: " << argv[1] << "\n";
      return 1;
    }
    //in = file;
  } else {
    std::cerr << "No file input given\n";
    return 0;
  }

  // Create a lexer object
  FrogLexer lexer(&file);
  // Call the lexer
  Tokens::Token *yylval = new Tokens::Token();
  //std::shared_ptr<ast::Node> root;
  yy::parser p(lexer);
  p();
  dynamic_pointer_cast<ast::ProgramNode>(ast_root)->codegen();

  std::ofstream ast_out("tree_output.txt");

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
