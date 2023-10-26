#include "ClassPrinter.h"

#include "Util.h"

static const std::string ACC_SYNCHRONIZED_STR = "ACC_SYNCHRONIZED";

const std::string& ClassPrinter::getTypeAsString(ConstantType type) const {
	if (constantTypes.find(type) != constantTypes.end()) {
		return constantTypes.at(type);
	}
	else {
		return Unknown;
	}
}

const std::string& ClassPrinter::getTypeAsString(AccessFlag flag, AccessFlagType type) const
{
	if (accessFlags.find(flag) != accessFlags.end()) {
		const std::string& str = accessFlags.at(flag);
		if (type == METHOD && flag == ACC_SYNCHRONIZED) {
			return ACC_SYNCHRONIZED_STR;
		}
		return str;
	}
	else {
		return Unknown;
	}
}

std::string ClassPrinter::getAsExternalReturnType(std::string returnType)
{
	std::stringstream output;
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
			output << c;
		}
		else if (c == '[') {
			arrCount++;
		}
		else {
			output << types.at(c);
		}
	}

	for (int curr = 0; curr < arrCount; curr++) {
		output << "[]";
	}

	return output.str();
}

std::string ClassPrinter::getAsExternalClassName(std::string className)
{
	std::replace(className.begin(), className.end(), '/', '.');
	return className;
}

void ClassPrinter::printField(const FieldInfo* fieldInfo, const ConstantPool* cp)
{
	std::string descriptor = cp->getString(fieldInfo->descriptorIndex);
	std::string name = cp->getString(fieldInfo->nameIndex);
	std::cout << "  ";
	if (fieldInfo->isPrivate) {
		std::cout << "private ";
	}
	std::cout << getAsExternalClassName(getAsExternalReturnType(descriptor)) << " " << name << ";" << std::endl;
	std::cout << "    descriptor: " << descriptor << std::endl;
	std::cout << "    flags: ";
	std::vector<std::string> flags;
	for (AccessFlag flag : fieldInfo->getAccessFlags()) {
		flags.push_back(getTypeAsString(flag, FIELD));
	}
	std::cout << joinStrings(flags, ", ") << std::endl;
	std::cout << std::endl;
}

void ClassPrinter::printMethodSignature(const MethodInfo* methodInfo, const ClassInfo& classInfo, const ConstantPool* cp)
{
	if (!methodInfo->isConstructor) {
		std::cout << getAsExternalClassName(getAsExternalReturnType(methodInfo->returnType)) << " ";
	}

	if (methodInfo->isConstructor) {
		// TODO: Reuse classname
		const CPClassInfo* classPtr = cp->getClassInfo(classInfo.thisClass);
		const std::string className = cp->getString(classPtr->nameIndex);
		std::cout << className;
	}
	else
	{
		std::cout << cp->getString(methodInfo->nameIndex);
	}
	std::cout << "(";

	std::vector<std::string> args;

	for (int currentArg = 0; currentArg < methodInfo->argsCount; ++currentArg) {
		std::string arg = methodInfo->args[currentArg];
		args.push_back(getAsExternalReturnType(arg));
	}

	std::cout << getAsExternalClassName(joinStrings(args, ","));

	std::cout << ");" << std::endl;
}

void ClassPrinter::printMethod(const MethodInfo* methodInfo, const ClassInfo& classInfo, const ConstantPool* cp)
{
	std::cout << "  ";
	// TODO: Maybe we can print these based on a lookup table and in a fixed order, e.g. does the flags list match?
	if (methodInfo->isPublic) {
		std::cout << "public ";
	}
	if (methodInfo->isStatic) {
		std::cout << "static ";
	}
	if (methodInfo->isNative) {
		std::cout << "native ";
	}
	if (methodInfo->isAbstract) {
		std::cout << "abstract ";
	}
	printMethodSignature(methodInfo, classInfo, cp);
	std::cout << "    descriptor: " << cp->getString(methodInfo->descriptorIndex) << std::endl;

	std::cout << "    flags: ";
	std::vector<std::string> flags;
	for (AccessFlag flag : methodInfo->getAccessFlags()) {
		flags.push_back(getTypeAsString(flag, METHOD));
	}
	std::cout << joinStrings(flags, ", ") << std::endl;

	if (methodInfo->isNative || methodInfo->isAbstract) {
	}
	else {
		std::cout << "    Code: " << std::endl;
		printCode(methodInfo->code, methodInfo, cp);
	}
	std::cout << std::endl;
}

