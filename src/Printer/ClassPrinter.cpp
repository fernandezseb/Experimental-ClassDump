#include "ClassPrinter.h"

#include "AttributePrinter.h"

#include "Util.h"
#include "PrintUtils.h"

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

void ClassPrinter::getAsExternalReturnType(char* returnType, char* buffer)
{
	int arrCount = 0;
	bool inClass = false;

	for (uint16_t currentChar = 0; returnType[currentChar] != 0; ++currentChar) {
		char c = returnType[currentChar];
		if (c == 'L') {
			inClass = true;
		}
		else if (c == ';') {
			inClass = false;
		}
		else if (inClass) {
			if (c == '/') {
				strcat(buffer, ".");
			}
			else {
				char str[2] = { c, 0 };
				strcat(buffer, str);
			}
		}
		else if (c == '[') {
			arrCount++;
		}
		else {
			strcat(buffer, types.at(c));
		}
	}

	for (int curr = 0; curr < arrCount; curr++) {
		strcat(buffer, "[]");
	}
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

	char bufferType[300] = { 0 };
	getAsExternalReturnType((char*)descriptor, bufferType);
	std::cout << bufferType
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
		char typeBuffer[300] = {0};
		getAsExternalReturnType(methodInfo->returnType, typeBuffer);
		std::cout << typeBuffer << " ";
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
		char typeBuffer[300] = { 0 };
		getAsExternalReturnType((char*)arg, typeBuffer);
		char* argPtr = (char*) memory->classAlloc(strlen(typeBuffer) + 1);
		args[currentArg] = argPtr;
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
		codePrinter.printCode(methodInfo->code, methodInfo, cp, memory);
	}
	printf("\n");
}


void ClassPrinter::printConstantPoolItem(uint16_t currentIndex, const ConstantPool* cp) {
	ConstantPoolItem* item = cp->constants[currentIndex];
	if (item == 0) {
		return;
	}
	char indexStr[5];
	sprintf(indexStr, "#%" PRIu16, (currentIndex+1));

	printf("%5s", indexStr);
	char out[300] = {0};
	printf(" = %-15s%-15s", getTypeAsString(item->getType()), PrintUtils::printData(out, item, cp));

	char buffer[300] = { 0 };
	char* out2 = PrintUtils::printResolved(buffer, item, cp);
	if (strlen(out2) > 0) {
		printf("// %s", out2);
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
