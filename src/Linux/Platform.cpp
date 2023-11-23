#include "Platform.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>


struct PlatformFile {
	int fd;
	const char* name;
	uint8_t* fileMemory;
};



void* Platform::AllocateMemory(size_t size, size_t baseAddress)
{
		return malloc(size);
}

void Platform::FreeMemory(void* allocatedMemory)
{
	free(allocatedMemory);
}

PlatformFile* Platform::getFile(const char* name)
{
	PlatformFile *file = (PlatformFile*) AllocateMemory(sizeof(PlatformFile), 0);

	file->fd = open(name, O_RDONLY);
	file->name = name;
	file->fileMemory = NULL;

	return file;
}

char* Platform::getFullPath(PlatformFile* file, char* charOut)
{
	char* absolutePath = realpath(file->name, NULL);	

	strcpy(charOut, absolutePath);

	free(absolutePath);

	return charOut;
}

uint8_t* Platform::readEntireFile(PlatformFile* file, size_t* sizeOut)
{
	struct stat st;
	stat(file->name, &st);
	uint64_t size = st.st_size;
	
	*sizeOut = size;

	uint8_t* fileMemory = (uint8_t*)Platform::AllocateMemory(size, 0);
	file->fileMemory = fileMemory;

	uint64_t bytesRead = read(file->fd, fileMemory, size);


	return fileMemory;
}

void Platform::getLastModifiedString(PlatformFile* file, char* stringOut)
{

	char time[50];
	struct stat attrib;
    stat(file->name, &attrib);
	strftime(time, 20, "%d-%m-%y", localtime(&(attrib.st_ctime)));
	strcpy(stringOut, time);
}

void Platform::closeFile(PlatformFile* file)
{
	close(file->fd);
	FreeMemory(file);
}

void Platform::ExitProgram(uint32_t exitCode)
{
	exit(exitCode);
}

size_t Platform::getPageSize()
{
	return getpagesize();
}
