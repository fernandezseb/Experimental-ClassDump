#include "AttributePrinter.h"

void AttributePrinter::printAttribute(AttributeInfo* attribute, const ConstantPool* cp)
{
	if (attribute->type == SourceFile) {
		AttributeSourceFile* att = (AttributeSourceFile*)attribute;
		printf("SourceFile: \"%s\"\n", cp->getString(att->sourceFileIndex));
	}
	else if (attribute->type == LineNumberTable) {
		AttributeLineNumberTable* att = (AttributeLineNumberTable*)attribute;
		for (uint16_t currentIndex = 0; currentIndex < att->size; ++currentIndex) {
			LineNumberTableEntry& entry = att->entries[currentIndex];
			printf("        line %" PRIu16 ": %" PRIu16 "\n", entry.lineNumber, entry.startPc);
		}
	}
	else if (attribute->type == LocalVariableTable) {
		AttributeLocalVariableTable* att = (AttributeLocalVariableTable*)attribute;
		printf("        Start  Length  Slot  Name   Signature\n");

		for (uint16_t currentIndex = 0; currentIndex < att->size; ++currentIndex) {
			LocalVariableTableEntry& entry = att->entries[currentIndex];
			
			printf("        %5" PRIu16, entry.startPc);
			printf("  %6" PRIu16, entry.length);
			printf("  %4" PRIu16, entry.index);
			printf("  %4s", cp->getString(entry.nameIndex));
			printf("   %-10s\n", cp->getString(entry.descriptorIndex));
		}
	}
	else if (attribute->type == StackMapTable) {
		StackMapTableAttribute* att = (StackMapTableAttribute*)attribute;
		printf("        number_of_entries = %" PRIu16"\n", att->entriesCount);
		for (uint16_t currentFrame = 0; currentFrame < att->entriesCount; ++currentFrame) {
			printf("          frame_type = %" PRIu16 "\n", att->entries[currentFrame]->frameType);
		}
	}
}
