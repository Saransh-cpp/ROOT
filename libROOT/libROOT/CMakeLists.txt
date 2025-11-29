include(GNUInstallDirs)

add_library(libROOT SHARED root_lib.cpp
        dummy_class.cpp)


target_include_directories(libROOT
  PUBLIC
    # when building from source, this directory is used
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/libROOT>
    # when installed, consumers should include from <libROOT/...>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/libROOT>
)

set_target_properties(libROOT PROPERTIES PREFIX "")
install(TARGETS libROOT
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libROOT)
install(FILES root_lib.hpp dummy_class.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libROOT)
