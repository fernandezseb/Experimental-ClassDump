#include "Platform.h"

#include <Windows.h>

void* Platform::AllocateMemory(size_t size, size_t baseAddress)
{
	return VirtualAlloc(
		0, 
		size, 
		MEM_COMMIT | MEM_RESERVE, 
		PAGE_READWRITE);
}

void Platform::FreeMemory(void* allocatedMemory)
{
	VirtualFree(
		allocatedMemory,
		0,
		MEM_RELEASE);
}