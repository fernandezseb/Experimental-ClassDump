#pragma once

#include "../ClassLoader/ClassLoader.h"

class ClassPrinter {
private:
	static std::string getTypeAsString(ConstantType type);
public:
	static void printClass(ClassInfo& classInfo);
};