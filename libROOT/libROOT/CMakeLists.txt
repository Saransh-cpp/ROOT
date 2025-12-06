include(GNUInstallDirs)

add_library(libROOT INTERFACE)


target_include_directories(libROOT
  INTERFACE
    # when building from source, this directory is used
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/libROOT>
    # when installed, consumers should include from <libROOT/...>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/libROOT>
)

set_target_properties(libROOT PROPERTIES PREFIX "")
install(TARGETS libROOT
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libROOT)
install(FILES solver.hpp stepper.hpp method.hpp solver_impl.hpp stepper_impl.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libROOT)
