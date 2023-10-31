#pragma once

class Platform {
public:
	static void* AllocateMemory(size_t size, size_t baseAddress);
	static void FreeMemory(void* allocatedMemory);
};