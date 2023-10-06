cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

project(ClassDump
    VERSION 0.1
    DESCRIPTION "A simple Java class file parser for learning purposes"
    LANGUAGES CXX)

    configure_file(ClassDumpConfig.h.in ClassDumpConfig.h)

    add_executable(${PROJECT_NAME}
    src/ClassDump.cpp
    src/Printer/ClassPrinter.cpp
    src/Printer/ClassPrinter.h
    src/ClassLoader/ClassLoader.cpp
    src/ClassLoader/ClassLoader.h
    src/ClassLoader/ConstantPool.cpp
    src/ClassLoader/ConstantPool.h
    src/Core.h 
    src/Util.cpp
    src/Util.h 
    src/ClassLoader/DescriptorParser.h
    src/ClassLoader/DescriptorParser.cpp)

    add_subdirectory(extern/md5)
    target_include_directories(${PROJECT_NAME} PUBLIC src/ extern/ ${PROJECT_BINARY_DIR} test/)
    target_link_directories(${PROJECT_NAME} PRIVATE MD5)
    target_link_libraries(${PROJECT_NAME} MD5)

    enable_testing()
    add_subdirectory("test")