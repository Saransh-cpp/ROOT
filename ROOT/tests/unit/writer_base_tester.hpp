#ifndef WRITER_TESTS_HPP
#define WRITER_TESTS_HPP
/**
 * @file writer_base_tester.hpp
 * @brief Tester class for Writer class unit tests
 *
 *@author andreasaporito
 * This class provides static methods to test the internal state and behavior of the Writer class.
 * It is designed to be used in unit tests to verify that the Writer class functions as expected
 */
#include <gtest/gtest.h>

#include "ROOT/writer.hpp"
#include "ROOT/writer_def.hpp"
/**
 * @brief Tester class for Writer class unit tests
 */
class WriterBaseTester {
  public:
    /**
     * @brief Tests the constructor of the Writer class
     *
     * @tparam T The type of values stored in the Writer
     * @param w The Writer instance to test
     * @param expectedValues The expected values to be stored in the Writer
     * @param expectedMethod The expected writing method to be stored in the Writer
     */
    template <typename T>
    static void testConstructor(const Writer<T>& w, const T& expectedValues, WritingMethod expectedMethod) {
        ASSERT_EQ(w.method, expectedMethod);
        ASSERT_TRUE(w.values.isApprox(expectedValues));
    }
    /**
     * @brief Tests the build_printer method for CONSOLE writing method
     *
     * @tparam T The type of values stored in the Writer
     * @param w The Writer instance to test
     */
    template <typename T>
    static void testBuildPrinterConsole(const Writer<T>& w) {
        std::unique_ptr<PrinterBase<Eigen::Vector2d>> p;
        const_cast<Writer<T>&>(w).template build_printer(p, "", ',', true);
        ASSERT_NE(dynamic_cast<PrinterCLI<Eigen::Vector2d>*>(p.get()), nullptr);
    }
    /**
     * @brief Tests the build_printer method for CONSOLE writing method
     *
     * @tparam T The type of values stored in the Writer
     * @param w The Writer instance to test
     */
    template <typename T>
    static void testBuildPrinterCSV(const Writer<T>& w) {
        std::unique_ptr<PrinterBase<Eigen::Vector2d>> p;
        const_cast<Writer<T>&>(w).template build_printer(p, "out", ';', true);
        ASSERT_NE(dynamic_cast<PrinterCSV<Eigen::Vector2d>*>(p.get()), nullptr);
    }
    /**
     * @brief Tests the build_printer method for DAT writing method
     *
     * @tparam T The type of values stored in the Writer
     * @param w The Writer instance to test
     */
    template <typename T>
    static void testBuildPrinterDAT(const Writer<T>& w) {
        std::unique_ptr<PrinterBase<Eigen::Vector2d>> p;
        const_cast<Writer<T>&>(w).template build_printer(p, "out", ',', true);
        ASSERT_NE(dynamic_cast<PrinterDAT<Eigen::Vector2d>*>(p.get()), nullptr);
    }
    /**
     * @brief Tests the build_printer method for GNUPLOT writing method
     *
     * @tparam T The type of values stored in the Writer
     * @param w The Writer instance to test
     */
    template <typename T>
    static void testBuildPrinterGnuplot(const Writer<T>& w) {
        std::unique_ptr<PrinterBase<Eigen::Vector2d>> p;
        const_cast<Writer<T>&>(w).template build_printer(p, "out", ',', true);
        ASSERT_NE(dynamic_cast<PrinterGNUPlot<Eigen::Vector2d>*>(p.get()), nullptr);
    }
};

#endif
