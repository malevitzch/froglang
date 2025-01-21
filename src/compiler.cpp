#include "compiler.hpp"

extern std::ostream* diagnostic_stream;
std::shared_ptr<ast::Node> ast_root;


void compile(std::istream* input_stream, std::string out_filename,
             std::ostream* out_debug_stream = &std::cerr) {
  diagnostic_stream = out_debug_stream;
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

  std::ofstream ast_out("tree_output.txt");

  // codegen goes here

  CompilerContext::TheModule->setDataLayout(TargetMachine->createDataLayout());
  CompilerContext::TheModule->setTargetTriple(TargetTriple);
  std::error_code EC;

  llvm::raw_fd_ostream dest(out_filename, EC, llvm::sys::fs::OF_None);
  llvm::legacy::PassManager pass;
  // This is the version used by modern LLVM
  auto FileType = llvm::CGFT_ObjectFile;
  if(TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
    *diagnostic_stream << "BUG\n";
    return;
  }
  llvm::raw_fd_ostream IR_out("IRdump", EC, llvm::sys::fs::OF_None);
  CompilerContext::TheModule->print(IR_out, nullptr);
  pass.run(*CompilerContext::TheModule);
  dest.flush();

}


int main(int argc, char** argv) {
  std::ifstream file;
  if(argc > 1) {
    // If a file is provided, open it and set as input stream
    file = std::ifstream(argv[1]);
    if(!file.is_open()) {
      std::cerr << "Cannot open file: " << argv[1] << "\n";
      return 1;
    }
    std::ofstream debug_stream("debug.txt");
    compile(&file, "out.o", &debug_stream);
  } else {
    std::cerr << "No file input given\n";
    return 0;
  }
}
