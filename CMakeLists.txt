cmake_minimum_required(VERSION 3.10)

project(froglang-compiler LANGUAGES CXX C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
find_package(LLVM REQUIRED CONFIG)
set(ALLOWED_LLVM_VERSIONS 14 15 16 17 18 19 20 21 22 23 24 25 26)
if (NOT LLVM_VERSION_MAJOR IN_LIST ALLOWED_LLVM_VERSIONS)
    message(FATAL_ERROR "Unsupported LLVM version: ${LLVM_VERSION_MAJOR}")
endif()

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_BINARY_DIR})

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
