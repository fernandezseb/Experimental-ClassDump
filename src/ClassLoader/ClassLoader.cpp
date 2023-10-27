#include "ClassLoader.h"

#include "Core.h"
#include "DescriptorParser.h"
#include "md5/md5.h"
#include "Util.h"

void ClassLoader::checkMagicNumber(ByteArray& byteArray) {
    uint32_t magic = byteArray.readUnsignedInt();
    if (magic != MAGIC_NUMBER) {
        std::cout << "Magic Number not OK.Exiting." << std::endl;
        exit(1);
    }
}

ConstantPool* ClassLoader::readConstantPool(ByteArray& byteArray)
{
    ConstantPool* constantPool = new ConstantPool();

    uint16_t cpCount = byteArray.readUnsignedShort();

    uint16_t arrCount = cpCount - 1;

    constantPool->constants = (ConstantPoolItem**) memory->classAlloc(sizeof(ConstantPoolItem*) * arrCount);
    constantPool->size = arrCount;
    // TODO: Only set the unused ones to 0
    memset(constantPool->constants, 0, sizeof(ConstantPoolItem*) * arrCount);

    for (uint32_t currentConstantIndex = 0; currentConstantIndex < arrCount; currentConstantIndex++) {
        uint8_t tag = byteArray.readUnsignedByte();

        ConstantPoolItem* constantPoolItem = readConstantPoolItem(tag, byteArray);

        constantPool->constants[currentConstantIndex] = constantPoolItem;

        // if tag is long or double we need to increment by 2
        if (tag == CT_LONG || tag == CT_DOUBLE) {
            currentConstantIndex++;
        }

    }

    return constantPool;
}

