#include "Core.h"
#include "ClassLoader/ClassLoader.h"
#include "Printer/ClassPrinter.h"
#include "ClassDumpConfig.h"

int main(int argc, char* argv[])
{
    if (argc > 1) {
        if (strncmp(argv[1], "-v", 2) == 0) {
            printf("ClassDump version %d.%d\n", CLASSDUMP_VERSION_MAJOR, CLASSDUMP_VERSION_MINOR);
        } else {
            Memory memory(50000);
            const char* className = argv[1];

            ClassLoader classLoader;
            ClassPrinter classPrinter;
            ClassInfo* classInfo = classLoader.readClass(className, &memory);
            classPrinter.printClass(*classInfo, &memory);
#ifdef _DEBUG
            memory.printSize();
#endif
        }
    } else {
        fprintf(stderr, "Error: Class name must be supplied\n");
        exit(1);
    }

    return 0;
}