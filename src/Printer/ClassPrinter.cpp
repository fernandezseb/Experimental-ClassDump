#include "ClassPrinter.h"

#include "AttributePrinter.h"

#include "Util.h"

const char* ClassPrinter::getTypeAsString(ConstantType type) const {
	if (constantTypes.find(type) != constantTypes.end()) {
		return constantTypes.at(type);
	}
	else {
		return Unknown;
	}
}

const char* ClassPrinter::getTypeAsString(AccessFlag flag, AccessFlagType type) const
{
	const std::map<AccessFlag, const char*>* flags = 0;
	switch (type) {
	case METHOD:
	{
		flags = &accessFlagsMethod;
		break;
	}
	case FIELD:
	{
		flags = &accessFlagsField;
		break;
	}
	case CLASS:
	{
		flags = &accessFlagsClass;
		break;
	}
	}
	if (flags != 0 && (flags->find(flag) != flags->end())) {
		const char* str = flags->at(flag);
		return str;
	}
	else {
		return Unknown;
	}
}

// TODO: Print to a char buffer that we get as param
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
			if (c == '/') {
				output << '.';
			}
			else {
				output << c;
			}
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

void ClassPrinter::printField(const FieldInfo* fieldInfo, const ConstantPool* cp, Memory* memory)
{
	const char* descriptor = cp->getString(fieldInfo->descriptorIndex);
	const char* name = cp->getString(fieldInfo->nameIndex);
	printf("  ");

	for (auto const& element : accessFlagsField) {
		if ((element.first & fieldInfo->accessFlags) != 0) {
			if (keywords.find(element.first) != keywords.end()) {
				printf("%s ", keywords.at(element.first));
			}
		}
	}


	std::cout << getAsExternalReturnType(descriptor)
		<< " " 
		<< name 
		<< ";" 
		<< std::endl;
	printf("    descriptor: %s\n", descriptor);
	printf("    flags: ");

	const char** flags = (const char**)memory->classAlloc(9 * sizeof(char*));
	int currentIndex = 0;
	for (auto const& element : accessFlagsMethod) {
		if ((element.first & fieldInfo->accessFlags) != 0) {
			flags[currentIndex] = getTypeAsString(element.first, FIELD);
			++currentIndex;
		}
	}
	char buffer[300];
	joinStrings((char**)flags, currentIndex, ", ", 300, buffer);
	printf("%s\n", buffer);


	printf("\n");
}

void ClassPrinter::printMethodSignature(
	const MethodInfo* methodInfo,
	const ClassInfo& classInfo,
	const char* className,
	const ConstantPool* cp,
	Memory* memory)
{
	if (!methodInfo->isConstructor()) {
		std::cout << getAsExternalReturnType(methodInfo->returnType) << " ";
	}

	if (methodInfo->isConstructor()) {
		printf("%s",className);
	}
	else
	{
		printf("%s", cp->getString(methodInfo->nameIndex));
	}
	printf("(");

	char** args = (char**)memory->classAlloc(sizeof(char*) * methodInfo->argsCount);
	for (int currentArg = 0; currentArg < methodInfo->argsCount; ++currentArg) {
		const char* arg = methodInfo->args[currentArg];
		std::string cppArg = getAsExternalReturnType(arg);
		args[currentArg] = toCharPtr(cppArg, memory);
	}

	char output[300];
	joinStrings(
		args, methodInfo->argsCount, ",", 200, output);
	printf("%s", output);

	printf(");\n");
}

