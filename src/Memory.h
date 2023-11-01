#pragma once

#include "Core.h"
#include "Platform.h"

class Memory {
public:
	uint8_t* classMemory;
	size_t ptr = 0;
	size_t size;

	Memory(size_t size);

	~Memory();

	void* classAlloc(size_t size);

	void printSize();
};