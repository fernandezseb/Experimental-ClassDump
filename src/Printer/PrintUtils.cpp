#include "PrintUtils.h"


char* PrintUtils::printResolvedInline(char* buffer, const ConstantPoolItem* item, const ConstantPool* cp)
{
	char temp[300];
	switch (item->getType())
	{
	case CT_NAMEANDTYPE:
	{
		sprintf(temp, "NameAndType ");
		strcat(buffer, temp);
		return PrintUtils::printResolved(buffer, item, cp);
	}
	case CT_STRING:
	{
		sprintf(temp, "string ");
		strcat(buffer, temp);
		return PrintUtils::printResolved(buffer, item, cp);
	}
	case CT_FIELDREF:
	{
		// TODO: Print the Class name here as well, even if the class is the class itself
		sprintf(temp, "Field ");
		strcat(buffer, temp);
		return PrintUtils::printResolved(buffer, item, cp);
	}
	case CT_METHODREF:
	{
		sprintf(temp, "Method ");
		strcat(buffer, temp);
		PrintUtils::printResolved(buffer, item, cp);
		break;
	}
	case CT_CLASS:
	{
		sprintf(temp, "class ");
		strcat(buffer, temp);
		PrintUtils::printResolved(buffer, item, cp);
		break;
	}
	case CT_INTEGER:
	{
		sprintf(temp, "int ");
		strcat(buffer, temp);
		return PrintUtils::printData(buffer, item, cp);
	}
	case CT_UTF8:
	{
		return PrintUtils::printData(buffer, item, cp);
	}
	case CT_FLOAT:
	{
		sprintf(temp, "float ");
		strcat(buffer, temp);
		return PrintUtils::printData(buffer, item, cp);
	}
	case CT_DOUBLE:
	{
		sprintf(temp, "double ");
		strcat(buffer, temp);
		return PrintUtils::printData(buffer, item, cp);
	}
	case CT_LONG:
	{
		sprintf(temp, "long ");
		strcat(buffer, temp);
		return PrintUtils::printData(buffer, item, cp);
	}
	default:
	{
		return buffer;
	}
	}
	return buffer;
}

char* PrintUtils::printData(char* buffer, const ConstantPoolItem* item, const ConstantPool* cp)
{
	char temp[300];
	switch (item->getType())
	{
	case CT_NAMEANDTYPE:
	{
		CPNameAndTypeInfo* nameAndType = (CPNameAndTypeInfo*)item;
		sprintf(temp, "#%" PRIu16 ".#%" PRIu16, nameAndType->nameIndex, nameAndType->descriptorIndex);
		break;
	}
	case CT_STRING:
	{
		CPStringInfo* stringInfo = (CPStringInfo*)item;
		sprintf(temp, "#%" PRIu16, stringInfo->stringIndex);
		break;
	}
	case CT_FIELDREF:
	{
		CPFieldRef* fieldRef = (CPFieldRef*)item;
		sprintf(temp, "#%" PRIu16 ".#%" PRIu16, fieldRef->classIndex, fieldRef->nameAndTypeIndex);
		break;
	}
	case CT_METHODREF:
	{
		CPMethodRef* methodRef = (CPMethodRef*)item;
		sprintf(temp, "#%" PRIu16 ".#%" PRIu16, methodRef->classIndex, methodRef->nameAndTypeIndex);
		break;
	}
	case CT_CLASS:
	{
		CPClassInfo* classInfo = (CPClassInfo*)item;
		sprintf(temp, "#%" PRIu16, classInfo->nameIndex);
		break;
	}
	case CT_INTEGER:
	{
		CPIntegerInfo* integerInfo = (CPIntegerInfo*)item;
		sprintf(temp, "%" PRIi32, integerInfo->bytes);
		break;
	}
	case CT_UTF8:
	{
		CPUTF8Info* utf8Info = (CPUTF8Info*)item;
		sprintf(temp, "%s", (char*)utf8Info->bytes);
		break;
	}
	case CT_FLOAT:
	{
		CPFloatInfo* floatInfo = (CPFloatInfo*)item;
		float f = *reinterpret_cast<float*> (&floatInfo->bytes);
		sprintf(temp, "%.10gf", f);
		break;
	}
	case CT_DOUBLE:
	{
		CPDoubleInfo* doubleInfo = (CPDoubleInfo*)item;
		uint64_t bytes = ((uint64_t)doubleInfo->highBytes << 32) + (uint64_t)doubleInfo->lowBytes;
		double d = *reinterpret_cast<double*> (&bytes);
		sprintf(temp, "%.10gd", d);
		break;
	}
	case CT_LONG:
	{
		CPLongInfo* longInfo = (CPLongInfo*)item;
		int64_t bytes = ((int64_t)longInfo->highBytes << 32) + (int64_t)longInfo->lowBytes;
		sprintf(temp, "%" PRIi64 "l", bytes);
		break;
	}
	default:
	{
		temp[0] = 0;
	}
	}

	strcat(buffer, temp);
	return buffer;
}

char* PrintUtils::printResolved(char* buffer, const ConstantPoolItem* item, const ConstantPool* cp)
{
	char temp[300];
	switch (item->getType())
	{
	case CT_NAMEANDTYPE:
	{
		CPNameAndTypeInfo* nameAndType = (CPNameAndTypeInfo*)item;
		const char* name = cp->getString(nameAndType->nameIndex);
		if (strcmp(name, "<init>") == 0) {
			sprintf(temp, "\"%s\":%s", name, cp->getString(nameAndType->descriptorIndex));
		}
		else {
			sprintf(temp, "%s:%s", name, cp->getString(nameAndType->descriptorIndex));
		}
		strcat(buffer, temp);
		break;
	}
	case CT_STRING:
	{
		CPStringInfo* stringInfo = (CPStringInfo*)item;
		sprintf(temp, "%s", cp->getString(stringInfo->stringIndex));
		strcat(buffer, temp);
		break;
	}
	case CT_FIELDREF:
	{
		CPFieldRef* fieldRef = (CPFieldRef*)item;
		printResolved(buffer, cp->getClassInfo(fieldRef->classIndex), cp);
		strcat(buffer, ".");
		printResolved(buffer, cp->constants[fieldRef->nameAndTypeIndex - 1], cp);
		break;
	}
	case CT_METHODREF:
	{
		CPMethodRef* methodRef = (CPMethodRef*)item;

		printResolved(buffer, cp->constants[methodRef->classIndex - 1], cp);
		strcat(buffer, ".");
		printResolved(buffer, cp->constants[methodRef->nameAndTypeIndex - 1], cp);
		break;
	}
	case CT_CLASS:
	{
		CPClassInfo* classInfo = (CPClassInfo*)item;
		sprintf(temp, "%s", cp->getString(classInfo->nameIndex));
		strcat(buffer, temp);
		break;
	}
	default:
	{
	}
	}
	return buffer;
}