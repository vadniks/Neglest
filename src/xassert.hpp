
#pragma once

#include <stdexcept>

void _xAssert(bool condition, const char* line) { if (!condition) throw std::runtime_error(line); }
#define xAssert(x) _xAssert(x, __LINE__)
