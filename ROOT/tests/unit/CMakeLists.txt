if(BUILD_TESTING)
    set(ROOT_FILES
        ${CMAKE_SOURCE_DIR}/ROOT/ROOT/reader.cpp
        ${CMAKE_SOURCE_DIR}/ROOT/ROOT/function_parser.cpp
    )

    add_executable(test_root_unit test_reader.cpp ${ROOT_FILES})
    target_include_directories(test_root_unit PRIVATE ${CMAKE_SOURCE_DIR}/libROOT ${CMAKE_SOURCE_DIR}/ROOT)
    target_link_libraries(test_root_unit PRIVATE GTest::gtest_main CLI11::CLI11)
    include(GoogleTest)
    gtest_discover_tests(test_root_unit)
endif()
