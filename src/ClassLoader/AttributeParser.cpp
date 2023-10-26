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

AttributeCollection::AttributeCollection(AttributeInfo** attributes, uint16_t attributesCount)
{
	this->attributes = attributes;
	this->attributesCount = attributesCount;
}

AttributeCollection::AttributeCollection() {
}

AttributeCollection::~AttributeCollection() {
	for (int i = 0; i < this->attributesCount; i++) {
		AttributeInfo* item = this->attributes[i];
		if (item != 0) {
			delete item;
			item = nullptr;
		}
	}

	if (attributes != 0) {
		free(attributes);
	}
}

std::string AttributeSourceFile::toString(const ConstantPool* cp) const {
	return std::string("SourceFile: \"") + cp->getString(sourceFileIndex) + "\"\n";
}

AttributeCode::AttributeCode() : attributes(nullptr), code(nullptr) {}

AttributeCode::~AttributeCode() {
	if (code != 0) {
		free(code);
		code = nullptr;
	}

	if (attributes != 0) {
		delete attributes;
		attributes = nullptr;
	}

	if (exceptionTable != 0) {
		free(exceptionTable);
	}
}

AttributeLocalVariableTable::~AttributeLocalVariableTable() {
	free(entries);
}

AttributeLineNumberTable::~AttributeLineNumberTable() {
	free(entries);
}

std::string AttributeLineNumberTable::toString(const ConstantPool* cp) const {
	std::string out = "";
	for (uint16_t currentIndex = 0; currentIndex < size; ++currentIndex) {
		LineNumberTableEntry& entry = entries[currentIndex];
		out += "        line " +
			std::to_string(entry.lineNumber)
			+ ": "
			+ std::to_string(entry.startPc)
			+ "\n";
	}

	return out;
}

std::string AttributeLocalVariableTable::toString(const ConstantPool* cp) const {
	std::stringstream ss;
	ss << "        Start  Length  Slot  Name   Signature\n";

	for (uint16_t currentIndex = 0; currentIndex < size; ++currentIndex) {
		LocalVariableTableEntry& entry = entries[currentIndex];
		ss << "        " << std::right << std::setfill(' ') << std::setw(5) << entry.startPc;
		ss << "  " << std::right << std::setfill(' ') << std::setw(6) << entry.length;
		ss << "  " << std::right << std::setfill(' ') << std::setw(4) << entry.index;
		ss << "  " << std::right << std::setfill(' ') << std::setw(4) << cp->getString(entry.nameIndex);
		ss << "   " << std::left << std::setfill(' ') << std::setw(10) << cp->getString(entry.descriptorIndex);
		ss << std::endl;
	}
	return ss.str();
}

void AttributeParser::readStackMapTable(ByteArray& byteArray) {
	uint16_t numberOfEntries = byteArray.readUnsignedShort();
	//printf("[%" PRIu16 "] \n", numberOfEntries);
	for (uint16_t currentEntry = 0; currentEntry < numberOfEntries; currentEntry++) {
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
			for (uint16_t currentLocal = 0; currentLocal < (frameType - 251); currentLocal++) {
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
			for (uint16_t currentLocal = 0; currentLocal < numberOfLocals; currentLocal++) {
				uint8_t tag = byteArray.readUnsignedByte();
				if (tag == 7 || tag == 8) {
					uint16_t crap = byteArray.readUnsignedShort();
				}
			}
			uint16_t numberOfStackItems = byteArray.readUnsignedShort();
			for (uint16_t currentLocal = 0; currentLocal < numberOfStackItems; currentLocal++) {
				uint8_t tag = byteArray.readUnsignedByte();
				if (tag == 7 || tag == 8) {
					uint16_t crap = byteArray.readUnsignedShort();
				}
			}
		}
		else {
			std::cout << "Unknown frame type detected:" << (unsigned int)frameType << std::endl;
			exit(1);
		}
	}
}

