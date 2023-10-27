#include "DescriptorParser.h"

std::string DescriptorParser::getArgsPart(const std::string& descriptor)
{
	std::size_t openBracket = 1;
	std::size_t closeBracket = descriptor.find(")");
	return descriptor.substr(openBracket, closeBracket-1);
}

std::string DescriptorParser::getReturnPart(const std::string& descriptor)
{
	std::size_t closeBracket = descriptor.find(")");
	return descriptor.substr(closeBracket + 1);
}

std::vector<std::string> DescriptorParser::getTypes(const std::string& descriptorPart)
{
	std::vector<std::string> parts;

	std::string temp = "";
	bool parsingArg = false;

	for (char c : descriptorPart) {
		temp += c;
		if (c == 'L') {
			parsingArg = true;
		}
		else if (c == ';') {
			parsingArg = false;
			parts.push_back(temp);
			temp = "";
		}
		else if (c == '[') {
			// Do nothing
		}
		else if (!parsingArg) {
			parts.push_back(temp);
			temp = "";
		}
	}

	return parts;
}

Descriptor DescriptorParser::parseDescriptor(const std::string descriptor)
{
	Descriptor desc;

	desc.args = getTypes(getArgsPart(descriptor));
	desc.returnType = getTypes(getReturnPart(descriptor))[0];

	return desc;
}
