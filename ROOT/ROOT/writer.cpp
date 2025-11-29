#include "writer.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

Writer::Writer(const Eigen::MatrixX2d& vals_to_write) {
    values = vals_to_write;
}

void Writer::print_final_result() const {
    std::cout << "The found root is" << values.row(values.rows() - 1)(0) << std::endl;
}
// Add the possibility to either overwrite the file or to flush it
void Writer::writing_process() {
    print_final_result();

    std::unique_ptr<Printer> printer;
    choose_how(printer);

    for (int i = 0; i < values.rows(); i++) {
        Eigen::Vector2d row = values.row(i);
        printer->write_values(row);
    }

    // LLM
    if (auto gp = dynamic_cast<GnuplotPrinter*>(printer.get())) {
        gp->generate_gnuplot_script();
        std::cout << "Gnuplot script generated: plot.plt\n";
    }
}

void Writer::choose_how(std::unique_ptr<Printer>& printer) {
    int choice1 = 1;
    std::cout << "Do you want to produce a plot or to print the results somewhere?" << std::endl;
    std::cin >> choice1;
    if (choice1 == 0) {
        printer = std::make_unique<OutputPrinter>();
    }
}
// I chose to let the Printer open the file each time it is called bc its job is just to handle the printing
// Could be possible to let it print some iterations somewhere and others elsewhere

// maybe ask file names and stuff in Writer and pass as input in the consttructors

OutputPrinter::OutputPrinter() {
    std::cout << "Here are the iterations of the method";
}

void OutputPrinter::write_values(const Eigen::Vector2d& value) {
    std::cout << "x = " << value(0) << "--- f(x) = " << value(1) << std::endl;
}

FilePrinter::FilePrinter() {
    std::cout << "Insert the filename of the file, without the extension" << std::endl;
    std::cin >> filename;
}

csvPrinter::csvPrinter() : FilePrinter() {
    filename.append(".csv");
    std::cout << "Insert the separator: " << std::endl;
    std::cin >> separator;
}

void csvPrinter::write_values(const Eigen::Vector2d& value) {
    std::ofstream file(filename, std::ios::app);
    if (!file.good()) {
        return;
    }  // add error throwing
    file << value(0) << separator << value(1) << std::endl;
}

datPrinter::datPrinter() : FilePrinter() {
    filename.append(".dat");
}

void datPrinter::write_values(const Eigen::Vector2d& value) {
    std::ofstream file(filename, std::ios::app);
    if (!file.good()) {
        return;
    }  // add error throwing
    file << value(0) << " " << value(1) << std::endl;
}
