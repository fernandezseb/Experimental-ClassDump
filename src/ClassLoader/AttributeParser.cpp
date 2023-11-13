#include "AttributeParser.h"

static inline VerificationTypeInfo readVerificationTypeInfo(ByteArray& byteArray)
{
	VerificationTypeInfo typeInfo;
	uint8_t tag = byteArray.readUnsignedByte();
	typeInfo.tag = tag;

	if (tag == 7 || tag == 8) {
		uint16_t data = byteArray.readUnsignedShort();
		typeInfo.data = data;
	}

	return typeInfo;
}

StackMapTableAttribute* AttributeParser::readStackMapTable(ByteArray& byteArray, Memory* memory)
{
	StackMapTableAttribute* attribute = (StackMapTableAttribute*) memory->alloc(sizeof(StackMapTableAttribute));

	uint16_t numberOfEntries = byteArray.readUnsignedShort();
	attribute->entriesCount = numberOfEntries;
	attribute->entries = (StackMapFrame**)memory->alloc(sizeof(StackMapFrame*) * numberOfEntries);
	for (uint16_t currentEntry = 0; currentEntry < numberOfEntries; ++currentEntry) {
		StackMapFrame* stackMapFrame = 0;
		
		uint8_t frameType = byteArray.readUnsignedByte();
		
		if (frameType >= 0 && frameType <= 63) {
			// same frame
			SameFrame* frame = (SameFrame*)memory->alloc(sizeof(SameFrame));
			frame->frameType = SameFrameType;
			frame->offsetDelta = frameType;
			stackMapFrame = frame;
		}
		else if (frameType >= 64 && frameType <= 127) {
			// same locals 1 stack item frame
			SameLocals1StackItemFrame* frame = (SameLocals1StackItemFrame*)memory->alloc(sizeof(SameLocals1StackItemFrame));
			frame->frameType = SameLocals1StackItemFrameType;
			frame->stack = readVerificationTypeInfo(byteArray);
			frame->offsetDelta = frameType - 64;

			stackMapFrame = frame;
		}
		else if (frameType == 247) {
			// same locals 1 stack item frame extended
			SameLocals1StackItemFrameExtended* frame = (SameLocals1StackItemFrameExtended*)memory->alloc(sizeof(SameLocals1StackItemFrameExtended));

			frame->frameType = SameLocals1StackItemFrameExtendedType;
			uint16_t offsetDelta = byteArray.readUnsignedShort();
			frame->offsetDelta = offsetDelta;
			frame->stack = readVerificationTypeInfo(byteArray);

			stackMapFrame = frame;
		}
		else if (frameType >= 248 && frameType <= 250) {
			// chop frame
			ChopFrame* frame = (ChopFrame*)memory->alloc(sizeof(ChopFrame));
			frame->frameType = ChopFrameType;
			uint16_t offsetDelta = byteArray.readUnsignedShort();
			frame->offsetDelta = offsetDelta;
			stackMapFrame = frame;
		}
		else if (frameType == 251) {
			// same frame extended
			SameFrameExtended* frame = (SameFrameExtended*)memory->alloc(sizeof(SameFrameExtended));
			frame->frameType = SameFrameExtendedType;
			uint16_t offsetDelta = byteArray.readUnsignedShort();
			frame->offsetDelta = offsetDelta;
			stackMapFrame = frame;
		}
		else if (frameType >= 252 && frameType <= 254) {
			// append frame
			AppendFrame* frame = (AppendFrame*)memory->alloc(sizeof(AppendFrame));
			frame->frameType = AppendFrameType;
			uint16_t offsetDelta = byteArray.readUnsignedShort();
			frame->offsetDelta = offsetDelta;
			uint16_t numberOfLocals = frameType - 251;
			frame->numberOfLocals = numberOfLocals;
			frame->locals = (VerificationTypeInfo*)memory->alloc(sizeof(VerificationTypeInfo) * numberOfLocals);
			for (uint16_t currentLocal = 0; currentLocal < numberOfLocals; ++currentLocal) {
				frame->locals[currentLocal] = readVerificationTypeInfo(byteArray);
			}
			stackMapFrame = frame;
		}
		else if (frameType == 255) {
			// full frame
			FullFrame* frame = (FullFrame*) memory->alloc(sizeof(FullFrame));
			frame->frameType = FullFrameType;

			uint16_t offsetDelta = byteArray.readUnsignedShort();
			
			uint16_t numberOfLocals = byteArray.readUnsignedShort();
			frame->offsetDelta = offsetDelta;
			frame->numberOfLocals = numberOfLocals;
			frame->locals = (VerificationTypeInfo*)memory->alloc(sizeof(VerificationTypeInfo) * numberOfLocals);
			for (uint16_t currentLocal = 0; currentLocal < numberOfLocals; ++currentLocal) {
				frame->locals[currentLocal] = readVerificationTypeInfo(byteArray);
			}
			
			uint16_t numberOfStackItems = byteArray.readUnsignedShort();
			frame->numberOfStackItems = numberOfStackItems;
			frame->stack = (VerificationTypeInfo*)memory->alloc(sizeof(VerificationTypeInfo) * numberOfStackItems);
			for (uint16_t currentStackItem = 0; currentStackItem < numberOfStackItems; ++currentStackItem) {
				frame->stack[currentStackItem] = readVerificationTypeInfo(byteArray);
			}

			stackMapFrame = frame;
		}
		else {
			fprintf(stderr, "Error: Unknown frame type detected : %" PRIu8 "\n", frameType);
			Platform::ExitProgram(1);
		}
		attribute->entries[currentEntry] = stackMapFrame;
	}

	return attribute;
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
			memory->alloc(sizeof(ExceptionTableEntry) * exceptionTableLength);
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
	AttributeInfo** attributes = (AttributeInfo**) memory->alloc(attributesCount * sizeof(AttributeInfo*));

	for (int currentAttrib = 0; currentAttrib < attributesCount; ++currentAttrib) {
		uint16_t attributeNameIndex = byteArray.readUnsignedShort();
		uint32_t attributeLength = byteArray.readUnsignedInt();

		const char* name = constantPool->getString(attributeNameIndex);

		if (strcmp(name, "Code") == 0) {
			uint16_t maxStack = byteArray.readUnsignedShort();
			uint16_t maxLocals = byteArray.readUnsignedShort();
			uint32_t codeLength = byteArray.readUnsignedInt();
			uint8_t* code = (uint8_t*)memory->alloc(sizeof(uint8_t) * codeLength);
			/*memcpy(code, &bytes[bytePtr], sizeof(uint8_t) * codeLength);
			bytePtr += codeLength;*/
			byteArray.copyBytes(code, codeLength);

			uint16_t exceptionTableSize;
			ExceptionTableEntry* exceptions = readExceptionTable(byteArray, &exceptionTableSize, memory);
			AttributeCollection* attribs = readAttributes(byteArray, constantPool, memory);


			AttributeCode* att = (AttributeCode*)memory->alloc(sizeof(AttributeCode));
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
			AttributeLineNumberTable* att = (AttributeLineNumberTable*) memory->alloc(sizeof(AttributeLineNumberTable));
			att->attributeNameIndex = attributeNameIndex;
			att->attributeLength = attributeLength;
			att->size = lineNumberTableLength;
			att->type = LineNumberTable;

			att->entries = (LineNumberTableEntry*)memory->alloc(sizeof(LineNumberTableEntry) * lineNumberTableLength);

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
			AttributeLocalVariableTable* att = (AttributeLocalVariableTable*) memory->alloc(sizeof(AttributeLocalVariableTable));
			att->attributeNameIndex = attributeNameIndex;
			att->attributeLength = attributeLength;
			att->size = localVariableTableLength;
			att->type = LocalVariableTable;
			att->entries = (LocalVariableTableEntry*) memory->alloc(sizeof(LocalVariableTableEntry) * localVariableTableLength);
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
			AttributeSourceFile* att = (AttributeSourceFile*) memory->alloc(sizeof(AttributeSourceFile));
			att->attributeNameIndex = attributeNameIndex;
			att->attributeLength = attributeLength;
			att->sourceFileIndex = sourceFileIndex;
			att->type = SourceFile;

			attributes[currentAttrib] = att;
		}
		else if (strcmp(name, "StackMapTable") == 0) {
			AttributeInfo* attribute = readStackMapTable(byteArray, memory);
			attribute->attributeLength = attributeLength;
			attribute->attributeNameIndex = attributeNameIndex;
			attribute->type = StackMapTable;
			attributes[currentAttrib] = attribute;
		}
		else if (strcmp(name, "ConstantValue") == 0) {
			AttributeConstantValue* attribute = (AttributeConstantValue*)memory->alloc(sizeof(AttributeConstantValue));
			attribute->type = ConstantValue;
			attribute->attributeLength = attributeLength;
			attribute->attributeNameIndex = attributeNameIndex;
			attribute->constantValueIndex = byteArray.readUnsignedShort();
			attributes[currentAttrib] = attribute;
		}
		else {
			printf("Error: Attribute parsing not implemented yet for type: %s\n", name);
			Platform::ExitProgram(1);
		}
	}

	AttributeCollection* attributeCollection = (AttributeCollection*)
		memory->alloc(sizeof(AttributeCollection));
	attributeCollection->attributes = attributes;
	attributeCollection->attributesCount = attributesCount;

	return attributeCollection;
}