ConstantPoolItem* ClassLoader::readConstantPoolItem(uint8_t tag, ByteArray& byteArray)
{
    ConstantPoolItem* item = nullptr;

    switch (tag) {
    case CT_METHODREF:
    {
        uint16_t classIndex = byteArray.readUnsignedShort();
        uint16_t nameAndTypeIndex = byteArray.readUnsignedShort();
        CPMethodRef* methodRef = new CPMethodRef;
        //item = new CPMethodRef(tag, classIndex, nameAndTypeIndex);
        methodRef->tag = tag;
        methodRef->classIndex = classIndex;
        methodRef->nameAndTypeIndex = nameAndTypeIndex;
        item = methodRef;
        break;
    }
    case CT_CLASS:
    {
        uint16_t nameIndex = byteArray.readUnsignedShort();
        
        CPClassInfo* classInfo = new CPClassInfo{};
        classInfo->tag = tag;
        classInfo->nameIndex = nameIndex;
        
        item = classInfo;
        break;
    }
    case CT_UTF8:
    {
        uint16_t size = byteArray.readUnsignedShort();
        uint16_t strBytes = size * sizeof(uint8_t) + 1u;
        uint8_t* buffer = (uint8_t*)memory->classAlloc(strBytes);
        /*memcpy(buffer, &bytes[bytePtr], sizeof(uint8_t) * size);
        bytePtr += size;*/
        byteArray.copyBytes(buffer, size);
        buffer[strBytes - 1] = '\0';
        //item = new CPUTF8Info(tag, strBytes, buffer);
        //CPUTF8Info* itemUtf8 = (CPUTF8Info*) memory->classAlloc(sizeof(CPUTF8Info));
        CPUTF8Info* itemUtf8 = new CPUTF8Info;
        itemUtf8->tag = tag;
        itemUtf8->length = strBytes;
        itemUtf8->bytes = buffer;
        item = itemUtf8;
        break;
    }
    case CT_NAMEANDTYPE:
    {
        uint16_t nameIndex = byteArray.readUnsignedShort();
        uint16_t descriptorIndex = byteArray.readUnsignedShort();
        CPNameAndTypeInfo* nameAndtype = new CPNameAndTypeInfo;
        //item = new CPNameAndTypeInfo(tag, nameIndex, descriptorIndex);
        nameAndtype->tag = tag;
        nameAndtype->nameIndex = nameIndex;
        nameAndtype->descriptorIndex = descriptorIndex;
        item = nameAndtype;
        break;
    }
    case CT_STRING:
    {
        uint16_t stringIndex = byteArray.readUnsignedShort();
        CPStringInfo* stringInfo = new CPStringInfo;
        stringInfo->tag = tag;
        stringInfo->stringIndex = stringIndex;
        item = stringInfo;
        break;
    }
    case CT_FIELDREF:
    {
        // TODO: De-duplicate from methodref
        uint16_t classIndex = byteArray.readUnsignedShort();
        uint16_t nameAndTypeIndex = byteArray.readUnsignedShort();
        CPFieldRef* fieldRef = new CPFieldRef;
        fieldRef->tag = tag;
        fieldRef->classIndex = classIndex;
        fieldRef->nameAndTypeIndex = nameAndTypeIndex;
        item = fieldRef;
        break;
    }
    case CT_INTERFACEMETHOD:
    {
        // TODO: De-duplicate from methodref
        uint16_t classIndex = byteArray.readUnsignedShort();
        uint16_t nameAndTypeIndex = byteArray.readUnsignedShort();
        CPInterfaceRef* interfaceRef = new CPInterfaceRef;
        interfaceRef->tag = tag;
        interfaceRef->classIndex = classIndex;
        interfaceRef->nameAndTypeIndex = nameAndTypeIndex;
        item = interfaceRef;
        break;
    }
    case CT_INTEGER:
    {
        // TODO: Parse the int as the correct type
        uint32_t intBytes = byteArray.readUnsignedInt();
        CPIntegerInfo* integerInfo = new CPIntegerInfo;
        integerInfo->tag = tag;
        integerInfo->bytes = intBytes;
        item = integerInfo;
        break;
    }
    case CT_FLOAT:
    {
        // TODO: Parse the int as the correct type
        uint32_t floatBytes = byteArray.readUnsignedInt();
        CPFloatInfo* floatInfo = new CPFloatInfo;
        floatInfo->tag = tag;
        floatInfo->bytes = floatBytes;
        item = floatInfo;
        break;
    }
    case CT_LONG:
    {
        uint32_t highBytes = byteArray.readUnsignedInt();
        uint32_t lowBytes = byteArray.readUnsignedInt();
        CPLongInfo* longInfo = new CPLongInfo;
        longInfo->tag = tag;
        longInfo->highBytes = highBytes;
        longInfo->lowBytes = lowBytes;
        item = longInfo;
        break;
    }
    case CT_DOUBLE:
    {
        uint32_t highBytes = byteArray.readUnsignedInt();
        uint32_t lowBytes = byteArray.readUnsignedInt();
        CPDoubleInfo* doubleInfo = new CPDoubleInfo;
        doubleInfo->tag = tag;
        doubleInfo->highBytes = highBytes;
        doubleInfo->lowBytes = lowBytes;
        item = doubleInfo;
        break;
    }
    default:
    {
        std::cout << "Unidentified constant pool item detected with tag: "
            << (int) tag << " " << std::endl;
    }
    }

    return item;
}

ClassLoader::ClassLoader()
{
}

ClassInfo* ClassLoader::readClass(ByteArray& byteArray)
{
    checkMagicNumber(byteArray);

    //void* classMemory = memory->classAlloc(sizeof(ClassInfo));

    ClassInfo* classInfo = new ClassInfo();

    classInfo->minorVersion = byteArray.readUnsignedShort();
    classInfo->majorVersion = byteArray.readUnsignedShort();
    classInfo->constantPool = readConstantPool(byteArray);
    classInfo->accessFlags = byteArray.readUnsignedShort();
    classInfo->thisClass = byteArray.readUnsignedShort();
    classInfo->superClass = byteArray.readUnsignedShort();
    
    uint16_t interfacesCount = byteArray.readUnsignedShort();
    classInfo->interfaces = readInterfaces(byteArray, interfacesCount);
    classInfo->interfacesCount = interfacesCount;

    uint16_t fieldsCount = byteArray.readUnsignedShort();
    classInfo->fields = readFields(byteArray, classInfo->constantPool, fieldsCount);
    classInfo->fieldsCount = fieldsCount;

    uint16_t methodsCount = byteArray.readUnsignedShort();
    classInfo->methods = readMethods(byteArray, classInfo->constantPool, methodsCount);
    classInfo->methodCount = methodsCount;

    classInfo->isPublic = ((classInfo->accessFlags & ACC_PUBLIC) == ACC_PUBLIC);

    AttributeCollection* attributeInfo = AttributeParser::readAttributes(byteArray, classInfo->constantPool, memory);
    classInfo->attributes = attributeInfo;
    AttributeSourceFile* sourceFile = (AttributeSourceFile*) attributeInfo->findAttributeWithName(classInfo->constantPool, "SourceFile");

    if (sourceFile != NULL) {
        std::string sourceFileString = classInfo->constantPool->getString(sourceFile->sourceFileIndex);
        char* sourceFileCharArr = (char*)memory->classAlloc(sourceFileString.length() + 1);
        strcpy(sourceFileCharArr, sourceFileString.c_str());
        classInfo->sourceFile = sourceFileCharArr;;
    }

    return classInfo;
}

