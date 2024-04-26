
#pragma once

#include <stdlib.h>

static_assert(sizeof(char) == 1 & sizeof(short) == 2 & sizeof(int) == 4 & sizeof(float) == 4 & sizeof(long) == 8 & sizeof(void*) == 8);

typedef unsigned char byte;

#define nullable
#define autoType __auto_type

#ifdef assert
#   undef assert
#endif

inline void assert(bool condition) { if (!condition) abort(); }
