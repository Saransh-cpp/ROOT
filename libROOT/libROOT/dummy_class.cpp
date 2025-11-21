#include "dummy_class.hpp"

constexpr int DUMMY_VALUE = 5;

DummyClass::DummyClass() = default;

int DummyClass::return_dummy_value() {
    return DUMMY_VALUE;
}
