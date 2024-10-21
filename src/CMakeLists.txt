
set(FLEX_INPUT ${CMAKE_SOURCE_DIR}/src/lexer.l)
set(FLEX_OUTPUT ${CMAKE_BINARY_DIR}/lex.yy.cpp)

add_custom_command(
  MAIN_DEPENDENCY ${FLEX_INPUT}
  OUTPUT ${FLEX_OUTPUT}
  COMMAND ${FLEX_EXECUTABLE} --c++ -o ${FLEX_OUTPUT} ${FLEX_INPUT}
)

set(SOURCES ${FLEX_OUTPUT})

add_executable(lexer ${SOURCES})
