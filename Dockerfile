FROM gcc:latest
RUN apt-get update && apt install flex bison llvm-14 cmake -y
WORKDIR /froglang-compiler
COPY src/ ./src/
COPY include/ ./include/
COPY tests/ ./tests/
COPY CMakeLists.txt ./
RUN mkdir build
WORKDIR /froglang-compiler/build
RUN cmake -S ..
RUN make
