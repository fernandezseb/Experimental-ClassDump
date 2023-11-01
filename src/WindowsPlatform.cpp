#include "Platform.h"

#include <iostream>
#include <filesystem>

#include <Windows.h>


struct PlatformFile {
	std::filesystem::path path;
	uint8_t* fileMemory;
};

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

PlatformFile* Platform::getFile(const char* name, Memory* memory)
{
	PlatformFile *file = (PlatformFile*) memory->classAlloc(sizeof(PlatformFile));
	try {
		file->path = std::filesystem::canonical(name);
	}
	catch (const std::exception& ex) {
		fprintf(stderr, "Error: Canonical path for %s threw exception: %s\n", name, ex.what());
		return nullptr;
	}
	return file;
}

wchar_t* Platform::getFullPath(PlatformFile* file, Memory* memory)
{
	const wchar_t* absolutePath = file->path.c_str();

	wchar_t* pathstr = (wchar_t*)memory->classAlloc(((wcslen(absolutePath) + 1) * sizeof(wchar_t)));
	wcscpy(pathstr, absolutePath);

	return pathstr;
}

uint8_t* Platform::readEntireFile(PlatformFile* file, size_t* sizeOut)
{
	uint64_t size = std::filesystem::file_size(file->path);

	*sizeOut = size;
	uint8_t* fileMemory = (uint8_t*)Platform::AllocateMemory(size, 0);
	file->fileMemory = fileMemory;

	std::ifstream myFile(file->path, std::ios::in | std::ios::binary);
	
	myFile.read((char*)fileMemory, size);
	myFile.close();
	

	return fileMemory;
}
