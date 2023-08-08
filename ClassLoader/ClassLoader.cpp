#include "ClassLoader.h"

#include "../Core.h"

inline uint8_t ClassLoader::readUnsignedByte(uint8_t* bytes) {
    uint8_t buffer = 0;
    buffer = bytes[bytePtr++];
    return buffer;
}

inline uint16_t ClassLoader::readUnsignedShort(uint8_t* bytes) {
    uint8_t buffer[2] = { bytes[bytePtr++], bytes[bytePtr++] };
    uint16_t value = (uint16_t)buffer[1] | (uint16_t)buffer[0] << 8;
    return value;
}

inline uint32_t ClassLoader::readUnsignedInt(uint8_t* bytes) {
    uint8_t buffer[4] = { bytes[bytePtr++], bytes[bytePtr++] , bytes[bytePtr++] , bytes[bytePtr++] };
    uint32_t value = (uint32_t)buffer[3] | (uint32_t)buffer[2] << 8 | (uint32_t)buffer[1] << 16 | (uint32_t)buffer[0] << 24;
    return (uint32_t)value;
}

static AttributeInfo* findAttributeWithName(std::vector<AttributeInfo*> attributes, ConstantPool& constantPool, std::string name)
{
    AttributeInfo* attrib = 0;

    for (AttributeInfo* attrib : attributes) {
        if (constantPool.getString(attrib->attributeNameIndex) == name) {
            return attrib;
        }
    }

    return attrib;
}

void ClassLoader::checkMagicNumber(uint8_t* bytes) {
    uint32_t magic = readUnsignedInt(bytes);
    if (magic == MAGIC_NUMBER) {
        std::cout << "Magic Number OK!" << std::endl;
    }
    else {
        std::cout << "Magic Number not OK.Exiting." << std::endl;
        exit(1);
    }
}

ConstantPool ClassLoader::readConstantPool(uint8_t* bytes)
{
    ConstantPool constantPool;

    uint16_t cpCount = readUnsignedShort(bytes);

    std::cout << "Loading constant pool of size: " << cpCount << std::endl;

    uint16_t arrCount = cpCount - 1;

    constantPool.constants = std::vector<ConstantPoolItem*>(arrCount);

    for (uint32_t currentConstantIndex = 0; currentConstantIndex < arrCount; currentConstantIndex++) {
        uint8_t tag = readUnsignedByte(bytes);

        ConstantPoolItem* constantPoolItem = readConstantPoolItem(tag, bytes);

        constantPool.constants[currentConstantIndex] = constantPoolItem;

        // if tag is long or double we need to increment by 2
    }

    return constantPool;
}

ConstantPoolItem* ClassLoader::readConstantPoolItem(uint8_t tag, uint8_t* bytes)
{
    ConstantPoolItem* item = nullptr;

    switch (tag) {
    case CT_METHODREF:
    {
        uint16_t classIndex = readUnsignedShort(bytes);
        uint16_t nameAndTypeIndex = readUnsignedShort(bytes);
        item = new CPMethodRef(tag, classIndex, nameAndTypeIndex);
        break;
    }
    case CT_CLASS:
    {
        uint16_t nameIndex = readUnsignedShort(bytes);
        item = new CPClassInfo(tag, nameIndex);
        break;
    }
    case CT_UTF8:
    {
        uint16_t size = readUnsignedShort(bytes);
        uint16_t strBytes = size * sizeof(uint8_t) + 1u;
        uint8_t* buffer = (uint8_t*)malloc(strBytes);
        memcpy(buffer, &bytes[bytePtr], sizeof(uint8_t) * size);
        bytePtr += size;
        buffer[strBytes - 1] = '\0';
        item = new CPUTF8Info(tag, strBytes, buffer);
        break;
    }
    case CT_NAMEANDTYPE:
    {
        uint16_t nameIndex = readUnsignedShort(bytes);
        uint16_t descriptorIndex = readUnsignedShort(bytes);
        item = new CPNameAndTypeInfo(tag, nameIndex, descriptorIndex);
        break;
    }
    case CT_STRING:
    {
        uint16_t stringIndex = readUnsignedShort(bytes);
        item = new CPStringInfo(tag, stringIndex);
        break;
    }
    case CT_FIELDREF:
    {
        // TODO: De-duplicate from methodref
        uint16_t classIndex = readUnsignedShort(bytes);
        uint16_t nameAndTypeIndex = readUnsignedShort(bytes);
        item = new CPFieldRef(tag, classIndex, nameAndTypeIndex);
        break;
    }
    case CT_INTERFACEMETHOD:
    {
        // TODO: De-duplicate from methodref
        uint16_t classIndex = readUnsignedShort(bytes);
        uint16_t nameAndTypeIndex = readUnsignedShort(bytes);
        item = new CPInterfaceRef (tag, classIndex, nameAndTypeIndex);
        break;
    }
    case CT_INTEGER:
    {
        // TODO: Parse the int as the correct type
        uint32_t intBytes = readUnsignedInt(bytes);
        item = new CPIntegerInfo(tag, intBytes);
        break;
    }
    case CT_FLOAT:
    {
        // TODO: Parse the int as the correct type
        uint32_t floatBytes = readUnsignedInt(bytes);
        item = new CPFloatInfo(tag, floatBytes);
        break;
    }
    case CT_LONG:
    {
        uint32_t highBytes = readUnsignedInt(bytes);
        uint32_t lowBytes = readUnsignedInt(bytes);
        item = new CPLongInfo(tag, highBytes, lowBytes);
        break;
    }
    case CT_DOUBLE:
    {
        uint32_t highBytes = readUnsignedInt(bytes);
        uint32_t lowBytes = readUnsignedInt(bytes);
        item = new CPDoubleInfo(tag, highBytes, lowBytes);
        break;
    }
    default:
    {
        std::cout << "Unidentified constant pool item detected with tag: "
            << tag << " " << std::endl;
    }
    }

    return item;
}

