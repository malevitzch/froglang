macro(EXEC_CHECK CMD)
  execute_process(COMMAND ${CMD} RESULT_VARIABLE CMD_RESULT)
  if(CMD_RESULT)
    message(FATAL_ERROR "Error running ${CMD}$")
  endif()
endmacro()

file(GLOB CRASH_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/crash_tests/*.in")
foreach(TEST_FILE ${CRASH_TESTS})
  get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
  add_test(NAME ${TEST_NAME} 
          COMMAND ${CMAKE_BINARY_DIR}/src/compiler ${TEST_FILE})
  set_tests_properties(${TEST_NAME} PROPERTIES LABELS "Crash")
endforeach()

file(GLOB OUTPUT_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/print_tests/in/*.in")
foreach(INPUT_FILE ${OUTPUT_TESTS})
  get_filename_component(TEST_NAME ${INPUT_FILE} NAME_WE)
  set(EXPECTED_FILE "${CMAKE_CURRENT_SOURCE_DIR}/print_tests/out/${TEST_NAME}.out")
  add_test(NAME ${TEST_NAME}
          COMMAND bash "${CMAKE_CURRENT_SOURCE_DIR}/run_output_test.sh"
          "${CMAKE_BINARY_DIR}/src/compiler" "${INPUT_FILE}" "${EXPECTED_FILE}")
  set_tests_properties(${TEST_NAME} PROPERTIES LABELS "Print")
endforeach()

add_custom_target(clean_tests
  COMMAND ${CMAKE_COMMAND} -E rm -f ${CMAKE_BINARY_DIR}/src/*.o
  COMMENT "Cleaning up generated object files after tests."
)
