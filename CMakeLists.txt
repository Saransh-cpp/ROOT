cmake_minimum_required(VERSION 3.15)
project(ROOT VERSION 0.0.1
             LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(TEST "Turn on testing with googletest" OFF)
if(TEST)
    if(EXISTS "googletest")
        if(EXISTS "googletest/CMakeLists.txt")
            add_subdirectory(googletest)
            include(CTest)
        else()
            message(FATAL_ERROR "googletest/CMakeLists.txt not found; update git submodules")
        endif()
    else()
        message(FATAL_ERROR "googletest not found; create a new git submodule")
    endif()
endif()

add_subdirectory(libROOT)
add_subdirectory(ROOT)
