#pragma once

#include "Core.h"
#include "ByteArray.h"
#include "ConstantPool.h"
#include "Memory.h"

struct ExceptionTableEntry {
	uint16_t startPc;
	uint16_t endPc;
	uint16_t handlerPc;
	uint16_t catchType;
};

class AttributeInfo {
public:
	uint16_t attributeNameIndex;
	uint32_t attributeLength;
public:
	virtual ~AttributeInfo() {
	}
	
	virtual std::string toString(const ConstantPool* cp) const { return ""; }
};

struct AttributeCollection {
	AttributeInfo** attributes;
	uint16_t attributesCount;

	AttributeCollection(AttributeInfo** attributes, uint16_t attributesCount);
	AttributeCollection();
	~AttributeCollection();
	
	AttributeInfo* findAttributeWithName(ConstantPool* constantPool, const char* name) const;
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
	ExceptionTableEntry* exceptionTable;
	uint16_t exceptionTableSize;
	AttributeCollection* attributes;

	AttributeCode();;
	virtual ~AttributeCode();
};

struct AttributeSourceFile : public AttributeInfo {
	uint16_t sourceFileIndex;
	std::string toString(const ConstantPool* cp) const;
};

struct LineNumberTableEntry {
	uint16_t startPc;
	uint16_t lineNumber;
};

struct AttributeLineNumberTable :public AttributeInfo {
	LineNumberTableEntry* entries;
	uint16_t size;

	virtual ~AttributeLineNumberTable();

	std::string toString(const ConstantPool* cp) const;
};

struct LocalVariableTableEntry {
	uint16_t startPc;
	uint16_t length;
	uint16_t nameIndex;
	uint16_t descriptorIndex;
	uint16_t index;
};

struct AttributeLocalVariableTable :public AttributeInfo {
	LocalVariableTableEntry* entries;
	uint16_t size;

	virtual ~AttributeLocalVariableTable();

	std::string toString(const ConstantPool* cp) const;
};

class AttributeParser {
public:
	static ExceptionTableEntry readExceptionTableEntry(ByteArray& byteArray);
	static ExceptionTableEntry* readExceptionTable(ByteArray& byteArray, uint16_t* size);
	static void readStackMapTable(ByteArray& byteArray);
	static AttributeCollection* readAttributes(ByteArray& byteArray, ConstantPool* constantPool, Memory* memory);
};