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
    src/Printer/AttributePrinter.h
    src/Printer/AttributePrinter.cpp
    src/Core.h 
    src/Util.cpp
    src/Util.h 
    src/Opcode.h
    src/ByteArray.h
    src/ByteArray.cpp
    src/Memory.h
    src/Platform.h
    src/ClassLoader/DescriptorParser.h
    src/ClassLoader/DescriptorParser.cpp 
    src/ClassLoader/AttributeParser.h
    src/ClassLoader/AttributeParser.cpp
    src/ClassLoader/ClassLoader.cpp
    src/ClassLoader/ClassLoader.h
    src/Data/Class.h
    src/Data/Attribute.h
    src/Data/Descriptor.h
    src/Data/ConstantPool.cpp
    src/Data/ConstantPool.h
    src/Data/Attribute.cpp
    src/Printer/CodePrinter.h
    src/Printer/CodePrinter.cpp
    src/Printer/PrintUtils.h
    src/Printer/PrintUtils.cpp
    src/OutputBuffer.cpp
    src/OutputBuffer.h)


    target_include_directories(${PROJECT_NAME} PUBLIC src/ extern/ ${PROJECT_BINARY_DIR} test/)
    
    add_subdirectory(extern/md5)
    target_link_directories(${PROJECT_NAME} PRIVATE MD5)
    target_link_libraries(${PROJECT_NAME} MD5)
 
    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} /GR- -EHa- -Gm- -DUNICODE -D_UNICODE")

        add_subdirectory(src/Windows)
        target_link_directories(${PROJECT_NAME} PRIVATE Windows)
        target_link_libraries(${PROJECT_NAME} Windows)
    endif()

    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        add_subdirectory(src/Linux)
        target_link_directories(${PROJECT_NAME} PRIVATE Linux)
        target_link_libraries(${PROJECT_NAME} Linux)
    endif()

    enable_testing()
    add_subdirectory("test")