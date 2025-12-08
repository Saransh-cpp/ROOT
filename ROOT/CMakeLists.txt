add_subdirectory(ROOT)

if(BUILD_TESTING)
    add_subdirectory(tests/unit)
    add_subdirectory(tests/integration)
endif()
