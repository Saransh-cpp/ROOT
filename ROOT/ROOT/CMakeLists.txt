include(GNUInstallDirs)

add_executable(root_cli main.cpp
        writer.hpp
        writer.cpp)

target_link_libraries(root_cli PRIVATE libROOT)

set_target_properties(root_cli PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
install(TARGETS root_cli RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
