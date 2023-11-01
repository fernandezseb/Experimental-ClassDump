#pragma once

#include "Core.h"
#include "ConstantPool.h"
#include "ByteArray.h"
#include "AttributeParser.h"
#include "Memory.h"

// MAGIC NUMBER
#define MAGIC_NUMBER       0xCAFEBABE

class FieldInfo {
public:
	uint16_t accessFlags;
	uint16_t nameIndex;
	uint16_t descriptorIndex;
	bool isPrivate;
	AttributeCollection* attributes;
};

class MethodInfo {
public:
	uint16_t accessFlags;
	uint16_t nameIndex;
	uint16_t descriptorIndex;
	AttributeCode* code;
	AttributeCollection* attributes;
	char* returnType;
	char** args;
	uint16_t argsCount;
	const char* name;
public:
	bool isStatic() const {
		return ((accessFlags & ACC_STATIC) != 0);
	}

	bool isPublic() const {
		return ((accessFlags & ACC_PUBLIC) != 0);
	}

	bool isNative() const {
		return ((accessFlags & ACC_NATIVE) != 0);
	}

	bool isConstructor() const {
		return (strcmp(name, "<init>") == 0);
	}

	bool isAbstract() const {
		return ((accessFlags & ACC_ABSTRACT) != 0);
	}
};

class ClassInfo {
public:
	wchar_t* filePath;
	uint64_t size;
	char lastModifiedString[50];
	char md5[33];
	uint16_t minorVersion;
	uint16_t majorVersion;
	ConstantPool* constantPool;
	uint16_t accessFlags;
	uint16_t thisClass;
	uint16_t superClass;
	
	uint16_t* interfaces;
	uint16_t interfacesCount;

	FieldInfo** fields;
	uint16_t fieldsCount;

	MethodInfo** methods;
	uint16_t methodCount;

	AttributeCollection* attributes;
	char* sourceFile;
	//bool isPublic;
	Memory *memory;
public:
	bool isPublic() const {
		return ((accessFlags & ACC_PUBLIC) != 0);
	}
};

class ClassLoader {
private:
	void checkMagicNumber(ByteArray& byteArray);
	ConstantPool* readConstantPool(ByteArray& byteArray);
	ConstantPoolItem* readConstantPoolItem(uint8_t tag, ByteArray& byteArray);
	void parseDescriptor(const char* descriptor, MethodInfo* method);
	uint16_t* readInterfaces(ByteArray& byteArray, uint16_t interfacesCount);
	FieldInfo** readFields(ByteArray& byteArray, ConstantPool* constantPool, uint16_t fieldsCount);
	MethodInfo** readMethods(ByteArray& byteArray, ConstantPool* constantPool, uint16_t methodCount);
	ClassInfo* readClass(ByteArray& byteArray);
	Memory* memory;
public:
	ClassInfo* readClass(const char* className, Memory* memory);
};