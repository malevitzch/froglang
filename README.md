# flex-lr1-llvm
Toolchain experiments
# todo
- Move the lexer/parser outputs to a stream of choice rather than printing them to cout

- Find all the dependencies using docker or something

- Generate code with llvm
- Find a way to implement scoping, especially the difference between block and function block scopes
- Implement a data structure that allows me to hold multiple named values of the same name, with the newest shadowing the previous ones
- Find a good way to use the print function from C in a way that does not look like a band-aid fix

- Add control flow
