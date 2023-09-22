cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

project(ClassDump
    VERSION 0.1
    DESCRIPTION "A simple Java class file parser for learning purposes"
    LANGUAGES CXX)

    add_executable(ClassDump
    src/ClassDump.cpp
    src/Printer/ClassPrinter.cpp
    src/ClassLoader/ClassLoader.cpp
    src/ClassLoader/ConstantPool.cpp)