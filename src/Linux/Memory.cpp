#include "Memory.h"

Memory::Memory(size_t size, size_t maxSize) 
	: size(size), maxSize(maxSize)
{
	this->size = maxSize;
	memoryPtr = (uint8_t*)Platform::AllocateMemory(this->maxSize, 0);
}

Memory::~Memory()
{
	Platform::FreeMemory(memoryPtr);
}

void* Memory::alloc(size_t size)
{
	if (ptr + size > this->size) {
		fprintf(stderr, "\nOut of memory\n");
		printSize();
		Platform::ExitProgram(3);
	}

	size_t oldPtr = ptr;
	ptr += size;
	return memoryPtr + oldPtr;
}

void Memory::printSize()
{
	printf("Memory stats:\n");
	printf("  Memory used: %zu bytes\n", ptr);
	printf("  Free memory: %zu bytes\n", (size-ptr));
}