void ClassPrinter::printMethod(const MethodInfo* methodInfo, const ClassInfo& classInfo, const char* className, const ConstantPool* cp, Memory* memory)
{
	printf("  ");

	for (auto const& element : accessFlagsMethod) {
		if ((element.first & methodInfo->accessFlags) != 0) {
			if (keywords.find(element.first) != keywords.end()) {
				printf("%s ", keywords.at(element.first));
			}
		}
	}


	printMethodSignature(methodInfo, classInfo, className, cp, memory);
	printf("    descriptor: %s\n", cp->getString(methodInfo->descriptorIndex));

	printf("    flags: ");
	const char** flags = (const char**)memory->classAlloc(12 * sizeof(char*));
	int currentIndex = 0;
	for (auto const& element : accessFlagsMethod) {
		if ((element.first & methodInfo->accessFlags) != 0) {
			flags[currentIndex] = getTypeAsString(element.first, METHOD);
			++currentIndex;
		}
	}

	char outBuffer[300];
	joinStrings((char**)flags, currentIndex, ", ", 300, outBuffer);
	printf("%s\n", outBuffer);

	if (methodInfo->isNative() || methodInfo->isAbstract()) {
	}
	else {
		printf("    Code: \n");
		printCode(methodInfo->code, methodInfo, cp, memory);
	}
	printf("\n");
}

void SignedBytePrinter(uint8_t* args, uint16_t argsCount, const ConstantPool* cp)
{
	for (uint16_t currentIndex = 0; currentIndex < argsCount; ++currentIndex) {
		int8_t signedInt = *reinterpret_cast<int8_t*>(&args[currentIndex]);
		printf(" %" PRIi8, signedInt);
	}
}

void UnsignedBytePrinter(uint8_t* args, uint16_t argsCount, const ConstantPool* cp)
{
	for (uint16_t currentIndex = 0; currentIndex < argsCount; ++currentIndex) {
		printf(" %" PRIu8, args[currentIndex]);
	}
}

void SignedShort(uint8_t* args, uint16_t argsCount, const ConstantPool* cp)
{
	for (int i = 0; i < argsCount; i += 2) {
		uint8_t byte1 = args[i];
		uint8_t byte2 = args[i + 1];

		int16_t shortIndex = (byte1 << 8) | byte2;

		printf(" %" PRIi16, shortIndex);
	}
}

void ShortIndices(uint8_t* args, uint16_t argsCount, const ConstantPool* cp)
{
	for (int i = 0; i < argsCount; i += 2) {
		uint8_t byte1 = args[i];
		uint8_t byte2 = args[i + 1];

		uint16_t shortIndex = (byte1 << 8) | byte2;

		char indexStr[6];
		sprintf(indexStr, " #%" PRIu16, shortIndex);
		printf("%-25s", indexStr);
	}

	if (argsCount > 0) {
		printf("//");
	}
	for (int i = 0; i < argsCount; i += 2) {
		uint8_t byte1 = args[i];
		uint8_t byte2 = args[i + 1];

		uint16_t shortIndex = (byte1 << 8) | byte2;
		ConstantPoolItem* item = cp->constants[shortIndex - 1];
		if (item != 0) {
			std::cout << " " << ClassPrinter::toStringInline(cp->constants[shortIndex - 1], cp);
		}
	}
}

void ArrayTypePrinter(uint8_t* args, uint16_t argsCount, const ConstantPool* cp)
{
	const char* type;
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

	printf(" %s", type);
}

void MultiArrayPrinter(uint8_t* args, uint16_t argsCount, const ConstantPool* cp)
{
	uint8_t byte1 = args[0];
	uint8_t byte2 = args[1];
	uint8_t dimensions = args[2];
	uint16_t shortIndex = (byte1 << 8) | byte2;
	printf(" #%" PRIu16 ", %" PRIu8, shortIndex, dimensions);
}

