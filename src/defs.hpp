
#pragma once

#ifdef assert
#   undef assert
#endif

#include <stdexcept>

static_assert(sizeof(char) == 1 & sizeof(int) == 4 & sizeof(long) == 8 & sizeof(nullptr) == 8);

typedef unsigned char byte;

inline void assert(bool condition) { if (!condition) throw std::runtime_error("assert"); }
