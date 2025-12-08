#include <gtest/gtest.h>

#include "libROOT/solver.hpp"

#include <gtest/gtest.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <array>
#include <sstream>

static std::string exec_command(const std::string& cmd) {
    std::array<char, 256> buffer{};
    std::string result;

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) throw std::runtime_error("popen() failed!");

    while (fgets(buffer.data(), buffer.size(), pipe)) {
        result += buffer.data();
    }

    pclose(pipe);
    return result;
}

TEST(NewtonMethodCLI, QuadraticConvergesToMinus2) {
    std::string exe = "../../../build/ROOT/ROOT/root_cli";

    std::string cmd =
        exe +
        " --wcli cli"
        " --function x^2-4"
        " newton"
        " --initial -1"
        " --derivative 2*x";

    std::string output = exec_command(cmd);

    ASSERT_FALSE(output.empty());

    // Look for the line: "The found root is -2"
    const std::string token = "The found root is ";
    auto pos = output.find(token);

    ASSERT_NE(pos, std::string::npos)
        << "Output did not contain expected root line.\nOutput was:\n"
        << output;

    pos += token.size();

    // Extract the number after the token
    std::stringstream ss(output.substr(pos));
    double root = 0.0;
    ss >> root;

    // Validate the root value
    EXPECT_NEAR(root, -2.0, 1e-4);
}

TEST(NewtonMethodCLI, DerivativeZeroError) {
    std::string exe = "../../../build/ROOT/ROOT/root_cli";

    std::string cmd =
        exe +
        " --wcli cli"
        " --function x^3-3"
        " newton"
        " --initial 0"
        " --derivative 3*x^2"
        " 2>&1";   // redirect stderr to stdout

    std::string output = exec_command(cmd);

    ASSERT_FALSE(output.empty());

    // Your exact expected output:
    std::string expectedError =
        "Caught error: Division by 0. The method will diverge";

    // Check that the output contains the message (ignoring ANSI color codes)
    ASSERT_NE(output.find(expectedError), std::string::npos)
        << "Expected error message not found.\nOutput was:\n"
        << output;
}

TEST(NewtonMethodCLI, SinusoidConvergesToZero) {
    std::string exe = "../../../build/ROOT/ROOT/root_cli";

    std::string cmd =
        exe +
        " --wcli cli"
        " --function 'sin(x)'"
        " newton"
        " --initial 1"
        " --derivative 'cos(x)'";

    std::string output = exec_command(cmd);

    ASSERT_FALSE(output.empty());

    const std::string token = "The found root is ";
    auto pos = output.find(token);

    ASSERT_NE(pos, std::string::npos)
        << "Output did not contain expected root line.\nOutput was:\n"
        << output;

    pos += token.size();

    // Extract the number after the token
    std::stringstream ss(output.substr(pos));
    double root = 0.0;
    ss >> root;

    // Validate the root value
    EXPECT_NEAR(root, 0.0, 1e-4);
}

TEST(NewtonMethodCLI, SinusoidConvergesToZero) {
    std::string exe = "../../../build/ROOT/ROOT/root_cli";

    std::string cmd =
        exe +
        " --wcli cli"
        " --function 'sin(x)'"
        " bisection"
        " --interval_a 0 --interval_b 2";

    std::string output = exec_command(cmd);

    ASSERT_FALSE(output.empty());

    const std::string token = "The found root is ";
    auto pos = output.find(token);

    ASSERT_NE(pos, std::string::npos)
        << "Output did not contain expected root line.\nOutput was:\n"
        << output;

    pos += token.size();

    // Extract the number after the token
    std::stringstream ss(output.substr(pos));
    double root = 0.0;
    ss >> root;

    // Validate the root value
    EXPECT_NEAR(root, 0.0, 1e-4);
}
