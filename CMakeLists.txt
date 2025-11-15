cmake_minimum_required(VERSION 3.15)
project(ROOT VERSION 0.0.1
             LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libROOT)
add_subdirectory(ROOT)