void ClassPrinter::printCode(
	const AttributeCode* code,
	const MethodInfo* method, 
	const ConstantPool* cp,
	Memory* memory
)
{
	int argsSize = method->argsCount;
	if (!method->isStatic()) {
		argsSize++;
	}

	printf("      stack=%" PRIu16 ", locals=%" PRIu16 ", args_size=%" PRIu16 "\n",
		code->maxStack, code->maxLocals, argsSize);

	ByteArray byteArray(code->code, code->codeLength);

	while (!byteArray.atEnd()) {
		uint8_t opcode = byteArray.readUnsignedByte();
		bool found = false;
		for (const Instruction& instruction : this->instructions) {
			if (((uint8_t)instruction.opcode) == opcode) {
				char indexStr[6];
				sprintf(indexStr, "%" PRIu64 ": ", byteArray.getPtr());
				printf("%12s%-13s", indexStr, instruction.name);
				uint8_t* args = 0;
				if (instruction.args > 0) {
					args = (uint8_t*)memory->classAlloc(instruction.args);
					byteArray.copyBytes(args, instruction.args);
				}
				if (instruction.printFunction != NULL) {
					instruction.printFunction(args, instruction.args, cp);
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

				char indexStr[6];
				sprintf(indexStr, "%" PRIu64 ": ", byteArray.getPtr());
				printf("%12s%-13s", indexStr, "lookupswitch");

				printf(" { // %" PRIi32 "\n", nPairs);

				for (int32_t currentPair = 0; currentPair < nPairs; currentPair++) {
					int32_t matchKey = byteArray.readSignedInt();
					int32_t offset = byteArray.readSignedInt();

					printf("%24" PRIi32, matchKey);

					printf(": %" PRIi32, (instructionIndex + offset));
					printf("\n");
				}

				printf("%24s", "default");
				printf(": %" PRIi32 "\n", defaultAddress);
				printf("            }");

				found = true;
			}

		}
		if (!found) {
			printf("Error: Instruction with opcode: %" PRIu8 " unrecognized\n", opcode);
			exit(1);
		}
		printf("\n");
	}

	for (size_t currentAttrib = 0; currentAttrib < code->attributes->attributesCount; ++currentAttrib) {
		AttributeInfo* att = code->attributes->attributes[currentAttrib];
		if (att != 0) {
			printf("      %s:\n", cp->getString(att->attributeNameIndex));
			AttributePrinter::printAttribute(att, cp);
		}
	}

}

std::string ClassPrinter::toString(const ConstantPoolItem* item, const ConstantPool* cp)
{
	switch (item->getType())
	{
	case CT_NAMEANDTYPE:
	{
		CPNameAndTypeInfo* nameAndType = (CPNameAndTypeInfo*)item;
		std::string str = "#" + std::to_string(nameAndType->nameIndex);
		str += ".#";
		str += std::to_string(nameAndType->descriptorIndex);
		return str;
	}
	case CT_STRING:
	{
		CPStringInfo* stringInfo = (CPStringInfo*)item;
		return "#" + std::to_string(stringInfo->stringIndex);
	}
	case CT_FIELDREF:
	{
		CPFieldRef* fieldRef = (CPFieldRef*)item;
		return "#" + std::to_string(fieldRef->classIndex) + "." + "#" + std::to_string(fieldRef->nameAndTypeIndex);
	}
	case CT_METHODREF:
	{
		CPMethodRef* methodRef = (CPMethodRef*)item;
		std::string str = "#" + std::to_string(methodRef->classIndex);
		str += ".#";
		str += std::to_string(methodRef->nameAndTypeIndex);
		return str;
	}
	case CT_CLASS:
	{
		CPClassInfo* classInfo = (CPClassInfo*)item;
		return "#" + std::to_string(classInfo->nameIndex);
	}
	case CT_INTEGER:
	{
		CPIntegerInfo* integerInfo = (CPIntegerInfo*)item;
		return std::to_string((int)integerInfo->bytes);
	}
	case CT_UTF8:
	{
		CPUTF8Info* utf8Info = (CPUTF8Info*)item;
		return (char*)utf8Info->bytes;
	}
	case CT_FLOAT:
	{
		CPFloatInfo* floatInfo = (CPFloatInfo*)item;
		float f = *reinterpret_cast<float*> (&floatInfo->bytes);
		return std::to_string((float)f) + "f";
	}
	case CT_DOUBLE:
	{
		CPDoubleInfo* doubleInfo = (CPDoubleInfo*)item;
		uint64_t bytes = ((uint64_t)doubleInfo->highBytes << 32) + (uint64_t)doubleInfo->lowBytes;
		double d = *reinterpret_cast<double*> (&bytes);
		return std::to_string((double)d) + "d";
	}
	case CT_LONG:
	{
		CPLongInfo* longInfo = (CPLongInfo*)item;
		int64_t bytes = ((int64_t)longInfo->highBytes << 32) + (int64_t)longInfo->lowBytes;
		return std::to_string(bytes) + "l";
	}
	default:
	{
		return "";
	}
	}
}

std::string ClassPrinter::toExpandedString(const ConstantPoolItem* item, const ConstantPool* cp)
{
	switch (item->getType())
	{
	case CT_NAMEANDTYPE:
	{
		CPNameAndTypeInfo* nameAndType = (CPNameAndTypeInfo*) item;
		std::string name = cp->getString(nameAndType->nameIndex);
		if (name == "<init>") {
			name = "\"" + name + "\"";
		}
		return  name + ":" + cp->getString(nameAndType->descriptorIndex);
	}
	case CT_STRING:
	{
		CPStringInfo* stringInfo = (CPStringInfo*)item;
		return cp->getString(stringInfo->stringIndex);
	}
	case CT_FIELDREF:
	{
		CPFieldRef* fieldRef = (CPFieldRef*)item;
		return toExpandedString(cp->getClassInfo(fieldRef->classIndex), cp) + "." + toExpandedString(cp->constants[fieldRef->nameAndTypeIndex - 1], cp);
	}
	case CT_METHODREF:
	{
		CPMethodRef* methodRef = (CPMethodRef*)item;
		return toExpandedString(cp->constants[methodRef->classIndex - 1],cp) 
			+ "." 
			+ toExpandedString(cp->constants[methodRef->nameAndTypeIndex - 1],cp);
	}
	case CT_CLASS:
	{
		CPClassInfo* classInfo = (CPClassInfo*)item;
		return cp->getString(classInfo->nameIndex);
	}
	default:
	{
		return "";
	}
	}
}

std::string ClassPrinter::toStringInline(const ConstantPoolItem* item, const ConstantPool* cp)
{
	switch (item->getType())
	{
	case CT_NAMEANDTYPE:
	{
		return "NameAndType " + ClassPrinter::toExpandedString(item, cp);
	}
	case CT_STRING:
	{
		return "string " + ClassPrinter::toExpandedString(item, cp);
	}
	case CT_FIELDREF:
	{
		// TODO: Print the Class name here as well, even if the class is the class itself
		return "Field " + ClassPrinter::toExpandedString(item, cp);
	}
	case CT_METHODREF:
	{
		return "Method " + ClassPrinter::toExpandedString(item, cp);
	}
	case CT_CLASS:
	{
		return "class " + ClassPrinter::toExpandedString(item, cp);
	}
	case CT_INTEGER:
	{
		return "int " + ClassPrinter::toString(item, cp);
	}
	case CT_UTF8:
	{
		return ClassPrinter::toString(item, cp);
	}
	case CT_FLOAT:
	{
		return "float " + ClassPrinter::toString(item, cp);
	}
	case CT_DOUBLE:
	{
		return "double " + ClassPrinter::toString(item, cp);
	}
	case CT_LONG:
	{
		return "long " + ClassPrinter::toString(item, cp);
	}
	default:
	{
		return "";
	}
	}
}

void ClassPrinter::printConstantPoolItem(uint16_t currentIndex, const ConstantPool* cp) {
	ConstantPoolItem* item = cp->constants[currentIndex];
	if (item == 0) {
		return;
	}
	char indexStr[5];
	sprintf(indexStr, "#%" PRIu16, (currentIndex+1));

	printf("%5s", indexStr);
	printf(" = %-15s%-15s", getTypeAsString(item->getType()), toString(item, cp).c_str());

	std::string expanded = toExpandedString(item, cp);
	if (expanded.size() > 0) {
		printf("// %s", expanded.c_str());
	}
	printf("\n");
}
static inline void binaryClassToExternalCopy(const char* className, char* output)
{
	size_t size = strlen(className);
	memcpy(output, className, size + 1);
	for (int currentCharacter = 0; currentCharacter < size; ++currentCharacter) {
		if (output[currentCharacter] == '/') {
			output[currentCharacter] = '.';
		}
	}
}

static inline void binaryClassToExternalInPlace(char* output)
{
	size_t size = strlen(output);
	for (int currentCharacter = 0; currentCharacter < size; ++currentCharacter) {
		if (output[currentCharacter] == '/') {
			output[currentCharacter] = '.';
		}
	}
}

void ClassPrinter::printClass(const ClassInfo& classInfo, Memory* memory)
{
	printf("Classfile %s\n", classInfo.filePath);
	printf("  Last modified %s; size %" PRIu64 " bytes\n", classInfo.lastModifiedString, classInfo.size);
	printf("  MD5 checksum %s\n", classInfo.md5);
	
	if (classInfo.sourceFile != "") {
		printf("  Compiled from \"%s\"\n", classInfo.sourceFile);
	}

	const ConstantPool* const cp = classInfo.constantPool;
	const CPClassInfo* const classPtr = cp->getClassInfo(classInfo.thisClass);
	const CPClassInfo* const superClassPtr = cp->getClassInfo(classInfo.superClass);

	for (auto const& element : accessFlagsClass) {
		if ((element.first & classInfo.accessFlags) != 0) {
			if (keywordsClass.find(element.first) != keywordsClass.end()) {
				printf("%s ", keywordsClass.at(element.first));
			}
		}
	}

	const char* className = cp->getString(classPtr->nameIndex);
	printf("class %s", className);

	const char* superClassName = cp->getString(superClassPtr->nameIndex);
	if (strcmp("java/lang/Object", superClassName) != 0) {
		char* outputBuffer = (char*) Platform::AllocateMemory(strlen(superClassName) + 1, 0);
		binaryClassToExternalCopy(superClassName, outputBuffer);
		printf(" extends %s", outputBuffer);
		Platform::FreeMemory(outputBuffer);
	}

	if (classInfo.interfacesCount > 0) {
		printf(" implements ");

		char** names = (char**) memory->classAlloc(sizeof(char*) * classInfo.interfacesCount);

		for (size_t currentIndex = 0; currentIndex < classInfo.interfacesCount; ++currentIndex) {
			uint16_t index = classInfo.interfaces[currentIndex];
			CPClassInfo* classPtr = cp->getClassInfo(index);
			names[currentIndex] = (char*)cp->getString(classPtr->nameIndex);
		}

		char* outputBuffer = (char*)Platform::AllocateMemory(300, 0);
		joinStrings(names, classInfo.interfacesCount, ", ", 300, outputBuffer);
		binaryClassToExternalInPlace(outputBuffer);
		printf("%s", outputBuffer);
		Platform::FreeMemory(outputBuffer);

	}
	printf("\n");

	printf("  minor version: %" PRIu16 "\n", classInfo.minorVersion);
	printf("  major version: %" PRIu16 "\n", classInfo.majorVersion);
	
	printf("  flags: ");
	const char** flags = (const char**)memory->classAlloc(8 * sizeof(char*));
	int currentIndex = 0;
	for (auto const& element : accessFlagsClass) {
		if ((element.first & classInfo.accessFlags) != 0) {
			flags[currentIndex] = getTypeAsString(element.first, CLASS);
			++currentIndex;
		}
	}
	char buffer[300];
	joinStrings((char**)flags, currentIndex, ", ", 300, buffer);
	printf("%s\n", buffer);

	printf("Constant pool:\n");
	for (uint16_t currentIndex = 0; currentIndex < cp->size; ++currentIndex) {
		printConstantPoolItem(currentIndex, cp);
	}

	printf("{\n");

	// Fields
	for (uint16_t currentField = 0; currentField < classInfo.fieldsCount; ++currentField) {
		FieldInfo* fieldInfo = classInfo.fields[currentField];
		printField(fieldInfo, cp, memory);
	}

	// Methods
	for (uint16_t currentMethod = 0; currentMethod < classInfo.methodCount; ++currentMethod) {
		MethodInfo* methodInfo = classInfo.methods[currentMethod];
		printMethod(methodInfo, classInfo, className, cp, memory);
	}
	
	printf("}\n");

	for (size_t currentAttrib = 0; currentAttrib < classInfo.attributes->attributesCount; ++currentAttrib) {
		AttributeInfo* att = classInfo.attributes->attributes[currentAttrib];
		AttributePrinter::printAttribute(att, cp);
	}
}
