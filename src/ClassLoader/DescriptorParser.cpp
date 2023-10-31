#include "DescriptorParser.h"
#include "Util.h"

char* DescriptorParser::getArgsPart(const char* descriptor, Memory* memory)
{
	const char* openBracket = descriptor + 1;
	const char* closeBracket = strchr(descriptor, ')');

	size_t size = closeBracket - openBracket;
	char* returnStr = (char*)memory->classAlloc(size + 1);

	strncpy(returnStr, openBracket, size);
	returnStr[size] = 0;

	return returnStr;
}

char* DescriptorParser::getReturnPart(const char* descriptor, Memory* memory)
{
	const char* closeBracket = strchr(descriptor, ')');
	size_t size = (descriptor + (strlen(descriptor)-1)) - closeBracket;
	char* returnStr = (char*)memory->classAlloc(size + 1);

	strcpy(returnStr, closeBracket + 1);

	return returnStr;
}

// TODO: Optimize this algorithm further
// A way this can be done is to optimize the binary classname parsing by not reading it character per
// character, but by getting a substring out of it. This way we know how much memory we should allocate
// for the string
char** DescriptorParser::getTypes(char* descriptorPart, uint16_t* size, Memory* memory)
{
	std::vector<char*> parts;

	std::stringstream temp;
	bool parsingArg = false;

	size_t i = 0;

	while (descriptorPart[i] != 0) {
		char c = descriptorPart[i];
		temp << c;
		if (c == 'L') {
			parsingArg = true;
		}
		else if (c == ';') {
			parsingArg = false;
			parts.push_back(toCharPtr(temp.str().c_str(), memory));
			temp.clear();
		}
		else if (c == '[') {
			// Do nothing
		}
		else if (!parsingArg) {
			parts.push_back(toCharPtr(temp.str().c_str(), memory));
			temp.clear();
		}
		++i;
	}

	*size = parts.size();

	char** partsArr = (char**) memory->classAlloc(parts.size() * sizeof(char*));
	std::copy(parts.begin(), parts.end(), partsArr);
	
	return partsArr;
}

Descriptor DescriptorParser::parseDescriptor(const char* descriptor, Memory* memory)
{
	Descriptor desc;
	
	uint16_t sizeArgs;
	uint16_t sizeReturnType;

	desc.args = getTypes(getArgsPart(descriptor, memory), &sizeArgs, memory);
	desc.returnType = getTypes(getReturnPart(descriptor, memory), &sizeReturnType, memory)[0];

	desc.argsCount = sizeArgs;

	return desc;
}
