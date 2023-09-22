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
	case ACC_PRIVATE:
		return "ACC_PRIVATE";
	case ACC_PROTECTED:
		return "ACC_PROTECTED";
	case ACC_STATIC:
		return "ACC_STATIC";
	case ACC_FINAL:
		return "ACC_FINAL";
	case ACC_SUPER:
		return "ACC_SUPER";
	case ACC_BRIDGE:
		return "ACC_BRIDGE";
	case ACC_VARARGS:
		return "ACC_VARARGS";
	case ACC_NATIVE:
		return "ACC_NATIVE";
	case ACC_INTERFACE:
		return "ACC_INTERFACE";
	case ACC_ABSTRACT:
		return "ACC_ABSTRACT";
	case ACC_STRICT:
		return "ACC_STRICT";
	case ACC_SYNTHETIC:
		return "ACC_SYNTHETIC";
	case ACC_ANNOTATION:
		return "ACC_ANNOTATION";
	case ACC_ENUM:
		return "ACC_ENUM";
	default:
		return "UNKNOWN";
	// TOOD: Add synchronized
	}
}

void ClassPrinter::printField(const FieldInfo& fieldInfo, const ConstantPool& cp)
{
}

void ClassPrinter::printMethod(const MethodInfo& methodInfo, const ConstantPool& cp)
{
	std::cout << "| Method: " << cp.getString(methodInfo.nameIndex) 
		<< " " << cp.getString(methodInfo.descriptorIndex)  << std::endl;
	
	std::cout << "| Flags:";
	for (AccessFlag flag : methodInfo.getAccessFlags()) {
		std::cout << " " << getTypeAsString(flag);
	}
	std::cout << std::endl;

	std::cout << "| Code: " << std::endl;

	if (methodInfo.isNative) {
		std::cout << "(Native Code)" << std::endl;
	}
	else {
		printCode(methodInfo.code);
	}

	std::cout << "| ============================================================" << std::endl;
}

void ClassPrinter::printCode(const AttributeCode* code)
{
	std::cout << "| Stack: " << code->maxStack << std::endl;
	std::cout << "| Locals: " << code->maxStack << std::endl;

	for (uint32_t index = 0; index < code->codeLength; index++) {
		uint8_t opcode = code->code[index];
		bool found = false;
		for (Instruction instruction : this->instructions) {
			if (((uint8_t)instruction.opcode) == opcode) {
				std::cout << "|" << index << " [" << instruction.name << "]";
				if (instruction.args > 0) {
					for (int arg = 0; arg < instruction.args; arg++) {
						std::cout << " " << (int) (code->code[++index]);
					}
				}
				found = true;
				break;
			}
		}
		if (!found) {
			std::cout << "|" << index << " [" << (int)opcode << "]";
		}
		std::cout << std::endl;
	}

	std::cout << "| Exceptions: " << std::endl;

	for (ExceptionTableEntry entry : code->exceptionTable) {
		std::cout << "PC Range: " << entry.startPc << " - " << entry.endPc
			<< " handlerPC: " << entry.handlerPc << " Catch type: " << entry.catchType << std::endl;
	}

}

ClassPrinter::ClassPrinter()
{
	instructions.push_back({nop, 0, "nop"});
	instructions.push_back({ bipush, 1, "bipush" });
	instructions.push_back({ istore_2, 0, "istore_2" });
	instructions.push_back({ i_return, 0, "return" });
	instructions.push_back({ i_new, 2, "new" });
	instructions.push_back({ dup, 0, "dup" });
	instructions.push_back({ invokespecial, 2, "invokespecial" });
	instructions.push_back({ aload_0, 0, "aload_0" });
	instructions.push_back({ aload_1, 0, "aload_1" });
	instructions.push_back({ astore_1, 0, "astore_1" });
	instructions.push_back({ aload_0, 0, "aload_0" });
	instructions.push_back({ ldc, 1, "ldc" });
	instructions.push_back({ invokevirtual, 2, "invokevirtual" });
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


	std::cout << "| Fields: " << std::endl;

	for (const FieldInfo fieldInfo : classInfo.fields) {
		printField(fieldInfo, cp);
	}

	std::cout << "| Methods: " << std::endl;

	std::cout << "| ============================================================" << std::endl;
	for (const MethodInfo& methodInfo : classInfo.methods) {
		printMethod(methodInfo, cp);
	}


}
