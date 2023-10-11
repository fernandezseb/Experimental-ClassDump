#pragma once

#include "../Core.h"
#include "ConstantPool.h"
#include "ByteArray.h"

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

struct AttributeLineNumberTable;
struct AttributeLocalVariableTable;

struct AttributeCode : public AttributeInfo {
	uint16_t maxStack;
	uint16_t maxLocals;
	uint32_t codeLength;
	uint8_t* code;
	std::vector<ExceptionTableEntry> exceptionTable;
	std::vector<AttributeInfo*> attributes;
	AttributeLineNumberTable *lineNumberTable;
	AttributeLocalVariableTable* localVariableTable;
};

struct AttributeSourceFile : public AttributeInfo {
	uint16_t sourceFileIndex;
	std::string toString(const ConstantPool& cp) {
		return "SourceFile: \"" + cp.getString(sourceFileIndex) + "\"";
	}
};

struct LineNumberTableEntry {
	uint16_t startPc;
	uint16_t lineNumber;
};

struct AttributeLineNumberTable :public AttributeInfo {
	std::vector<LineNumberTableEntry*> entries;
};

struct LocalVariableTableEntry {
	uint16_t startPc;
	uint16_t length;
	uint16_t nameIndex;
	uint16_t descriptorIndex;
	uint16_t index;
};

struct AttributeLocalVariableTable :public AttributeInfo {
	std::vector<LocalVariableTableEntry*> entries;
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
	//uint64_t bytePtr;
private:
	void checkMagicNumber(ByteArray& byteArray);
	ConstantPool readConstantPool(ByteArray& byteArray);
	ConstantPoolItem* readConstantPoolItem(uint8_t tag, ByteArray& byteArray);
	ExceptionTableEntry readExceptionTableEntry(ByteArray& byteArray);
	std::vector<ExceptionTableEntry> readExceptionTable(ByteArray& byteArray);
	void readStackMapTable(ByteArray& byteArray);
	std::vector<AttributeInfo*> readAttributes(ByteArray& byteArray, ConstantPool& constantPool);
	void parseDescriptor(const std::string& descriptor, MethodInfo& method);
public:
	ClassLoader();
	ClassInfo readClass(ByteArray& byteArray);
	ClassInfo readClass(const std::string& className);
	std::vector<uint16_t> readInterfaces(ByteArray& byteArray, uint16_t interfacesCount);
	std::vector<FieldInfo> readFields(ByteArray& byteArray, ConstantPool& constantPool);
	std::vector<MethodInfo> readMethods(ByteArray& byteArray, ConstantPool& constantPool);
};