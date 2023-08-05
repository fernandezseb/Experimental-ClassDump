// ClassDump.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Core.h"
#include "ClassLoader/ClassLoader.h"
#include "Printer/ClassPrinter.h"

int main(int argc, char* argv[])
{
    std::cout << "Starting ClassDump v0.1..." << std::endl;

    const std::string className = argv[1];

    ClassLoader classLoader;
    ClassInfo classInfo = classLoader.readClass("Main.class");
    ClassPrinter::printClass(classInfo);

    std::cout << "" << std::endl;
}