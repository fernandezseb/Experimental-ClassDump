#pragma once

#include "Core.h"

class Memory {
public:
	uint8_t* classMemory;
	size_t ptr = 0;
	size_t size;

	Memory(size_t size) : size(size)
	{
		classMemory = (uint8_t*) Platform::AllocateMemory(size, 0);
	}

	~Memory()
	{
		Platform::FreeMemory(classMemory);
	}

	void* classAlloc(size_t size)
	{
		if (ptr + size > this->size) {
			fprintf(stderr, "Out of memory\n");
			exit(3);
		}
		size_t oldPtr = ptr;
		ptr += size;
		return classMemory + oldPtr;
	}

	void printSize()
	{
		printf("Memory used: %zu bytes\n", ptr);
		printf("Free memory: %zu bytes\n", (size - ptr));
	}
};