add_test(
    NAME end_to_end_gpu
    COMMAND ${CMAKE_COMMAND} -E env VERBOSE=1 bash end_to_end.sh
)

set_tests_properties(end_to_end_gpu PROPERTIES
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    LABELS "end_to_end_gpu"
)

