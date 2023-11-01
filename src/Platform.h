#pragma once

#include "Memory.h"

class Memory;
struct PlatformFile;

class Platform {
public:
	static void* AllocateMemory(size_t size, size_t baseAddress);
	static void FreeMemory(void* allocatedMemory);
	static PlatformFile* getFile(const char* name, Memory* memory);
	static wchar_t* getFullPath(PlatformFile* file, Memory* memory);
	static uint8_t* readEntireFile(PlatformFile* file, size_t* sizeOut);
};