include(GNUInstallDirs)

add_library(libROOT SHARED root.cpp
        dummy_class.cpp)

target_include_directories(libROOT PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(libROOT PROPERTIES PREFIX "")
install(TARGETS libROOT LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin)
install(FILES root.hpp dummy_class.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
