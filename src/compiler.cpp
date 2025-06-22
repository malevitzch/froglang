#include "compiler.hpp"
#include "ast/globals.hpp"
#include "tokens.hpp"
#include "parser.hpp"
#include "libgen/print.hpp"

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/raw_os_ostream.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Target/TargetMachine.h>
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
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
#include "llvm/Support/Program.h"
#include "llvm/Support/Host.h"
#include <iostream>
#include <fstream>


#include "ast/visitors/tree_printer.hpp"

std::shared_ptr<ast::Node> ast_root;

static const std::string compiler_help_message =
  "The usage is <compiler executable> "
  "[source file 1] [source file 2] ...\n"
  "The possible flags are: \n"
  "\t-o [output name] - to set the name of the output file\n"
  "\t-genstdlib - to make the compiler generate the language standard library\n"
  "\t-ast/-AST - to make the compiler generate an AST of the given file "
  "rather than compiling it to an object file\n"
  "\t-c - to make the compiler generate the .o file only, "
  "without linking to executable\n"
  "\t-ir - to make the compiler only generate LLVM IR, "
  "without compiling to object files\n"
  ;

bool Compiler::is_valid_filename(const std::string& filename) {
    if(filename.empty()) return false;
    static const std::set<char> illegal_characters =
      {'\"', '?', ':', '*', '>', '<', '|'};
    static auto is_legal_char = [](char ch) {
      return !illegal_characters.contains(ch);
    };
    return std::ranges::all_of(filename, is_legal_char);
}
std::string Compiler::remove_extension(const std::string& filename) {
  // We find the first dot and remove everything starting from the dot
  int end_pos = filename.find_first_of('.');
  if(end_pos >= filename.size()) {
    return filename;
  }
  return filename.substr(0, end_pos);
}

void Compiler::prepare_llvm() {
  // LLVM needs those to work

  // This one is not needed for the tests to run
  // TODO: figure out if it's useful (it was in the LLVM tutorial)
  llvm::InitializeAllTargetInfos();

  // This one is needed for codegen
  llvm::InitializeAllTargets();
  // Same as above
  llvm::InitializeAllTargetMCs();
  // This is needed for .o file generation
  llvm::InitializeAllAsmPrinters();
}

std::optional<std::string> Compiler::get_compiler_path() {
  static const std::vector<std::string> possible_compilers =
    {"clang", "gcc", "cc", "cl"};
  std::optional<std::string> compiler_path = std::nullopt;

  // Query all known C compilers and see if any is available
  // if not, simply return the nullopt
  // TODO: maybe add an option to instantiate compiler
  // with extra C compiler options?
  for(const std::string& compiler : possible_compilers) {
    llvm::ErrorOr<std::string> error_or_compiler_path =
      llvm::sys::findProgramByName(compiler);
    if(error_or_compiler_path) {
      compiler_path = error_or_compiler_path.get();
      break;
    }
  }
  return compiler_path;
}

void Compiler::print_AST(
  std::shared_ptr<ast::Node> node,
  int depth,
  std::ostream* output_stream) {

  std::shared_ptr<ast::TreePrinter> printer 
    = std::make_shared<ast::TreePrinter>(ast::TreePrinter(output_stream));
  printer->visit_node(*node);
}

std::optional<std::string> Compiler::parse_to_AST(std::istream* input_stream) {
  FrogLexer lexer(input_stream);
  std::shared_ptr<Tokens::Token> yylval
    = std::make_shared<Tokens::Token>(Tokens::Token());
  yy::parser p(lexer);
  int parser_return_val = p();
  // This returns the ast in the "ast_root" global variable
  // which is honestly quite a silly workaround
  // TODO: find a better way if possible
  if(parser_return_val != 0) {
    return "Parsing error";
  }
  return std::nullopt;
}

