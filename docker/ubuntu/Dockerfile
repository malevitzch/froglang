FROM debian:bookworm-slim
RUN apt-get update && apt install -y clang flex bison llvm-14-dev cmake 
WORKDIR /froglang-compiler
COPY src/ ./src/
COPY include/ ./include/
COPY tests/ ./tests/
COPY CMakeLists.txt ./
RUN mkdir build
WORKDIR /froglang-compiler/build
RUN cmake -S ..
RUN make
WORKDIR /froglang-compiler/examples
RUN echo 'frog() { /froglang-compiler/build/src/compiler "$@"; }' >> ~/.bashrc
