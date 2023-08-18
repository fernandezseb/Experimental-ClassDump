#include "ClassPrinter.h"

std::string ClassPrinter::getTypeAsString(ConstantType type) {
	switch (type) {
	case CT_UTF8:
		return "UTF8";
	case CT_INTEGER:
		return "INTEGER";
	case CT_FLOAT:
		return "FLOAT";
	case CT_LONG:
		return "LONG";
	case CT_DOUBLE:
		return "DOUBLE";
	case CT_CLASS:
		return "CLASS";
	case CT_STRING:
		return "STRING";
	case CT_FIELDREF:
		return "FIELDREF";
	case CT_METHODREF:
		return "METHODREF";
	case CT_INTERFACEMETHOD:
		return "INTERFACEMETHOD";
	case CT_NAMEANDTYPE:
		return "NAMEANDTYPE";
	case CT_METHODHANDLE:
		return "METHODHANDLE";
	case CT_METHODTYPE:
		return "METHODTYPE";
	case CT_INVOKEDYNAMIC:
		return "INVOKEDYNAMIC";
	default:
		return "UNKNOWN";
	}
}

std::string ClassPrinter::getTypeAsString(AccessFlag flag)
{
	switch (flag) {
	case ACC_PUBLIC:
		return "ACC_PUBLIC";
	case ACC_FINAL:
		return "ACC_FINAL";
	case ACC_SUPER:
		return "ACC_SUPER";
	case ACC_INTERFACE:
		return "ACC_INTERFACE";
	case ACC_ABSTRACT:
		return "ACC_ABSTRACT";
	case ACC_SYNTHETIC:
		return "ACC_SYNTHETIC";
	case ACC_ANNOTATION:
		return "ACC_ANNOTATION";
	case ACC_ENUM:
		return "ACC_ENUM";
	default:
		return "UNKNOWN";
	}
}

void ClassPrinter::printClass(const ClassInfo& classInfo)
{
	std::cout << "| Class Version: " << classInfo.majorVersion 
		<< "." 
		<< classInfo.minorVersion 
		<< std::endl;

	const ConstantPool& cp = classInfo.constantPool;
	std::cout << "| Constants:" << std::endl;
	int current = 1;
	for (ConstantPoolItem* item : cp.constants) {
		std::cout << "| " << "[" << current << "] " << getTypeAsString(item->getType()) << " \"" << item->toString() << "\"" << std::endl;
		current++;
	}

	std::cout << "| Flags:";
	for (AccessFlag flag : classInfo.getAccessFlags()) {
		std::cout << " " << getTypeAsString(flag);
	}
	std::cout << std::endl;

	const CPClassInfo* classPtr = cp.getClassInfo(classInfo.thisClass);
	const CPClassInfo* superClassPtr = cp.getClassInfo(classInfo.superClass);

	std::cout << "| Name: " << cp.getString(classPtr->nameIndex) << std::endl;
	std::cout << "| Superclass name: " << cp.getString(superClassPtr->nameIndex) << std::endl;


	std::cout << "| Interfaces: " << std::endl;

	for (uint16_t index : classInfo.interfaces) {
		CPClassInfo* classPtr = cp.getClassInfo(classInfo.thisClass);
		std::cout << "| " << cp.getString(superClassPtr->nameIndex) << std::endl;
	}



}
