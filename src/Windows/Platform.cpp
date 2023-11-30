#include "Platform.h"

#include <Windows.h>

#include "Util.h"

struct PlatformFile {
	uint8_t* fileMemory;
	const char* name;
	HANDLE hFile;

};

void Platform::Initialize()
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	if (hStdout == INVALID_HANDLE_VALUE)
	{
		AllocConsole();
		AttachConsole(ATTACH_PARENT_PROCESS);

		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

		if (hStdout == INVALID_HANDLE_VALUE)
		{
			Platform:ExitProgram(-6);
		}
	}

	SetConsoleCP(65001);
}

void Platform::print(const char* string, uint64_t length)
{
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), string, (DWORD)length, NULL, NULL);
}

void Platform::printModifiedUtf8String(char* string)
{
	JString jstring = {0};
	size_t length = strlen(string);
	jstring.chars = (char*) AllocateMemory(length, 0); // In the worst case, the string has the same length
	jstring.length = length;
	modifiedUtf8ToStandardUtf8(string, &jstring);
	print(jstring.chars, jstring.length);
	FreeMemory(jstring.chars);
}

void* Platform::AllocateMemory(size_t size, size_t baseAddress)
{
	LPVOID lpBaseAddress = (LPVOID)baseAddress;
	return VirtualAlloc(
		lpBaseAddress, 
		size, 
		MEM_COMMIT, 
		PAGE_READWRITE);
}

void Platform::FreeMemory(void* allocatedMemory)
{
	VirtualFree(
		allocatedMemory,
		0,
		MEM_RELEASE);
}

PlatformFile* Platform::getFile(const char* name)
{
	PlatformFile *file = (PlatformFile*) AllocateMemory(sizeof(PlatformFile), 0);

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

char* Platform::getFullPath(PlatformFile* file, char* charOut)
{
	char absolutePath[500];
	char** lastPart = 0;

	GetFullPathNameA(file->name, 500, absolutePath, lastPart);

	for (uint8_t index = 0; absolutePath[index] != 0; ++index) {
		if (absolutePath[index] == '\\') {
			absolutePath[index] = '/';
		}
	}

	strcpy(charOut, absolutePath);

	return charOut;
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
	FreeMemory(file);
}

void Platform::ExitProgram(uint32_t exitCode)
{
	ExitProcess(exitCode);
}

size_t Platform::getPageSize()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwPageSize;
}