ExceptionTableEntry ClassLoader::readExceptionTableEntry(uint8_t* bytes) {
    uint16_t startPc = readUnsignedShort(bytes);
    uint16_t endPc = readUnsignedShort(bytes);
    uint16_t handlerPc = readUnsignedShort(bytes);
    uint16_t catchType = readUnsignedShort(bytes);

    ExceptionTableEntry entry;
    entry.startPc = startPc;
    entry.endPc = endPc;
    entry.handlerPc = handlerPc;
    entry.catchType = catchType;

    return entry;
}

std::vector<ExceptionTableEntry> ClassLoader::readExceptionTable(uint8_t* bytes) {
    std::vector<ExceptionTableEntry> table;
    uint16_t exceptionTableLength = readUnsignedShort(bytes);

    for (uint16_t currentException = 0; currentException < exceptionTableLength; currentException++) {
        ExceptionTableEntry entry = readExceptionTableEntry(bytes);
        table.push_back(entry);
    }

    return table;
}

std::vector<AttributeInfo*> ClassLoader::readAttributes(uint8_t* bytes, ConstantPool& constantPool)
{
    std::vector<AttributeInfo*> attributes;

    uint16_t attributesCount = readUnsignedShort(bytes);

    for (int currentAttrib = 0; currentAttrib < attributesCount; currentAttrib++) {
        uint16_t attributeNameIndex = readUnsignedShort(bytes);
        uint32_t attributeLength = readUnsignedInt(bytes);

        std::string name = constantPool.getString(attributeNameIndex);

        if (name == "Code") {
            uint16_t maxStack = readUnsignedShort(bytes);
            uint16_t maxLocals = readUnsignedShort(bytes);
            uint32_t codeLength = readUnsignedInt(bytes);
            uint8_t* code = (uint8_t*)malloc(sizeof(uint8_t) * codeLength);
            memcpy(code, &bytes[bytePtr], sizeof(uint8_t) * codeLength);
            bytePtr += codeLength;

            std::vector<ExceptionTableEntry> exceptions = readExceptionTable(bytes);
            std::vector<AttributeInfo*> attribs = readAttributes(bytes, constantPool);


            AttributeCode* att = new AttributeCode();
            att->attributeNameIndex = attributeNameIndex;
            att->attributeLength = attributeLength;
            att->maxStack = maxStack;
            att->maxLocals = maxLocals;
            att->codeLength = codeLength;
            att->code = code;
            att->exceptionTable = exceptions;
            att->attributes = attribs;

            attributes.push_back(att);
        }
        else if (name == "LineNumberTable") {
            uint16_t lineNumberTableLength = readUnsignedShort(bytes);
            for (int lineNumerTableIndex = 0; lineNumerTableIndex < lineNumberTableLength; lineNumerTableIndex++) {
                uint16_t startPc = readUnsignedShort(bytes);
                uint16_t lineNumber = readUnsignedShort(bytes);
            }
        } else if (name == "LocalVariableTable") {
            uint16_t localVariableTableLength = readUnsignedShort(bytes);
            for (int localVariableTableIndex = 0; localVariableTableIndex < localVariableTableLength; localVariableTableIndex++) {
                uint16_t startPc = readUnsignedShort(bytes);
                uint16_t length = readUnsignedShort(bytes);
                uint16_t nameIndex = readUnsignedShort(bytes);
                uint16_t descriptorIndex = readUnsignedShort(bytes);
                uint16_t index = readUnsignedShort(bytes);
            }
        } else {
            std::cout << "Attribute parsing not implemented yet" << std::endl;
            exit(1);
        }           
    }

    return attributes;
}

