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
  compiler.compile_to_exec(filename, "exec");
}
