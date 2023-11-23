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

void modifiedUtf8ToStandardUtf8(char *input, char *output)
{
	// TODO: This definitely needs some optimizing
	for (int i = 0; i < strlen(input); ++i)
	{
		if (((uint8_t)input[i]) == 0xED)
		{
			uint32_t codepoint = 0x10000 +
						   ((input[i + 1] & 0x0f) << 16) +
						   ((input[i + 2] & 0x3f) << 10) +
						   ((input[i + 4] & 0x0f) << 6) +
						   (input[i + 5] & 0x3f);
			char fourBytes[5];
			fourBytes[0] = (char)(((codepoint >> 18) & 0x07) | 0xF0);
			fourBytes[1] = (char)(((codepoint >> 12) & 0x3F) | 0x80);
			fourBytes[2] = (char)(((codepoint >> 6) & 0x3F) | 0x80);
			fourBytes[3] = (char)(((codepoint >> 0) & 0x3F) | 0x80);
			fourBytes[4] = 0;
			strcat(output, fourBytes);
			i += 5;
		} else {
			char out[2] = {input[i], 0};
			strcat(output, out);
		}
	}
}