std::optional<std::string> Compiler::generate_IR(std::istream* input_stream) {
  std::optional<std::string> parsing_error = parse_to_AST(input_stream);
  if(parsing_error) return parsing_error;
  // We know exactly what type the AST root will be
  dynamic_pointer_cast<ast::ProgramNode>(ast_root)->codegen();
  // Clear out the root we made so that it's not left in memory
  // once we stop using it
  ast_root = nullptr;
  //FIXME: implement AST error handling
  return std::nullopt;
}

std::optional<std::string> Compiler::compile_to_AST(
  std::istream* input_stream,
  std::string output_filename) {
  std::optional<std::string> parsing_error = parse_to_AST(input_stream);
  if(parsing_error) return parsing_error;

  std::ofstream output_stream(output_filename);
  print_AST(ast_root, 0, &output_stream);

  ast_root = nullptr;
  return std::nullopt;
}

std::optional<std::string> Compiler::compile_to_AST(
  std::string input_filename,
  std::string output_filename) {
  std::ifstream input_stream(input_filename);
  if(!input_stream.is_open()) {
    return "Cannot open file \"" + input_filename + "\"";
  }
  return compile_to_AST(&input_stream, output_filename);
}

std::optional<std::string> Compiler::compile_to_IR(
  std::string input_filename,
  std::string output_filename) {
  std::ifstream input_stream(input_filename);
  return compile_to_IR(&input_stream, output_filename);
}

Compiler::Compiler(std::ostream* debug_output_stream) {
  diagnostic_stream = debug_output_stream;
}

std::optional<std::string> Compiler::compile_to_IR(
  std::istream* input_stream,
  std::string output_filename) {
  generate_IR(input_stream);
  std::error_code EC;

  // It seems that the IR output can only be sent to a file
  // using a special LLVM ostream (at least as far as I know)
  llvm::raw_fd_ostream IR_out(output_filename, EC, llvm::sys::fs::OF_None);

  if(EC) {
    return "Error opening file \"" + output_filename + "\": " + EC.message();
  }

  // nullptr here means that there are no special annotations
  CompilerContext::TheModule->print(IR_out, nullptr);
  return std::nullopt;
}

std::optional<std::string> Compiler::compile_to_obj(
  std::istream* input_stream,
  std::string output_filename) {

  prepare_llvm();

  // Find the build target
  auto TargetTriple = llvm::sys::getDefaultTargetTriple();
  std::string Error;
  auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);
  if(!Target) {
    return "Failed to find target: " + Error;
  }

  // Here we create a representation of the machine that is later
  // used to generate code for the correct architecture
  auto CPU = "generic";
  // CPU features would go here (like enabling SIMD)
  auto Features = "";
  llvm::TargetOptions opt;
  auto TargetMachine = Target->createTargetMachine(
    TargetTriple,
    CPU,
    Features,
    opt,
    llvm::Reloc::PIC_);

  std::optional<std::string> parsing_error = generate_IR(input_stream);

  if(parsing_error) {
    return "Parsing failed due to: \"" + *parsing_error + "\""; 
  }

  // Set the Module data layout so that it matches that of the machine
  CompilerContext::TheModule->setDataLayout(TargetMachine->createDataLayout());
  CompilerContext::TheModule->setTargetTriple(TargetTriple);

  // The output goes in a special ostream from LLVM
  std::error_code EC;
  llvm::raw_fd_ostream dest(output_filename, EC, llvm::sys::fs::OF_None);
  llvm::legacy::PassManager pass;

  // This is the version used by modern LLVM
  auto FileType = llvm::CGFT_ObjectFile;
  if(TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
    return "Something went wrong with LLVM during compilation";
  }
  pass.run(*CompilerContext::TheModule);
  dest.flush();
  return std::nullopt;
}

std::optional<std::string> Compiler::compile_to_obj(
  std::string input_filename,
  std::string output_filename) {
  std::ifstream input_stream(input_filename);
  if(!input_stream.is_open()) {
    return "Cannot open file \"" + input_filename + "\"";
  }
  return compile_to_obj(&input_stream, output_filename);
}

