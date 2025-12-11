# ROOT

[![Build and Install](https://github.com/Saransh-cpp/ROOT/actions/workflows/build-and-install.yml/badge.svg?branch=main)](https://github.com/Saransh-cpp/ROOT/actions/workflows/build-and-install.yml)
[![Unit tests](https://github.com/Saransh-cpp/ROOT/actions/workflows/unit-tests.yml/badge.svg?branch=main)](https://github.com/Saransh-cpp/ROOT/actions/workflows/unit-tests.yml)
[![Documentation Deployment](https://github.com/Saransh-cpp/ROOT/actions/workflows/build-and-deploy-docs.yml/badge.svg?branch=main)](https://saransh-cpp.github.io/ROOT)
[![pre-commit.ci status](https://results.pre-commit.ci/badge/github/Saransh-cpp/ROOT/main.svg)](https://results.pre-commit.ci/latest/github/Saransh-cpp/ROOT/main)

ROOT, but not the [particle physics one](https://github.com/root-project/root). Project submission for MATH-458: Programming concepts in scientific computing.

The project bundles a header-only C++ library (`libROOT`) implementing root-finding algorithms and a CLI application (`root_cli`) to read and parse input, run the algorithms implemented in libROOT, and write the output to a file of specific format.

## Project structure

The project uses the recommended [Canonical Project Structure](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1204r0.html) for C++ projects.

```
.
├── .clang-format             # ClangFormat configuration
├── .clang-tidy               # ClangTidy configuration
├── .github                   # GitHub related files
│   └── workflows             # CI files to build/test/check the project
├── .gitignore                # To ignore files from git
├── .gitmodules
├── .pre-commit-config.yaml   # Pre-commit configuration
├── CMakeLists.txt            # Top-level CMakeLists.txt
├── Doxyfile                  # Doxygen configuration
├── README.md                 # This file
├── ROOT                      # User-facing root_cli application/executable
│   ├── CMakeLists.txt        # CMakeLists.txt for user-facing root_cli application/executable
│   ├── ROOT                  # CXX and HXX files
│   └── tests                 # Tests for the root_cli application/executable
├── external                  # External libraries (as git submodules)
│   ├── CLI11                 # CLI11 submodule for the root_cli executable
│   ├── eigen                 # Eigen submodule for matrix and vector calculations
│   └── googletest            # Googletest submodule to run tests
└── libROOT                   # Developer-facing ROOT library
    ├── CMakeLists.txt        # CMakeLists.txt for developer-facing ROOT library
    ├── libROOT               # CXX and HXX files
    └── tests                 # Tests for the ROOT library
```

Apart from being divided into a library and a user-facing application/executable, the design on the project
is concretely split into four phases.

### CLI

The CLI application is written (and should be written) within the `main` function. The `main` function further calls the `Reader`, `Solver`, and `Writer` classes (in this order) on the input passed through the CLI application.

### Readers and Parsers

Reading and parsing is handled by the `ReaderBase` and `FunctionParserBase` daughter classes. Adding a new reading method should include writing a new `ReaderBase` daughter class and adding functionality to parse a new type of function should include writing a new `FunctionParserBase` daughter class. The information read is stored by the `ConfigBase` daughter classes (these are data classes to be specific, and can ideally by just `struct`s, but they use some object-oriented features, requiring them to be `class`es). Adding a new stepper type should include adding a new `ConfigBase` daughter class. The `read` method of the `ReaderBase` daughter classes accept a pointer of the type `CLI::App` and return a pointer to an object of the type of one of the daughter classes of `ConfigBase`. The `Reader` classes further implement helper methods for reading and parsing different things, and functions for constructing the `ConfigBase` object itself. Similarly, the `parse` function of the `FunctionParser` classes takes in a `string` and returns a C++ function (parses a specific type of function). The classes also include helper methods for parsing functions, and a method (in `FunctionParserBase`) to infer the type of the function (from the string) and dispatch the appropriate daughter class objects (and methods) to parse the function.

### Solver and Steppers

The solution of the non-linear equation is completely handled by two types of `class`es: `Solver` and `StepperBase`, with her daughter specialized for each method (for now: Newton-Raphson, Bisection, Chords, Fixed Point).
The `Solver` class is constructed with all the inputs required and taken from the previous reading and configuring steps, and has methods to manage the outer passages involved in the solution, all of which are called inside a unique `solve` method. These steps involve mainly the convergence check, the results saving, and the definition and call of the object specialized in computing the single step of the numerical method itself.
`Solver` has no daughter classes but could be refactored to be daughter of a `SolverBase` class which does everything which is in common for all the numerical methods (convergence check and while loop); this refactored `SolverNonLinear` would inherit all the methods from the mother class and add arguments for the functions and the boolean to require Aitken's acceleration. This new `SolverNonLinear` could have daughter classes for solving single equations (our current `Solver`) or systems of them,  which would differ just in the type of the arguments saved (e.g. derivative/jacobian for Newton-Raphson). This draft idea, which could be substituted by a fully templated version of the `SolverNonLinear` class, comes from the fact that templating is already used to define the different kinds of initial guesses allowed, and it is not possible  (in C++) to partially specialize different templates. Another more brute-force idea could be to define all the different arguments as matrices and then use them as 1 by 1 matrices (or vectors) for the single equation case, without creating two daughter classes. All of these ideas would have to be adapted for the `Stepper` classes too.
A `StepperBase` object is constructed inside the `Solver::solve` method, initially as completely virtual. Then it is converted into a specialized daughter class of it, with all the required arguments to use for the single step computation.
The only method executed by the Steppers is `compute_step` which computes a single step of the numerical method and returns the results for it.
To allow more numerical methods, it is possible to simply define new daughter classes with different `compute_step` algorithms and potentially different arguments to store.

### Writer and Printers

The writing part of the project is done again by two major `class`es: `Writer` and `PrinterBase`, with her daughter classes for each output destination available.
All the inputs required to define how and where to write the results are defined in the reading and configuring step.
What is important to point out is that these classes are not defined as only applicable for our specific project, but can write anything correctly passed (potentially with slight refactoring of the code).
This classes' methods are coded just for the typed version required in out project, but different typed version would be easy to add.
`Writer` has arguments to store what to write and how, and methods (all of which are called by a unique `write` one) to define, convert and handle a `PrinterBase` object.
`PrinterBase` is then specialized for a certain output destination, all of which have a overriden `write_values` method which prints a given input on a stored output.
To allow different writing destinations, new daughter classes can be defined inheriting from existing ones.

## Dependencies

### Dependencies for the project

#### Required

The required dependencies are included within the project as git submodules and are pinned to specific
versions for reproducibility.

- `CLI11` (`v2.6.1`): for constructing the CLI interface for the user-facing `root_cli` application.
- `Eigen3` (`v5.0.1`): for matric and vector usage / calculations.

#### Optional

These can be installed by a user and are not installed through the project's build system.

- `gnuplot`: for plotting results

### Required dependencies for the tests

`gnuplot` must be installed before building the project with `-DTEST=ON`. `GoogleTest` is installed automatically if the project is built with `-DTEST=ON`.

- `GoogleTest` (`v1.17.0`): for all tests.
- `gnuplot`: for testing `gnuplot` related code.

### Dependencies for the documentation

These can be installed by a user and are not installed through the project's build system.

#### Required

- `doxygen`: for generating the documentation.

#### Optional

- `graphviz`: for generating hierarchy and flow diagrams in the documentation.


## Building and installing the project

To build the project, clone the repository locally:

```
git clone https://github.com/Saransh-cpp/ROOT --recursive
```

And build the library and the application using:

```
cmake -S. -Bbuild
cmake --build build
```

The library (`libROOT` - header-only) and the application (`root_cli`) can be installed using:

```
cmake -S. -DCMAKE_INSTALL_PREFIX=<installation_directory_such_as_/usr/local/> -Bbuild
cmake --build build --target install
```

Which will put the library's header files and the application `<install_path>/include/libROOT`,  and `<install_path>/bin` respectively.

## Usage

TLDR;
```
ROOT Command Line Interface


root_cli [OPTIONS] SUBCOMMAND


OPTIONS:
  -h,     --help              Print this help message and exit
  -v,     --verbose           Enable verbose output
          --wcli, --write-to-cli
                              Write results to command line
          --wcsv, --write-to-csv TEXT
                              Path for writing results to CSV file
          --ocsvsep, --output-csv-sep CHAR [,]
                              Separator character for CSV output
          --wdat, --write-to-dat TEXT
                              Path for writing results to DAT file
          --wgnuplot, --write-to-gnuplot TEXT
                              Path for writing results to Gnuplot file
          --ofmode, --output-file-mode CHAR:{a,o} [o]
                              Append or overwrite output file: 'a' for append, 'o' for
                              overwrite

SUBCOMMANDS:
  csv                         Use CSV input
  dat                         Use DAT input
  cli                         Use CLI input
```

Assuming that `root_cli` is on your `PATH`, the program can be executed with:

```
root_cli <arguments>
# or use the full install/build path of the executable: /path/to/root_cli <arguments>
```

In order to print out more information about the arguments and the subcommands:

```
root_cli --help
root_cli <subcommand> --help
```

Every additional needed function must be added together with the function to find the root of.
Here's a list of examples of possible execution syntax:

- CLI input, CLI output, Newton-Raphson method to find the root of x^2-4, starting from initial guess 1 (default tolerance and maximum iterations):

    ```
    root_cli --wcli cli --function "x^2-4" newton --initial 1.0 --derivative "2*x"
    ```

- DAT input file called input.dat, DAT output file called output.dat, Bisection method to find the root of x^3-1, with initial interval [-2,2], and verbose output (given tolerance and maximum iterations):

    ```
    root_cli --verbose --wdat output dat --file input.dat
    ```

    where input.dat is:

    ```
    function = x^3-1
    method = bisection
    interval_a = -2
    interval_b = 2
    tolerance = 1e-5
    max-iterations = 100
    derivative = 2*x
    ```

- CSV input file called input.csv with first row which is a header and "," separating different values, CSV output file called output.csv, Fixed Point Method to find the root of x^2-x, with initial guess 0.5, fixed point function x^2, and verbose output (given tolerance and maximum iterations)::

    ```
    root_cli --verbose --wcsv output --ocsvsep , csv --file input.csv --sep ,
    ```

    where input.csv is:

    ```
    function,method,initial,tolerance,max_iterations,g-function
    x^2-x,fixed_point,0.5,1e-65,100,x^2
    ```

- Same as above but with aitken acceleration (will converge faster):

    ```
    root_cli --verbose --wcsv output --ocsvsep , csv --file input.csv --sep ,
    ```

    where input.csv is:

    ```
    function,method,initial,tolerance,max_iterations,g-function,aitken
    x^2-x,fixed_point,0.5,1e-5,100,x^2,true
    ```

- CLI input, DAT output file called output.dat, gnuplot writing method (a GNU Plot named output.png is created), Chords method to solve the equation x^3-8 starting from the two initial points 1 and 3:

    ```
    root_cli --wgnuplot output cli --function x^3-8 chords --x0 1 --x1 3
    ```

## Typical program execution

Input reading is handled by a CLI implemented using `CLI11`, which passes the read options to the appropriate `ReaderBase` daughter class. The `read` method of the `ReaderBase` daughter classes construct and return a `ConfigBase` daughter class object. The `ReaderBase` daughter classes also use the `FunctionParserBase` daughter classes internally to parse the function (and derivation + g function) inputted by user (string to a C++ function). The information stored in `ConfigBase` daughter classes is then passed down to the `Solver` class to run the algorithm.

The `solve` method of `Solver` construct and converts a `StepperBase` daughter class object, handles its methods calls, and finally returns the matrix of the results of the computation performed.
`compute_step` method of each `StepperBase` daughter class gets the previous iteration and computes and returns the new guess, which will be saved and checked by `Solver`'s methods.
The final results returned by `solve` are then passed down to the `Writer` class to write them.

The `write` method of `Writer` construct and converts a `PrinterBase` daughter class object, and handles its methods calls.
`write_values` method of each `StepperBase` daughter class gets a certain value to be printed and prints it out in a defined destination.

## Tests

Tests for the library (`libROOT`) can be found in `libROOT/tests`, and the tests for the application (`root_cli`) can be found in `ROOT/tests`. We follow [test-driven development (TDD)](https://en.wikipedia.org/wiki/Test-driven_development), which means that each code changing PR must have tests to validate the changes.

All the following tests and checks run as part of a Continuous Integration pipeline on GitHub Actions (on every PR and push to `main`).

### Structure of tests

```
./ROOT/tests                                # Tests for ROOT
├── integration                             # Integration tests for ROOT
│   ├── CMakeLists.txt                      # Build file for Integration tests
│   └── test_cli.cpp                        # Tests for the entire root_cli application
├── test_data                               # Data files used in tests
│   ├── config.csv
│   └── config.dat
└── unit                                    # Unit tests for ROOT
    ├── CMakeLists.txt                      # Build file for unit tests
    ├── function_parser_base_tester.hpp     # The parameterized testing class (friend of the class being tested)
    ├── polynomial_parser_tester.hpp
    ├── reader_base_tester.hpp
    ├── reader_csv_tester.hpp
    ├── test_function_parser.cpp            # Actual tests (calling paramaterized functions from the testing class)
    ├── test_polynomial_parser.cpp
    ├── test_reader.cpp
    ├── test_trigonometric_parser.cpp
    ├── test_writer.cpp
    ├── trigonometric_parser_tester.hpp
    └── writer_tester.hpp
...
./libROOT/tests                             # Tests for libROOT
└── unit                                    # Unit tests for libROOT
    ├── CMakeLists.txt                      # Build file for unit tests
    ├── solver_tester.hpp                   # The parameterized testing class (friend of the class being tested)
    └── test_solver.cpp                     # Actual tests (calling paramaterized functions from the testing class)
```

## Building and running tests

To build the tests, make sure your submodules are up-to-date (more specifically, the `googletest` submodule):

```
git submodule update --init
```

And build the code with `-DTEST=ON`:

```
cmake -S. -Bbuild -DTEST=ON
cmake --build build
```

To run the tests, use the `ctest` command:

```
ctest --test-dir build --output-on-failure
```

### Linters and formatters

#### Clang Tidy

The project uses `clang-tidy` to lint C++ and related files (except test files).

One can generate make files with `clang-tidy` enabled which will enable cmake to run `clang-tidy` along while compiling the files.

```
cmake -S. -Bbuild -DCMAKE_CXX_CLANG_TIDY=clang-tidy
cmake --build build -j8
```

#### Pre-commit

The project also uses a set of `pre-commit` hooks (including ClangFormat and generic linters and formatters) to perform static analysis and formatting on the code. The hooks can be installed locally using:

```bash
pre-commit install  # inside your environment
```

This would run the checks every time a commit is created locally. The checks will only run on the files modified by that commit, but the checks can be triggered for all the files using:

```bash
pre-commit run --all-files
```

If you would like to skip the failing checks and push the code for further discussion, use the `--no-verify` option with `git commit`.

## Documentation

The project uses Doxygen to document the code. The project also provides CMake integration for building documentation.

As with the tests, each code change should be accompanied with an updated documentation.

### Building docs locally

One can build the documentation locally using:

```
cmake -S . -B build
cmake --build build --target doc
```

which will write the HTML files to `docs/html`.

### Building docs on GH Pages

The documentation is automatically built (on every PR) and deployed (on every push to `main`) to GH Pages using the `build-and-deploy-docs` workflow.

## Limitations and problems

Most of the limitations and problems can be found as independent issues in the [issue tracker on GitHub](https://github.com/Saransh-cpp/ROOT/issues), or in the previous Project Structure section.

## Authors and their contributions

- **Andrea Saporito** ([@andreasaporito](https://github.com/andreasaporito)): Stepper, Solver, Writer, Printer classes/functionalities, most of the integration tests and some unit tests, and some fixes/refactoring here and there (touching Reader and Parser classes/functionalities, and the build system).

- **Saransh Chopra** ([@Saransh-cpp](https://github.com/Saransh-cpp)): Top-level CLI executable/application, Reader and Parser classes/functionalities, Project infrastructure (build system {code, docs, tests}, project structure, CI/CD), most of the unit tests and some integration tests, and some refactoring here and there (touching Stepper, Solver, Writer, and Printer classes/functionalities).
