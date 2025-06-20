cmake_minimum_required(VERSION 3.10)

project(froglang-compiler LANGUAGES CXX C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LLVM_USE_STATIC_LIBS OFF)
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
find_package(LLVM 14 REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_BINARY_DIR})

add_subdirectory(src)

enable_testing()
add_custom_target(check
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
  DEPENDS compiler
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
add_subdirectory(tests)
