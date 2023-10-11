#include "ByteArray.h"

ByteArray::ByteArray(uint64_t size) {
	bytes = new uint8_t[size];
}

ByteArray::~ByteArray() {
	delete[] bytes;
}

uint8_t ByteArray::readUnsignedByte()
{
	uint8_t buffer = 0;
	buffer = bytes[bytePtr++];
	return buffer;
}

uint16_t ByteArray::readUnsignedShort()
{
	uint8_t buffer[2] = { bytes[bytePtr++], bytes[bytePtr++] };
	uint16_t value = (uint16_t)buffer[1] | (uint16_t)buffer[0] << 8;
	return value;
}

uint32_t ByteArray::readUnsignedInt()
{
	uint8_t buffer[4] = { bytes[bytePtr++], bytes[bytePtr++] , bytes[bytePtr++] , bytes[bytePtr++] };
	uint32_t value = (uint32_t)buffer[3] | (uint32_t)buffer[2] << 8 | (uint32_t)buffer[1] << 16 | (uint32_t)buffer[0] << 24;
	return (uint32_t)value;
}

void ByteArray::copyBytes(uint8_t* target, uint64_t size)
{
	memcpy(target, &bytes[bytePtr], sizeof(uint8_t) * size);
	bytePtr += size;
}
