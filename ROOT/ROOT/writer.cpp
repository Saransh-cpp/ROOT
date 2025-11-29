#include "writer.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

Writer::Writer(Eigen::Matrix2d& vals_to_write) {
    values = vals_to_write;
}

void Writer::choose_how(auto printer) {
    int choice1 = 1;
    std::cout << "Do you want to produce a plot or to print the results somewhere?" << std::endl;
    std::cin >> choice1;
    if (choice1 == 0) {
        printer = std::make_unique<VisualPrinter>(*this);
    }
}
