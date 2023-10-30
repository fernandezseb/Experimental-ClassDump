#pragma once

#define __STDC_FORMAT_MACROS

#include <inttypes.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#ifdef _CPPRTTI
#error "RTTI enabled"
#endif


#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

#include <string>
#include <vector>
#include <map>

#include <sys/stat.h>
#include <filesystem>
#include <sstream>
#include <iterator>