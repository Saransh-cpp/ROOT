add_library(libROOT SHARED root.cpp
        dummy_class.hpp
        dummy_class.cpp)

target_include_directories(libROOT PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
