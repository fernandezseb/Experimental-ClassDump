#include "Util.h"

std::string joinStrings(std::vector<std::string> const& strings, std::string delim)
{
	if (strings.empty()) {
		return std::string();
	}

	std::stringstream ss;

	ss << strings[0];

	if (strings.size() > 1) {
		std::vector<std::string>::const_iterator it = strings.begin() + 1;
		while (it != strings.end()) {
			ss << delim;
			ss << *it;
			it++;
		}
	}

	return ss.str();
}

std::string joinStrings(std::string* strings, size_t size, std::string delim)
{
	if (size == 0) {
		return std::string();
	}

	std::stringstream ss;

	ss << strings[0];

	if (size > 1) {
		size_t index = 1;
		while (index < size) {
			ss << delim;
			ss << strings[index];
			++index;
		}
	}

	return ss.str();
}

char* toCharPtr(const std::string& str)
{
	char* charPtr = (char*)malloc(str.length() + 1);
	
	strcpy(charPtr, str.c_str());
	
	return charPtr;
}
