#pragma once

#define __STDC_FORMAT_MACROS

#ifdef _CPPRTTI
#error "RTTI enabled"
#endif

#include <cstdio>
#include <cstring>
#include <cinttypes>

#include <algorithm>
#include <iomanip>

#include <map>
#include <sstream>
#include <iterator>

// Type strings
#define T_VOID    "void"
#define T_BYTE    "byte"
#define T_CHAR    "char"
#define T_DOUBLE  "double"
#define T_FLOAT   "float"
#define T_INT     "int"
#define T_LONG    "long"
#define T_SHORT   "short"
#define T_BOOLEAN "boolean"
#define T_DOUBLE  "double"