cmake_minimum_required(VERSION 3.10)

project(froglang-compiler LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(src)
