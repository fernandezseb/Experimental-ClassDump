#pragma once

#include "Core.h"

#include "ConstantPool.h"

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
	LocalVariableTable,
	StackMapTable
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

struct VerificationTypeInfo {
	uint8_t tag;
	uint16_t data;
};


struct StackMapFrame {
	uint8_t frameType;
};

struct SameFrame : public StackMapFrame {
};

struct SameLocals1StackItemFrame : public StackMapFrame {
	VerificationTypeInfo stack;
};

struct SameLocals1StackItemFrameExtended : public StackMapFrame {
	uint16_t offsetDelta;
	VerificationTypeInfo stack;
};

struct ChopFrame : public StackMapFrame {
	uint16_t offsetDelta;
};

struct SameFrameExtended : public StackMapFrame {
	uint16_t offsetDelta;
};

struct AppendFrame : public StackMapFrame {
	uint16_t offsetDelta;
	// The size of this array is: frame_type - 251
	VerificationTypeInfo* locals;
};

struct FullFrame : public StackMapFrame {
	uint16_t offsetDelta;
	uint16_t numberOfLocals;
	VerificationTypeInfo* locals;
	uint16_t numberOfStackItems;
	VerificationTypeInfo* stack;
};

struct StackMapTableAttribute : public AttributeInfo {
	uint16_t entriesCount; // Number of stack_map_frame entries
	StackMapFrame** entries;
};