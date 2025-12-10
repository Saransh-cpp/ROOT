if(BUILD_TESTING)
    set(ROOT_FILES
        ${CMAKE_SOURCE_DIR}/ROOT/ROOT/reader.cpp
        ${CMAKE_SOURCE_DIR}/ROOT/ROOT/function_parser.cpp
    )
    set(TEST_FILES
        ${CMAKE_SOURCE_DIR}/ROOT/tests/unit/test_reader.cpp
        ${CMAKE_SOURCE_DIR}/ROOT/tests/unit/test_function_parser.cpp
        ${CMAKE_SOURCE_DIR}/ROOT/tests/unit/test_polynomial_parser.cpp
        ${CMAKE_SOURCE_DIR}/ROOT/tests/unit/test_trigonometric_parser.cpp
        ${CMAKE_SOURCE_DIR}/ROOT/tests/unit/test_writer.cpp
    )

    add_executable(test_root_unit ${TEST_FILES} ${ROOT_FILES})
    target_include_directories(test_root_unit PRIVATE ${CMAKE_SOURCE_DIR}/ROOT)
    target_link_libraries(test_root_unit PRIVATE GTest::gtest_main CLI11::CLI11 libROOT)
    include(GoogleTest)
    gtest_discover_tests(test_root_unit)
endif()
