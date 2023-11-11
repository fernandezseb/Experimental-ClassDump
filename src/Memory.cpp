#include "Memory.h"

Memory::Memory(size_t size) : size(size)
{
	classMemory = (uint8_t*)Platform::AllocateMemory(size, 0);
}

Memory::~Memory()
{
	Platform::FreeMemory(classMemory);
}

void* Memory::classAlloc(size_t size)
{
	if (ptr + size > this->size) {
		fprintf(stderr, "Out of memory\n");
		Platform::ExitProgram(3);
	}
	size_t oldPtr = ptr;
	ptr += size;
	return classMemory + oldPtr;
}

void Memory::printSize()
{
	printf("Memory used: %zu bytes\n", ptr);
	printf("Free memory: %zu bytes\n", (size - ptr));
}