ClassLoader::ClassLoader()
    : bytePtr(0u)
{
}

ClassInfo ClassLoader::readClass(uint8_t* bytes)
{
    bytePtr = 0u;

    checkMagicNumber(bytes);

    ClassInfo classInfo;
    classInfo.minorVersion = readUnsignedShort(bytes);
    classInfo.majorVersion = readUnsignedShort(bytes);
    classInfo.constantPool = readConstantPool(bytes);
    classInfo.accessFlags = readUnsignedShort(bytes);
    classInfo.thisClass = readUnsignedShort(bytes);
    classInfo.superClass = readUnsignedShort(bytes);
    uint16_t interfacesCount = readUnsignedShort(bytes);
    classInfo.interfaces = readInterfaces(bytes, interfacesCount);
    classInfo.fields = readFields(bytes, classInfo.constantPool);
    classInfo.methods = readMethods(bytes, classInfo.constantPool);

    return classInfo;
}

ClassInfo ClassLoader::readClass(const std::string& className)
{

    struct stat results;

    if (stat(className.c_str(), &results) == 0) {
        uint8_t* bytes = new uint8_t[results.st_size];

        std::ifstream myFile(className.c_str(), std::ios::in | std::ios::binary);

        myFile.read((char*)bytes, results.st_size);
        ClassInfo classInfo = readClass(bytes);

        std::cout << "" << std::endl;
        myFile.close();
        delete[] bytes;

        return classInfo;
    }
    else {
        std::cout << "File not found: " << className << " " << std::endl;
    }
    return ClassInfo();
}

std::vector<uint16_t> ClassLoader::readInterfaces(uint8_t* bytes, uint16_t interfacesCount)
{
    std::vector<uint16_t> interfaces;

    for (uint16_t currentInterface = 0; currentInterface < interfacesCount; currentInterface++) {
        uint16_t interfaceIndex = readUnsignedShort(bytes);
        interfaces.push_back(interfaceIndex);
    }

    return interfaces;
}

std::vector<FieldInfo> ClassLoader::readFields(uint8_t* bytes, ConstantPool& constantPool)
{
    std::vector<FieldInfo> fields;

    uint16_t fieldsCount = readUnsignedShort(bytes);

    for (uint16_t currentField = 0; currentField < fieldsCount; currentField++) {
        uint16_t accessFlags = readUnsignedShort(bytes);
        uint16_t nameIndex = readUnsignedShort(bytes);
        uint16_t descriptorIndex = readUnsignedShort(bytes);
        std::vector<AttributeInfo*> attributeInfo = readAttributes(bytes, constantPool);
        FieldInfo fieldInfo;
        fieldInfo.accessFlags = accessFlags;
        fieldInfo.descriptorIndex = descriptorIndex;
        fieldInfo.nameIndex = nameIndex;
        // TODO: Parse attributes and add to FieldInfo
        fields.push_back(fieldInfo);
    }

    return fields;
}

inline static AttributeCode* getCodeAttribute(std::vector<AttributeInfo*> attributes, ConstantPool& constantPool) 
{
    return (AttributeCode*) findAttributeWithName(attributes, constantPool, "Code");
}

std::vector<MethodInfo> ClassLoader::readMethods(uint8_t* bytes, ConstantPool& constantPool)
{
    std::vector<MethodInfo> methods;

    uint16_t methodsCount = readUnsignedShort(bytes);

    while (methods.size() < methodsCount) {
        uint16_t accessFlags = readUnsignedShort(bytes);
        uint16_t nameIndex = readUnsignedShort(bytes);
        uint16_t descriptorIndex = readUnsignedShort(bytes);
        std::vector<AttributeInfo*> attributes = readAttributes(bytes, constantPool);
        MethodInfo info;
        info.accessFlags = accessFlags;
        info.nameIndex = nameIndex;
        info.descriptorIndex = descriptorIndex;
        // TODO: Fix these
        info.code = 0;
        info.isNative = ((accessFlags & ACC_NATIVE) == ACC_NATIVE);
        if (!info.isNative) {
            info.code = getCodeAttribute(attributes, constantPool);
        }

        methods.push_back(info);
    }

    return methods;
}
