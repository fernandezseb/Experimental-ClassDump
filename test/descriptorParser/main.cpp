#include "ClassLoader/DescriptorParser.h"

#include <iostream>

int test1() {
	std::string descriptor = "([Ljava/lang/String;)V";

	std::string argsPart = DescriptorParser::getArgsPart(descriptor);
	if (argsPart != "[Ljava/lang/String;") {
		std::cout << "Expected: " << "[Ljava/lang/String;" << " but was : " << argsPart << std::endl;
		return -1;
	}

	std::string returnPart = DescriptorParser::getReturnPart(descriptor);
	if (returnPart != "V") {
		std::cout << "Expected: " << "V" << " but was : " << returnPart << std::endl;
		return -1;
	}

	return 0;
}

int test2() {
	std::string descriptor = "([Ljava/lang/String;I[SSI)[[I";

	std::string argsPart = DescriptorParser::getArgsPart(descriptor);
	if (argsPart != "[Ljava/lang/String;I[SSI") {
		std::cout << "Expected: " << "[Ljava/lang/String;I[SSI" << " but was : " << argsPart << std::endl;
		return -1;
	}

	std::string returnPart = DescriptorParser::getReturnPart(descriptor);
	if (returnPart != "[[I") {
		std::cout << "Expected: " << "[[I" << " but was : " << returnPart << std::endl;
		return -1;
	}

	return 0;
}

int main(int argc, char* argv[])
{
	int ret = test1();

	if (ret != 0) {
		return ret;
	}

	ret = test2();

	if (ret != 0) {
		return ret;
	}

	std::vector<std::string> parts = DescriptorParser::getTypes("[Ljava/lang/String;I[SS[[I");
	if (parts.size() != 5) {
		return -2;
	}

	parts = DescriptorParser::getTypes("I");
	if (parts.size() != 1) {
		return -2;
	}

	return 0;
}