ClassInfo* ClassLoader::readClass(const char* className)
{
    Memory *mem = new Memory(10000);
    memory = mem;
    try {
        std::filesystem::path path = std::filesystem::canonical(className);
        const wchar_t* absolutePath = path.c_str();

        wchar_t* pathstr = (wchar_t*)mem->classAlloc(((wcslen(absolutePath)+1) * sizeof(wchar_t)));
        wcscpy(pathstr, absolutePath);

        std::filesystem::file_time_type lastWritten =  std::filesystem::last_write_time(path);

        struct stat attr;
        stat(className, &attr);

        std::ifstream myFile(path, std::ios::in | std::ios::binary);
        uint64_t size = std::filesystem::file_size(path);

        ByteArray bytes(size);

        myFile.read((char*)bytes.bytes, size);

        std::string checksum =  md5(bytes.bytes, size);

        ClassInfo* classInfo = readClass(bytes);
        classInfo->memory = mem;
        classInfo->filePath = pathstr;
        classInfo->size = size;
        classInfo->lastModified = attr.st_mtime;
        strcpy(classInfo->md5, checksum.c_str());
        myFile.close();

        return classInfo;
    }
    catch (const std::exception& ex) {
        std::cout << "Canonical path for " << className << " threw exception:\n"
            << ex.what() << std::endl;
        exit(-1);
    }
    return new ClassInfo();
}

uint16_t* ClassLoader::readInterfaces(ByteArray& byteArray, uint16_t interfacesCount)
{
    uint16_t* interfaces = (uint16_t*) memory->classAlloc(sizeof(uint16_t) * interfacesCount);

    for (uint16_t currentInterface = 0; currentInterface < interfacesCount; currentInterface++) {
        uint16_t interfaceIndex = byteArray.readUnsignedShort();
        interfaces[currentInterface] = interfaceIndex;
    }

    return interfaces;
}

FieldInfo** ClassLoader::readFields(ByteArray& byteArray, ConstantPool* constantPool, uint16_t fieldsCount)
{
    FieldInfo** fields = (FieldInfo**)memory->classAlloc(sizeof(FieldInfo*) * fieldsCount);

    for (uint16_t currentField = 0; currentField < fieldsCount; ++currentField) {
        uint16_t accessFlags = byteArray.readUnsignedShort();
        uint16_t nameIndex = byteArray.readUnsignedShort();
        uint16_t descriptorIndex = byteArray.readUnsignedShort();
        AttributeCollection* attributeInfo = AttributeParser::readAttributes(byteArray, constantPool, memory);
        FieldInfo* fieldInfo = (FieldInfo*) memory->classAlloc(sizeof(FieldInfo));
        fieldInfo->accessFlags = accessFlags;
        fieldInfo->descriptorIndex = descriptorIndex;
        fieldInfo->nameIndex = nameIndex;
        fieldInfo->isPrivate = ((accessFlags & ACC_PRIVATE) == ACC_PRIVATE);
        fieldInfo->attributes = attributeInfo;
        fields[currentField] = fieldInfo;
    }

    return fields;
}

void ClassLoader::parseDescriptor(const char* descriptor, MethodInfo* method)
{
    Descriptor desc = DescriptorParser::parseDescriptor(descriptor);
    method->returnType = toCharPtr(desc.returnType, memory);

    std::vector<std::string> args = desc.args;
    char** argsArr = (char**)memory->classAlloc(args.size() * sizeof(char*));
    for (int currentArg = 0; currentArg < args.size(); ++currentArg) {
        argsArr[currentArg] = toCharPtr(args[currentArg], memory);
    }
    method->args = argsArr;
    method->argsCount = args.size();
}

