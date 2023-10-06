

add_subdirectory(descriptorParser)

add_test(NAME DescriptorTests COMMAND descriptor_test)
add_test(NAME e2e COMMAND ClassDump "Main.class" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})