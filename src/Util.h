#pragma once

#include "Core.h"
#include "Memory.h"

void joinStrings(char** strings, size_t arraySize, const char* delim, size_t bufferSize, char* buffer);
void modifiedUtf8ToStandardUtf8(char *input, char *output);