cmake_minimum_required(VERSION 3.10)

project(froglang-compiler LANGUAGES CXX C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
find_package(LLVM 16.0 REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_BINARY_DIR})

add_subdirectory(src)

enable_testing()
file(GLOB CRASH_TESTS "${CMAKE_SOURCE_DIR}/tests/crash_tests/*")

foreach(TEST_FILE ${CRASH_TESTS})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_test(NAME CrashTest_${TEST_NAME} 
             COMMAND ${CMAKE_BINARY_DIR}/src/compiler ${TEST_FILE})
endforeach()
