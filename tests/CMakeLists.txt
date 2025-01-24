file(GLOB CRASH_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/crash_tests/*")

foreach(TEST_FILE ${CRASH_TESTS})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_test(NAME ${TEST_NAME} 
             COMMAND ${CMAKE_BINARY_DIR}/src/compiler ${TEST_FILE})
endforeach()

file(GLOB OUTPUT_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/print_tests/*.in")

foreach(INPUT_FILE ${OUTPUT_TESTS})
    get_filename_component(TEST_NAME ${INPUT_FILE} NAME_WE)
    set(EXPECTED_FILE "${CMAKE_CURRENT_SOURCE_DIR}/print_tests/out/${TEST_NAME}.out")

    add_test(NAME ${TEST_NAME}
      COMMAND ${CMAKE_COMMAND} -E env 
      ${CMAKE_BINARY_DIR}/src/compiler ${INPUT_FILE} &&
      ${CMAKE_BINARY_DIR}/exec > ${OUTPUT_FILE} &&
      diff -w ${OUTPUT_FILE} ${EXPECTED_FILE})
endforeach()
