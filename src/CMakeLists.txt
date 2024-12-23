set(BISON_INPUT ${CMAKE_SOURCE_DIR}/src/parser/parser.y)
set(BISON_OUTPUT ${CMAKE_BINARY_DIR}/parser.cpp)
set(BISON_HEADER ${CMAKE_BINARY_DIR}/parser.hpp)

add_custom_command(
    OUTPUT ${BISON_OUTPUT} ${BISON_HEADER}
    COMMAND ${BISON_EXECUTABLE} -d --defines=${BISON_HEADER} -o ${BISON_OUTPUT} ${BISON_INPUT}
    MAIN_DEPENDENCY ${BISON_INPUT}
)

set(FLEX_INPUT ${CMAKE_SOURCE_DIR}/src/lexer/lexer.l)
set(FLEX_OUTPUT ${CMAKE_BINARY_DIR}/lex.yy.cpp)

add_custom_command(
  MAIN_DEPENDENCY ${FLEX_INPUT}
  OUTPUT ${FLEX_OUTPUT}
  COMMAND ${FLEX_EXECUTABLE} --c++ -o ${FLEX_OUTPUT} ${FLEX_INPUT}
)

set(SOURCES ${BISON_OUTPUT} ${FLEX_OUTPUT} ${CMAKE_SOURCE_DIR}/src/tokens/tokens.cpp ${CMAKE_SOURCE_DIR}/src/main.cpp)

llvm_map_components_to_libnames(llvm_libs support core irreader)

add_executable(lexer ${SOURCES})
target_link_libraries(lexer PUBLIC fl)
target_link_libraries(lexer PRIVATE ${llvm_libs})
