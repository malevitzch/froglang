# froglang
The project requires Flex, Bison, CMake, a C++ compiler, ZLIB, LLVM 14, and a C compiler (prferably clang or gcc, the compatibility with others is not guaranteed) to run.\
To build the project run the following command:
```sh
mkdir build && cd build && cmake ../ && make
```
The repository also contains tests that can be run using CTest. To run them simply go to the build directory after building the project and run the following command:
```sh
ctest
```
There is also a Dockerfile situated in the docker/ubuntu/ directory which can be used to build an image which can run the project without worrying about dependencies, barring docker itself. To build the image use the following command inside the root directory of the project:
```sh
docker build -f docker/ubuntu/Dockerfile -t froglang .
```
To run the container:
```sh
docker container run -it froglang
```
Inside the container the compiler can be ran using the 'frog' command.\
Note: the docker commands might require sudo if the user is not in the docker group
# todo
- Change the codegen implementation to use the visitor pattern instead

- Add consts to all the functions that should be const

- Finish the new variable system

- Make functions return new variable representation instead of LLVM values

- Change the way the graphical AST is generated

- Add a way to name the standard library output file

- Add a linking option

- Make the CLI errors say to use -help rather than just fail completely with "unknown option"

- Add read_i32()

- Fix the number of arguments not being correctly checked

- A document with code examples

- Language standard documentaion

- A reasonable way to build and run the docker image

- Certain compilation functions still do not support multiple sources which should probably be fixed asap

- Mutable variables

- "while" loop

- "times(x)" loop

- Type casts

- Write functions that create a nice .o file name for a given string, change compiler functions so that they use that to generate object files

- Add more tests

- lvalues and rvalues

- Add powershell alternatives to .sh scripts for testing

- Clean up the parser file, especially the diagnostics

- Add a reasonable compilation error/warning log to the AST

- Change the ValueHolder interface to support Allocas

- Add mutable variables

- Add code coverage tests

- Add some more tests (there are large chunks of code that are not tested yet, mostly boolean-related stuff)

- Remove the outdated TokType which is not used for anything beside token creation

- Make Binary Operators have their own classes for each. Or maybe make them take a void function pointer as codegen? We need a good way to solve the problem of having a huge block of code inside the binary operator eval function

- Implement name-mangling

- Find a good way to use the print function from C in a way that does not look like a band-aid fix

- Move the print function to a standard library rather than than manually adding it to every object

# planned features
- booleans, floats, strings, unsigned integers
- casts of form "\<TYPE_ID\> expression", combined with a data structure that runs bfs on a graph to check if cast is possible and applies the correct functions in order
- else if statements
- for loops, while loops, times loops (a loop that executes a given number of times, given as "times(unsigned integer)" ).
- labels for loops, break statements
- named function parameter calls, function f(x: int, y: int) can be called as f(y: 3, x: 8).
- arrays
- structs and potentially other user-defined types
- a standard library
- exceptions
- classes
- object file identification so that the compiler can just link the files rather then compile all of them
