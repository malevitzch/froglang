#include <iostream>

#include <FlexLexer.h>

extern std::ostream* diagnostic_stream;

class Compiler {
private:
  void prepare_llvm();
public:
  Compiler();
  Compiler(std::ostream* debug_output_stream);
  //FIXME: those things should probably have a bool that returns whether or not the compilation was successful
  void compile_to_obj(std::istream* input_stream, std::string output_filename, std::string IR_out_filename = "");
  void compile_to_obj(std::string input_filename, std::string output_filename, std::string IR_out_filename = "");
  //FIXME: IR?
  void compile_to_exec(std::istream* input_stream, std::string output_filename);
  void compile_to_exec(std::string input_filename, std::string output_filename);
};
