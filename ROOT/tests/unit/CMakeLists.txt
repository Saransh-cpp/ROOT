if(BUILD_TESTING)
    set(ROOT_FILES
        ${CMAKE_SOURCE_DIR}/ROOT/ROOT/reader.cpp
        ${CMAKE_SOURCE_DIR}/ROOT/ROOT/function_parser.cpp
    )
    set(TEST_FILES
        ${CMAKE_SOURCE_DIR}/ROOT/tests/unit/test_reader.cpp
    )

    add_executable(test_root_unit ${TEST_FILES} ${ROOT_FILES})
    target_include_directories(test_root_unit PRIVATE ${CMAKE_SOURCE_DIR}/libROOT ${CMAKE_SOURCE_DIR}/ROOT)
    target_link_libraries(test_root_unit PRIVATE GTest::gtest_main CLI11::CLI11)
    include(GoogleTest)
    gtest_discover_tests(test_root_unit)
endif()
