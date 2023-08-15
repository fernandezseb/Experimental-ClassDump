#pragma once

#include "../ClassLoader/ClassLoader.h"

class ClassPrinter {
private:
	static std::string getTypeAsString(ConstantType type);
	static std::string getTypeAsString(AccessFlag flag);
public:
	static void printClass(ClassInfo& classInfo);
};