// /**
//  * @file writer_base_tester.hpp
//  * @brief Tester class for Writer class unit tests
//  *
//  *@author andreasaporito
//  * This class provides static methods to test the internal state and behavior of the Writer class.
//  * It is designed to be used in unit tests to verify that the Writer class functions as expected
//  */
// #ifndef WRITER_TESTS_HPP
// #define WRITER_TESTS_HPP

// #include <gtest/gtest.h>

// #include "ROOT/writer.hpp"

// /**
//  * @brief Tester class for Writer class unit tests
//  */
// class WriterBaseTester {
//   public:
//   template <typename T>
//     void testBuildPrinter(const T& values, WritingMethod method,
//                           std::unique_ptr<PrinterBase<Eigen::Vector2d>>& printer) {
//         Writer<T> writer(values, method);
//         writer.build_printer(printer);

//         switch (method) {
//             case WritingMethod::CONSOLE:
//                 EXPECT_NE(dynamic_cast<PrinterCLI<Eigen::Vector2d>*>(printer.get()), nullptr);
//                 break;
//             case WritingMethod::CSV:
//                 EXPECT_NE(dynamic_cast<PrinterCSV<Eigen::Vector2d>*>(printer.get()), nullptr);
//                 break;
//             case WritingMethod::DAT:
//                 EXPECT_NE(dynamic_cast<PrinterDAT<Eigen::Vector2d>*>(printer.get()), nullptr);
//                 break;
//             case WritingMethod::GNUPLOT:
//                 EXPECT_NE(dynamic_cast<PrinterGNUPlot<Eigen::Vector2d>*>(printer.get()), nullptr);
//                 break;
//             default:
//                 FAIL() << "Unknown WritingMethod";
//         }
//     }

//     template <typename T>
//     void testWrite(const T& values, WritingMethod method) {
//         Writer<T> writer(values, method);
//         writer.write();
//         EXPECT_NO_THROW(writer.write());
//         // check if the method is CLI it is written on CLI
//         switch (method) {
//             case WritingMethod::CONSOLE:
//                 // CLI output is not easily testable; assume success if no exceptions are thrown
//                 break;
//             case WritingMethod::CSV:

//         // check if the method is CSV file is created

//         // check if the method is DAT file is created

//         // check if the method is GNUPLOT file is created and gnuplot script is generated

//     }
// };

// #endif
