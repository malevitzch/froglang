cmake_minimum_required(VERSION 3.10)
project(froglang-compiler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(FLEX REQUIRED)

add_subdirectory(src)
