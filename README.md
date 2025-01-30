# flex-lr1-llvm
The project requires CMake, LLVM 16+, a C compiler (prferably clang or gcc) to run.\
The repository contains tests that can be run using CTest.
# todo
- Booleans

- Negation

- Greater or equal, less or equal operators

- Remove the outdated TokType which is not used for anything beside token creation

- Make Binary Operators have their own classes for each. Or maybe make them take a void function pointer as codegen? We need a good way to solve the problem of having a huge block of code inside the binary operator eval function

- Find all the dependencies using docker or something

- Add multi-file support

- Extend the CLI, allowing the compilation of object files without linking them to executable

- Implement name-mangling

- Find a good way to use the print function from C in a way that does not look like a band-aid fix
- Move the print function to a standard library rather than than manually adding it to every object

# features to add
- booleans, floats, strings, unsigned integers
- if statements, else statements (maybe elif instead of else if)
- for loops, while loops (called just loop), times loops (a loop that executes a given number of times, given as "times(unsigned integer)" ).
- labels for loops, break statements
- named function parameter calls, function f(x: int, y: int) can be called as f(y: 3, x: 8).
- arrays
- structs and potentially other user-defined types
- a standard library
- exceptions
- classes

