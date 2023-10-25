#pragma once

#include "../Core.h"
#include "ConstantPool.h"
#include "ByteArray.h"
#include "AttributeParser.h"

// MAGIC NUMBER
#define MAGIC_NUMBER       0xCAFEBABE

class FieldInfo {
private:
	const std::vector<AccessFlag> fieldFlags = {
		ACC_PUBLIC,
		ACC_PRIVATE,
		ACC_PROTECTED,
		ACC_STATIC,
		ACC_FINAL,
		ACC_VOLATILE,
		ACC_TRANSIENT,
		ACC_SYNTHETIC,
		ACC_ENUM
	};
public:
	uint16_t accessFlags;
	uint16_t nameIndex;
	uint16_t descriptorIndex;
	bool isPrivate;
	AttributeCollection* attributes;
public:
	FieldInfo();
	~FieldInfo();
	std::vector<AccessFlag> getAccessFlags() const;
};

class MethodInfo {
private:
	const std::vector<AccessFlag> methodFlags = {
		ACC_PUBLIC,
		ACC_PRIVATE,
		ACC_PROTECTED,
		ACC_STATIC,
		ACC_FINAL,
		ACC_SYNCHRONIZED,
		ACC_BRIDGE,
		ACC_VARARGS,
		ACC_NATIVE,
		ACC_ABSTRACT,
		ACC_STRICT,
		ACC_SYNTHETIC
	};
public:
	uint16_t accessFlags;
	uint16_t nameIndex;
	uint16_t descriptorIndex;
	AttributeCode* code;
	bool isNative;
	bool isPublic;
	bool isStatic;
	bool isConstructor;
	AttributeCollection* attributes;
	std::string returnType;
	std::vector<std::string> args;
public:
	MethodInfo();
	~MethodInfo();

	std::vector<AccessFlag> getAccessFlags() const;
};

class ClassInfo {
private:
	const std::vector<AccessFlag> classFlags = {
		ACC_PUBLIC,
		ACC_FINAL,
		ACC_SUPER,
		ACC_INTERFACE,
		ACC_ABSTRACT,
		ACC_SYNTHETIC,
		ACC_ANNOTATION,
		ACC_ENUM
	};
public:
	std::wstring filePath;
	uint64_t size;
	time_t lastModified;
	std::string md5;
	uint16_t minorVersion;
	uint16_t majorVersion;
	ConstantPool* constantPool;
	uint16_t accessFlags;
	uint16_t thisClass;
	uint16_t superClass;
	
	uint16_t* interfaces;
	uint16_t interfacesCount;

	std::vector<FieldInfo*> fields;

	std::vector<MethodInfo*> methods;

	AttributeCollection* attributes;
	std::string sourceFile;
	bool isPublic;
public:
	ClassInfo();
	virtual ~ClassInfo();

	std::vector<AccessFlag> getAccessFlags() const;
};

class ClassLoader {
private:
	void checkMagicNumber(ByteArray& byteArray);
	ConstantPool* readConstantPool(ByteArray& byteArray);
	ConstantPoolItem* readConstantPoolItem(uint8_t tag, ByteArray& byteArray);
	void parseDescriptor(const std::string& descriptor, MethodInfo* method);
public:
	ClassLoader();

	ClassInfo* readClass(ByteArray& byteArray);
	ClassInfo* readClass(const std::string& className);
	uint16_t* readInterfaces(ByteArray& byteArray, uint16_t interfacesCount);
	std::vector<FieldInfo*> readFields(ByteArray& byteArray, ConstantPool* constantPool);
	std::vector<MethodInfo*> readMethods(ByteArray& byteArray, ConstantPool* constantPool);
};