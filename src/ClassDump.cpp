// ClassDump.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Core.h"
#include "ClassLoader/ClassLoader.h"
#include "Printer/ClassPrinter.h"

int main(int argc, char* argv[])
{
    std::cout << "Starting ClassDump v0.1..." << std::endl;

    if (argc > 1) {
        const std::string className = argv[1];

        ClassLoader classLoader;
        ClassPrinter classPrinter;
        ClassInfo classInfo = classLoader.readClass(className);
        classPrinter.printClass(classInfo);

        std::cout << "" << std::endl;
    } else {
        std::cout << "Class name must be supplied" << std::endl;
        exit(1);
    }
}