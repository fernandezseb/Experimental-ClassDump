#include "ClassLoader/DescriptorParser.h"

#include <iostream>

int test1(Memory* memory) {
	const char* descriptor = "([Ljava/lang/String;)V";

	char* argsPart = DescriptorParser::getArgsPart(descriptor, memory);
	if (strcmp("[Ljava/lang/String;", argsPart) != 0) {
		fprintf(stderr, "Expected: %s but was %s\n", "[Ljava/lang/String;", argsPart);
		return -1;
	}

	char* returnPart = DescriptorParser::getReturnPart(descriptor, memory);
	if (strcmp(returnPart, "V") != 0) {
		fprintf(stderr, "Expected: %s but was: %s\n", "V", returnPart);
		return -1;
	}

	return 0;
}

int test2(Memory* memory) {
	const char* descriptor = "([Ljava/lang/String;I[SSI)[[I";

	char* argsPart = DescriptorParser::getArgsPart(descriptor, memory);
	if (strcmp(argsPart, "[Ljava/lang/String;I[SSI") != 0) {
		fprintf(stderr, "Expected: %s but was %s\n", "[Ljava/lang/String;I[SSI", argsPart);
		return -1;
	}

	char* returnPart = DescriptorParser::getReturnPart(descriptor, memory);
	if (strcmp(returnPart, "[[I") != 0) {
		fprintf(stderr, "Expected: %s but was: %s\n", "[[I", returnPart);
		return -1;
	}

	return 0;
}

int main(int argc, char* argv[])
{
	Memory memory(5000);
	int ret = test1(&memory);

	if (ret != 0) {
		return ret;
	}

	ret = test2(&memory);

	if (ret != 0) {
		return ret;
	}

	uint16_t size;
	char** parts = DescriptorParser::getTypes("[Ljava/lang/String;I[SS[[I", &size, &memory);
	if (size != 5) {
		return -2;
	}

	parts = DescriptorParser::getTypes("I", &size, &memory);
	if (size != 1) {
		return -2;
	}

	return 0;
}