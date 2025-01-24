file(GLOB CRASH_TESTS "${CMAKE_SOURCE_DIR}/tests/crash_tests/*")

foreach(TEST_FILE ${CRASH_TESTS})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_test(NAME ${TEST_NAME} 
             COMMAND ${CMAKE_BINARY_DIR}/src/compiler ${TEST_FILE})
endforeach()