void SignedBytePrinter(std::vector<uint8_t> args, const ConstantPool* cp)
{
	for (uint8_t arg : args) {
		int8_t signedInt = *reinterpret_cast<int8_t*>(&arg);
		std::cout << " " << std::to_string(signedInt);
	}
}

void UnsignedBytePrinter(std::vector<uint8_t> args, const ConstantPool* cp)
{
	for (uint8_t arg : args) {
		std::cout << " " << std::to_string(arg);
	}
}

void SignedShort(std::vector<uint8_t> args, const ConstantPool* cp)
{
	for (int i = 0; i < args.size(); i += 2) {
		uint8_t byte1 = args[i];
		uint8_t byte2 = args[i + 1];

		int16_t shortIndex = (byte1 << 8) | byte2;

		std::cout << " " << (unsigned int)shortIndex;
	}
}

void ByteIndices(std::vector<uint8_t> args, const ConstantPool* cp)
{
	for (uint8_t arg : args) {
		std::cout << " #" << (unsigned int)arg;
	}
}

void ShortIndices(std::vector<uint8_t> args, const ConstantPool* cp)
{
	for (int i = 0; i < args.size(); i += 2) {
		uint8_t byte1 = args[i];
		uint8_t byte2 = args[i + 1];

		uint16_t shortIndex = (byte1 << 8) | byte2;

		std::cout << " #" << (unsigned int)shortIndex;
	}
}

void ArrayTypePrinter(std::vector<uint8_t> args, const ConstantPool* cp)
{
	std::string type;
	uint8_t arg = args[0];
	switch (arg) {
	case 4:
		type = T_BOOLEAN;
		break;
	case 5:
		type = T_CHAR;
		break;
	case 6:
		type = T_FLOAT;
		break;
	case 7:
		type = T_DOUBLE;
		break;
	case 8:
		type = T_BYTE;
		break;
	case 9:
		type = T_SHORT;
		break;
	case 10:
		type = T_INT;
		break;
	case 11:
		type = T_LONG;
		break;
	default:
		type = "(unknown)";
		break;
	}

	std::cout << " " << type;
}

void MultiArrayPrinter(std::vector<uint8_t> args, const ConstantPool* cp)
{
	uint8_t byte1 = args[0];
	uint8_t byte2 = args[1];
	uint8_t dimensions = args[2];

	uint16_t shortIndex = (byte1 << 8) | byte2;

	std::cout << " #" << (unsigned int)shortIndex << ",  " << (unsigned int)dimensions;
}

