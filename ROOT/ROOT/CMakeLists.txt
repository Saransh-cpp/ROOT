add_executable(ROOT main.cpp
        solver.cpp
        stepper.hpp
        stepper.cpp)

target_link_libraries(ROOT PRIVATE libROOT)