MethodInfo** ClassLoader::readMethods(ByteArray& byteArray, ConstantPool* constantPool, uint16_t methodCount)
{
    MethodInfo** methods = (MethodInfo**)memory->classAlloc(sizeof(MethodInfo*) * methodCount);

    for (uint16_t currentMethod = 0; currentMethod < methodCount; ++currentMethod) {
        uint16_t accessFlags = byteArray.readUnsignedShort();
        uint16_t nameIndex = byteArray.readUnsignedShort();
        uint16_t descriptorIndex = byteArray.readUnsignedShort();
        AttributeCollection* attributes = AttributeParser::readAttributes(byteArray, constantPool, memory);
        MethodInfo* info = (MethodInfo*) memory->classAlloc(sizeof(MethodInfo));
        info->accessFlags = accessFlags;
        info->nameIndex = nameIndex;
        info->descriptorIndex = descriptorIndex;
        // TODO: Fix these
        info->code = 0;
        info->attributes = attributes;
        info->isNative = ((accessFlags & ACC_NATIVE) == ACC_NATIVE);
        info->isAbstract = ((accessFlags & ACC_ABSTRACT) == ACC_ABSTRACT);
        if (!(info->isNative || info->isAbstract)) {
            info->code = (AttributeCode*) attributes->findAttributeWithName(constantPool, "Code");
        }
        info->isPublic = ((accessFlags & ACC_PUBLIC) == ACC_PUBLIC);
        info->isStatic = ((accessFlags & ACC_STATIC) == ACC_STATIC);

        parseDescriptor(constantPool->getString(descriptorIndex), info);

        const char* name = constantPool->getString(nameIndex);
        info->isConstructor = (strcmp(name,"<init>") == 0);

        methods[currentMethod] = info;
    }

    return methods;
}

ClassInfo::ClassInfo() :
    constantPool(nullptr),
    attributes(nullptr)
{
}

ClassInfo::~ClassInfo()
{
    delete memory;
}

std::vector<AccessFlag> ClassInfo::getAccessFlags() const
{
    static const AccessFlag classFlags[8] = {
        ACC_PUBLIC,
        ACC_FINAL,
        ACC_SUPER,
        ACC_INTERFACE,
        ACC_ABSTRACT,
        ACC_SYNTHETIC,
        ACC_ANNOTATION,
        ACC_ENUM
    };

    std::vector<AccessFlag> flags;
    for (AccessFlag flag : classFlags) {
        if ((accessFlags & flag) == flag) {
            flags.push_back(flag);
        }
    }

    return flags;
}

MethodInfo::MethodInfo() :
    attributes(nullptr),
    code(0)
{

}

MethodInfo::~MethodInfo() {
    if (attributes != 0) {
        delete attributes;
        attributes = nullptr;
    }

    if (args != 0) {
        for (int i = 0; i < argsCount; ++i) {
            free(args[i]);
        }
        free(args);
    }

    if (returnType != 0) {
        free(returnType);
    }
}

std::vector<AccessFlag> MethodInfo::getAccessFlags() const
{
    static const AccessFlag methodFlags[12] = {
        ACC_PUBLIC,
        ACC_PRIVATE,
        ACC_PROTECTED,
        ACC_STATIC,
        ACC_FINAL,
        ACC_SYNCHRONIZED,
        ACC_BRIDGE,
        ACC_VARARGS,
        ACC_NATIVE,
        ACC_ABSTRACT,
        ACC_STRICT,
        ACC_SYNTHETIC
    };
    std::vector<AccessFlag> flags;
    for (AccessFlag flag : methodFlags) {
        if ((accessFlags & flag) == flag) {
            flags.push_back(flag);
        }
    }

    return flags;
}

std::vector<AccessFlag> FieldInfo::getAccessFlags() const
{
    static const AccessFlag fieldFlags[9] = {
        ACC_PUBLIC,
        ACC_PRIVATE,
        ACC_PROTECTED,
        ACC_STATIC,
        ACC_FINAL,
        ACC_VOLATILE,
        ACC_TRANSIENT,
        ACC_SYNTHETIC,
        ACC_ENUM
    };

    std::vector<AccessFlag> flags;
    for (AccessFlag flag : fieldFlags) {
        if ((accessFlags & flag) == flag) {
            flags.push_back(flag);
        }
    }

    return flags;
}
