



add_executable(descriptor_test 
main.cpp
${PROJECT_SOURCE_DIR}/src/ClassLoader/DescriptorParser.cpp
${PROJECT_SOURCE_DIR}/src/Util.h
${PROJECT_SOURCE_DIR}/src/Util.cpp
${PROJECT_SOURCE_DIR}/src/WindowsPlatform.cpp)

target_include_directories(descriptor_test PUBLIC ${PROJECT_SOURCE_DIR}/src/)