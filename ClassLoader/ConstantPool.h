#pragma once

#include "../Core.h"

enum ConstantType : uint8_t {
	CT_UTF8            = 1u,
	CT_INTEGER         = 3u,
	CT_FLOAT           = 4u,
	CT_LONG            = 5u,
	CT_DOUBLE          = 6u,
	CT_CLASS           = 7u,
	CT_STRING          = 8u,
	CT_FIELDREF        = 9u,
	CT_METHODREF       = 10u,
	CT_INTERFACEMETHOD = 11u,
	CT_NAMEANDTYPE     = 12u,
	CT_METHODHANDLE	   = 15u,
	CT_METHODTYPE      = 16u,
	CT_INVOKEDYNAMIC   = 18u
};

enum AccessFlag : uint16_t {
	ACC_PUBLIC =       0x0001,
	ACC_PRIVATE =      0x0002,
	ACC_PROTECTED =    0x0004,
	ACC_STATIC =       0x0004,
	ACC_FINAL =        0x0010,
	ACC_SYNCHRONIZED = 0x0010,
	ACC_BRIDGE =       0x0010,
	ACC_VARARGS =      0x0080,
	ACC_SUPER =        0x0020,
	ACC_NATIVE =       0x0100,
	ACC_INTERFACE =    0x0200,
	ACC_ABSTRACT =     0x0400,
	ACC_STRICT =       0x0800,
	ACC_SYNTHETIC =    0x1000,
	ACC_ANNOTATION =   0x2000,
	ACC_ENUM =         0x4000
};

struct ConstantPoolItem {
	uint8_t tag;
	ConstantPoolItem(uint8_t tag) :
		tag(tag)
	{
	}

	ConstantType getType() {
		return (ConstantType)tag;
	}

	virtual std::string toString() {
		return "";
	}
};

struct ConstantPoolNamedReference : public ConstantPoolItem {
	uint16_t classIndex;
	uint16_t nameAndTypeIndex;
	ConstantPoolNamedReference(uint8_t tag, uint16_t classIndex, uint16_t nameAndTypeIndex)
		: ConstantPoolItem(tag), classIndex(classIndex), nameAndTypeIndex(nameAndTypeIndex) 
	{
	}
};

struct CPMethodRef : public ConstantPoolNamedReference {
	CPMethodRef(uint8_t tag, uint16_t classIndex, uint16_t nameAndTypeIndex)
		: ConstantPoolNamedReference(tag, classIndex, nameAndTypeIndex)
	{
	}
};

struct CPFieldRef : public ConstantPoolNamedReference {
	CPFieldRef(uint8_t tag, uint16_t classIndex, uint16_t nameAndTypeIndex)
		: ConstantPoolNamedReference(tag, classIndex, nameAndTypeIndex)
	{
	}
};

struct CPInterfaceRef : public ConstantPoolNamedReference {
	CPInterfaceRef(uint8_t tag, uint16_t classIndex, uint16_t nameAndTypeIndex)
		: ConstantPoolNamedReference(tag, classIndex, nameAndTypeIndex)
	{
	}
};

struct CPClassInfo : public ConstantPoolItem {
	uint16_t nameIndex;
	CPClassInfo(uint8_t tag, uint16_t nameIndex)
		: ConstantPoolItem(tag), nameIndex(nameIndex)
	{
	}
};

struct CPIntegerInfo : public ConstantPoolItem {
	uint32_t bytes;
	CPIntegerInfo(uint8_t tag, uint32_t bytes)
		: ConstantPoolItem(tag), bytes(bytes)
	{
	}
};

struct CPFloatInfo : public ConstantPoolItem {
	uint32_t bytes;
	CPFloatInfo(uint8_t tag, uint32_t bytes)
		: ConstantPoolItem(tag), bytes(bytes)
	{
	}
};

struct CPLongInfo : public ConstantPoolItem {
	uint32_t highBytes;
	uint32_t lowBytes;
	CPLongInfo(uint8_t tag, uint32_t highBytes, uint32_t lowBytes)
		: ConstantPoolItem(tag), highBytes(highBytes), lowBytes(lowBytes)
	{
	}
};

struct CPDoubleInfo : public ConstantPoolItem {
	uint32_t highBytes;
	uint32_t lowBytes;
	CPDoubleInfo(uint8_t tag, uint32_t highBytes, uint32_t lowBytes)
		: ConstantPoolItem(tag), highBytes(highBytes), lowBytes(lowBytes)
	{
	}
};

struct CPUTF8Info : public ConstantPoolItem {
	uint16_t length;
	uint8_t* bytes;
	CPUTF8Info(uint8_t tag, uint16_t length, uint8_t* bytes)
		: ConstantPoolItem(tag), length(length), bytes(bytes)
	{
	}
	std::string toString() {
		return (char*) bytes;
	}
};

struct CPNameAndTypeInfo : public ConstantPoolItem {
	uint16_t nameIndex;
	uint16_t descriptorIndex;
	CPNameAndTypeInfo(uint8_t tag, uint16_t nameIndex, uint16_t descriptorIndex)
		: ConstantPoolItem(tag), nameIndex(nameIndex), descriptorIndex(descriptorIndex)
	{
	}
};

struct CPStringInfo : public ConstantPoolItem {
	uint16_t stringIndex;
	CPStringInfo(uint8_t tag, uint16_t stringIndex)
		: ConstantPoolItem(tag), stringIndex(stringIndex)
	{
	}
};

class ConstantPool {
private:
	void checkIndex(uint16_t index);
public:
	std::vector<ConstantPoolItem*> constants;
	std::string getString(uint16_t index);
	CPClassInfo* getClassInfo(uint16_t index);
};