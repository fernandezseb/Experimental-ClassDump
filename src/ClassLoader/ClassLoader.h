#pragma once

#include "../Core.h"
#include "ConstantPool.h"

// MAGIC NUMBER
#define MAGIC_NUMBER       0xCAFEBABE

struct ExceptionTableEntry {
	uint16_t startPc;
	uint16_t endPc;
	uint16_t handlerPc;
	uint16_t catchType;
};

struct AttributeInfo {
	uint16_t attributeNameIndex;
	uint32_t attributeLength;
	virtual std::string toString(const ConstantPool& cp) { return ""; }
};

struct AttributeConstantValue : public AttributeInfo {
	uint16_t constantValueIndex;
};

struct AttributeCode : public AttributeInfo {
	uint16_t maxStack;
	uint16_t maxLocals;
	uint32_t codeLength;
	uint8_t* code;
	std::vector<ExceptionTableEntry> exceptionTable;
	std::vector<AttributeInfo*> attributes;
};

struct AttributeSourceFile : public AttributeInfo {
	uint16_t sourceFileIndex;
	std::string toString(const ConstantPool& cp) {
		return "SourceFile: \"" + cp.getString(sourceFileIndex) + "\"";
	}
};

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
	std::vector<AccessFlag> getAccessFlags() const;
	std::string returnType;
	std::vector<std::string> args;
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
	ConstantPool constantPool;
	uint16_t accessFlags;
	uint16_t thisClass;
	uint16_t superClass;
	std::vector<uint16_t> interfaces;
	std::vector<FieldInfo> fields;
	std::vector<MethodInfo> methods;
	std::vector<AccessFlag> getAccessFlags() const;
	std::vector<AttributeInfo*> attributes;
	std::string sourceFile;
	bool isPublic;
};

class ClassLoader {
private:
	uint64_t bytePtr;
private:
	uint8_t readUnsignedByte(uint8_t* bytes);
	uint16_t readUnsignedShort(uint8_t* bytes);
	uint32_t readUnsignedInt(uint8_t* bytes);
	void checkMagicNumber(uint8_t* bytes);
	ConstantPool readConstantPool(uint8_t* bytes);
	ConstantPoolItem* readConstantPoolItem(uint8_t tag, uint8_t* bytes);
	ExceptionTableEntry readExceptionTableEntry(uint8_t* bytes);
	std::vector<ExceptionTableEntry> readExceptionTable(uint8_t* bytes);
	std::vector<AttributeInfo*> readAttributes(uint8_t* bytes, ConstantPool& constantPool);
	void parseDescriptor(const std::string& descriptor, MethodInfo& method);
public:
	ClassLoader();
	ClassInfo readClass(uint8_t* bytes);
	ClassInfo readClass(const std::string& className);
	std::vector<uint16_t> readInterfaces(uint8_t* bytes, uint16_t interfacesCount);
	std::vector<FieldInfo> readFields(uint8_t* bytes, ConstantPool& constantPool);
	std::vector<MethodInfo> readMethods(uint8_t* bytes, ConstantPool& constantPool);
};