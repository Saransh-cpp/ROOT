cmake_minimum_required(VERSION 3.15)
project(ROOT VERSION 0.0.1
             LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(TEST "Turn on testing with googletest" OFF)
if(TEST)
    if(EXISTS "external/googletest")
        if(EXISTS "external/googletest/CMakeLists.txt")
            add_subdirectory(external/googletest)
            include(CTest)
        else()
            message(FATAL_ERROR "external/googletest/CMakeLists.txt not found; update git submodules")
        endif()
    else()
        message(FATAL_ERROR "external/googletest not found; create a new git submodule")
    endif()
endif()

find_package(Doxygen)

if (DOXYGEN_FOUND)
    set(DOC_OUTPUT_DIR ${CMAKE_BINARY_DIR}/documentation)

    set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/Doxyfile)

    add_custom_target(doc
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating documentation with Doxygen"
            VERBATIM
    )

    add_custom_target(doc-html ALL DEPENDS document)
else()
    message(STATUS "Doxygen not found. 'document' target will be unavailable.")
endif()

add_subdirectory(libROOT)
add_subdirectory(ROOT)
