#pragma once

#include "Core.h"
#include "Platform.h"

class Memory {
public:
	uint8_t* classMemory;
	size_t ptr = 0;
	size_t size;
	size_t maxSize;
	size_t pageSize;

	Memory(size_t size, size_t maxSize);

	~Memory();

	void* alloc(size_t size);

	void printSize();
};