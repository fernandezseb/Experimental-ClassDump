#pragma once

#include "Opcode.h"
#include "ClassLoader/ClassLoader.h"
#include "CodePrinter.h"

#include <iostream>

class ClassPrinter {
private:
	const char* Unknown = "(unknown)";
	const char* ACC_PUBLIC_STR = "ACC_PUBLIC";
	const char* ACC_PRIVATE_STR = "ACC_PRIVATE";
	const char* ACC_PROTECTED_STR = "ACC_PROTECTED";
	const char* ACC_STATIC_STR = "ACC_STATIC";
	const char* ACC_FINAL_STR = "ACC_FINAL";
	const char* ACC_SUPER_STR = "ACC_SUPER";
	const char* ACC_BRIDGE_STR = "ACC_BRIDGE";
	const char* ACC_VARARGS_STR = "ACC_VARARGS";
	const char* ACC_NATIVE_STR = "ACC_NATIVE";
	const char* ACC_INTERFACE_STR = "ACC_INTERFACE";
	const char* ACC_ABSTRACT_STR = "ACC_ABSTRACT";
	const char* ACC_STRICT_STR = "ACC_STRICT";
	const char* ACC_SYNTHETIC_STR = "ACC_SYNTHETIC";
	const char* ACC_ANNOTATION_STR = "ACC_ANNOTATION";
	const char* ACC_ENUM_STR = "ACC_ENUM";
	const char* ACC_SYNCHRONIZED_STR = "ACC_SYNCHRONIZED";
	const char* ACC_VOLATILE_STR = "ACC_VOLATILE";
	const char* ACC_TRANSIENT_STR = "ACC_TRANSIENT";

	const char* ACC_PUBLIC_KEYWORD = "public";
	const char* ACC_PRIVATE_KEYWORD = "private";
	const char* ACC_PROTECTED_KEYWORD = "protected";
	const char* ACC_STATIC_KEYWORD = "static";
	const char* ACC_FINAL_KEYWORD = "final";
	const char* ACC_NATIVE_KEYWORD = "native";
	const char* ACC_INTERFACE_KEYWORD = "interface";
	const char* ACC_ABSTRACT_KEYWORD = "abstract";
	const char* ACC_SYNCHRONIZED_KEYWORD = "synchronized";
	const char* ACC_VOLATILE_KEYWORD = "volatile";
	const char* ACC_TRANSIENT_KEYWORD = "transient";

	enum AccessFlagType {
		CLASS, METHOD, FIELD
	};
	
	const std::map<ConstantType, const char*> constantTypes =
	{
		{CT_UTF8,    "Utf8"},
		{CT_INTEGER, "Integer"},
		{CT_FLOAT,   "Float"},
		{CT_LONG,    "Long"},
		{CT_DOUBLE,  "Double"},
		{CT_CLASS,   "Class"},
		{CT_STRING,  "String"},
		{CT_FIELDREF, "Fieldref"},
		{CT_METHODREF,"Methodref"},
		{CT_INTERFACEMETHOD, "Interfacemethod"},
		{CT_NAMEANDTYPE,  "NameAndType"},
		{CT_METHODHANDLE, "MethodHandle"},
		{CT_METHODTYPE, "Methodtype"},
		{CT_INVOKEDYNAMIC, "InvokeDynamic"}
	};
	const std::map<AccessFlag, const char*> accessFlagsField = {
		{ACC_PUBLIC, ACC_PUBLIC_STR},
		{ACC_PRIVATE, ACC_PRIVATE_STR},
		{ACC_PROTECTED, ACC_PROTECTED_STR},
		{ACC_STATIC, ACC_STATIC_STR},
		{ACC_FINAL, ACC_FINAL_STR},
		{ACC_VOLATILE, ACC_VOLATILE_STR},
		{ACC_TRANSIENT, ACC_TRANSIENT_STR},
		{ACC_SYNTHETIC, ACC_SYNTHETIC_STR},
		{ACC_ENUM, ACC_ENUM_STR}
	};

