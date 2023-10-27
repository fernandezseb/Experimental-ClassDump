#pragma once

#include "Core.h"

class Memory {
public:
	uint8_t* classMemory;
	size_t ptr = 0;
	size_t size;

	Memory(size_t size) : size(size)
	{
		classMemory = (uint8_t*)malloc(size);
	}

	~Memory()
	{
		free(classMemory);
	}

	void* classAlloc(size_t size)
	{
		size_t oldPtr = ptr;
		ptr += size;
		return classMemory + oldPtr;
	}

	void printSize()
	{
		std::cout << "Memory used: " << ptr << " bytes" << std::endl;
		std::cout << "Free class memory: " << (size - ptr) << " bytes" << std::endl;
	}
};