std::optional<std::string> Compiler::compile_to_exec(
  std::istream* input_stream,
  std::string output_filename) {
  std::cout << "TEST" << std::endl;
  std::optional<std::string> compilation_error = 
    compile_to_obj(input_stream, "out.o");
  if(compilation_error) {
    return "Compilation to object failed due to: \"" 
      + *compilation_error + "\"";
  }

  std::optional<std::string> compiler_path = get_compiler_path();
  if(!compiler_path) {
    return "Can't find a C compiler";
  }
  std::vector<std::string> args = 
      {*compiler_path, "-o", output_filename, "out.o", STDLIB_PATH};
  llvm::SmallVector<llvm::StringRef, 16> argv;
  for(auto& arg : args) {
    argv.push_back(arg);
  }
  std::string error_message;
  int result = llvm::sys::ExecuteAndWait(
    argv[0],
    argv,
    llvm::None,
    {},
    0,
    0,
    &error_message);
  if(result == -1) {
    return "Cannot execute the compilation command";
  }
  else if(result == -2) {
    return "The C compiler used for linking crashed";
  }
  return std::nullopt;
}

std::optional<std::string> Compiler::compile_to_exec(
  std::vector<std::string> filenames,
  std::string output_filename) {
  for(std::string& filename : filenames) {
    CompilerContext::reset_context();
    std::ifstream input_stream(filename);
    if(!input_stream) 
      return "Couldn't open file: \"" + filename +"\"\n";
    std::optional<std::string> compilation_error =
      compile_to_obj(&input_stream, remove_extension(filename) + ".o");
    if(compilation_error) {
      return "Compilation of file \""
        + filename
        + "\" to object failed due to: \""
        + *compilation_error + "\"";
    }
  }
  std::optional<std::string> compiler_path = get_compiler_path();
  if(!compiler_path) {
    return "Can't find a C compiler";
  }
  std::vector<std::string> args = 
    {*compiler_path, "-o", output_filename, STDLIB_PATH};
  for(std::string filename : filenames) {
    args.push_back(remove_extension(filename) + ".o");
  }
  llvm::SmallVector<llvm::StringRef, 16> argv;
  for(auto& arg : args) {
    argv.push_back(arg);
  }
  std::string error_message;
  int result = llvm::sys::ExecuteAndWait(
    argv[0],
    argv,
    llvm::None,
    {},
    0,
    0,
    &error_message);
  if(result == -1) {
    return "Cannot execute the compilation command";
  }
  else if(result == -2) {
    return "The compiler used for linking crashed";
  }
  return std::nullopt;

}

std::optional<std::string> Compiler::compile_to_exec(
  std::string input_filename,
  std::string output_filename) {
  std::ifstream input_stream(input_filename);
    if(!input_stream.is_open()) {
      return "Cannot open file: \"" + input_filename + "\"";
  }
  return compile_to_exec(&input_stream, output_filename);
}

std::optional<std::string> Compiler::compile_stdlib() {
  auto TargetTriple = llvm::sys::getDefaultTargetTriple();

  prepare_llvm();

  std::string Error;
  auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

  auto CPU = "generic";
  auto Features = "";
  llvm::TargetOptions opt;
  auto TargetMachine = Target->createTargetMachine(
    TargetTriple,
    CPU,
    Features,
    opt,
    llvm::Reloc::PIC_);

  libgen::register_print_i32(); 

  CompilerContext::TheModule->setDataLayout(TargetMachine->createDataLayout());
  CompilerContext::TheModule->setTargetTriple(TargetTriple);
  std::error_code EC;

  llvm::raw_fd_ostream dest("froglib.o", EC, llvm::sys::fs::OF_None);
  llvm::legacy::PassManager pass;

  // This is the version used by modern LLVM
  auto FileType = llvm::CGFT_ObjectFile;
  if(TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
    return "Something went wrong with LLVM during compilation";
  }
  pass.run(*CompilerContext::TheModule);
  dest.flush();
  return std::nullopt;
}

