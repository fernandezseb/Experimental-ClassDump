#pragma once

#include <cinttypes>
#include <cstdio>

struct PlatformFile;

class Platform {
public:
	static void* AllocateMemory(size_t size, size_t baseAddress);
	static void FreeMemory(void* allocatedMemory);
	static PlatformFile* getFile(const char* name);
	static char* getFullPath(PlatformFile* file, char* charOut);
	static uint8_t* readEntireFile(PlatformFile* file, size_t* sizeOut);
	static void getLastModifiedString(PlatformFile* file, char* stringOut);
	static void closeFile(PlatformFile* file);
	static void ExitProgram(uint32_t exitCode);
};