#include "ClassLoader.h"

#include "Core.h"
#include "DescriptorParser.h"
#include "md5/md5.h"

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

void ClassLoader::checkMagicNumber(ByteArray& byteArray) {
    uint32_t magic = byteArray.readUnsignedInt();
    if (magic != MAGIC_NUMBER) {
        std::cout << "Magic Number not OK.Exiting." << std::endl;
        exit(1);
    }
}

ConstantPool ClassLoader::readConstantPool(ByteArray& byteArray)
{
    ConstantPool constantPool;

    uint16_t cpCount = byteArray.readUnsignedShort();

    uint16_t arrCount = cpCount - 1;

    constantPool.constants = std::vector<ConstantPoolItem*>(arrCount);

    for (uint32_t currentConstantIndex = 0; currentConstantIndex < arrCount; currentConstantIndex++) {
        uint8_t tag = byteArray.readUnsignedByte();

        ConstantPoolItem* constantPoolItem = readConstantPoolItem(tag, byteArray);

        constantPool.constants[currentConstantIndex] = constantPoolItem;

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

ExceptionTableEntry ClassLoader::readExceptionTableEntry(ByteArray& byteArray) {
    uint16_t startPc = byteArray.readUnsignedShort();
    uint16_t endPc = byteArray.readUnsignedShort();
    uint16_t handlerPc = byteArray.readUnsignedShort();
    uint16_t catchType = byteArray.readUnsignedShort();

    ExceptionTableEntry entry;
    entry.startPc = startPc;
    entry.endPc = endPc;
    entry.handlerPc = handlerPc;
    entry.catchType = catchType;

    return entry;
}

std::vector<ExceptionTableEntry> ClassLoader::readExceptionTable(ByteArray& byteArray) {
    std::vector<ExceptionTableEntry> table;
    uint16_t exceptionTableLength = byteArray.readUnsignedShort();

    for (uint16_t currentException = 0; currentException < exceptionTableLength; currentException++) {
        ExceptionTableEntry entry = readExceptionTableEntry(byteArray);
        table.push_back(entry);
    }

    return table;
}

void ClassLoader::readStackMapTable(ByteArray& byteArray) {
    uint16_t numberOfEntries = byteArray.readUnsignedShort();
    //printf("[%" PRIu16 "] \n", numberOfEntries);
    for (uint16_t currentEntry = 0; currentEntry < numberOfEntries; currentEntry++) {
        uint8_t frameType = byteArray.readUnsignedByte();
        if (frameType >= 0 && frameType <= 63) {
            // same frame
        }
        else if (frameType >= 64 && frameType <= 127) {
            // same locals 1 stack item frame
            uint8_t tag = byteArray.readUnsignedByte();
            if (tag == 7 || tag == 8) {
                uint16_t crap = byteArray.readUnsignedShort();
            }

        }
        else if (frameType == 247) {
            // same locals 1 stack item frame extended
        }
        else if (frameType >= 248 && frameType <= 250) {
            // chop frame
            uint16_t offsetDelta = byteArray.readUnsignedShort();
        }
        else if (frameType == 251) {
            uint16_t offsetDelta = byteArray.readUnsignedShort();
            // same frame extended
        }
        else if (frameType >= 252 && frameType <= 254) {
            // append frame
            uint16_t offsetDelta = byteArray.readUnsignedShort();
            for (uint16_t currentLocal = 0; currentLocal < (frameType - 251); currentLocal++) {
                uint8_t tag = byteArray.readUnsignedByte();
                if (tag == 7 || tag == 8) {
                    uint16_t crap = byteArray.readUnsignedShort();
                }
            }
        }
        else if (frameType == 255) {
            // full frame
            uint16_t offsetDelta = byteArray.readUnsignedShort();
            uint16_t numberOfLocals = byteArray.readUnsignedShort();
            for (uint16_t currentLocal = 0; currentLocal < numberOfLocals; currentLocal++) {
                uint8_t tag = byteArray.readUnsignedByte();
                if (tag == 7 || tag == 8) {
                    uint16_t crap = byteArray.readUnsignedShort();
                }
            }
            uint16_t numberOfStackItems = byteArray.readUnsignedShort();
            for (uint16_t currentLocal = 0; currentLocal < numberOfStackItems; currentLocal++) {
                uint8_t tag = byteArray.readUnsignedByte();
                if (tag == 7 || tag == 8) {
                    uint16_t crap = byteArray.readUnsignedShort();
                }
            }
        }
        else {
            std::cout << "Unknown frame type detected:" << (unsigned int) frameType;
            exit(1);
        }
    }
}

std::vector<AttributeInfo*> ClassLoader::readAttributes(ByteArray& byteArray, ConstantPool& constantPool)
{
    std::vector<AttributeInfo*> attributes;

    uint16_t attributesCount = byteArray.readUnsignedShort();

    for (int currentAttrib = 0; currentAttrib < attributesCount; currentAttrib++) {
        uint16_t attributeNameIndex = byteArray.readUnsignedShort();
        uint32_t attributeLength = byteArray.readUnsignedInt();

        std::string name = constantPool.getString(attributeNameIndex);

        if (name == "Code") {
            uint16_t maxStack = byteArray.readUnsignedShort();
            uint16_t maxLocals = byteArray.readUnsignedShort();
            uint32_t codeLength = byteArray.readUnsignedInt();
            uint8_t* code = (uint8_t*)malloc(sizeof(uint8_t) * codeLength);
            /*memcpy(code, &bytes[bytePtr], sizeof(uint8_t) * codeLength);
            bytePtr += codeLength;*/
            byteArray.copyBytes(code, codeLength);

            std::vector<ExceptionTableEntry> exceptions = readExceptionTable(byteArray);
            std::vector<AttributeInfo*> attribs = readAttributes(byteArray, constantPool);


            AttributeCode* att = new AttributeCode();
            att->attributeNameIndex = attributeNameIndex;
            att->attributeLength = attributeLength;
            att->maxStack = maxStack;
            att->maxLocals = maxLocals;
            att->codeLength = codeLength;
            att->code = code;
            att->exceptionTable = exceptions;
            att->attributes = attribs;
            AttributeLineNumberTable *lineNumberTable = (AttributeLineNumberTable*) findAttributeWithName(attribs, constantPool, "LineNumberTable");
            att->lineNumberTable = lineNumberTable;
            att->localVariableTable = (AttributeLocalVariableTable*)findAttributeWithName(attribs, constantPool, "LocalVariableTable");

            attributes.push_back(att);
        }
        else if (name == "LineNumberTable") {
            uint16_t lineNumberTableLength = byteArray.readUnsignedShort();
            AttributeLineNumberTable* att = new AttributeLineNumberTable();
            att->attributeNameIndex = attributeNameIndex;
            att->attributeLength = attributeLength;
            for (int lineNumerTableIndex = 0; lineNumerTableIndex < lineNumberTableLength; lineNumerTableIndex++) {
                uint16_t startPc = byteArray.readUnsignedShort();
                uint16_t lineNumber = byteArray.readUnsignedShort();
                LineNumberTableEntry *entry = new LineNumberTableEntry();
                entry->startPc = startPc;
                entry->lineNumber = lineNumber;
                att->entries.push_back(entry);
            }
            attributes.push_back(att);
        }
        else if (name == "LocalVariableTable") {
            uint16_t localVariableTableLength = byteArray.readUnsignedShort();
            AttributeLocalVariableTable* att = new AttributeLocalVariableTable();
            att->attributeNameIndex = attributeNameIndex;
            att->attributeLength = attributeLength;
            for (int localVariableTableIndex = 0; localVariableTableIndex < localVariableTableLength; localVariableTableIndex++) {
                uint16_t startPc = byteArray.readUnsignedShort();
                uint16_t length = byteArray.readUnsignedShort();
                uint16_t nameIndex = byteArray.readUnsignedShort();
                uint16_t descriptorIndex = byteArray.readUnsignedShort();
                uint16_t index = byteArray.readUnsignedShort();
                LocalVariableTableEntry* entry = new LocalVariableTableEntry();
                entry->startPc = startPc;
                entry->length = length;
                entry->nameIndex = nameIndex;
                entry->descriptorIndex = descriptorIndex;
                entry->index = index;
                att->entries.push_back(entry);
            }
            attributes.push_back(att);
        }
        else if (name == "SourceFile") {
            uint16_t sourceFileIndex = byteArray.readUnsignedShort();
            AttributeSourceFile* att = new AttributeSourceFile();
            att->attributeNameIndex = attributeNameIndex;
            att->attributeLength = attributeLength;
            att->sourceFileIndex = sourceFileIndex;
            
            attributes.push_back(att);
        }
        else if (name == "StackMapTable") {
            readStackMapTable(byteArray);
        }
        else {
            std::cout << "Attribute parsing not implemented yet for type: " << name << std::endl;
            exit(1);
        }           
    }

    return attributes;
}

ClassLoader::ClassLoader()
{
}

inline static AttributeSourceFile* getSourceFileAttribute(std::vector<AttributeInfo*> attributes, ConstantPool& constantPool)
{
    return (AttributeSourceFile*)findAttributeWithName(attributes, constantPool, "SourceFile");
}

ClassInfo ClassLoader::readClass(ByteArray& byteArray)
{
    checkMagicNumber(byteArray);

    ClassInfo classInfo;
    classInfo.minorVersion = byteArray.readUnsignedShort();
    classInfo.majorVersion = byteArray.readUnsignedShort();
    classInfo.constantPool = readConstantPool(byteArray);
    classInfo.accessFlags = byteArray.readUnsignedShort();
    classInfo.thisClass = byteArray.readUnsignedShort();
    classInfo.superClass = byteArray.readUnsignedShort();
    uint16_t interfacesCount = byteArray.readUnsignedShort();
    classInfo.interfaces = readInterfaces(byteArray, interfacesCount);
    classInfo.fields = readFields(byteArray, classInfo.constantPool);
    classInfo.methods = readMethods(byteArray, classInfo.constantPool);
    classInfo.isPublic = ((classInfo.accessFlags & ACC_PUBLIC) == ACC_PUBLIC);

    std::vector<AttributeInfo*> attributeInfo = readAttributes(byteArray, classInfo.constantPool);
    classInfo.attributes = attributeInfo;
    // TODO: Don't crash when this is not defined
    AttributeSourceFile* sourceFile = getSourceFileAttribute(attributeInfo, classInfo.constantPool);

    if (sourceFile != NULL) {
        classInfo.sourceFile = classInfo.constantPool.getString(sourceFile->sourceFileIndex);
    }

    return classInfo;
}

ClassInfo ClassLoader::readClass(const std::string& className)
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

        ClassInfo classInfo = readClass(bytes);
        classInfo.filePath = absolutePath;
        classInfo.size = size;
        classInfo.lastModified = attr.st_mtime;
        classInfo.md5 = checksum;
        myFile.close();

        return classInfo;
    }
    catch (const std::exception& ex) {
        std::cout << "Canonical path for " << className << " threw exception:\n"
            << ex.what() << '\n';
        exit(-1);
    }
    return ClassInfo();
}

