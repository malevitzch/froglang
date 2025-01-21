#include "compiler.hpp"
#include <fstream>

//FIXME: the driver can be simplified a lot
int main(int argc, char** argv) {
  std::ifstream file;
  if(argc >= 2) {
    // If a file is provided, open it and set as input stream
    file = std::ifstream(argv[1]);
    if(!file.is_open()) {
      std::cerr << "Cannot open file: " << argv[1] << "\n";
      return 1;
    }
    std::ofstream debug_stream("debug.txt");
    Compiler compiler(&debug_stream);
    compiler.compile_to_obj(&file, "out.o");
  } else {
    std::cerr << "No file input given\n";
    return 0;
  }
}
