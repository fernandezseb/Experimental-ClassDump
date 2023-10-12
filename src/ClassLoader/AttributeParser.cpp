#include "AttributeParser.h"

// TODO: De-duplicate this stuff
static AttributeInfo* findAttributeWithName(std::vector<AttributeInfo*> attributes, ConstantPool& constantPool, std::string name)
{
    AttributeInfo* attrib = 0;

    for (AttributeInfo* attrib : attributes) {
        if (constantPool.getString(attrib->attributeNameIndex) == name) {
            return attrib;
        }
    }

    return attrib;
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
            std::cout << "Unknown frame type detected:" << (unsigned int)frameType;
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

std::vector<ExceptionTableEntry> AttributeParser::readExceptionTable(ByteArray& byteArray) {
    std::vector<ExceptionTableEntry> table;
    uint16_t exceptionTableLength = byteArray.readUnsignedShort();

    for (uint16_t currentException = 0; currentException < exceptionTableLength; currentException++) {
        ExceptionTableEntry entry = readExceptionTableEntry(byteArray);
        table.push_back(entry);
    }

    return table;
}

std::vector<AttributeInfo*> AttributeParser::readAttributes(ByteArray& byteArray, ConstantPool& constantPool)
{
    std::vector<AttributeInfo*> attributes;

    uint16_t attributesCount = byteArray.readUnsignedShort();

    for (int currentAttrib = 0; currentAttrib < attributesCount; currentAttrib++) {
        uint16_t attributeNameIndex = byteArray.readUnsignedShort();
        uint32_t attributeLength = byteArray.readUnsignedInt();

        std::string name = constantPool.getString(attributeNameIndex);

        if (name == "Code") {
            uint16_t maxStack = byteArray.readUnsignedShort();
            uint16_t maxLocals = byteArray.readUnsignedShort();
            uint32_t codeLength = byteArray.readUnsignedInt();
            uint8_t* code = (uint8_t*)malloc(sizeof(uint8_t) * codeLength);
            /*memcpy(code, &bytes[bytePtr], sizeof(uint8_t) * codeLength);
            bytePtr += codeLength;*/
            byteArray.copyBytes(code, codeLength);

            std::vector<ExceptionTableEntry> exceptions = readExceptionTable(byteArray);
            std::vector<AttributeInfo*> attribs = readAttributes(byteArray, constantPool);


            AttributeCode* att = new AttributeCode();
            att->attributeNameIndex = attributeNameIndex;
            att->attributeLength = attributeLength;
            att->maxStack = maxStack;
            att->maxLocals = maxLocals;
            att->codeLength = codeLength;
            att->code = code;
            att->exceptionTable = exceptions;
            att->attributes = attribs;
            AttributeLineNumberTable* lineNumberTable = (AttributeLineNumberTable*)findAttributeWithName(attribs, constantPool, "LineNumberTable");
            att->lineNumberTable = lineNumberTable;
            att->localVariableTable = (AttributeLocalVariableTable*)findAttributeWithName(attribs, constantPool, "LocalVariableTable");

            attributes.push_back(att);
        }
        else if (name == "LineNumberTable") {
            uint16_t lineNumberTableLength = byteArray.readUnsignedShort();
            AttributeLineNumberTable* att = new AttributeLineNumberTable();
            att->attributeNameIndex = attributeNameIndex;
            att->attributeLength = attributeLength;
            for (int lineNumerTableIndex = 0; lineNumerTableIndex < lineNumberTableLength; lineNumerTableIndex++) {
                uint16_t startPc = byteArray.readUnsignedShort();
                uint16_t lineNumber = byteArray.readUnsignedShort();
                LineNumberTableEntry* entry = new LineNumberTableEntry();
                entry->startPc = startPc;
                entry->lineNumber = lineNumber;
                att->entries.push_back(entry);
            }
            attributes.push_back(att);
        }
        else if (name == "LocalVariableTable") {
            uint16_t localVariableTableLength = byteArray.readUnsignedShort();
            AttributeLocalVariableTable* att = new AttributeLocalVariableTable();
            att->attributeNameIndex = attributeNameIndex;
            att->attributeLength = attributeLength;
            for (int localVariableTableIndex = 0; localVariableTableIndex < localVariableTableLength; localVariableTableIndex++) {
                uint16_t startPc = byteArray.readUnsignedShort();
                uint16_t length = byteArray.readUnsignedShort();
                uint16_t nameIndex = byteArray.readUnsignedShort();
                uint16_t descriptorIndex = byteArray.readUnsignedShort();
                uint16_t index = byteArray.readUnsignedShort();
                LocalVariableTableEntry* entry = new LocalVariableTableEntry();
                entry->startPc = startPc;
                entry->length = length;
                entry->nameIndex = nameIndex;
                entry->descriptorIndex = descriptorIndex;
                entry->index = index;
                att->entries.push_back(entry);
            }
            attributes.push_back(att);
        }
        else if (name == "SourceFile") {
            uint16_t sourceFileIndex = byteArray.readUnsignedShort();
            AttributeSourceFile* att = new AttributeSourceFile();
            att->attributeNameIndex = attributeNameIndex;
            att->attributeLength = attributeLength;
            att->sourceFileIndex = sourceFileIndex;

            attributes.push_back(att);
        }
        else if (name == "StackMapTable") {
            readStackMapTable(byteArray);
        }
        else {
            std::cout << "Attribute parsing not implemented yet for type: " << name << std::endl;
            exit(1);
        }
    }

    return attributes;
}