#pragma once

#include "Core.h"

class ByteArray {
private:
	uint64_t bytePtr = 0;
	uint64_t size = 0;
public:
	uint8_t* bytes = 0;
	
	ByteArray(uint64_t size);
	~ByteArray();

	uint8_t readUnsignedByte();
    uint16_t readUnsignedShort();
	uint32_t readUnsignedInt();

	void copyBytes(uint8_t* target, uint64_t size);
};