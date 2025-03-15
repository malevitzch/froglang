#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <iostream>
#include <optional>
#include <vector>

#include <FlexLexer.h>

extern std::ostream* diagnostic_stream;

class Compiler {
private:
  enum class Mode {
    Exec,
    IR,
    Stdlib,
    Obj
  };
  struct CommandData {
    std::vector<std::string> sources;
    Mode mode = Mode::Exec;
    std::optional<std::string> output_name;
  };

  void prepare_llvm();
  std::optional<std::string> get_compiler_path();
  std::optional<std::string> generate_IR(std::istream* input_stream);

  std::optional<std::string> parse_command(
    std::vector<std::string>::iterator& it,
    std::vector<std::string>::iterator& end,
    CommandData& data);

  std::optional<std::string> parse_option(
    std::vector<std::string>::iterator& it,
    std::vector<std::string>::iterator& end,
    CommandData& data);

  std::optional<std::string> parse_source(
    std::vector<std::string>::iterator& it,
    std::vector<std::string>::iterator& end,
    CommandData& data);

  std::optional<std::string> run_command(CommandData& data);
public:
  Compiler() = default;
  Compiler(std::ostream* debug_output_stream);

  std::optional<std::string> compile_to_IR(
    std::istream* input_stream,
    std::string output_filename);

  std::optional<std::string> compile_to_IR(
    std::string input_filename,
    std::string output_filename);

  std::optional<std::string> compile_to_obj(
    std::istream* input_stream,
    std::string output_filename);

  std::optional<std::string> compile_to_obj(
    std::string input_filename,
    std::string output_filename);

  std::optional<std::string> compile_to_exec(
    std::istream* input_stream,
    std::string output_filename);

  std::optional<std::string> compile_to_exec(
    std::string input_filename,
    std::string output_filename);

  std::optional<std::string> compile_to_exec(
    std::vector<std::string> filenames,
    std::string output_filename);

  // Generates the standard library file
  // This might potentially get replaced by something different in the future
  std::optional<std::string> compile_stdlib();

  // Parses a vector of CLI arguments given as strings 
  std::optional<std::string> compile_from_args(std::vector<std::string> args);
};

#endif
