# ROOT

[![Build Checks](https://github.com/Saransh-cpp/ROOT/actions/workflows/build-checks.yml/badge.svg?branch=main)](https://github.com/Saransh-cpp/ROOT/actions/workflows/build-checks.yml)
[![pre-commit.ci status](https://results.pre-commit.ci/badge/github/Saransh-cpp/ROOT/main.svg)](https://results.pre-commit.ci/latest/github/Saransh-cpp/ROOT/main)

ROOT, but not the [particle physics one](https://github.com/root-project/root). Project submission for MATH-458: Programming concepts in scientific computing.

## Project structure

Given that project is relatively simple to structure and does not require installation or packaging, we use the recommended [Canonical Project Structure](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1204r0.html) for C++ projects.

```
.
├── .clang-format             # ClangFormat configuration
├── .clang-tidy               # ClangTidy configuration
├── .github                   # GitHub related files
│   └── workflows             # CI files to build/test/check the project
├── .gitignore                # To ignore files from git
├── .pre-commit-config.yaml   # Pre-commit configuration
├── CMakeLists.txt            # Top-level CMakeLists.txt
├── Doxyfile                  # Doxygen configuration
├── libROOT                   # Developer-facing ROOT library
│   ├── CMakeLists.txt        # CMakeLists.txt for developer-facing ROOT library
│   ├── libROOT               # CXX and HXX files
│   └── tests                 # Tests for the ROOT library
├── README.md                 # This file
└── ROOT                      # User-facing ROOT application/executable
    ├── CMakeLists.txt        # CMakeLists.txt for user-facing ROOT application/executable
    ├── ROOT                  # CXX and HXX files
    └── tests                 # Tests for the ROOT application/executable
```

## Usage


## Tests


## Documentation
