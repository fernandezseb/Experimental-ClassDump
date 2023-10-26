#include "ConstantPool.h"

void ConstantPool::checkIndex(uint16_t index) const
{
	if (index > this->constants.size()) {
		std::cout << "Invalid index into constant pool: %" << index << std::endl;
		exit(1);
	}
}

const char* ConstantPool::getString(uint16_t index) const
{
	checkIndex(index);

	ConstantPoolItem*  item = this->constants[index - 1];
	if (item->getType() != CT_UTF8) {
		std::cout << "Trying to read UTF8 string at non UTF8 string in constant pool :" << index << std::endl;
		exit(1);
	}
	else {
		CPUTF8Info* utf8item = (CPUTF8Info*)item;
		char* chars = (char*) utf8item->bytes;
		return chars;
	}
}

CPClassInfo* ConstantPool::getClassInfo(uint16_t index) const
{
	checkIndex(index);

	ConstantPoolItem* item = this->constants[index - 1];
	if (item->getType() != CT_CLASS) {
		std::cout << "Trying to read class info at non class info item in constant pool :" << index << std::endl;
		exit(1);
	}
	else {
		CPClassInfo* cpClassInfo = (CPClassInfo*)item;
		return cpClassInfo;
	}
}

ConstantPool::~ConstantPool()
{
	for (int i = 0; i < this->constants.size(); i++) {
		ConstantPoolItem* item = this->constants[i];
		if (item != 0) {
			delete item;
			item = nullptr;
		}
	}
}

ConstantPoolItem::ConstantPoolItem(uint8_t tag) :
	tag(tag)
{
}

ConstantPoolItem::~ConstantPoolItem()
{

}

std::string ConstantPoolItem::toString()
{
	return "";
}

std::string ConstantPoolItem::toExpandedString(const ConstantPool* cp)
{
	return "";
}

std::string CPMethodRef::toString()
{
	std::string str = "#" + std::to_string(classIndex);
	str += ".#";
	str += std::to_string(nameAndTypeIndex);
	return str;
}

std::string CPMethodRef::toExpandedString(const ConstantPool* cp)
{
	return cp->constants[classIndex - 1]->toExpandedString(cp) + "." + cp->constants[nameAndTypeIndex - 1]->toExpandedString(cp);
}

std::string CPClassInfo::toString()
{
	return "#" + std::to_string(nameIndex);
}

std::string CPClassInfo::toExpandedString(const ConstantPool* cp)
{
	return cp->getString(nameIndex);
}

std::string CPIntegerInfo::toString()
{
	return std::to_string((int)bytes);
}

CPUTF8Info::~CPUTF8Info()
{
	if (bytes != 0) {
		free(bytes);
		bytes = nullptr;
	}
}

std::string CPUTF8Info::toString()
{
	return (char*)bytes;
}

std::string CPNameAndTypeInfo::toString()
{
	std::string str = "#" + std::to_string(nameIndex);
	str += ".#";
	str += std::to_string(descriptorIndex);
	return str;
}

std::string CPNameAndTypeInfo::toExpandedString(const ConstantPool* cp)
{
	std::string name = cp->getString(nameIndex);
	if (name == "<init>") {
		name = "\"" + name + "\"";
	}
	return  name + ":" + cp->getString(descriptorIndex);
}

std::string CPStringInfo::toString()
{
	return "#" + std::to_string(stringIndex);
}

std::string CPStringInfo::toExpandedString(const ConstantPool* cp)
{
	return cp->getString(stringIndex);
}

std::string CPFieldRef::toString()
{
	return "#" + std::to_string(classIndex) + "." + "#" + std::to_string(nameAndTypeIndex);
}

std::string CPFieldRef::toExpandedString(const ConstantPool* cp)
{
	return cp->getClassInfo(classIndex)->toExpandedString(cp) + "." + cp->constants[nameAndTypeIndex - 1]->toExpandedString(cp);
}

std::string CPFloatInfo::toString()
{
	float f = *reinterpret_cast<float*> ( & bytes);
	return std::to_string((float)f) + "f";
}

std::string CPDoubleInfo::toString()
{
	uint64_t bytes = ((uint64_t)highBytes << 32) + (uint64_t)lowBytes;
	double d = *reinterpret_cast<double*> (&bytes);
	return std::to_string((double)d) + "d";
}

std::string CPLongInfo::toString()
{
	int64_t bytes = ((int64_t)highBytes << 32) + (int64_t)lowBytes;
	return std::to_string(bytes) + "l";
}
