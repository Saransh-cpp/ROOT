include(GNUInstallDirs)

add_library(libROOT INTERFACE)


target_include_directories(libROOT
  INTERFACE
    # when building from source, this directory is used
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/libROOT>
    # when installed, consumers should include from <libROOT/...>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/libROOT>
)
target_include_directories(libROOT
  INTERFACE
    # when building from source, this directory is used
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/external/eigen>
    # when installed, consumers should have eigen3 in their include path
    # this is a common convention for Eigen installations
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/eigen3>
)

set_target_properties(libROOT PROPERTIES PREFIX "")
install(TARGETS libROOT
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libROOT)
install(FILES solver.hpp stepper.hpp method.hpp solver_def.hpp stepper_def.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libROOT)
