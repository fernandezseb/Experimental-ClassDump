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
	instructions.push_back({ istore_0, 0, "istore_0", DefaultPrinter });
	instructions.push_back({ istore_1, 0, "istore_1", DefaultPrinter});
	instructions.push_back({ istore_2, 0, "istore_2", DefaultPrinter});
	instructions.push_back({ istore_3, 0, "istore_3", DefaultPrinter });
	instructions.push_back({ i_return, 0, "return", DefaultPrinter});
	instructions.push_back({ i_new, 2, "new", ShortIndices});
	instructions.push_back({ dup, 0, "dup", DefaultPrinter});
	instructions.push_back({ invokespecial, 2, "invokespecial", ShortIndices});
	instructions.push_back({ aaload, 0, "aaload", DefaultPrinter });
	instructions.push_back({ aastore, 0, "aastore", DefaultPrinter });
	instructions.push_back({ aconst_null, 0, "aconst_null", DefaultPrinter });
	instructions.push_back({ aload, 1, "aload", DefaultPrinter });
	instructions.push_back({ aload_0, 0, "aload_0", DefaultPrinter});
	instructions.push_back({ aload_1, 0, "aload_1", DefaultPrinter});
	instructions.push_back({ aload_2, 0, "aload_2", DefaultPrinter });
	instructions.push_back({ aload_3, 0, "aload_3", DefaultPrinter });
	instructions.push_back({ anewarray, 0, "anewarray", ShortIndices });
	instructions.push_back({ areturn, 0, "areturn", DefaultPrinter });
	instructions.push_back({ aload_1, 0, "aload_1", DefaultPrinter });
	instructions.push_back({ arraylength, 0, "arraylength", DefaultPrinter });
	instructions.push_back({ arraylength, 0, "arraylength", DefaultPrinter });
	instructions.push_back({ astore, 1, "astore", DefaultPrinter });
	instructions.push_back({ arraylength, 0, "arraylength", DefaultPrinter });
	instructions.push_back({ astore_0, 0, "astore_0", DefaultPrinter });
	instructions.push_back({ astore_1, 0, "astore_1", DefaultPrinter});
	instructions.push_back({ astore_2, 0, "astore_2", DefaultPrinter });
	instructions.push_back({ astore_3, 0, "astore_3", DefaultPrinter });
	instructions.push_back({ aload_0, 0, "aload_0", DefaultPrinter});
	instructions.push_back({ ldc, 1, "ldc", ByteIndices });
	instructions.push_back({ ldc_w, 2, "ldc_w", ShortIndices });
	instructions.push_back({ ldc2_w, 2, "ldc2_w", ShortIndices });
	instructions.push_back({ invokevirtual, 2, "invokevirtual", ShortIndices });
	instructions.push_back({ invokestatic, 2, "invokestatic", ShortIndices });
	instructions.push_back({ athrow, 0, "athrow", DefaultPrinter });
	instructions.push_back({ baload, 0, "baload", DefaultPrinter });
	instructions.push_back({ bastore, 0, "bastore", DefaultPrinter });
	instructions.push_back({ caload, 0, "caload", DefaultPrinter });
	instructions.push_back({ castore, 0, "castore", DefaultPrinter });
	instructions.push_back({ checkcast, 2, "checkcast", ShortIndices });
	instructions.push_back({ d2f, 0, "d2f", DefaultPrinter });
	instructions.push_back({ d2i, 0, "d2i", DefaultPrinter });
	instructions.push_back({ d2l, 0, "d2l", DefaultPrinter });
	instructions.push_back({ dadd, 0, "dadd", DefaultPrinter });
	instructions.push_back({ daload, 0, "daload", DefaultPrinter });
	instructions.push_back({ dastore, 0, "dastore", DefaultPrinter });
	instructions.push_back({ dconst_0, 0, "dconst_0", DefaultPrinter });
	instructions.push_back({ dconst_1, 0, "dconst_1", DefaultPrinter });
	instructions.push_back({ ddiv, 0, "ddiv", DefaultPrinter });
	instructions.push_back({ dload, 0, "dload", DefaultPrinter });
	instructions.push_back({ dload_0, 0, "dload_0", DefaultPrinter });
	instructions.push_back({ dload_1, 0, "dload_1", DefaultPrinter });
	instructions.push_back({ dload_2, 0, "dload_2", DefaultPrinter });
	instructions.push_back({ dload_3, 0, "dload_3", DefaultPrinter });
	instructions.push_back({ dmul, 0, "dmul", DefaultPrinter });
	instructions.push_back({ dneg, 0, "dneg", DefaultPrinter });
	instructions.push_back({ drem, 0, "drem", DefaultPrinter });
	instructions.push_back({ dreturn, 0, "dreturn", DefaultPrinter });
	instructions.push_back({ dstore, 1, "dstore", DefaultPrinter });
	instructions.push_back({ dstore_0, 0, "dstore_0", DefaultPrinter });
	instructions.push_back({ dstore_1, 0, "dstore_1", DefaultPrinter });
	instructions.push_back({ dstore_2, 0, "dstore_2", DefaultPrinter });
	instructions.push_back({ dstore_3, 0, "dstore_3", DefaultPrinter });
	instructions.push_back({ dsub, 0, "dsub", DefaultPrinter });
	instructions.push_back({ dup, 0, "dup", DefaultPrinter });
	instructions.push_back({ dup_x1, 0, "dup_x1", DefaultPrinter });
	instructions.push_back({ dup_x2, 0, "dup_x2", DefaultPrinter });
	instructions.push_back({ dup2, 0, "dup2", DefaultPrinter });
	instructions.push_back({ dup2_x1, 0, "dup2_x1", DefaultPrinter });
	instructions.push_back({ dup2_x2, 0, "dup2_x2", DefaultPrinter });
	instructions.push_back({ f2d, 0, "f2d", DefaultPrinter });
	instructions.push_back({ f2i, 0, "f2i", DefaultPrinter });
	instructions.push_back({ f2l, 0, "f2l", DefaultPrinter });
	instructions.push_back({ fadd, 0, "fadd", DefaultPrinter });
	instructions.push_back({ faload, 0, "faload", DefaultPrinter });
	instructions.push_back({ fastore, 0, "fastore", DefaultPrinter });
	instructions.push_back({ fconst_0, 0, "fconst_0", DefaultPrinter });
	instructions.push_back({ fconst_1, 0, "fconst_1", DefaultPrinter });
	instructions.push_back({ fconst_2, 0, "fconst_2", DefaultPrinter });
	instructions.push_back({ fdiv, 0, "fdiv", DefaultPrinter });
	instructions.push_back({ fload, 1, "fload", DefaultPrinter });
	instructions.push_back({ fload_0, 0, "fload_0", DefaultPrinter });
	instructions.push_back({ fload_1, 0, "fload_1", DefaultPrinter });
	instructions.push_back({ fload_2, 0, "fload_2", DefaultPrinter });
	instructions.push_back({ fload_3, 0, "fload_3", DefaultPrinter });
	instructions.push_back({ fmul, 0, "fmul", DefaultPrinter });
	instructions.push_back({ fneg, 0, "fneg", DefaultPrinter });
	instructions.push_back({ frem, 0, "frem", DefaultPrinter });
	instructions.push_back({ freturn, 0, "freturn", DefaultPrinter });
	instructions.push_back({ fstore, 1, "fstore", DefaultPrinter });
	instructions.push_back({ fstore_0, 0, "fstore_0", DefaultPrinter });
	instructions.push_back({ fstore_1, 0, "fstore_1", DefaultPrinter });
	instructions.push_back({ fstore_2, 0, "fstore_2", DefaultPrinter });
	instructions.push_back({ fstore_3, 0, "fstore_3", DefaultPrinter });
	instructions.push_back({ fsub, 0, "fsub", DefaultPrinter });
	instructions.push_back({ getfield, 2, "getfield", ShortIndices });
	instructions.push_back({ getstatic, 2, "getstatic", ShortIndices });
	instructions.push_back({ i_goto, 2, "goto", ShortIndices });
	instructions.push_back({ goto_w, 4, "goto_w", ShortIndices }); // TODO: needs to be a wider printer
	instructions.push_back({ i2b, 0, "i2b", DefaultPrinter });
	instructions.push_back({ i2c, 0, "i2c", DefaultPrinter });
	instructions.push_back({ i2d, 0, "i2d", DefaultPrinter });
	instructions.push_back({ i2f, 0, "i2f", DefaultPrinter });
	instructions.push_back({ i2l, 0, "i2l", DefaultPrinter });
	instructions.push_back({ i2s, 0, "i2s", DefaultPrinter });
	instructions.push_back({ iadd, 0, "iadd", DefaultPrinter });
	instructions.push_back({ iaload, 0, "iaload", DefaultPrinter });
	instructions.push_back({ iand, 0, "iand", DefaultPrinter });
	instructions.push_back({ iastore, 0, "iastore", DefaultPrinter });
	instructions.push_back({ iconst_m1, 0, "iconst_m1", DefaultPrinter });
	instructions.push_back({ iconst_0, 0, "iconst_0", DefaultPrinter });
	instructions.push_back({ iconst_1, 0, "iconst_1", DefaultPrinter });
	instructions.push_back({ iconst_2, 0, "iconst_2", DefaultPrinter });
	instructions.push_back({ iconst_3, 0, "iconst_3", DefaultPrinter });
	instructions.push_back({ iconst_4, 0, "iconst_4", DefaultPrinter });
	instructions.push_back({ iconst_5, 0, "iconst_5", DefaultPrinter });
	instructions.push_back({ idiv, 0, "idiv", DefaultPrinter });
	instructions.push_back({ iinc, 2, "iinc", DefaultPrinter });
	instructions.push_back({ iload, 1, "iload", DefaultPrinter });
	instructions.push_back({ iload_0, 0, "iload_0", DefaultPrinter });
	instructions.push_back({ iload_1, 0, "iload_1", DefaultPrinter });
	instructions.push_back({ iload_2, 0, "iload_2", DefaultPrinter });
	instructions.push_back({ iload_3, 0, "iload_3", DefaultPrinter });
	instructions.push_back({ imul, 0, "imul", DefaultPrinter });
	instructions.push_back({ ineg, 0, "ineg", DefaultPrinter });
	instructions.push_back({ instanceof, 2, "instanceof", ShortIndices });
	instructions.push_back({ invokedynamic, 4, "invokedynamic", ShortIndices }); // TODO: Investigate 0 0
	instructions.push_back({ invokeinterface, 4, "invokeinterface", DefaultPrinter }); // TODO: Investigate
	instructions.push_back({ invokespecial, 2, "invokespecial", ShortIndices });
	instructions.push_back({ ior, 0, "ior", DefaultPrinter });
	instructions.push_back({ irem, 0, "irem", DefaultPrinter });
	instructions.push_back({ ior, 0, "ior", DefaultPrinter });
	instructions.push_back({ ireturn, 0, "ireturn", DefaultPrinter });
	instructions.push_back({ ishr, 0, "ishr", DefaultPrinter });
	instructions.push_back({ istore, 1, "istore", DefaultPrinter });
	instructions.push_back({ isub, 0, "isub", DefaultPrinter });
	instructions.push_back({ iushr, 0, "iushr", DefaultPrinter });
	instructions.push_back({ ixor, 0, "ixor", DefaultPrinter });
	instructions.push_back({ jsr, 2, "jsr", ShortIndices });
	instructions.push_back({ jsr_w, 0, "jsr_w", ShortIndices }); // TODO: Use wide printer
	instructions.push_back({ l2d, 0, "l2d", DefaultPrinter });
	instructions.push_back({ l2f, 0, "l2f", DefaultPrinter });
	instructions.push_back({ l2i, 0, "l2i", DefaultPrinter });
	instructions.push_back({ ladd, 0, "ladd", DefaultPrinter });
	instructions.push_back({ laload, 0, "laload", DefaultPrinter });
	instructions.push_back({ land, 0, "land", DefaultPrinter });
	instructions.push_back({ lastore, 0, "lastore", DefaultPrinter });
	instructions.push_back({ lconst_0, 0, "lconst_0", DefaultPrinter });
	instructions.push_back({ lconst_1, 0, "lconst_1", DefaultPrinter });
	instructions.push_back({ i_ldiv, 0, "ldiv", DefaultPrinter });
	instructions.push_back({ lload, 1, "lload", DefaultPrinter });
	instructions.push_back({ lload_0, 0, "lload_0", DefaultPrinter });
	instructions.push_back({ lload_1, 0, "lload_1", DefaultPrinter });
	instructions.push_back({ lload_2, 0, "lload_2", DefaultPrinter });
	instructions.push_back({ lload_3, 0, "lload_3", DefaultPrinter });
	instructions.push_back({ lmul, 0, "lmul", DefaultPrinter });
	instructions.push_back({ lneg, 0, "lneg", DefaultPrinter });
	// TODO: implementlookupswitch
	instructions.push_back({ lor, 0, "lor", DefaultPrinter });
	instructions.push_back({ lrem, 0, "lrem", DefaultPrinter });
	instructions.push_back({ lreturn, 0, "lreturn", DefaultPrinter });
	instructions.push_back({ lshl, 0, "lshl", DefaultPrinter });
	instructions.push_back({ lshr, 0, "lshr", DefaultPrinter });
	instructions.push_back({ lstore, 1, "lstore", DefaultPrinter });
	instructions.push_back({ lstore_0, 0, "lstore_0", DefaultPrinter });
	instructions.push_back({ lstore_1, 0, "lstore_1", DefaultPrinter });
	instructions.push_back({ lstore_2, 0, "lstore_2", DefaultPrinter });
	instructions.push_back({ lstore_3, 0, "lstore_3", DefaultPrinter });
	instructions.push_back({ lsub, 0, "lsub", DefaultPrinter });
	instructions.push_back({ lushr, 0, "lushr", DefaultPrinter });
	instructions.push_back({ lxor, 0, "lxor", DefaultPrinter });
	instructions.push_back({ monitorenter, 0, "monitorenter", DefaultPrinter });
	instructions.push_back({ monitorexit, 0, "monitorexit", DefaultPrinter });
	instructions.push_back({ multianewarray, 3, "multianewarray", DefaultPrinter }); // Look into custom printer
	instructions.push_back({ newarray, 1, "newarray", DefaultPrinter }); // TODO: Look into printing the type
	instructions.push_back({ nop, 0, "nop", DefaultPrinter });
	instructions.push_back({ pop, 0, "pop", DefaultPrinter });
	instructions.push_back({ pop2, 0, "pop2", DefaultPrinter });
	instructions.push_back({ putstatic, 2, "putstatic", ShortIndices });
	instructions.push_back({ ret, 1, "ret", DefaultPrinter });
	instructions.push_back({ saload, 0, "saload", DefaultPrinter });
	instructions.push_back({ sastore, 0, "sastore", DefaultPrinter });
	instructions.push_back({ sipush, 2, "sipush", ShortIndices });
	instructions.push_back({ swap, 0, "swap", DefaultPrinter });
	// TODO: Add custom printer for tableswitch
	// TODO: Add custom printer for wide

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
		std::cout << " = " << std::left << std::setfill(' ') << std::setw(15) << getTypeAsString(item->getType())
			<< std::left << std::setfill(' ') << std::setw(15) << item->toString();
		std::string expanded = item->toExpandedString(cp);
		if (expanded.size() > 0) {
			std::cout << "// " << expanded;
		}
		std::cout << std::endl;
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
