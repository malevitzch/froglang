cmake_minimum_required(VERSION 3.10)

project(froglang-compiler LANGUAGES CXX C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

find_package(LLVM QUIET CONFIG)
if (NOT LLVM_FOUND OR LLVM_VERSION_MAJOR LESS 16)
    message(WARNING "LLVM version is less than 16. Installing LLVM 16.0...")
    set(LLVM_INSTALL_DIR "${CMAKE_BINARY_DIR}/llvm")
    include(ExternalProject)
    ExternalProject_Add(llvm16
        URL https://github.com/llvm/llvm-project/releases/download/llvmorg-16.0.0/llvm-16.0.0.src.tar.xz
        SOURCE_DIR ${LLVM_INSTALL_DIR}/src
        BINARY_DIR ${LLVM_INSTALL_DIR}/build
        INSTALL_DIR ${LLVM_INSTALL_DIR}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${LLVM_INSTALL_DIR}
                   -DCMAKE_BUILD_TYPE=Release
                   -DLLVM_ENABLE_PROJECTS="clang"
    )
    set(LLVM_DIR "${LLVM_INSTALL_DIR}/lib/cmake/llvm" CACHE PATH "Path to new LLVM install")
    set(CMAKE_PREFIX_PATH "${LLVM_DIR};${CMAKE_PREFIX_PATH}")
    add_dependencies(MyProject llvm16)
endif()

find_package(LLVM 16 REQUIRED CONFIG)


include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_BINARY_DIR})

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
