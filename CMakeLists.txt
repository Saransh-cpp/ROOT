cmake_minimum_required(VERSION 3.15)
project(ROOT VERSION 0.0.1
             LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(TEST "Turn on testing with googletest" OFF)
if(TEST)
    if(EXISTS "googletest")
        add_subdirectory(googletest)
        include(CTest)
    else()
        message(FATAL_ERROR "googletest not found; update git submodules")
    endif()
endif()

add_subdirectory(libROOT)
add_subdirectory(ROOT)
