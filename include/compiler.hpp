#include <iostream>
#include <optional>
#include <vector>

#include <FlexLexer.h>

extern std::ostream* diagnostic_stream;

class Compiler {
private:
  void prepare_llvm();
  std::optional<std::string> generate_IR(std::istream* input_stream);
  std::optional<std::string> get_compiler_path();
public:
  Compiler() = default;
  Compiler(std::ostream* debug_output_stream);
  std::optional<std::string> compile_to_IR(std::istream* input_stream, std::string output_filename);
  std::optional<std::string> compile_to_IR(std::string input_filename, std::string output_filename);
  std::optional<std::string> compile_to_obj(std::istream* input_stream, std::string output_filename);
  std::optional<std::string> compile_to_obj(std::string input_filename, std::string output_filename);
  std::optional<std::string> compile_to_exec(std::istream* input_stream, std::string output_filename);
  std::optional<std::string> compile_to_exec(std::string input_filename, std::string output_filename);
  std::optional<std::string> compile_from_args(std::vector<std::string> args);
};
