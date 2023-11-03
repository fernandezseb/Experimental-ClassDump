#pragma once

#include "Core.h"
#include "ByteArray.h"
#include "Data/ConstantPool.h"
#include "Memory.h"
#include "Data/Attribute.h"

class AttributeParser {
public:
	static ExceptionTableEntry readExceptionTableEntry(ByteArray& byteArray);
	static ExceptionTableEntry* readExceptionTable(ByteArray& byteArray, uint16_t* size, Memory* memory);
	static void readStackMapTable(ByteArray& byteArray);
	static AttributeCollection* readAttributes(ByteArray& byteArray, ConstantPool* constantPool, Memory* memory);
};