std::optional<std::string> 
Compiler::compile_from_args(std::vector<std::string> args) {

  auto begin = args.begin();
  auto end = args.end();
  auto data = CommandData();

  return parse_command(begin, end, data);
}

std::optional<std::string> Compiler::parse_command(
  std::vector<std::string>::iterator& it,
  std::vector<std::string>::iterator& end,
  CommandData& data) {
  while(it != end) {
    std::string arg = *it;
    if(arg.size() == 0) {
      return "Empty argument";
    }
    if(arg[0] == '-') {
      auto error = parse_option(it, end, data);
      if(error) return error;
    }
    else {
      auto error = parse_source(it, end, data);
      if(error) return error;
    }
  }
  return run_command(data);
}

std::optional<std::string> Compiler::parse_option(
  std::vector<std::string>::iterator& it,
  std::vector<std::string>::iterator& end,
  CommandData& data) {
  std::string option = (*it).substr(1, (*it).size() - 1);
  it++;
  if(option == "o") {
    if(it == end) {
      return "The \"-o\" option requires an argument";
    }
    auto arg = *it;
    if(!is_valid_filename(arg)) {
      return "Invalid output filename: \"" + arg + "\"";
    }
    data.output_name = arg;
    it++;
  }
  else if(option == "ir") {
    if(data.mode != Mode::Exec) 
      return "The compiler mode should only be set once";
    data.mode = Mode::IR;
  }
  else if(option == "c") {
    if(data.mode != Mode::Exec) 
      return "The compiler mode should only be set once";
    data.mode = Mode::Obj;
  }
  else if(option == "genstdlib") {
    if(data.mode != Mode::Exec) 
      return "The compiler mode should only be set once";
    data.mode = Mode::Stdlib;
  }
  else if(option == "ast" || option == "AST") {
    if(data.mode != Mode::Exec) 
      return "The compiler mode should only be set once";
    data.mode = Mode::AST;
  }
  else if(option == "help") {
    if(data.mode != Mode::Exec) 
      return "The compiler mode should only be set once";
    data.mode = Mode::Help;
  }
  else {
    return "Unknown option: \"-" + option + "\"";
  }
  return std::nullopt;
}

std::optional<std::string> Compiler::parse_source(
  std::vector<std::string>::iterator& it,
  std::vector<std::string>::iterator& end,
  CommandData& data) {
  auto arg = *it;
  if(!is_valid_filename(arg)) {
    return "Invalid input filename: \"" + arg + "\""; 
  }
  it++;
  data.sources.push_back(arg);
  return std::nullopt;
}

std::optional<std::string> Compiler::run_command(CommandData& data) {

  if(data.sources.empty()
    && data.mode != Mode::Stdlib 
    && data.mode != Mode::Help) {
    return "No source file given";
  }

  switch(data.mode) {

    case Mode::Stdlib:
      CompilerContext::reset_context();
      return compile_stdlib();

    case Mode::Help:
      std::cout << compiler_help_message;
      return std::nullopt;

    case Mode::Exec:
      if(!data.output_name) data.output_name = "exec";
      CompilerContext::reset_context();
      return compile_to_exec(data.sources, *data.output_name);

    case Mode::IR:
      if(!data.output_name) data.output_name = "IR";
      CompilerContext::reset_context();
      return compile_to_IR(data.sources[0], *data.output_name);

    case Mode::Obj:
      if(!data.output_name) data.output_name = "obj.o";
      CompilerContext::reset_context();
      return compile_to_obj(data.sources[0], *data.output_name);

    case Mode::AST:
      if(!data.output_name) data.output_name = "AST.txt";
      CompilerContext::reset_context();
      return compile_to_AST(data.sources[0], *data.output_name);

    default:
      return "The compiler couldn't deduce the compilation mode";
  }
}
