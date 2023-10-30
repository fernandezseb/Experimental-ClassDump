#include "AttributeParser.h"

AttributeInfo* AttributeCollection::findAttributeWithName(ConstantPool* constantPool, const char* name) const
{
	AttributeInfo* attrib = 0;

	for (int currentAttrib = 0; currentAttrib < attributesCount; ++currentAttrib) {
		attrib = attributes[currentAttrib];
		if (strcmp(constantPool->getString(attrib->attributeNameIndex), name) == 0) {
			return attrib;
		}
	}

	return attrib;
}

void AttributeParser::readStackMapTable(ByteArray& byteArray)
{
	uint16_t numberOfEntries = byteArray.readUnsignedShort();
	for (uint16_t currentEntry = 0; currentEntry < numberOfEntries; ++currentEntry) {
		uint8_t frameType = byteArray.readUnsignedByte();
		if (frameType >= 0 && frameType <= 63) {
			// same frame
		}
		else if (frameType >= 64 && frameType <= 127) {
			// same locals 1 stack item frame
			uint8_t tag = byteArray.readUnsignedByte();
			if (tag == 7 || tag == 8) {
				uint16_t crap = byteArray.readUnsignedShort();
			}

		}
		else if (frameType == 247) {
			// same locals 1 stack item frame extended
		}
		else if (frameType >= 248 && frameType <= 250) {
			// chop frame
			uint16_t offsetDelta = byteArray.readUnsignedShort();
		}
		else if (frameType == 251) {
			uint16_t offsetDelta = byteArray.readUnsignedShort();
			// same frame extended
		}
		else if (frameType >= 252 && frameType <= 254) {
			// append frame
			uint16_t offsetDelta = byteArray.readUnsignedShort();
			for (uint16_t currentLocal = 0; currentLocal < (frameType - 251); ++currentLocal) {
				uint8_t tag = byteArray.readUnsignedByte();
				if (tag == 7 || tag == 8) {
					uint16_t crap = byteArray.readUnsignedShort();
				}
			}
		}
		else if (frameType == 255) {
			// full frame
			uint16_t offsetDelta = byteArray.readUnsignedShort();
			uint16_t numberOfLocals = byteArray.readUnsignedShort();
			for (uint16_t currentLocal = 0; currentLocal < numberOfLocals; ++currentLocal) {
				uint8_t tag = byteArray.readUnsignedByte();
				if (tag == 7 || tag == 8) {
					uint16_t crap = byteArray.readUnsignedShort();
				}
			}
			uint16_t numberOfStackItems = byteArray.readUnsignedShort();
			for (uint16_t currentLocal = 0; currentLocal < numberOfStackItems; ++currentLocal) {
				uint8_t tag = byteArray.readUnsignedByte();
				if (tag == 7 || tag == 8) {
					uint16_t crap = byteArray.readUnsignedShort();
				}
			}
		}
		else {
			fprintf(stderr, "Unknown frame type detected : %" PRIu8 "\n", frameType);
			exit(1);
		}
	}
}

ExceptionTableEntry AttributeParser::readExceptionTableEntry(ByteArray& byteArray)
{
	uint16_t startPc = byteArray.readUnsignedShort();
	uint16_t endPc = byteArray.readUnsignedShort();
	uint16_t handlerPc = byteArray.readUnsignedShort();
	uint16_t catchType = byteArray.readUnsignedShort();

	ExceptionTableEntry entry;
	entry.startPc = startPc;
	entry.endPc = endPc;
	entry.handlerPc = handlerPc;
	entry.catchType = catchType;

	return entry;
}

ExceptionTableEntry* AttributeParser::readExceptionTable(ByteArray& byteArray, uint16_t *size, Memory* memory)
{
	uint16_t exceptionTableLength = byteArray.readUnsignedShort();

	ExceptionTableEntry* table = 0;

	if (exceptionTableLength > 0)
	{
		ExceptionTableEntry* table = (ExceptionTableEntry*)
			memory->classAlloc(sizeof(ExceptionTableEntry) * exceptionTableLength);
		*size = exceptionTableLength;

		for (uint16_t currentException = 0; currentException < exceptionTableLength; currentException++) {
			ExceptionTableEntry entry = readExceptionTableEntry(byteArray);
			table[currentException] = entry;
		}
	}

	return table;
}

