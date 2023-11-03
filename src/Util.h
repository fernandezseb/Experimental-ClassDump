#pragma once

#include "Core.h"
#include "Memory.h"

#include <string>

void joinStrings(char** strings, size_t arraySize, const char* delim, size_t bufferSize, char* buffer);

char* toCharPtr(const std::string& str, Memory* memory);