# flex-lr1-llvm
Toolchain experiments
# todo

- Split the main compiler.cpp file into a header and a main file
- Add a command-line interface for the compiler

- Solve the ExprNode type issue (maybe make eval return a <Value*, Type*> pair for compile-time type verification)
- Create automated tests

- Find all the dependencies using docker or something

- Update the ValueHolder data structure to hold type information as well

- Implement name-mangling

- Find a good way to use the print function from C in a way that does not look like a band-aid fix

# features to add
- division operator
- booleans, floats, strings, unsigned integers
- if statements, else statements (maybe elif instead of else if)
- for loops, while loops (called just loop), times loops (a loop that executes a given number of times, given as "times(unsigned integer)" ).
- named function parameter calls, function f(x: int, y: int) can be called as f(y: 3, x: 8).
- arrays
- structs and potentially other user-defined types
- a standard library
- exceptions
- classes

