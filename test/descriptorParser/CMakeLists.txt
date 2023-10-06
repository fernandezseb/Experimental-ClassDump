



add_executable(descriptor_test 
main.cpp
${PROJECT_SOURCE_DIR}/src/ClassLoader/DescriptorParser.cpp)

target_include_directories(descriptor_test PUBLIC ${PROJECT_SOURCE_DIR}/src/)