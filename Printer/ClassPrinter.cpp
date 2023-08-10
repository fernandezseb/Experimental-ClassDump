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

void ClassPrinter::printClass(ClassInfo& classInfo)
{
	std::cout << "| Class Version: " << classInfo.majorVersion 
		<< "." 
		<< classInfo.minorVersion 
		<< std::endl;


	std::cout << "| Constants: " << std::endl;

	int current = 1;
	for (ConstantPoolItem* item : classInfo.constantPool.constants) {
		std::cout << "| " << "[" << current << "] " << getTypeAsString(item->getType()) << " \"" << item->toString() << "\"" << std::endl;
		current++;
	}

	CPClassInfo* classPtr = (CPClassInfo*)classInfo.constantPool.constants[classInfo.thisClass - 1];
	CPClassInfo* superClassPtr = (CPClassInfo*)classInfo.constantPool.constants[classInfo.superClass - 1];

	std::cout << "| Name: " << classInfo.constantPool.getString(classPtr->nameIndex) << std::endl;
	std::cout << "| Superclass name: " << classInfo.constantPool.getString(superClassPtr->nameIndex) << std::endl;



}
