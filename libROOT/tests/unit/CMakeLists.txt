if(BUILD_TESTING)
    set(TEST_FILES
        ${CMAKE_SOURCE_DIR}/libROOT/tests/unit/test_solver.cpp
    )

    add_executable(test_libroot test_solver.cpp)
    target_link_libraries(test_libroot PRIVATE GTest::gtest_main Eigen3::Eigen libROOT)
    include(GoogleTest)
    gtest_discover_tests(test_libroot)
endif()
