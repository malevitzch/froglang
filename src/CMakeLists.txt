set(BISON_INPUT ${CMAKE_SOURCE_DIR}/src/parser/parser.y)
set(BISON_OUTPUT ${CMAKE_BINARY_DIR}/parser.cpp)
set(BISON_HEADER ${CMAKE_BINARY_DIR}/parser.hpp)

add_custom_command(
  DEPENDS ${BISON_INPUT}
  OUTPUT ${BISON_OUTPUT} ${BISON_HEADER}
  COMMAND ${BISON_EXECUTABLE} -d 
    --defines=${BISON_HEADER} 
    -o ${BISON_OUTPUT} 
    ${BISON_INPUT}
)

set(FLEX_INPUT ${CMAKE_SOURCE_DIR}/src/lexer/lexer.l)
set(FLEX_OUTPUT ${CMAKE_BINARY_DIR}/lex.yy.cpp)

add_custom_command(
  DEPENDS ${FLEX_INPUT}
  OUTPUT ${FLEX_OUTPUT}
  COMMAND ${FLEX_EXECUTABLE} --c++ -o ${FLEX_OUTPUT} ${FLEX_INPUT}
)

llvm_map_components_to_libnames(llvm_libs
  core
  support
  irreader
  target
  native
  asmparser
  asmprinter
)

set(LIB_SOURCES 
  ${BISON_OUTPUT} 
  ${FLEX_OUTPUT} 
  tokens/tokens.cpp 

  ast/node.cpp
  ast/expression_nodes.cpp
  ast/statement_nodes.cpp
  ast/globject_nodes.cpp
  ast/binary_operators.cpp
  ast/unary_operators.cpp
  ast/globals.cpp

  ast/visitors/tree_visitor.cpp
  ast/visitors/tree_printer.cpp

  dsa/value_holder.cpp
  dsa/type_holder.cpp

  libgen/print.cpp
)

add_library(app_lib ${LIB_SOURCES})

add_executable(compiler compiler.cpp compiler_driver.cpp)
target_link_libraries(compiler PRIVATE LLVM)
target_link_libraries(compiler PRIVATE app_lib)

target_compile_definitions(compiler PRIVATE 
  STDLIB_PATH=\"${CMAKE_BINARY_DIR}/src/froglib.o\")

set(LIB_OUTPUT ${CMAKE_BINARY_DIR}/froglib.o)

add_custom_command(
  DEPENDS compiler
  OUTPUT ${LIB_OUTPUT}
  COMMAND ${CMAKE_BINARY_DIR}/src/compiler -genstdlib -o ${LIB_OUTPUT}
)

add_custom_target(generate_lib ALL DEPENDS ${LIB_OUTPUT})

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(compiler PRIVATE --coverage)
    target_link_options(compiler PRIVATE --coverage)
    target_compile_options(app_lib PRIVATE --coverage)
    target_link_options(app_lib PRIVATE --coverage)
  endif()
endif()
