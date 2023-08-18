#pragma once

#include "../ClassLoader/ClassLoader.h"

class ClassPrinter {
private:
	static std::string getTypeAsString(ConstantType type);
	static std::string getTypeAsString(AccessFlag flag);
	static void printField(const FieldInfo& fieldInfo, const ConstantPool& cp);
	static void printMethod(const MethodInfo& methodInfo, const ConstantPool& cp);
public:
	static void printClass(const ClassInfo& classInfo);
};