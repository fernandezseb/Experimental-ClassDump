#include "ConstantPool.h"

void ConstantPool::checkIndex(uint16_t index) const
{
	if (index > this->constants.size()) {
		std::cout << "Invalid index into constant pool: %" << index << std::endl;
		exit(1);
	}
}

std::string ConstantPool::getString(uint16_t index) const
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
