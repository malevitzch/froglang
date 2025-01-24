# flex-lr1-llvm
Toolchain experiments
# todo
- Fix the print test output

- Add a command-line interface for the compiler

- Create automated tests

- Find all the dependencies using docker or something

- Implement name-mangling

- Find a good way to use the print function from C in a way that does not look like a band-aid fix

# features to add
- booleans, floats, strings, unsigned integers
- if statements, else statements (maybe elif instead of else if)
- for loops, while loops (called just loop), times loops (a loop that executes a given number of times, given as "times(unsigned integer)" ).
- named function parameter calls, function f(x: int, y: int) can be called as f(y: 3, x: 8).
- arrays
- structs and potentially other user-defined types
- a standard library
- exceptions
- classes

