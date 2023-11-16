#include "Memory.h"

Memory::Memory(size_t size, size_t maxSize) 
	: size(size), maxSize(maxSize)
{
	pageSize = Platform::getPageSize();

	// We want to at least allocate one page if possible
	if (this->size < pageSize) {
		this->size = pageSize;

		// Make sure that we don't allocate more than the max allowed
		if (this->size > maxSize) {
			this->size = maxSize;
		}
	}

	uint8_t* reservedMemory = (uint8_t*)Platform::ReserveMemory(maxSize, 0);
	classMemory = (uint8_t*)Platform::AllocateMemory(this->size, (size_t)reservedMemory);
}

Memory::~Memory()
{
	Platform::FreeMemory(classMemory);
}

void* Memory::alloc(size_t size)
{

	if (ptr + size > this->size) {
		printf(">> Adding memory <<\n");

		size_t toAlloc = pageSize;
		if (this->size + pageSize > maxSize) {
			toAlloc = maxSize - this->size;
		}

		Platform::AllocateMemory(toAlloc, this->size);
		this->size += toAlloc;
	}

	if (ptr + size > this->size) {
		fprintf(stderr, "\nOut of memory\n");
		printSize();
		Platform::ExitProgram(3);
	}

	size_t oldPtr = ptr;
	ptr += size;
	return classMemory + oldPtr;
}

void Memory::printSize()
{
	printf("Memory stats:\n");
	printf("  Memory used: %zu bytes\n", ptr);
	printf("  Memory commited: %zu bytes\n", size);
	printf("  Free memory: %zu bytes\n", (size-ptr));
	printf("  Max memory (reserved): %zu bytes\n", maxSize);
}
