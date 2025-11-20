add_library(libROOT SHARED root.cpp)

target_include_directories(libROOT PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
