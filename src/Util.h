#pragma once

#include "Core.h"
#include "Memory.h"

std::string joinStrings(std::vector<std::string> const& strings, std::string delim);
std::string joinStrings(std::string *strings, size_t size, std::string delim);
char* joinStrings(char** strings, size_t arraySize, const char* delim, size_t allocSize, Memory* memory);

char* toCharPtr(const std::string& str, Memory* memory);