#include "ClassLoader.h"

#include "Core.h"
#include "DescriptorParser.h"
#include "md5/md5.h"

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

    constantPool->constants = std::vector<ConstantPoolItem*>(arrCount);

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
        item = new CPMethodRef(tag, classIndex, nameAndTypeIndex);
        break;
    }
    case CT_CLASS:
    {
        uint16_t nameIndex = byteArray.readUnsignedShort();
        item = new CPClassInfo(tag, nameIndex);
        break;
    }
    case CT_UTF8:
    {
        uint16_t size = byteArray.readUnsignedShort();
        uint16_t strBytes = size * sizeof(uint8_t) + 1u;
        uint8_t* buffer = (uint8_t*)malloc(strBytes);
        /*memcpy(buffer, &bytes[bytePtr], sizeof(uint8_t) * size);
        bytePtr += size;*/
        byteArray.copyBytes(buffer, size);
        buffer[strBytes - 1] = '\0';
        item = new CPUTF8Info(tag, strBytes, buffer);
        break;
    }
    case CT_NAMEANDTYPE:
    {
        uint16_t nameIndex = byteArray.readUnsignedShort();
        uint16_t descriptorIndex = byteArray.readUnsignedShort();
        item = new CPNameAndTypeInfo(tag, nameIndex, descriptorIndex);
        break;
    }
    case CT_STRING:
    {
        uint16_t stringIndex = byteArray.readUnsignedShort();
        item = new CPStringInfo(tag, stringIndex);
        break;
    }
    case CT_FIELDREF:
    {
        // TODO: De-duplicate from methodref
        uint16_t classIndex = byteArray.readUnsignedShort();
        uint16_t nameAndTypeIndex = byteArray.readUnsignedShort();
        item = new CPFieldRef(tag, classIndex, nameAndTypeIndex);
        break;
    }
    case CT_INTERFACEMETHOD:
    {
        // TODO: De-duplicate from methodref
        uint16_t classIndex = byteArray.readUnsignedShort();
        uint16_t nameAndTypeIndex = byteArray.readUnsignedShort();
        item = new CPInterfaceRef (tag, classIndex, nameAndTypeIndex);
        break;
    }
    case CT_INTEGER:
    {
        // TODO: Parse the int as the correct type
        uint32_t intBytes = byteArray.readUnsignedInt();
        item = new CPIntegerInfo(tag, intBytes);
        break;
    }
    case CT_FLOAT:
    {
        // TODO: Parse the int as the correct type
        uint32_t floatBytes = byteArray.readUnsignedInt();
        item = new CPFloatInfo(tag, floatBytes);
        break;
    }
    case CT_LONG:
    {
        uint32_t highBytes = byteArray.readUnsignedInt();
        uint32_t lowBytes = byteArray.readUnsignedInt();
        item = new CPLongInfo(tag, highBytes, lowBytes);
        break;
    }
    case CT_DOUBLE:
    {
        uint32_t highBytes = byteArray.readUnsignedInt();
        uint32_t lowBytes = byteArray.readUnsignedInt();
        item = new CPDoubleInfo(tag, highBytes, lowBytes);
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

    classInfo->fields = readFields(byteArray, classInfo->constantPool);
    classInfo->methods = readMethods(byteArray, classInfo->constantPool);
    classInfo->isPublic = ((classInfo->accessFlags & ACC_PUBLIC) == ACC_PUBLIC);

    AttributeCollection* attributeInfo = AttributeParser::readAttributes(byteArray, classInfo->constantPool);
    classInfo->attributes = attributeInfo;
    AttributeSourceFile* sourceFile = (AttributeSourceFile*) attributeInfo->findAttributeWithName(classInfo->constantPool, "SourceFile");

    if (sourceFile != NULL) {
        classInfo->sourceFile = classInfo->constantPool->getString(sourceFile->sourceFileIndex);
    }

    return classInfo;
}

