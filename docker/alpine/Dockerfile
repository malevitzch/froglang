FROM alpine:3.17
RUN apk update && apk add \
  g++ \
  cmake \
  ninja \
  llvm14-dev \
  llvm14-static \
  flex-dev \
  bison \
  libffi-dev \
  zlib-dev \
  libxml2-dev \
  bash
WORKDIR /froglang-compiler
COPY src/ ./src/
COPY include/ ./include/
COPY tests/ ./tests/
COPY CMakeLists.txt ./
RUN mkdir build
WORKDIR /froglang-compiler/build
RUN cmake -S .. -G Ninja
RUN ninja
WORKDIR /froglang-compiler/examples
RUN echo 'frog() { /froglang-compiler/build/src/compiler "$@"; }' > /etc/profile.d/frog.sh
CMD ["/bin/bash", "--login"]
