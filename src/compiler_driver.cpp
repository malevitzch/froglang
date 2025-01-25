#include "compiler.hpp"

#include <fstream>

int main(int argc, char** argv) {
  if(argc == 1) {
    std::cerr << "Error: No arguments given\n";
    return 1;
  }
  std::vector<std::string> compiler_args(argv+1, argv + argc);

  std::ofstream debug_stream("debug.txt");
  Compiler compiler(&debug_stream);

  std::optional<std::string> compilation_error = compiler.compile_from_args(compiler_args);

  if(compilation_error) {
    std::cerr << "Error: " << *compilation_error << "\n";
    return 1;
  }
  return 0;
}