ClassInfo* ClassLoader::readClass(const std::string& className)
{

    try {
        std::filesystem::path path = std::filesystem::canonical(className);
        std::wstring absolutePath = path.c_str();

        std::filesystem::file_time_type lastWritten =  std::filesystem::last_write_time(path);

        struct stat attr;
        stat(className.c_str(), &attr);

        std::ifstream myFile(path, std::ios::in | std::ios::binary);
        uint64_t size = std::filesystem::file_size(path);

        ByteArray bytes(size);

        myFile.read((char*)bytes.bytes, size);

        std::string checksum =  md5(bytes.bytes, size);

        ClassInfo* classInfo = readClass(bytes);
        classInfo->filePath = absolutePath;
        classInfo->size = size;
        classInfo->lastModified = attr.st_mtime;
        classInfo->md5 = checksum;
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
    uint16_t* interfaces = (uint16_t*) malloc(sizeof(uint16_t) * interfacesCount);

    for (uint16_t currentInterface = 0; currentInterface < interfacesCount; currentInterface++) {
        uint16_t interfaceIndex = byteArray.readUnsignedShort();
        interfaces[currentInterface] = interfaceIndex;
    }

    return interfaces;
}

std::vector<FieldInfo*> ClassLoader::readFields(ByteArray& byteArray, ConstantPool* constantPool)
{
    std::vector<FieldInfo*> fields;

    uint16_t fieldsCount = byteArray.readUnsignedShort();

    for (uint16_t currentField = 0; currentField < fieldsCount; currentField++) {
        uint16_t accessFlags = byteArray.readUnsignedShort();
        uint16_t nameIndex = byteArray.readUnsignedShort();
        uint16_t descriptorIndex = byteArray.readUnsignedShort();
        AttributeCollection* attributeInfo = AttributeParser::readAttributes(byteArray, constantPool);
        FieldInfo* fieldInfo = new FieldInfo;
        fieldInfo->accessFlags = accessFlags;
        fieldInfo->descriptorIndex = descriptorIndex;
        fieldInfo->nameIndex = nameIndex;
        fieldInfo->isPrivate = ((accessFlags & ACC_PRIVATE) == ACC_PRIVATE);
        fieldInfo->attributes = attributeInfo;
        fields.push_back(fieldInfo);
    }

    return fields;
}

void ClassLoader::parseDescriptor(const std::string& descriptor, MethodInfo* method)
{
    Descriptor desc = DescriptorParser::parseDescriptor(descriptor);
    method->returnType = desc.returnType;
    method->args = desc.args;
}

std::vector<MethodInfo*> ClassLoader::readMethods(ByteArray& byteArray, ConstantPool* constantPool)
{
    std::vector<MethodInfo*> methods;

    uint16_t methodsCount = byteArray.readUnsignedShort();

    while (methods.size() < methodsCount) {
        uint16_t accessFlags = byteArray.readUnsignedShort();
        uint16_t nameIndex = byteArray.readUnsignedShort();
        uint16_t descriptorIndex = byteArray.readUnsignedShort();
        AttributeCollection* attributes = AttributeParser::readAttributes(byteArray, constantPool);
        MethodInfo* info = new MethodInfo;
        info->accessFlags = accessFlags;
        info->nameIndex = nameIndex;
        info->descriptorIndex = descriptorIndex;
        // TODO: Fix these
        info->code = 0;
        info->isNative = ((accessFlags & ACC_NATIVE) == ACC_NATIVE);
        info->attributes = attributes;
        if (!info->isNative) {
            info->code = (AttributeCode*) attributes->findAttributeWithName(constantPool, "Code");
        }
        info->isPublic = ((accessFlags & ACC_PUBLIC) == ACC_PUBLIC);
        info->isStatic = ((accessFlags & ACC_STATIC) == ACC_STATIC);

        parseDescriptor(constantPool->getString(descriptorIndex), info);

        std::string name = constantPool->getString(nameIndex);
        info->isConstructor = (name == "<init>");

        methods.push_back(info);
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
    if (constantPool != 0) {
        delete constantPool;
    }
    if (attributes != 0) {
        delete attributes;
    }

    if (interfacesCount > 0) {
        if (interfaces != 0) {
            free(interfaces);
        }
    }

    for (int i = 0; i < this->fields.size(); i++) {
        FieldInfo* item = this->fields[i];
        if (item != 0) {
            delete item;
            item = nullptr;
        }
    }

    for (int i = 0; i < this->methods.size(); i++) {
        MethodInfo* item = this->methods[i];
        if (item != 0) {
            delete item;
            item = nullptr;
        }
    }
}

std::vector<AccessFlag> ClassInfo::getAccessFlags() const
{
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
}

std::vector<AccessFlag> MethodInfo::getAccessFlags() const
{
    std::vector<AccessFlag> flags;
    for (AccessFlag flag : methodFlags) {
        if ((accessFlags & flag) == flag) {
            flags.push_back(flag);
        }
    }

    return flags;
}

FieldInfo::FieldInfo() : attributes(nullptr) {}

FieldInfo::~FieldInfo() {
    if (attributes != 0) {
        delete attributes;
        attributes = nullptr;
    }
}

std::vector<AccessFlag> FieldInfo::getAccessFlags() const
{
    std::vector<AccessFlag> flags;
    for (AccessFlag flag : fieldFlags) {
        if ((accessFlags & flag) == flag) {
            flags.push_back(flag);
        }
    }

    return flags;
}
