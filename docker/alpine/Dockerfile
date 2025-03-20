FROM alpine 
RUN apk update && apk add g++ cmake make llvm15-dev flex bison
WORKDIR /froglang-compiler
COPY src/ ./src/
COPY include/ ./include/
COPY tests/ ./tests/
COPY CMakeLists.txt ./
RUN mkdir build
WORKDIR /froglang-compiler/build
RUN cmake -S ..
RUN make
RUN echo 'frog() { /froglang-compiler/build/src/compiler "$@"; }' >> ~/.bashrc
