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
public:
	std::vector<AccessFlag> getAccessFlags() const;
};

class MethodInfo {
public:
	uint16_t accessFlags;
	uint16_t nameIndex;
	uint16_t descriptorIndex;
	AttributeCode* code;
	bool isNative;
	bool isPublic;
	bool isStatic;
	bool isConstructor;
	bool isAbstract;
	AttributeCollection* attributes;
	char* returnType;
	char** args;
	uint16_t argsCount;
public:
	MethodInfo();
	~MethodInfo();

	std::vector<AccessFlag> getAccessFlags() const;
};

class ClassInfo {
public:
	wchar_t* filePath;
	uint64_t size;
	time_t lastModified;
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
	bool isPublic;
	Memory *memory;
public:
	ClassInfo();
	~ClassInfo();

	std::vector<AccessFlag> getAccessFlags() const;
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
	ClassLoader();
	ClassInfo* readClass(const char* className);
};