add_library(libROOT SHARED root.cpp
        dummy_class.cpp)

target_link_libraries(libROOT PUBLIC Eigen3::Eigen)

target_include_directories(libROOT PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
