#pragma once

#include "Data/Attribute.h"
#include "Data/ConstantPool.h"

class AttributePrinter {
public:
	static void printAttribute(AttributeInfo* attribute, const ConstantPool* cp);
};