std::vector<uint16_t> ClassLoader::readInterfaces(ByteArray& byteArray, uint16_t interfacesCount)
{
    std::vector<uint16_t> interfaces;

    for (uint16_t currentInterface = 0; currentInterface < interfacesCount; currentInterface++) {
        uint16_t interfaceIndex = byteArray.readUnsignedShort();
        interfaces.push_back(interfaceIndex);
    }

    return interfaces;
}

std::vector<FieldInfo> ClassLoader::readFields(ByteArray& byteArray, ConstantPool& constantPool)
{
    std::vector<FieldInfo> fields;

    uint16_t fieldsCount = byteArray.readUnsignedShort();

    for (uint16_t currentField = 0; currentField < fieldsCount; currentField++) {
        uint16_t accessFlags = byteArray.readUnsignedShort();
        uint16_t nameIndex = byteArray.readUnsignedShort();
        uint16_t descriptorIndex = byteArray.readUnsignedShort();
        std::vector<AttributeInfo*> attributeInfo = readAttributes(byteArray, constantPool);
        FieldInfo fieldInfo;
        fieldInfo.accessFlags = accessFlags;
        fieldInfo.descriptorIndex = descriptorIndex;
        fieldInfo.nameIndex = nameIndex;
        fieldInfo.isPrivate = ((accessFlags & ACC_PRIVATE) == ACC_PRIVATE);
        // TODO: Parse attributes and add to FieldInfo
        fields.push_back(fieldInfo);
    }

    return fields;
}

