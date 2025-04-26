FROM alpine:3.17
RUN apk update && apk add \
  g++ \
  cmake \
  make \
  llvm14-dev \
  flex \
  bison \
  libffi-dev \
  zlib-dev \
  libxml2-dev
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
