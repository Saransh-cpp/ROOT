include(GNUInstallDirs)

add_executable(root_cli main.cpp function_parser.cpp reader.cpp)

target_include_directories(root_cli PRIVATE "${CMAKE_SOURCE_DIR}/libROOT/" "${CMAKE_SOURCE_DIR}/external/eigen")
target_link_libraries(root_cli PRIVATE CLI11::CLI11)

set_target_properties(root_cli PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
install(TARGETS root_cli RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