inline static AttributeCode* getCodeAttribute(std::vector<AttributeInfo*> attributes, ConstantPool& constantPool) 
{
    return (AttributeCode*) findAttributeWithName(attributes, constantPool, "Code");
}

void ClassLoader::parseDescriptor(const std::string& descriptor, MethodInfo& method)
{
    Descriptor desc = DescriptorParser::parseDescriptor(descriptor);
    method.returnType = desc.returnType;
    method.args = desc.args;
}

std::vector<MethodInfo> ClassLoader::readMethods(ByteArray& byteArray, ConstantPool& constantPool)
{
    std::vector<MethodInfo> methods;

    uint16_t methodsCount = byteArray.readUnsignedShort();

    while (methods.size() < methodsCount) {
        uint16_t accessFlags = byteArray.readUnsignedShort();
        uint16_t nameIndex = byteArray.readUnsignedShort();
        uint16_t descriptorIndex = byteArray.readUnsignedShort();
        std::vector<AttributeInfo*> attributes = readAttributes(byteArray, constantPool);
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
        info.isPublic = ((accessFlags & ACC_PUBLIC) == ACC_PUBLIC);
        info.isStatic = ((accessFlags & ACC_STATIC) == ACC_STATIC);

        parseDescriptor(constantPool.getString(descriptorIndex), info);

        std::string name = constantPool.getString(nameIndex);
        info.isConstructor = (name == "<init>");

        methods.push_back(info);
    }

    return methods;
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
