#pragma once

#include "Core.h"

#include <Windows.h>

class Memory {
public:
	uint8_t* classMemory;
	size_t ptr = 0;
	size_t size;

	Memory(size_t size) : size(size)
	{
		classMemory = (uint8_t*) VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	}

	~Memory()
	{
		VirtualFree(classMemory, 0, MEM_RELEASE);
	}

	void* classAlloc(size_t size)
	{
		if (ptr + size > this->size) {
			std::cout << "Out of memory" << std::endl;
			exit(3);
		}
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