AttributeCollection* AttributeParser::readAttributes(ByteArray& byteArray, ConstantPool* constantPool, Memory* memory)
{
	uint16_t attributesCount = byteArray.readUnsignedShort();
	AttributeInfo** attributes = (AttributeInfo**) memory->classAlloc(attributesCount * sizeof(AttributeInfo*));

	for (int currentAttrib = 0; currentAttrib < attributesCount; ++currentAttrib) {
		uint16_t attributeNameIndex = byteArray.readUnsignedShort();
		uint32_t attributeLength = byteArray.readUnsignedInt();

		const char* name = constantPool->getString(attributeNameIndex);

		if (strcmp(name, "Code") == 0) {
			uint16_t maxStack = byteArray.readUnsignedShort();
			uint16_t maxLocals = byteArray.readUnsignedShort();
			uint32_t codeLength = byteArray.readUnsignedInt();
			uint8_t* code = (uint8_t*)memory->classAlloc(sizeof(uint8_t) * codeLength);
			/*memcpy(code, &bytes[bytePtr], sizeof(uint8_t) * codeLength);
			bytePtr += codeLength;*/
			byteArray.copyBytes(code, codeLength);

			uint16_t exceptionTableSize;
			ExceptionTableEntry* exceptions = readExceptionTable(byteArray, &exceptionTableSize, memory);
			AttributeCollection* attribs = readAttributes(byteArray, constantPool, memory);


			AttributeCode* att = (AttributeCode*)memory->classAlloc(sizeof(AttributeCode));
			att->attributeNameIndex = attributeNameIndex;
			att->attributeLength = attributeLength;
			att->maxStack = maxStack;
			att->maxLocals = maxLocals;
			att->codeLength = codeLength;
			att->code = code;
			att->exceptionTable = exceptions;
			att->exceptionTableSize = exceptionTableSize;
			att->attributes = attribs;
			att->type = Code;

			attributes[currentAttrib] = att;
		}
		else if (strcmp(name, "LineNumberTable") == 0) {
			uint16_t lineNumberTableLength = byteArray.readUnsignedShort();
			AttributeLineNumberTable* att = (AttributeLineNumberTable*) memory->classAlloc(sizeof(AttributeLineNumberTable));
			att->attributeNameIndex = attributeNameIndex;
			att->attributeLength = attributeLength;
			att->size = lineNumberTableLength;
			att->type = LineNumberTable;

			att->entries = (LineNumberTableEntry*)memory->classAlloc(sizeof(LineNumberTableEntry) * lineNumberTableLength);

			for (int lineNumberTableIndex = 0; lineNumberTableIndex < lineNumberTableLength; ++lineNumberTableIndex) {
				uint16_t startPc = byteArray.readUnsignedShort();
				uint16_t lineNumber = byteArray.readUnsignedShort();
				LineNumberTableEntry entry;
				entry.startPc = startPc;
				entry.lineNumber = lineNumber;
				att->entries[lineNumberTableIndex] = entry;
			}
			attributes[currentAttrib] = att;
		}
		else if (strcmp(name, "LocalVariableTable") == 0) {
			uint16_t localVariableTableLength = byteArray.readUnsignedShort();
			AttributeLocalVariableTable* att = (AttributeLocalVariableTable*) memory->classAlloc(sizeof(AttributeLocalVariableTable));
			att->attributeNameIndex = attributeNameIndex;
			att->attributeLength = attributeLength;
			att->size = localVariableTableLength;
			att->type = LocalVariableTable;
			att->entries = (LocalVariableTableEntry*) memory->classAlloc(sizeof(LocalVariableTableEntry) * localVariableTableLength);
			for (int localVariableTableIndex = 0; localVariableTableIndex < localVariableTableLength; ++localVariableTableIndex) {
				uint16_t startPc = byteArray.readUnsignedShort();
				uint16_t length = byteArray.readUnsignedShort();
				uint16_t nameIndex = byteArray.readUnsignedShort();
				uint16_t descriptorIndex = byteArray.readUnsignedShort();
				uint16_t index = byteArray.readUnsignedShort();
				LocalVariableTableEntry entry;
				entry.startPc = startPc;
				entry.length = length;
				entry.nameIndex = nameIndex;
				entry.descriptorIndex = descriptorIndex;
				entry.index = index;
				att->entries[localVariableTableIndex] = entry;
			}
			attributes[currentAttrib] = att;
		}
		else if (strcmp(name, "SourceFile") == 0) {
			uint16_t sourceFileIndex = byteArray.readUnsignedShort();
			AttributeSourceFile* att = (AttributeSourceFile*) memory->classAlloc(sizeof(AttributeSourceFile));
			att->attributeNameIndex = attributeNameIndex;
			att->attributeLength = attributeLength;
			att->sourceFileIndex = sourceFileIndex;
			att->type = SourceFile;

			attributes[currentAttrib] = att;
		}
		else if (strcmp(name, "StackMapTable") == 0) {
			readStackMapTable(byteArray);
			attributes[currentAttrib] = 0;
		}
		else {
			printf("Attribute parsing not implemented yet for type: %s\n", name);
			exit(1);
		}
	}

	AttributeCollection* attributeCollection = (AttributeCollection*)
		memory->classAlloc(sizeof(AttributeCollection));
	attributeCollection->attributes = attributes;
	attributeCollection->attributesCount = attributesCount;

	return attributeCollection;
}