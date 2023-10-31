#include "Core.h"
#include "Memory.h"

struct Descriptor {
	char** args;
	uint16_t argsCount;
	char* returnType;
};

class DescriptorParser {
public:
	static char* getArgsPart(const char* descriptor, Memory* memory);
	static char* getReturnPart(const char* descriptor, Memory* memory);
	static char** getTypes(char* descriptorPart, uint16_t* size, Memory* memory);
	static Descriptor parseDescriptor(const char* descriptor, Memory* memory);
};