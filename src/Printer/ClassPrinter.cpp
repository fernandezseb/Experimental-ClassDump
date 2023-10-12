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
	std::string output = "";
	int arrCount = 0;
	bool inClass = false;

	for (char c : returnType) {
		if (c == 'L') {
			inClass = true;
		}
		else if (c == ';') {
			inClass = false;
		}
		else if (inClass) {
			output += c;
		}
		else if (c == '[') {
			arrCount++;
		}
		else if (c == 'V') {
			output += "void";
		}
		else if (c == 'B') {
			output += "byte";
		}
		else if (c == 'C') {
			output += "char";
		}
		else if (c == 'D') {
			output += "double";
		}
		else if (c == 'F') {
			output += "float";
		}
		else if (c == 'I') {
			output += "int";
		}
		else if (c == 'J') {
			output += "long";
		}
		else if (c == 'S') {
			output += "short";
		}
		else if (c == 'Z') {
			output += "boolean";
		}
		else if (c == 'D') {
			output += "double";
		}
	}

	for (int curr = 0; curr < arrCount; curr++) {
		output += "[]";
	}

	return output;
}

std::string ClassPrinter::getAsExternalClassName(std::string className)
{
	std::replace(className.begin(), className.end(), '/', '.');
	return className;
}

void ClassPrinter::printField(const FieldInfo& fieldInfo, const ConstantPool& cp)
{
	std::string descriptor = cp.getString(fieldInfo.descriptorIndex);
	std::string name = cp.getString(fieldInfo.nameIndex);
	std::cout << "  ";
	if (fieldInfo.isPrivate) {
		std::cout << "private ";
	}
	std::cout << getAsExternalClassName(getAsExternalReturnType(descriptor)) << " " << name << ";" << std::endl;
	std::cout << "    descriptor: " << descriptor << std::endl;
	std::cout << "    flags: ";
	std::vector<std::string> flags;
	for (AccessFlag flag : fieldInfo.getAccessFlags()) {
		flags.push_back(getTypeAsString(flag));
	}
	std::cout << joinStrings(flags, ", ") << std::endl;
	std::cout << std::endl;
}

