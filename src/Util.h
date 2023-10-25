#pragma once

#include "Core.h"

std::string joinStrings(std::vector<std::string> const& strings, std::string delim);
std::string joinStrings(std::string *strings, size_t size, std::string delim);