ExceptionTableEntry AttributeParser::readExceptionTableEntry(ByteArray& byteArray) {
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

ExceptionTableEntry* AttributeParser::readExceptionTable(ByteArray& byteArray, uint16_t *size) {
	uint16_t exceptionTableLength = byteArray.readUnsignedShort();

	ExceptionTableEntry* table = 0;

	if (exceptionTableLength > 0)
	{
		ExceptionTableEntry* table = (ExceptionTableEntry*)
			malloc(sizeof(ExceptionTableEntry) * exceptionTableLength);
		*size = exceptionTableLength;

		for (uint16_t currentException = 0; currentException < exceptionTableLength; currentException++) {
			ExceptionTableEntry entry = readExceptionTableEntry(byteArray);
			table[currentException] = entry;
		}
	}

	return table;
}

AttributeCollection* AttributeParser::readAttributes(ByteArray& byteArray, ConstantPool* constantPool)
{
	uint16_t attributesCount = byteArray.readUnsignedShort();
	AttributeInfo** attributes = (AttributeInfo**) malloc(attributesCount * sizeof(AttributeInfo*));

	for (int currentAttrib = 0; currentAttrib < attributesCount; currentAttrib++) {
		uint16_t attributeNameIndex = byteArray.readUnsignedShort();
		uint32_t attributeLength = byteArray.readUnsignedInt();

		std::string name = constantPool->getString(attributeNameIndex);

		if (name == "Code") {
			uint16_t maxStack = byteArray.readUnsignedShort();
			uint16_t maxLocals = byteArray.readUnsignedShort();
			uint32_t codeLength = byteArray.readUnsignedInt();
			uint8_t* code = (uint8_t*)malloc(sizeof(uint8_t) * codeLength);
			/*memcpy(code, &bytes[bytePtr], sizeof(uint8_t) * codeLength);
			bytePtr += codeLength;*/
			byteArray.copyBytes(code, codeLength);

			uint16_t exceptionTableSize;
			ExceptionTableEntry* exceptions = readExceptionTable(byteArray, &exceptionTableSize);
			AttributeCollection* attribs = readAttributes(byteArray, constantPool);


			AttributeCode* att = new AttributeCode();
			att->attributeNameIndex = attributeNameIndex;
			att->attributeLength = attributeLength;
			att->maxStack = maxStack;
			att->maxLocals = maxLocals;
			att->codeLength = codeLength;
			att->code = code;
			att->exceptionTable = exceptions;
			att->exceptionTableSize = exceptionTableSize;
			att->attributes = attribs;

			attributes[currentAttrib] = att;
		}
		else if (name == "LineNumberTable") {
			uint16_t lineNumberTableLength = byteArray.readUnsignedShort();
			AttributeLineNumberTable* att = new AttributeLineNumberTable();
			att->attributeNameIndex = attributeNameIndex;
			att->attributeLength = attributeLength;
			att->size = lineNumberTableLength;

			att->entries = (LineNumberTableEntry*)malloc(sizeof(LineNumberTableEntry) * lineNumberTableLength);

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
		else if (name == "LocalVariableTable") {
			uint16_t localVariableTableLength = byteArray.readUnsignedShort();
			AttributeLocalVariableTable* att = new AttributeLocalVariableTable();
			att->attributeNameIndex = attributeNameIndex;
			att->attributeLength = attributeLength;
			att->size = localVariableTableLength;
			att->entries = (LocalVariableTableEntry*) malloc(sizeof(LocalVariableTableEntry) * localVariableTableLength);
			for (int localVariableTableIndex = 0; localVariableTableIndex < localVariableTableLength; localVariableTableIndex++) {
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
		else if (name == "SourceFile") {
			uint16_t sourceFileIndex = byteArray.readUnsignedShort();
			AttributeSourceFile* att = new AttributeSourceFile();
			att->attributeNameIndex = attributeNameIndex;
			att->attributeLength = attributeLength;
			att->sourceFileIndex = sourceFileIndex;

			attributes[currentAttrib] = att;
		}
		else if (name == "StackMapTable") {
			readStackMapTable(byteArray);
			attributes[currentAttrib] = 0;
		}
		else {
			std::cout << "Attribute parsing not implemented yet for type: " << name << std::endl;
			exit(1);
		}
	}

	return new AttributeCollection(attributes, attributesCount);
}