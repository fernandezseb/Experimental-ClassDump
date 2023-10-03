#include "ClassPrinter.h"

#include "Util.h"

std::string ClassPrinter::getTypeAsString(ConstantType type) {
	switch (type) {
	case CT_UTF8:
		return "Utf8";
	case CT_INTEGER:
		return "Integer";
	case CT_FLOAT:
		return "Float";
	case CT_LONG:
		return "Long";
	case CT_DOUBLE:
		return "Double";
	case CT_CLASS:
		return "Class";
	case CT_STRING:
		return "String";
	case CT_FIELDREF:
		return "Fieldref";
	case CT_METHODREF:
		return "Methodref";
	case CT_INTERFACEMETHOD:
		return "Interfacemethod";
	case CT_NAMEANDTYPE:
		return "NameAndType";
	case CT_METHODHANDLE:
		return "MethodHandle";
	case CT_METHODTYPE:
		return "Methodtype";
	case CT_INVOKEDYNAMIC:
		return "InvokeDynamic";
	default:
		return "Unknown";
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

std::string ClassPrinter::getAsExternalReturnType(std::string returnType)
{
	if (returnType == "V") {
		return "void";
	}
	// TODO: Add other return types
}

std::string ClassPrinter::getAsExternalClassName(std::string className)
{
	std::replace(className.begin(), className.end(), '/', '.');
	return className;
}

void ClassPrinter::printField(const FieldInfo& fieldInfo, const ConstantPool& cp)
{
}

void ClassPrinter::printMethodSignature(const MethodInfo& methodInfo, const ClassInfo& classInfo, const ConstantPool& cp)
{
	if (!methodInfo.isConstructor) {
		std::cout << getAsExternalReturnType(methodInfo.returnType) << " ";
	}

	if (methodInfo.isConstructor) {
		// TODO: Reuse classname
		const CPClassInfo* classPtr = cp.getClassInfo(classInfo.thisClass);
		const std::string className = cp.getString(classPtr->nameIndex);
		std::cout << className;
	}
	else
	{
		std::cout << cp.getString(methodInfo.nameIndex);
	}
	std::cout << "(";

	std::cout << getAsExternalClassName(joinStrings(methodInfo.args, ","));

	std::cout << ");" << std::endl;
}

void ClassPrinter::printMethod(const MethodInfo& methodInfo, const ClassInfo& classInfo, const ConstantPool& cp)
{
	std::cout << "  ";
	if (methodInfo.isPublic) {
		std::cout << "public ";
	}
	if (methodInfo.isStatic) {
		std::cout << "static ";
	}
	if (methodInfo.isNative) {
		std::cout << "native ";
	}
	printMethodSignature(methodInfo, classInfo, cp);
	std::cout << "    descriptor: " << cp.getString(methodInfo.descriptorIndex) << std::endl;
	
	std::cout << "    flags:";
	for (AccessFlag flag : methodInfo.getAccessFlags()) {
		std::cout << " " << getTypeAsString(flag);
	}
	std::cout << std::endl;

	if (methodInfo.isNative) {
	}
	else {
		std::cout << "    Code: " << std::endl;
		printCode(methodInfo.code, &methodInfo, cp);
	}
	std::cout << std::endl;
}

void DefaultPrinter(std::vector<uint8_t> args, const ConstantPool& cp)
{
	for (uint8_t arg : args) {
		std::cout << " " << (unsigned int) arg;
	}
}

void ByteIndices(std::vector<uint8_t> args, const ConstantPool& cp)
{
	for (uint8_t arg : args) {
		std::cout << " #" << (unsigned int) arg;
	}
}

void ShortIndices(std::vector<uint8_t> args, const ConstantPool& cp)
{
	for (int i = 0; i < args.size(); i += 2) {
		uint8_t byte1 = args[i];
		uint8_t byte2 = args[i + 1];

		uint16_t shortIndex = (byte1 << 8) | byte2;

		std::cout << " #" << (unsigned int)shortIndex;
	}
}

void ClassPrinter::printCode(const AttributeCode* code, const MethodInfo* method, const ConstantPool& cp)
{
	std::cout << "      stack=" << code->maxStack << ", " 
		<< "locals=" << code->maxStack
		<< ", args_size=" << std::to_string(method->args.size())
		<< std::endl;

	for (uint32_t index = 0; index < code->codeLength; index++) {
		uint8_t opcode = code->code[index];
		bool found = false;
		for (Instruction instruction : this->instructions) {
			if (((uint8_t)instruction.opcode) == opcode) {
				std::string indexStr = std::to_string(index) + ": ";
				std::cout << std::right << std::setfill(' ') << std::setw(12) << indexStr 
					<< std::left << std::setfill(' ') << std::setw(15) << instruction.name;
				
				std::vector<uint8_t> args;
				if (instruction.args > 0) {
					for (int arg = 0; arg < instruction.args; arg++) {
						args.push_back(code->code[++index]);
					}
				}
				instruction.printFunction(args, cp);
				
				found = true;
				break;
			}
		}
		if (!found) {
			std::cout << "|" << index << " [" << (int)opcode << "]";
		}
		std::cout << std::endl;
	}

	//std::cout << "      Exceptions: " << std::endl;

	//for (ExceptionTableEntry entry : code->exceptionTable) {
	//	std::cout << "PC Range: " << entry.startPc << " - " << entry.endPc
	//		<< " handlerPC: " << entry.handlerPc << " Catch type: " << entry.catchType << std::endl;
	//}

}

ClassPrinter::ClassPrinter()
{
	instructions.push_back({nop, 0, "nop", DefaultPrinter});
	instructions.push_back({ bipush, 1, "bipush", DefaultPrinter});
	instructions.push_back({ istore_1, 0, "istore_1", DefaultPrinter});
	instructions.push_back({ istore_2, 0, "istore_2", DefaultPrinter});
	instructions.push_back({ i_return, 0, "return", DefaultPrinter});
	instructions.push_back({ i_new, 2, "new", ShortIndices});
	instructions.push_back({ dup, 0, "dup", DefaultPrinter});
	instructions.push_back({ invokespecial, 2, "invokespecial", ShortIndices});
	instructions.push_back({ aload_0, 0, "aload_0", DefaultPrinter});
	instructions.push_back({ aload_1, 0, "aload_1", DefaultPrinter});
	instructions.push_back({ astore_1, 0, "astore_1", DefaultPrinter});
	instructions.push_back({ aload_0, 0, "aload_0", DefaultPrinter});
	instructions.push_back({ ldc, 1, "ldc", ByteIndices });
	instructions.push_back({ invokevirtual, 2, "invokevirtual", ShortIndices });
	instructions.push_back({ invokestatic, 2, "invokestatic", ShortIndices });
	instructions.push_back({ athrow, 0, "athrow", DefaultPrinter });
	instructions.push_back({ putfield, 2, "putfield", ShortIndices });
}

void ClassPrinter::printClass(const ClassInfo& classInfo)
{
	std::cout << "Classfile ";
	std::wcout << classInfo.filePath;
	std::cout << std::endl;

	char time[50];
	strftime(time, 50, "%b %d, %Y", localtime(&classInfo.lastModified));

	std::cout << "  Last modified " << time << "; size " << classInfo.size << " bytes" << std::endl;
	std::cout << "  MD5 checksum " << classInfo.md5 << std::endl;

	const ConstantPool& cp = classInfo.constantPool;
	const CPClassInfo* classPtr = cp.getClassInfo(classInfo.thisClass);
	const CPClassInfo* superClassPtr = cp.getClassInfo(classInfo.superClass);

	if (classInfo.isPublic) {
		std::cout << "public ";
	}

	std::cout << "class " << cp.getString(classPtr->nameIndex);

	std::string superClassName =  cp.getString(superClassPtr->nameIndex);
	if (superClassName != "java/lang/Object") {
		std::cout << " extends " << getAsExternalClassName(cp.getString(superClassPtr->nameIndex));
	}

	// TODO: print the interfaces here
	if (classInfo.interfaces.size() > 0) {
		std::cout << " implements ";


		// TODO: OPTIMIZE: Replace vector by array 
		std::vector<std::string> names;

		for (uint16_t index : classInfo.interfaces) {
			CPClassInfo* classPtr = cp.getClassInfo(index);
			names.push_back(cp.getString(classPtr->nameIndex));
		}

		std::cout << getAsExternalClassName(joinStrings(names, ", "));

	}
	std::cout << std::endl;

	std::cout << "  minor version" << " " << classInfo.minorVersion  << std::endl;
	std::cout << "  major version" << " " << classInfo.majorVersion  << std::endl;

	std::cout << "  flags:";
	for (AccessFlag flag : classInfo.getAccessFlags()) {
		std::cout << " " << getTypeAsString(flag);
	}
	std::cout << std::endl;

	std::cout << "Constant pool:" << std::endl;
	int current = 1;
	for (ConstantPoolItem* item : cp.constants) {
		std::string indexStr = std::string("#");
		indexStr = indexStr + std::to_string(current);
		std::cout << std::right << std::setfill(' ') << std::setw(5) << indexStr;
		std::cout << " = " << std::left << std::setfill(' ') << std::setw(15) << getTypeAsString(item->getType()) << item->toString() << std::endl;
		current++;
	}

	std::cout << "{" << std::endl;

	// Fields
	for (const FieldInfo fieldInfo : classInfo.fields) {
		printField(fieldInfo, cp);
	}

	// Methods
	for (const MethodInfo& methodInfo : classInfo.methods) {
		printMethod(methodInfo, classInfo, cp);
	}

	std::cout << "}" << std::endl;


}