void ClassPrinter::printMethodSignature(const MethodInfo& methodInfo, const ClassInfo& classInfo, const ConstantPool& cp)
{
	if (!methodInfo.isConstructor) {
		std::cout << getAsExternalClassName(getAsExternalReturnType(methodInfo.returnType)) << " ";
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

	std::vector<std::string> args;

	for (std::string arg : methodInfo.args) {
		args.push_back(getAsExternalReturnType(arg));
	}

	std::cout << getAsExternalClassName(joinStrings(args, ","));

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
	
	std::cout << "    flags: ";
	std::vector<std::string> flags;
	for (AccessFlag flag : methodInfo.getAccessFlags()) {
		flags.push_back(getTypeAsString(flag));
	}
	std::cout << joinStrings(flags, ", ") << std::endl;

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
}

void SignedBytePrinter(std::vector<uint8_t> args, const ConstantPool& cp)
{
	for (uint8_t arg : args) {
		int8_t signedInt = *reinterpret_cast<int8_t*>(&arg);
		std::cout << " " << std::to_string(signedInt);
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
	int argsSize = method->args.size();
	if (!method->isStatic) {
		argsSize++;
	}
	std::cout << "      stack=" << code->maxStack << ", " 
		<< "locals=" << code->maxLocals
		<< ", args_size=" << std::to_string(argsSize)
		<< std::endl;

	for (uint32_t index = 0; index < code->codeLength; index++) {
		uint8_t opcode = code->code[index];
		bool found = false;
		for (const Instruction& instruction : this->instructions) {
			if (((uint8_t)instruction.opcode) == opcode) {
				std::string indexStr = std::to_string(index) + ": ";
				std::cout << std::right << std::setfill(' ') << std::setw(12) << indexStr 
					<< std::left << std::setfill(' ') << std::setw(13) << instruction.name;
				
				std::vector<uint8_t> args;
				if (instruction.args > 0) {
					for (int arg = 0; arg < instruction.args; arg++) {
						args.push_back(code->code[++index]);
					}
				}
				if (instruction.printFunction != NULL) {
					instruction.printFunction(args, cp);
				}
				
				found = true;
				break;
			}
		}
		if (!found) {
			if (opcode == 0xab) {

				uint8_t instructionIndex = index;
				// std::cout << "Current index is: " <<  (unsigned int) index + 1 << std::endl;
				++index;
				// Next byte is 4 byte aligned
				while (index % 4 != 0) {
					index++;
				}
				// std::cout << "Next index is: " << (unsigned int)index << std::endl;
				uint8_t buffer[4] = { code->code[index++], code->code[index++] , code->code[index++] , code->code[index++] };
				int32_t defaultOffset = (int32_t)buffer[3] | (int32_t)buffer[2] << 8 | (int32_t)buffer[1] << 16 | (int32_t)buffer[0] << 24;
				int32_t defaultAddress = instructionIndex + defaultOffset;
				// std::cout << "default is: " << defaultAddress << std::endl;

				int8_t buffer2[4] = {code->code[index++], code->code[index++] , code->code[index++] , code->code[index++]};
				int32_t nPairs = (int32_t)buffer2[3] | (int32_t)buffer2[2] << 8 | (int32_t)buffer2[1] << 16 | (int32_t)buffer2[0] << 24;
				// std::cout << "The amount of npairs is: " << nPairs << std::endl;

				std::string indexStr = std::to_string(instructionIndex) + ": ";
				std::cout << std::right << std::setfill(' ') << std::setw(12) << indexStr
					<< std::left << std::setfill(' ') << std::setw(13) << "lookupswitch";

				std::cout << " { // " << nPairs << std::endl;

				for (int32_t currentPair = 0; currentPair < nPairs; currentPair++) {
					uint8_t buffer[4] = { code->code[index++], code->code[index++] , code->code[index++] , code->code[index++] };
					int32_t matchKey = (int32_t)buffer[3] | (int32_t)buffer[2] << 8 | (int32_t)buffer[1] << 16 | (int32_t)buffer[0] << 24;

					int8_t buffer2[4] = { code->code[index++], code->code[index++] , code->code[index++] , code->code[index++] };
					int32_t offset = (int32_t)buffer2[3] | (int32_t)buffer2[2] << 8 | (int32_t)buffer2[1] << 16 | (int32_t)buffer2[0] << 24;
					//std::cout << "";

					std::cout << std::right << std::setfill(' ') << std::setw(24) << (matchKey);

					std::cout << ": " << (instructionIndex + offset);
					std::cout << std::endl;
				}

				std::cout << std::right << std::setfill(' ') << std::setw(24) << "default";

				std::cout << ": " << (defaultAddress) << std::endl;

				std::cout << "            }";

				// We don't want to be on the next instruction yet
				index -= 1;

				found = true;
			}

		}
		if (!found) {
			std::cout << "Instruction with opcode: " << (int)opcode << " unrecognized" << std::endl;
			exit(1);
		}
		std::cout << std::endl;
	}


	for (const AttributeInfo* att : code->attributes.attributes) {
		std::cout << "      " << cp.getString(att->attributeNameIndex) << ":" << std::endl;
		std::cout << att->toString(cp);
	}

}

ClassPrinter::ClassPrinter()
{
	instructions.push_back({nop, 0, "nop"});
	instructions.push_back({ bipush, 1, "bipush", SignedBytePrinter}); // SIGN EXTENDED
	instructions.push_back({ istore_0, 0, "istore_0"});
	instructions.push_back({ istore_1, 0, "istore_1"});
	instructions.push_back({ istore_2, 0, "istore_2"});
	instructions.push_back({ istore_3, 0, "istore_3"});
	instructions.push_back({ i_return, 0, "return"});
	instructions.push_back({ i_new, 2, "new", ShortIndices});
	instructions.push_back({ dup, 0, "dup"});
	instructions.push_back({ invokespecial, 2, "invokespecial", ShortIndices});
	instructions.push_back({ aaload, 0, "aaload"});
	instructions.push_back({ aastore, 0, "aastore"});
	instructions.push_back({ aconst_null, 0, "aconst_null" });
	instructions.push_back({ aload, 1, "aload", SignedBytePrinter }); // UNSIGNED BYTE INDEX TO LOCAL VARIABLE
	instructions.push_back({ aload_0, 0, "aload_0"});
	instructions.push_back({ aload_1, 0, "aload_1"});
	instructions.push_back({ aload_2, 0, "aload_2" });
	instructions.push_back({ aload_3, 0, "aload_3" });
	instructions.push_back({ anewarray, 2, "anewarray", ShortIndices });
	instructions.push_back({ areturn, 0, "areturn"});
	instructions.push_back({ aload_1, 0, "aload_1" });
	instructions.push_back({ arraylength, 0, "arraylength" });
	instructions.push_back({ astore, 1, "astore", SignedBytePrinter }); // UNSIGNED BYTE LOCAL VARIABLE
	instructions.push_back({ astore_0, 0, "astore_0" });
	instructions.push_back({ astore_1, 0, "astore_1"});
	instructions.push_back({ astore_2, 0, "astore_2" });
	instructions.push_back({ astore_3, 0, "astore_3" });
	instructions.push_back({ aload_0, 0, "aload_0"});
	instructions.push_back({ ldc, 1, "ldc", SignedBytePrinter });
	instructions.push_back({ ldc_w, 2, "ldc_w", ShortIndices });
	instructions.push_back({ ldc2_w, 2, "ldc2_w", ShortIndices });
	instructions.push_back({ invokevirtual, 2, "invokevirtual", ShortIndices });
	instructions.push_back({ invokestatic, 2, "invokestatic", ShortIndices });
	instructions.push_back({ athrow, 0, "athrow" });
	instructions.push_back({ baload, 0, "baload" });
	instructions.push_back({ bastore, 0, "bastore" });
	instructions.push_back({ caload, 0, "caload" });
	instructions.push_back({ castore, 0, "castore" });
	instructions.push_back({ checkcast, 2, "checkcast", ShortIndices });
	instructions.push_back({ d2f, 0, "d2f" });
	instructions.push_back({ d2i, 0, "d2i" });
	instructions.push_back({ d2l, 0, "d2l" });
	instructions.push_back({ dadd, 0, "dadd" });
	instructions.push_back({ daload, 0, "daload" });
	instructions.push_back({ dastore, 0, "dastore" });
	instructions.push_back({ dcmpl, 0, "dcmpl" });
	instructions.push_back({ dcmpg, 0, "dcmpg" });
	instructions.push_back({ dconst_0, 0, "dconst_0" });
	instructions.push_back({ dconst_1, 0, "dconst_1" });
	instructions.push_back({ ddiv, 0, "ddiv" });
	instructions.push_back({ dload, 1, "dload" }); // INDEX AND INDEX+1 IS UNSIGNED BYTE
	instructions.push_back({ dload_0, 0, "dload_0" });
	instructions.push_back({ dload_1, 0, "dload_1" });
	instructions.push_back({ dload_2, 0, "dload_2" });
	instructions.push_back({ dload_3, 0, "dload_3" });
	instructions.push_back({ dmul, 0, "dmul" });
	instructions.push_back({ dneg, 0, "dneg" });
	instructions.push_back({ drem, 0, "drem" });
	instructions.push_back({ dreturn, 0, "dreturn" });
	instructions.push_back({ dstore, 1, "dstore", SignedBytePrinter }); // UNSIGNED BYTE INDEX AND INDEX+1
	instructions.push_back({ dstore_0, 0, "dstore_0" });
	instructions.push_back({ dstore_1, 0, "dstore_1" });
	instructions.push_back({ dstore_2, 0, "dstore_2" });
	instructions.push_back({ dstore_3, 0, "dstore_3" });
	instructions.push_back({ dsub, 0, "dsub" });
	instructions.push_back({ dup, 0, "dup" });
	instructions.push_back({ dup_x1, 0, "dup_x1" });
	instructions.push_back({ dup_x2, 0, "dup_x2" });
	instructions.push_back({ dup2, 0, "dup2" });
	instructions.push_back({ dup2_x1, 0, "dup2_x1" });
	instructions.push_back({ dup2_x2, 0, "dup2_x2" });
	instructions.push_back({ f2d, 0, "f2d" });
	instructions.push_back({ f2i, 0, "f2i" });
	instructions.push_back({ f2l, 0, "f2l" });
	instructions.push_back({ fadd, 0, "fadd" });
	instructions.push_back({ faload, 0, "faload" });
	instructions.push_back({ fastore, 0, "fastore" });
	instructions.push_back({ fcmpl, 0, "fcmpl" });
	instructions.push_back({ fcmpg, 0, "fcmpg" });
	instructions.push_back({ fconst_0, 0, "fconst_0" });
	instructions.push_back({ fconst_1, 0, "fconst_1" });
	instructions.push_back({ fconst_2, 0, "fconst_2" });
	instructions.push_back({ fdiv, 0, "fdiv" });
	instructions.push_back({ fload, 1, "fload", SignedBytePrinter }); // UNSIGNED BYTE: LOCAL VARIABLE
	instructions.push_back({ fload_0, 0, "fload_0" });
	instructions.push_back({ fload_1, 0, "fload_1" });
	instructions.push_back({ fload_2, 0, "fload_2" });
	instructions.push_back({ fload_3, 0, "fload_3" });
	instructions.push_back({ fmul, 0, "fmul" });
	instructions.push_back({ fneg, 0, "fneg" });
	instructions.push_back({ frem, 0, "frem" });
	instructions.push_back({ freturn, 0, "freturn" });
	instructions.push_back({ fstore, 1, "fstore", SignedBytePrinter }); // UNSIGNED BYTE LOCAL VARIABLE
	instructions.push_back({ fstore_0, 0, "fstore_0" });
	instructions.push_back({ fstore_1, 0, "fstore_1" });
	instructions.push_back({ fstore_2, 0, "fstore_2" });
	instructions.push_back({ fstore_3, 0, "fstore_3" });
	instructions.push_back({ fsub, 0, "fsub" });
	instructions.push_back({ getfield, 2, "getfield", ShortIndices });
	instructions.push_back({ getstatic, 2, "getstatic", ShortIndices });
	// TODO: we must add the current address to the offset provided by the instruction (GOTO)
	instructions.push_back({ i_goto, 2, "goto", ShortIndices }); // SIGNED BRANCHOFFSET
	instructions.push_back({ goto_w, 4, "goto_w", ShortIndices }); // 32 bit SIGNED
	instructions.push_back({ i2b, 0, "i2b" });
	instructions.push_back({ i2c, 0, "i2c" });
	instructions.push_back({ i2d, 0, "i2d" });
	instructions.push_back({ i2f, 0, "i2f" });
	instructions.push_back({ i2l, 0, "i2l" });
	instructions.push_back({ i2s, 0, "i2s" });
	instructions.push_back({ iadd, 0, "iadd" });
	instructions.push_back({ iaload, 0, "iaload" });
	instructions.push_back({ iand, 0, "iand" });
	instructions.push_back({ iastore, 0, "iastore" });
	instructions.push_back({ iconst_m1, 0, "iconst_m1" });
	instructions.push_back({ iconst_0, 0, "iconst_0" });
	instructions.push_back({ iconst_1, 0, "iconst_1" });
	instructions.push_back({ iconst_2, 0, "iconst_2" });
	instructions.push_back({ iconst_3, 0, "iconst_3" });
	instructions.push_back({ iconst_4, 0, "iconst_4" });
	instructions.push_back({ iconst_5, 0, "iconst_5" });
	instructions.push_back({ idiv, 0, "idiv" });
	instructions.push_back({ if_acmpeq, 2, "if_acmpeq", SignedBytePrinter }); // SIGNED 16 bit offset 
	instructions.push_back({ if_acmpne, 2, "if_acmpne",SignedBytePrinter }); // SIGNED  16 bit offset
	instructions.push_back({ if_icmpeq, 2, "if_icmpeq",SignedBytePrinter }); // SIGNED  16 bit offset
	instructions.push_back({ if_icmpne, 2, "if_icmpne",SignedBytePrinter }); // SIGNED  16 bit offset
	instructions.push_back({ if_icmplt, 2, "if_icmplt",SignedBytePrinter }); // SIGNED  16 bit offset
	instructions.push_back({ if_icmpge, 2, "if_icmpge",SignedBytePrinter }); // SIGNED  16 bit offset
	instructions.push_back({ if_icmpgt, 2, "if_icmpgt",SignedBytePrinter }); // SIGNED  16 bit offset
	instructions.push_back({ if_icmple, 2, "if_icmple",SignedBytePrinter }); // SIGNED  16 bit offset
	instructions.push_back({ ifeq, 2, "ifeq",SignedBytePrinter }); // SIGNED  16 bit offset
	instructions.push_back({ ifne, 2, "ifne",SignedBytePrinter }); // SIGNED  16 bit offset
	instructions.push_back({ iflt, 2, "iflt",SignedBytePrinter }); // SIGNED  16 bit offset
	instructions.push_back({ ifge, 2, "ifge",SignedBytePrinter }); // SIGNED  16 bit offset
	instructions.push_back({ ifgt, 2, "ifgt",SignedBytePrinter }); // SIGNED  16 bit offset
	instructions.push_back({ ifle, 2, "ifle",SignedBytePrinter }); // SIGNED  16 bit offset
	instructions.push_back({ ifnonnull, 2, "ifnonnull",SignedBytePrinter }); // SIGNED  16 bit offset
	instructions.push_back({ ifnull, 2, "ifnull",SignedBytePrinter }); // SIGNED  16 bit offset
	instructions.push_back({ iinc, 2, "iinc", SignedBytePrinter }); // 1 UNSIGNED BYTE LOCAL VARIABLE AND SIGNED BYTE, we need to print both
	instructions.push_back({ iload, 1, "iload", SignedBytePrinter }); // UNSIGNED BYTE LOCAL VARIABLE
	instructions.push_back({ iload_0, 0, "iload_0" });
	instructions.push_back({ iload_1, 0, "iload_1" });
	instructions.push_back({ iload_2, 0, "iload_2" });
	instructions.push_back({ iload_3, 0, "iload_3" });
	instructions.push_back({ imul, 0, "imul" });
	instructions.push_back({ ineg, 0, "ineg" });
	instructions.push_back({ instanceof, 2, "instanceof", ShortIndices });
	instructions.push_back({ invokedynamic, 4, "invokedynamic", ShortIndices }); // TODO: Investigate 0 0
	instructions.push_back({ invokeinterface, 4, "invokeinterface", SignedBytePrinter }); // TODO: Investigate
	instructions.push_back({ invokespecial, 2, "invokespecial", ShortIndices });
	instructions.push_back({ ior, 0, "ior" });
	instructions.push_back({ irem, 0, "irem" });
	instructions.push_back({ ireturn, 0, "ireturn" });
	instructions.push_back({ ishl , 0, "ishl" });
	instructions.push_back({ ishr, 0, "ishr" });
	instructions.push_back({ istore, 1, "istore", SignedBytePrinter }); // UNSIGNED BYTE LOCAL VARIABLE
	instructions.push_back({ istore_0, 0, "istore_0"});
	instructions.push_back({ istore_1, 0, "istore_1" });
	instructions.push_back({ istore_2, 0, "istore_2" });
	instructions.push_back({ istore_3, 0, "istore_3" });
	instructions.push_back({ isub, 0, "isub" });
	instructions.push_back({ iushr, 0, "iushr" });
	instructions.push_back({ ixor, 0, "ixor" });
	instructions.push_back({ jsr, 2, "jsr", ShortIndices }); // SIGNED 16 bit address
	instructions.push_back({ jsr_w, 4, "jsr_w", ShortIndices }); // SIGNED 32 bit address
	instructions.push_back({ l2d, 0, "l2d" });
	instructions.push_back({ l2f, 0, "l2f" });
	instructions.push_back({ l2i, 0, "l2i" });
	instructions.push_back({ ladd, 0, "ladd" });
	instructions.push_back({ laload, 0, "laload" });
	instructions.push_back({ land, 0, "land", });
	instructions.push_back({ lastore, 0, "lastore" });
	instructions.push_back({ lcmp, 0, "lcmp" });
	instructions.push_back({ lconst_0, 0, "lconst_0" });
	instructions.push_back({ lconst_1, 0, "lconst_1" });
	instructions.push_back({ i_ldiv, 0, "ldiv" });
	instructions.push_back({ lload, 1, "lload", SignedBytePrinter }); // UNSIGNED BYTTE AND INDEX+1 UNSIGNED BYTE
	instructions.push_back({ lload_0, 0, "lload_0" });
	instructions.push_back({ lload_1, 0, "lload_1" });
	instructions.push_back({ lload_2, 0, "lload_2" });
	instructions.push_back({ lload_3, 0, "lload_3" });
	instructions.push_back({ lmul, 0, "lmul" });
	instructions.push_back({ lneg, 0, "lneg" });
	instructions.push_back({ lor, 0, "lor" });
	instructions.push_back({ lrem, 0, "lrem" });
	instructions.push_back({ lreturn, 0, "lreturn" });
	instructions.push_back({ lshl, 0, "lshl" });
	instructions.push_back({ lshr, 0, "lshr" });
	instructions.push_back({ lstore, 1, "lstore", SignedBytePrinter }); // INDEX AND INDEX+1 UNSIGNED BYTES
	instructions.push_back({ lstore_0, 0, "lstore_0" });
	instructions.push_back({ lstore_1, 0, "lstore_1" });
	instructions.push_back({ lstore_2, 0, "lstore_2" });
	instructions.push_back({ lstore_3, 0, "lstore_3" });
	instructions.push_back({ lsub, 0, "lsub" });
	instructions.push_back({ lushr, 0, "lushr" });
	instructions.push_back({ lxor, 0, "lxor" });
	instructions.push_back({ monitorenter, 0, "monitorenter" });
	instructions.push_back({ monitorexit, 0, "monitorexit" });
	instructions.push_back({ multianewarray, 3, "multianewarray", SignedBytePrinter }); // Look into custom printer
	instructions.push_back({ newarray, 1, "newarray", SignedBytePrinter }); // TODO: Look into printing the type
	instructions.push_back({ nop, 0, "nop" });
	instructions.push_back({ pop, 0, "pop" });
	instructions.push_back({ pop2, 0, "pop2" });
	instructions.push_back({ putstatic, 2, "putstatic", ShortIndices });
	instructions.push_back({ ret, 1, "ret", SignedBytePrinter });
	instructions.push_back({ saload, 0, "saload" });
	instructions.push_back({ sastore, 0, "sastore" });
	instructions.push_back({ sipush, 2, "sipush", ShortIndices }); // SIGNED 16 bit value
	instructions.push_back({ swap, 0, "swap" });
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

	if (classInfo.sourceFile != "") {
		std::cout << "  Compiled from \"" << classInfo.sourceFile << "\"" << std::endl;
	}

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

	std::cout << "  flags: ";
	std::vector<std::string> flags;
	for (AccessFlag flag : classInfo.getAccessFlags()) {
		flags.push_back(getTypeAsString(flag));
	}
	std::cout << joinStrings(flags, ", ") << std::endl;

	std::cout << "Constant pool:" << std::endl;
	int current = 1;
	for (ConstantPoolItem* item : cp.constants) {
		if (item == 0) {
			current++;
			continue;
		}
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

	for (AttributeInfo* att : classInfo.attributes.attributes) {
		std::cout << att->toString(cp);
	}
}
