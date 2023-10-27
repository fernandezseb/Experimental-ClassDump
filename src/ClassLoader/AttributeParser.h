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

enum AttributeType {
	ConstantValue,
	Code,
	SourceFile,
	LineNumberTable,
	LocalVariableTable
};

struct AttributeInfo {
	AttributeType type;
	uint16_t attributeNameIndex;
	uint32_t attributeLength;
};

struct AttributeCollection {
	AttributeInfo** attributes;
	uint16_t attributesCount;
	
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
};

struct AttributeSourceFile : public AttributeInfo {
	uint16_t sourceFileIndex;
};

struct LineNumberTableEntry {
	uint16_t startPc;
	uint16_t lineNumber;
};

struct AttributeLineNumberTable : public AttributeInfo {
	LineNumberTableEntry* entries;
	uint16_t size;
};

struct LocalVariableTableEntry {
	uint16_t startPc;
	uint16_t length;
	uint16_t nameIndex;
	uint16_t descriptorIndex;
	uint16_t index;
};

struct AttributeLocalVariableTable : public AttributeInfo {
	LocalVariableTableEntry* entries;
	uint16_t size;
};

class AttributeParser {
public:
	static ExceptionTableEntry readExceptionTableEntry(ByteArray& byteArray);
	static ExceptionTableEntry* readExceptionTable(ByteArray& byteArray, uint16_t* size, Memory* memory);
	static void readStackMapTable(ByteArray& byteArray);
	static AttributeCollection* readAttributes(ByteArray& byteArray, ConstantPool* constantPool, Memory* memory);
};