#include "compiler.hpp"

#include <fstream>

int main(int argc, char** argv) {
  if(argc < 2) {
    std::cerr << "No file input given\n";
    return 0;
  }
  std::string filename = argv[1];
  std::ofstream debug_stream("debug.txt");
  Compiler compiler(&debug_stream);
  std::optional<std::string> compilation_error = compiler.compile_to_exec(filename, "exec");
  if(compilation_error) {
    std::cerr << "Error: " << *compilation_error << "\n";
  }
}
