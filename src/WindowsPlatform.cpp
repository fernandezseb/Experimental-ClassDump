#include "Platform.h"

#include <Windows.h>

struct PlatformFile {
	uint8_t* fileMemory;
	const char* name;
	HANDLE hFile;

};

void* Platform::AllocateMemory(size_t size, size_t baseAddress)
{
	return VirtualAlloc(
		0, 
		size, 
		MEM_COMMIT | MEM_RESERVE, 
		PAGE_READWRITE);
}

void Platform::FreeMemory( void* allocatedMemory)
{
	VirtualFree(
		allocatedMemory,
		0,
		MEM_RELEASE);
}

PlatformFile* Platform::getFile(const char* name, Memory* memory)
{
	PlatformFile *file = (PlatformFile*) memory->classAlloc(sizeof(PlatformFile));

	file->hFile = CreateFileA(
		name,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	file->name = name;

	return file;
}

char* Platform::getFullPath(PlatformFile* file, Memory* memory)
{
	char absolutePath[500];
	char** lastPart = 0;

	GetFullPathNameA(file->name, 500, absolutePath, lastPart);
	char* pathstr = (char*)memory->classAlloc(((strlen(absolutePath) + 1) * sizeof(char)));

	for (uint8_t index = 0; absolutePath[index] != 0; ++index) {
		if (absolutePath[index] == '\\') {
			absolutePath[index] = '/';
		}
	}

	strcpy(pathstr, absolutePath);

	return pathstr;
}

uint8_t* Platform::readEntireFile(PlatformFile* file, size_t* sizeOut)
{
	uint64_t size = GetFileSize(file->hFile, 0);

	*sizeOut = size;

	DWORD readBytes;
	uint8_t* fileMemory = (uint8_t*)Platform::AllocateMemory(size, 0);
	file->fileMemory = fileMemory;

	ReadFile(
		file->hFile,
		fileMemory,
		size,
		&readBytes,
		NULL);
	

	return fileMemory;
}

void Platform::getLastModifiedString(PlatformFile* file, char* stringOut)
{
	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC, stLocal;
	char months[12][4] = {
		"Jan",
		"Feb",
		"Mar",
		"Apr",
		"May",
		"Jun",
		"Jul",
		"Aug",
		"Sep",
		"Oct",
		"Nov",
		"Dec"
	};
	// Retrieve the file times for the file.
	if (!GetFileTime(file->hFile, &ftCreate, &ftAccess, &ftWrite))
		return;

	// Convert the last-write time to local time.
	FileTimeToSystemTime(&ftWrite, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	char time[50];
	sprintf(time, "%s %02d, %d", months[stLocal.wMonth-1], stLocal.wDay, stLocal.wYear);
	strcpy(stringOut, time);
}

void Platform::closeFile(PlatformFile* file)
{
	CloseHandle(file->hFile);
}