	const std::map<AccessFlag, const char*> keywords = {
		{ACC_PUBLIC, ACC_PUBLIC_KEYWORD},
		{ACC_PRIVATE, ACC_PRIVATE_KEYWORD},
		{ACC_PROTECTED, ACC_PROTECTED_KEYWORD},
		{ACC_STATIC, ACC_STATIC_KEYWORD},
		{ACC_FINAL, ACC_FINAL_KEYWORD},
		{ACC_NATIVE, ACC_NATIVE_KEYWORD},
		{ACC_ABSTRACT, ACC_ABSTRACT_KEYWORD},
		{ACC_SYNCHRONIZED, ACC_SYNCHRONIZED_KEYWORD},
		{ACC_VOLATILE, ACC_VOLATILE_KEYWORD},
		{ACC_TRANSIENT, ACC_TRANSIENT_KEYWORD}
	};

	const std::map<AccessFlag, const char*> keywordsClass = {
		{ACC_PUBLIC, ACC_PUBLIC_KEYWORD},
		{ACC_PRIVATE, ACC_PRIVATE_KEYWORD},
		{ACC_PROTECTED, ACC_PROTECTED_KEYWORD},
		{ACC_STATIC, ACC_STATIC_KEYWORD},
		{ACC_FINAL, ACC_FINAL_KEYWORD},
		{ACC_ABSTRACT, ACC_ABSTRACT_KEYWORD}
	};

	const std::map<AccessFlag, const char*> accessFlagsMethod = {
		{ACC_PUBLIC, ACC_PUBLIC_STR},
		{ACC_PRIVATE, ACC_PRIVATE_STR},
		{ACC_PROTECTED, ACC_PROTECTED_STR},
		{ACC_STATIC, ACC_STATIC_STR},
		{ACC_FINAL, ACC_FINAL_STR},
		{ACC_SYNCHRONIZED, ACC_SYNCHRONIZED_STR},
		{ACC_BRIDGE, ACC_BRIDGE_STR},
		{ACC_VARARGS, ACC_VARARGS_STR},
		{ACC_NATIVE, ACC_NATIVE_STR},
		{ACC_ABSTRACT, ACC_ABSTRACT_STR},
		{ACC_STRICT, ACC_STRICT_STR},
		{ACC_SYNTHETIC, ACC_SYNTHETIC_STR}
	};

	const std::map<AccessFlag, const char*> accessFlagsClass = {
		{ACC_PUBLIC, ACC_PUBLIC_STR},
		{ACC_FINAL, ACC_FINAL_STR},
		{ACC_SUPER, ACC_SUPER_STR},
		{ACC_INTERFACE, ACC_INTERFACE_STR},
		{ACC_ABSTRACT, ACC_ABSTRACT_STR},
		{ACC_SYNTHETIC, ACC_SYNTHETIC_STR},
		{ACC_ANNOTATION, ACC_ANNOTATION_STR},
		{ACC_ENUM, ACC_ENUM_STR}
	};
	const std::map<char, const char*> types = {
		{'V', T_VOID},
		{'B', T_BYTE},
		{'C', T_CHAR},
		{'D', T_DOUBLE},
		{'F', T_FLOAT},
		{'I', T_INT},
		{'J', T_LONG},
		{'S', T_SHORT},
		{'Z', T_BOOLEAN},
		{'D', T_DOUBLE}
	};
	CodePrinter codePrinter;
	const char* getTypeAsString(ConstantType type) const;
	const char* getTypeAsString(AccessFlag flag, AccessFlagType type) const;
	void getAsExternalReturnType(char* returnType, char* outputBuffer);
	void printField(const FieldInfo* fieldInfo, const ConstantPool* cp, Memory* memory);
	void printMethodSignature(const MethodInfo* methodInfo, 
		const ClassInfo& classInfo, 
		const char* className, 
		const ConstantPool* cp,
		Memory* memory);
	void printMethod(const MethodInfo* methodInfo, const ClassInfo& classInfo, const char* className, const ConstantPool* cp, Memory* memory);
	void printConstantPoolItem(uint16_t currentIndex, const ConstantPool* cp);
public:
	void printClass(const ClassInfo& classInfo, Memory* memory);
};