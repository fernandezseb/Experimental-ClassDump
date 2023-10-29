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

char* joinStrings(char** strings, size_t arraySize, const char* delim, size_t allocSize, Memory* memory)
{
	if (arraySize == 0) {
		return "";
	}

	char* buffer = (char*)memory->classAlloc(allocSize);

	strcpy(buffer, strings[0]);

	if (arraySize > 1) {
		size_t index = 1;
		while (index < arraySize) {
				strcat(buffer, delim);
				strcat(buffer, strings[index]);
				++index;
		}
	}

	return buffer;
}

char* toCharPtr(const std::string& str, Memory* memory)
{
	char* charPtr = (char*)memory->classAlloc(str.length() + 1);
	
	strcpy(charPtr, str.c_str());
	
	return charPtr;
}