void ClassPrinter::printCode(const AttributeCode* code, const MethodInfo* method, const ConstantPool* cp)
{
	int argsSize = method->argsCount;
	if (!method->isStatic) {
		argsSize++;
	}
	std::cout << "      stack=" << code->maxStack << ", "
		<< "locals=" << code->maxLocals
		<< ", args_size=" << std::to_string(argsSize)
		<< std::endl;

	ByteArray byteArray(code->code, code->codeLength);

	while (!byteArray.atEnd()) {
		uint8_t opcode = byteArray.readUnsignedByte();
		bool found = false;
		for (const Instruction& instruction : this->instructions) {
			if (((uint8_t)instruction.opcode) == opcode) {
				std::string indexStr = std::to_string(byteArray.getPtr()) + ": ";
				std::cout << std::right << std::setfill(' ') << std::setw(12) << indexStr
					<< std::left << std::setfill(' ') << std::setw(13) << instruction.name;

				std::vector<uint8_t> args;
				if (instruction.args > 0) {
					for (int arg = 0; arg < instruction.args; arg++) {
						args.push_back(byteArray.readUnsignedByte());
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

				uint32_t instructionIndex = byteArray.getPtr();
				// Next byte is 4 byte aligned
				while ((byteArray.getPtr() + 1) % 4 != 0) {
					byteArray.readUnsignedByte();
				}

				int32_t defaultOffset = byteArray.readSignedInt();
				int32_t defaultAddress = instructionIndex + defaultOffset;
				int32_t nPairs = byteArray.readSignedInt();

				std::string indexStr = std::to_string(instructionIndex) + ": ";
				std::cout << std::right << std::setfill(' ') << std::setw(12) << indexStr
					<< std::left << std::setfill(' ') << std::setw(13) << "lookupswitch";

				std::cout << " { // " << nPairs << std::endl;

				for (int32_t currentPair = 0; currentPair < nPairs; currentPair++) {
					int32_t matchKey = byteArray.readSignedInt();
					int32_t offset = byteArray.readSignedInt();

					std::cout << std::right << std::setfill(' ') << std::setw(24) << (matchKey);

					std::cout << ": " << (instructionIndex + offset);
					std::cout << std::endl;
				}

				std::cout << std::right << std::setfill(' ') << std::setw(24) << "default";
				std::cout << ": " << (defaultAddress) << std::endl;
				std::cout << "            }";

				found = true;
			}

		}
		if (!found) {
			std::cout << "Instruction with opcode: " << (int)opcode << " unrecognized" << std::endl;
			exit(1);
		}
		std::cout << std::endl;
	}

	for (size_t currentAttrib = 0; currentAttrib < code->attributes->attributesCount; ++currentAttrib) {
		AttributeInfo* att = code->attributes->attributes[currentAttrib];
		if (att != 0) {
			std::cout << "      " << cp->getString(att->attributeNameIndex) << ":" << std::endl;
			std::cout << att->toString(cp);
		}
	}

}

ClassPrinter::ClassPrinter()
{
	instructions.push_back({ nop, 0, "nop" });
	instructions.push_back({ bipush, 1, "bipush", SignedBytePrinter });
	instructions.push_back({ istore_0, 0, "istore_0" });
	instructions.push_back({ istore_1, 0, "istore_1" });
	instructions.push_back({ istore_2, 0, "istore_2" });
	instructions.push_back({ istore_3, 0, "istore_3" });
	instructions.push_back({ i_return, 0, "return" });
	instructions.push_back({ i_new, 2, "new", ShortIndices });
	instructions.push_back({ dup, 0, "dup" });
	instructions.push_back({ invokespecial, 2, "invokespecial", ShortIndices });
	instructions.push_back({ aaload, 0, "aaload" });
	instructions.push_back({ aastore, 0, "aastore" });
	instructions.push_back({ aconst_null, 0, "aconst_null" });
	instructions.push_back({ aload, 1, "aload", UnsignedBytePrinter });
	instructions.push_back({ aload_0, 0, "aload_0" });
	instructions.push_back({ aload_1, 0, "aload_1" });
	instructions.push_back({ aload_2, 0, "aload_2" });
	instructions.push_back({ aload_3, 0, "aload_3" });
	instructions.push_back({ anewarray, 2, "anewarray", ShortIndices });
	instructions.push_back({ areturn, 0, "areturn" });
	instructions.push_back({ aload_1, 0, "aload_1" });
	instructions.push_back({ arraylength, 0, "arraylength" });
	instructions.push_back({ astore, 1, "astore", UnsignedBytePrinter });
	instructions.push_back({ astore_0, 0, "astore_0" });
	instructions.push_back({ astore_1, 0, "astore_1" });
	instructions.push_back({ astore_2, 0, "astore_2" });
	instructions.push_back({ astore_3, 0, "astore_3" });
	instructions.push_back({ aload_0, 0, "aload_0" });
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
	instructions.push_back({ dload, 1, "dload", UnsignedBytePrinter });
	instructions.push_back({ dload_0, 0, "dload_0" });
	instructions.push_back({ dload_1, 0, "dload_1" });
	instructions.push_back({ dload_2, 0, "dload_2" });
	instructions.push_back({ dload_3, 0, "dload_3" });
	instructions.push_back({ dmul, 0, "dmul" });
	instructions.push_back({ dneg, 0, "dneg" });
	instructions.push_back({ drem, 0, "drem" });
	instructions.push_back({ dreturn, 0, "dreturn" });
	instructions.push_back({ dstore, 1, "dstore", UnsignedBytePrinter });
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
	instructions.push_back({ fload, 1, "fload", UnsignedBytePrinter });
	instructions.push_back({ fload_0, 0, "fload_0" });
	instructions.push_back({ fload_1, 0, "fload_1" });
	instructions.push_back({ fload_2, 0, "fload_2" });
	instructions.push_back({ fload_3, 0, "fload_3" });
	instructions.push_back({ fmul, 0, "fmul" });
	instructions.push_back({ fneg, 0, "fneg" });
	instructions.push_back({ frem, 0, "frem" });
	instructions.push_back({ freturn, 0, "freturn" });
	instructions.push_back({ fstore, 1, "fstore", UnsignedBytePrinter });
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
	instructions.push_back({ iload, 1, "iload", UnsignedBytePrinter });
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
	instructions.push_back({ istore, 1, "istore", UnsignedBytePrinter });
	instructions.push_back({ istore_0, 0, "istore_0" });
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
	instructions.push_back({ lload, 1, "lload", UnsignedBytePrinter });
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
	instructions.push_back({ lstore, 1, "lstore", UnsignedBytePrinter });
	instructions.push_back({ lstore_0, 0, "lstore_0" });
	instructions.push_back({ lstore_1, 0, "lstore_1" });
	instructions.push_back({ lstore_2, 0, "lstore_2" });
	instructions.push_back({ lstore_3, 0, "lstore_3" });
	instructions.push_back({ lsub, 0, "lsub" });
	instructions.push_back({ lushr, 0, "lushr" });
	instructions.push_back({ lxor, 0, "lxor" });
	instructions.push_back({ monitorenter, 0, "monitorenter" });
	instructions.push_back({ monitorexit, 0, "monitorexit" });
	instructions.push_back({ multianewarray, 3, "multianewarray", MultiArrayPrinter });
	instructions.push_back({ newarray, 1, "newarray", ArrayTypePrinter });
	instructions.push_back({ nop, 0, "nop" });
	instructions.push_back({ pop, 0, "pop" });
	instructions.push_back({ pop2, 0, "pop2" });
	instructions.push_back({ putstatic, 2, "putstatic", ShortIndices });
	instructions.push_back({ ret, 1, "ret", SignedBytePrinter });
	instructions.push_back({ saload, 0, "saload" });
	instructions.push_back({ sastore, 0, "sastore" });
	instructions.push_back({ sipush, 2, "sipush", SignedShort });
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

	const ConstantPool* const cp = classInfo.constantPool;
	const CPClassInfo* const classPtr = cp->getClassInfo(classInfo.thisClass);
	const CPClassInfo* const superClassPtr = cp->getClassInfo(classInfo.superClass);

	if (classInfo.isPublic) {
		std::cout << "public ";
	}

	std::cout << "class " << cp->getString(classPtr->nameIndex);

	std::string superClassName = cp->getString(superClassPtr->nameIndex);
	if (superClassName != "java/lang/Object") {
		std::cout << " extends " << getAsExternalClassName(cp->getString(superClassPtr->nameIndex));
	}

	if (classInfo.interfacesCount > 0) {
		std::cout << " implements ";

		//std::string *names = (std::string*) malloc(sizeof(std::string) * classInfo.interfacesCount);
		std::string* names = new std::string[classInfo.interfacesCount];

		for (size_t currentIndex = 0; currentIndex < classInfo.interfacesCount; ++currentIndex) {
			uint16_t index = classInfo.interfaces[currentIndex];
			CPClassInfo* classPtr = cp->getClassInfo(index);
			names[currentIndex] = std::string(cp->getString(classPtr->nameIndex));
		}

		std::cout << getAsExternalClassName(joinStrings(names, classInfo.interfacesCount, ", "));

		delete[] names;

	}
	std::cout << std::endl;

	std::cout << "  minor version:" << " " << classInfo.minorVersion << std::endl;
	std::cout << "  major version:" << " " << classInfo.majorVersion << std::endl;

	std::cout << "  flags: ";
	std::vector<std::string> flags;
	for (AccessFlag flag : classInfo.getAccessFlags()) {
		flags.push_back(getTypeAsString(flag, CLASS));
	}
	std::cout << joinStrings(flags, ", ") << std::endl;

	std::cout << "Constant pool:" << std::endl;
	int current = 1;
	for (ConstantPoolItem* item : cp->constants) {
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
	for (uint16_t currentField = 0; currentField < classInfo.fieldsCount; ++currentField) {
		FieldInfo* fieldInfo = classInfo.fields[currentField];
		printField(fieldInfo, cp);
	}

	// Methods
	for (uint16_t currentMethod = 0; currentMethod < classInfo.methodCount; ++currentMethod) {
		MethodInfo* methodInfo = classInfo.methods[currentMethod];
		printMethod(methodInfo, classInfo, cp);
	}

	std::cout << "}" << std::endl;

	for (size_t currentAttrib = 0; currentAttrib < classInfo.attributes->attributesCount; ++currentAttrib) {
		AttributeInfo* att = classInfo.attributes->attributes[currentAttrib];
		std::cout << att->toString(cp);
	}
}
