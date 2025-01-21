#include "compiler.hpp"
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/raw_os_ostream.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Target/TargetMachine.h>
#include <iostream>

std::shared_ptr<ast::Node> ast_root;

Compiler::Compiler(std::ostream* debug_output_stream) {
  diagnostic_stream = debug_output_stream;
}

void Compiler::compile_to_obj(std::istream* input_stream, std::string output_filename, std::string IR_out_filename) {
  auto TargetTriple = llvm::sys::getDefaultTargetTriple();
  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  std::string Error;
  auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

  auto CPU = "generic";
  auto Features = "";
  llvm::TargetOptions opt;
  auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, llvm::Reloc::PIC_);

 // Create a lexer object
  FrogLexer lexer(input_stream);
  // Call the lexer
  Tokens::Token *yylval = new Tokens::Token();
  //std::shared_ptr<ast::Node> root;
  yy::parser p(lexer);
  p();
  dynamic_pointer_cast<ast::ProgramNode>(ast_root)->codegen();
  
  //FIXME: we do not necessarily want to emit the tree_output.txt every time in the finished compiler
  std::ofstream ast_out("tree_output.txt");

  CompilerContext::TheModule->setDataLayout(TargetMachine->createDataLayout());
  CompilerContext::TheModule->setTargetTriple(TargetTriple);
  std::error_code EC;

  llvm::raw_fd_ostream dest(output_filename, EC, llvm::sys::fs::OF_None);
  llvm::legacy::PassManager pass;
  // This is the version used by modern LLVM
  auto FileType = llvm::CGFT_ObjectFile;
  if(TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
    *diagnostic_stream << "BUG\n";
    return;
  }
  if(!IR_out_filename.empty()) {
    llvm::raw_fd_ostream IR_out("IRdump", EC, llvm::sys::fs::OF_None);
    CompilerContext::TheModule->print(IR_out, nullptr);
  }
  pass.run(*CompilerContext::TheModule);
  dest.flush();
}

void Compiler::compile_to_obj(std::string input_filename, std::string output_filename, std::string IR_out_filename) {
  //FIXME: this should validate the stream
  std::ifstream input_file(input_filename);
  compile_to_obj(&input_file, output_filename, IR_out_filename);
}
