#include "Util.h"

void joinStrings(char** strings, size_t arraySize, const char* delim, size_t bufferSize, char* buffer)
{
	if (arraySize == 0) {
		buffer[0] = 0;
		return;
	}

	strcpy(buffer, strings[0]);

	if (arraySize > 1) {
		size_t index = 1;
		while (index < arraySize) {
			strcat(buffer, delim);
			strcat(buffer, strings[index]);
			++index;
		}
	}
}
