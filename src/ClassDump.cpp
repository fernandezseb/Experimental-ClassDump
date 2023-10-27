// ClassDump.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Core.h"
#include "ClassLoader/ClassLoader.h"
#include "Printer/ClassPrinter.h"
#include "ClassDumpConfig.h"

int main(int argc, char* argv[])
{
    std::cout << "Starting ClassDump v" << CLASSDUMP_VERSION_MAJOR << "." << CLASSDUMP_VERSION_MINOR << std::endl;

    if (argc > 1) {
        {
            const char* className = argv[1];

            ClassLoader classLoader;
            ClassPrinter classPrinter;
            ClassInfo* classInfo = classLoader.readClass(className);
            classPrinter.printClass(*classInfo);
            classInfo->memory->printSize();
            delete classInfo->memory;
            //classInfo->~ClassInfo();
            //delete classInfo;
        }
        _CrtDumpMemoryLeaks();
    } else {
        std::cout << "Class name must be supplied" << std::endl;
        exit(1);
    }

    return 0;
}