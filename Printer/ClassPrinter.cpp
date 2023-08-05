#include "ClassPrinter.h"

std::string ClassPrinter::getTypeAsString(ConstantType type) {
	//CT_UTF8 = 1u,
	//	CT_INTEGER = 3u,
	//	CT_FLOAT = 4u,
	//	CT_LONG = 5u,
	//	CT_DOUBLE = 6u,
	//	CT_CLASS = 7u,
	//	CT_STRING = 8u,
	//	CT_FIELDREF = 9u,
	//	CT_METHODREF = 10u,
	//	CT_INTERFACEMETHOD = 11u,
	//	CT_NAMEANDTYPE = 12u,
	//	CT_METHODHANDLE = 15u,
	//	CT_METHODTYPE = 16u,
	//	CT_INVOKEDYNAMIC = 18u

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
	std::cout << "| Class Version " << classInfo.majorVersion 
		<< "." 
		<< classInfo.minorVersion 
		<< std::endl;

	std::cout << "| Constants: " << std::endl;

	int current = 1;
	for (ConstantPoolItem* item : classInfo.constantPool.constants) {
		std::cout << "| " << "[" << current << "] " << getTypeAsString(item->getType()) << std::endl;
		current++;
	}

}
