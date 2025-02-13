# flex-lr1-llvm
The project requires CMake, a C++ compiler, LLVM 16+, a C compiler (prferably clang or gcc) to run.\
The repository contains tests that can be run using CTest.
# todo
- Add multi-file support

- Clean up the parser file, especially the diagnostics

- Change the way operators are represented in code, they should just inherit from the Binary or Unary Operator superclass which should be abstract. Then the operators could be generated through a factory function that returns a shared pointer to the corresponding class (maybe a static function for Operator)

- Add a reasonable compilation error/warning log to the AST

- Change the ValueHolder interface to support Allocas

- Add mutable variables

- Add code coverage tests

- Do something about the function keyword because it's a bit too long

- Add some more tests (there are large chunks of code that are not tested yet, mostly boolean-related stuff)

- Remove the outdated TokType which is not used for anything beside token creation

- Make Binary Operators have their own classes for each. Or maybe make them take a void function pointer as codegen? We need a good way to solve the problem of having a huge block of code inside the binary operator eval function

- Find all the dependencies using docker or something


- Implement name-mangling

- Find a good way to use the print function from C in a way that does not look like a band-aid fix
- Move the print function to a standard library rather than than manually adding it to every object

# features to add
- booleans, floats, strings, unsigned integers
- casts of form "\<TYPE_ID\> expression", combined with a data structure that runs bfs on a graph to check if cast is possible and applies the correct functions in order
- else if statements
- for loops, while loops (called just loop), times loops (a loop that executes a given number of times, given as "times(unsigned integer)" ).
- labels for loops, break statements
- named function parameter calls, function f(x: int, y: int) can be called as f(y: 3, x: 8).
- arrays
- structs and potentially other user-defined types
- a standard library
- exceptions
- classes
