#include "Core.h"

struct Descriptor {
	std::vector<std::string> args;
	std::string returnType;
};

class DescriptorParser {
public:
	static std::string getArgsPart(const std::string& descriptor);
	static std::string getReturnPart(const std::string& descriptor);
	static std::vector<std::string> getTypes(const std::string& descriptorPart);
	static Descriptor parseDescriptor(const std::string descriptor);
};