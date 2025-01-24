#include "compiler.hpp"
#include "tokens.hpp"
#include "parser.hpp"

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/raw_os_ostream.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Target/TargetMachine.h>
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
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
#include "llvm/Support/Program.h"
#include "llvm/Support/Host.h"
#include <iostream>
#include <fstream>

std::shared_ptr<ast::Node> ast_root;

void Compiler::prepare_llvm() {
  static bool initialized = false;
  if(!initialized) {
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
  }
}

std::optional<std::string> Compiler::get_compiler_path() {

  static const std::vector<std::string> possible_compilers = {"clang", "gcc", "cc", "cl"};
  std::optional<std::string> compiler_path = "";
  for(const std::string& compiler : possible_compilers) {
    llvm::ErrorOr<std::string> error_or_compiler_path = llvm::sys::findProgramByName(compiler);
    if(error_or_compiler_path) {
      compiler_path = error_or_compiler_path.get();
      break;
    }
  }
  return compiler_path;
}

Compiler::Compiler(std::ostream* debug_output_stream) {
  diagnostic_stream = debug_output_stream;
}

//FIXME: split the huge functions into multiple smaller ones

std::optional<std::string> Compiler::compile_to_obj(std::istream* input_stream, std::string output_filename, std::string IR_out_filename) {
  auto TargetTriple = llvm::sys::getDefaultTargetTriple();

  prepare_llvm();

  std::string Error;
  auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

  auto CPU = "generic";
  auto Features = "";
  llvm::TargetOptions opt;
  auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, llvm::Reloc::PIC_);

  FrogLexer lexer(input_stream);
  Tokens::Token *yylval = new Tokens::Token();
  yy::parser p(lexer);
  p();
  delete yylval;

  //FIXME: we do not necessarily want to emit the tree_output.txt every time in the finished compiler
  std::ofstream ast_out("tree_output.txt");
  //TODO: bring back the tree_output dfs because it was a cool thing to have

  dynamic_pointer_cast<ast::ProgramNode>(ast_root)->codegen(); 

  CompilerContext::TheModule->setDataLayout(TargetMachine->createDataLayout());
  CompilerContext::TheModule->setTargetTriple(TargetTriple);
  std::error_code EC;

  llvm::raw_fd_ostream dest(output_filename, EC, llvm::sys::fs::OF_None);
  llvm::legacy::PassManager pass;

  // This is the version used by modern LLVM
  auto FileType = llvm::CGFT_ObjectFile;
  if(TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
    return "Something went wrong with LLVM during compilation";
  }
  //FIXME: printing IR can be its own function maybe
  if(!IR_out_filename.empty()) {
    llvm::raw_fd_ostream IR_out(IR_out_filename, EC, llvm::sys::fs::OF_None);
    CompilerContext::TheModule->print(IR_out, nullptr);
  }
  pass.run(*CompilerContext::TheModule);
  dest.flush();
  return std::nullopt;
}

std::optional<std::string> Compiler::compile_to_obj(std::string input_filename, std::string output_filename, std::string IR_out_filename) {
  std::ifstream input_stream(input_filename);
  if(!input_stream.is_open()) {
    return "Cannot open file \"" + input_filename + "\"";
  }
  return compile_to_obj(&input_stream, output_filename, IR_out_filename);
}

std::optional<std::string> Compiler::compile_to_exec(std::istream* input_stream, std::string output_filename) {
  compile_to_obj(input_stream, "out.o");
  std::optional<std::string> compiler_path = get_compiler_path();
  if(!compiler_path) {
    return "Can't find a C compiler";
  }
  std::vector<std::string> args = {*compiler_path, "-o", output_filename, "out.o"};
  llvm::SmallVector<llvm::StringRef, 16> argv;
  for(auto& arg : args) {
    argv.push_back(arg);
  }
  std::string error_message;
  int result = llvm::sys::ExecuteAndWait(argv[0], argv, std::nullopt, {}, 0, 0, &error_message);
  if(result == -1) {
    return "Cannot execute the compilation command";
  }
  else if(result == -2) {
    return "The compiler used for linking crashed";
  }
  return std::nullopt;
}

std::optional<std::string> Compiler::compile_to_exec(std::string input_filename, std::string output_filename) {
  std::ifstream input_stream(input_filename);
    if(!input_stream.is_open()) {
      return "Cannot open file: \"" + input_filename + "\"";
  } 
  return compile_to_exec(&input_stream, output_filename);
}
