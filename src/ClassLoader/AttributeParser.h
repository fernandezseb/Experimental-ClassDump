#pragma once

#include "Core.h"
#include "ByteArray.h"
#include "ConstantPool.h"

struct ExceptionTableEntry {
	uint16_t startPc;
	uint16_t endPc;
	uint16_t handlerPc;
	uint16_t catchType;
};

struct AttributeInfo {
	uint16_t attributeNameIndex;
	uint32_t attributeLength;
	virtual std::string toString(const ConstantPool* cp) const { return ""; }
	virtual ~AttributeInfo() {
	}
};

class AttributeCollection {
public:
	std::vector<AttributeInfo*> attributes;
	AttributeInfo* findAttributeWithName(ConstantPool* constantPool, const std::string& name) const;

	AttributeCollection(std::vector<AttributeInfo*> attributes);
	AttributeCollection() {
	}
	~AttributeCollection() {
		for (int i = 0; i < this->attributes.size(); i++) {
			AttributeInfo* item = this->attributes[i];
			if (item != 0) {
				delete item;
				item = nullptr;
			}
		}
	}
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
	AttributeCollection* attributes;
	AttributeCode() : attributes(nullptr) {};
	virtual ~AttributeCode() {
		if (code != 0) {
			free(code);
			code = nullptr;
		}

		if (attributes != 0) {
			delete attributes;
			attributes = nullptr;
		}
	}
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
	std::vector<LineNumberTableEntry*> entries;
	std::string toString(const ConstantPool* cp) const;

	virtual ~AttributeLineNumberTable() {
		for (int i = 0; i < this->entries.size(); i++) {
			LineNumberTableEntry* item = this->entries[i];
			if (item != 0) {
				delete item;
				item = nullptr;
			}
		}
	}
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

	std::string toString(const ConstantPool* cp) const;
	virtual ~AttributeLocalVariableTable() {
		for (int i = 0; i < this->entries.size(); i++) {
			LocalVariableTableEntry* item = this->entries[i];
			if (item != 0) {
				delete item;
				item = nullptr;
			}
		}
	}
};

class AttributeParser {
public:
	static ExceptionTableEntry readExceptionTableEntry(ByteArray& byteArray);
	static std::vector<ExceptionTableEntry> readExceptionTable(ByteArray& byteArray);
	static void readStackMapTable(ByteArray& byteArray);
	static AttributeCollection* readAttributes(ByteArray& byteArray, ConstantPool* constantPool);
};