#pragma once

#include "Opcode.h"
#include "ClassLoader/ClassLoader.h"

#include <iostream>

// Type strings
#define T_VOID    "void"
#define T_BYTE    "byte"
#define T_CHAR    "char"
#define T_DOUBLE  "double"
#define T_FLOAT   "float"
#define T_INT     "int"
#define T_LONG    "long"
#define T_SHORT   "short"
#define T_BOOLEAN "boolean"
#define T_DOUBLE  "double"

void SignedBytePrinter(uint8_t* args, uint16_t argsCount, const ConstantPool* cp);
void UnsignedBytePrinter(uint8_t* args, uint16_t argsCount, const ConstantPool* cp);
void ShortIndices(uint8_t* args, uint16_t argsCount, const ConstantPool* cp);
void ArrayTypePrinter(uint8_t* args, uint16_t argsCount, const ConstantPool* cp);
void MultiArrayPrinter(uint8_t* args, uint16_t argsCount, const ConstantPool* cp);
void SignedShort(uint8_t* args, uint16_t argsCount, const ConstantPool* cp);

struct Instruction {
	Opcode opcode;
	uint8_t args;
	const char* name;
	void(*printFunction)(uint8_t* args, uint16_t argsCount, const ConstantPool* cp) = NULL;
};

class ClassPrinter {
private:

	Instruction instructions[208] =
	{
		{ nop, 0, "nop" },
		{ bipush, 1, "bipush", SignedBytePrinter },
		{ istore_0, 0, "istore_0" },
		{ istore_1, 0, "istore_1" },
		{ istore_2, 0, "istore_2" },
		{ istore_3, 0, "istore_3" },
		{ i_return, 0, "return" },
		{ i_new, 2, "new", ShortIndices },
		{ dup, 0, "dup" },
		{ invokespecial, 2, "invokespecial", ShortIndices },
		{ aaload, 0, "aaload" },
		{ aastore, 0, "aastore" },
		{ aconst_null, 0, "aconst_null" },
		{ aload, 1, "aload", UnsignedBytePrinter },
		{ aload_0, 0, "aload_0" },
		{ aload_1, 0, "aload_1" },
		{ aload_2, 0, "aload_2" },
		{ aload_3, 0, "aload_3" },
		{ anewarray, 2, "anewarray", ShortIndices },
		{ areturn, 0, "areturn" },
		{ aload_1, 0, "aload_1" },
		{ arraylength, 0, "arraylength" },
		{ astore, 1, "astore", UnsignedBytePrinter },
		{ astore_0, 0, "astore_0" },
		{ astore_1, 0, "astore_1" },
		{ astore_2, 0, "astore_2" },
		{ astore_3, 0, "astore_3" },
		{ aload_0, 0, "aload_0" },
		{ ldc, 1, "ldc", SignedBytePrinter },
		{ ldc_w, 2, "ldc_w", ShortIndices },
		{ ldc2_w, 2, "ldc2_w", ShortIndices },
		{ invokevirtual, 2, "invokevirtual", ShortIndices },
		{ invokestatic, 2, "invokestatic", ShortIndices },
		{ athrow, 0, "athrow" },
		{ baload, 0, "baload" },
		{ bastore, 0, "bastore" },
		{ caload, 0, "caload" },
		{ castore, 0, "castore" },
		{ checkcast, 2, "checkcast", ShortIndices },
		{ d2f, 0, "d2f" },
		{ d2i, 0, "d2i" },
		{ d2l, 0, "d2l" },
		{ dadd, 0, "dadd" },
		{ daload, 0, "daload" },
		{ dastore, 0, "dastore" },
		{ dcmpl, 0, "dcmpl" },
		{ dcmpg, 0, "dcmpg" },
		{ dconst_0, 0, "dconst_0" },
		{ dconst_1, 0, "dconst_1" },
		{ ddiv, 0, "ddiv" },
		{ dload, 1, "dload", UnsignedBytePrinter },
		{ dload_0, 0, "dload_0" },
		{ dload_1, 0, "dload_1" },
		{ dload_2, 0, "dload_2" },
		{ dload_3, 0, "dload_3" },
		{ dmul, 0, "dmul" },
		{ dneg, 0, "dneg" },
		{ drem, 0, "drem" },
		{ dreturn, 0, "dreturn" },
		{ dstore, 1, "dstore", UnsignedBytePrinter },
		{ dstore_0, 0, "dstore_0" },
		{ dstore_1, 0, "dstore_1" },
		{ dstore_2, 0, "dstore_2" },
		{ dstore_3, 0, "dstore_3" },
		{ dsub, 0, "dsub" },
		{ dup, 0, "dup" },
		{ dup_x1, 0, "dup_x1" },
		{ dup_x2, 0, "dup_x2" },
		{ dup2, 0, "dup2" },
		{ dup2_x1, 0, "dup2_x1" },
		{ dup2_x2, 0, "dup2_x2" },
		{ f2d, 0, "f2d" },
		{ f2i, 0, "f2i" },
		{ f2l, 0, "f2l" },
		{ fadd, 0, "fadd" },
		{ faload, 0, "faload" },
		{ fastore, 0, "fastore" },
		{ fcmpl, 0, "fcmpl" },
		{ fcmpg, 0, "fcmpg" },
		{ fconst_0, 0, "fconst_0" },
		{ fconst_1, 0, "fconst_1" },
		{ fconst_2, 0, "fconst_2" },
		{ fdiv, 0, "fdiv" },
		{ fload, 1, "fload", UnsignedBytePrinter },
		{ fload_0, 0, "fload_0" },
		{ fload_1, 0, "fload_1" },
		{ fload_2, 0, "fload_2" },
		{ fload_3, 0, "fload_3" },
		{ fmul, 0, "fmul" },
		{ fneg, 0, "fneg" },
		{ frem, 0, "frem" },
		{ freturn, 0, "freturn" },
		{ fstore, 1, "fstore", UnsignedBytePrinter },
		{ fstore_0, 0, "fstore_0" },
		{ fstore_1, 0, "fstore_1" },
		{ fstore_2, 0, "fstore_2" },
		{ fstore_3, 0, "fstore_3" },
		{ fsub, 0, "fsub" },
		{ getfield, 2, "getfield", ShortIndices },
		{ getstatic, 2, "getstatic", ShortIndices },
		// TODO: we must add the current address to the offset provided by the instruction (GOTO)
		{ i_goto, 2, "goto", ShortIndices }, // SIGNED BRANCHOFFSET
		{ goto_w, 4, "goto_w", ShortIndices }, // 32 bit SIGNED
		{ i2b, 0, "i2b" },
		{ i2c, 0, "i2c" },
		{ i2d, 0, "i2d" },
		{ i2f, 0, "i2f" },
		{ i2l, 0, "i2l" },
		{ i2s, 0, "i2s" },
		{ iadd, 0, "iadd" },
		{ iaload, 0, "iaload" },
		{ iand, 0, "iand" },
		{ iastore, 0, "iastore" },
		{ iconst_m1, 0, "iconst_m1" },
		{ iconst_0, 0, "iconst_0" },
		{ iconst_1, 0, "iconst_1" },
		{ iconst_2, 0, "iconst_2" },
		{ iconst_3, 0, "iconst_3" },
		{ iconst_4, 0, "iconst_4" },
		{ iconst_5, 0, "iconst_5" },
		{ idiv, 0, "idiv" },
		{ if_acmpeq, 2, "if_acmpeq", SignedBytePrinter }, // SIGNED 16 bit offset 
		{ if_acmpne, 2, "if_acmpne",SignedBytePrinter }, // SIGNED  16 bit offset
		{ if_icmpeq, 2, "if_icmpeq",SignedBytePrinter }, // SIGNED  16 bit offset
		{ if_icmpne, 2, "if_icmpne",SignedBytePrinter }, // SIGNED  16 bit offset
		{ if_icmplt, 2, "if_icmplt",SignedBytePrinter }, // SIGNED  16 bit offset
		{ if_icmpge, 2, "if_icmpge",SignedBytePrinter }, // SIGNED  16 bit offset
		{ if_icmpgt, 2, "if_icmpgt",SignedBytePrinter }, // SIGNED  16 bit offset
		{ if_icmple, 2, "if_icmple",SignedBytePrinter }, // SIGNED  16 bit offset
		{ ifeq, 2, "ifeq",SignedBytePrinter }, // SIGNED  16 bit offset
		{ ifne, 2, "ifne",SignedBytePrinter }, // SIGNED  16 bit offset
		{ iflt, 2, "iflt",SignedBytePrinter }, // SIGNED  16 bit offset
		{ ifge, 2, "ifge",SignedBytePrinter }, // SIGNED  16 bit offset
		{ ifgt, 2, "ifgt",SignedBytePrinter }, // SIGNED  16 bit offset
		{ ifle, 2, "ifle",SignedBytePrinter }, // SIGNED  16 bit offset
		{ ifnonnull, 2, "ifnonnull",SignedBytePrinter }, // SIGNED  16 bit offset
		{ ifnull, 2, "ifnull",SignedBytePrinter }, // SIGNED  16 bit offset
		{ iinc, 2, "iinc", SignedBytePrinter }, // 1 UNSIGNED BYTE LOCAL VARIABLE AND SIGNED BYTE, we need to print both
		{ iload, 1, "iload", UnsignedBytePrinter },
		{ iload_0, 0, "iload_0" },
		{ iload_1, 0, "iload_1" },
		{ iload_2, 0, "iload_2" },
		{ iload_3, 0, "iload_3" },
		{ imul, 0, "imul" },
		{ ineg, 0, "ineg" },
		{ instanceof, 2, "instanceof", ShortIndices },
		{ invokedynamic, 4, "invokedynamic", ShortIndices }, // TODO: Investigate 0 0
		{ invokeinterface, 4, "invokeinterface", SignedBytePrinter }, // TODO: Investigate
		{ invokespecial, 2, "invokespecial", ShortIndices },
		{ ior, 0, "ior" },
		{ irem, 0, "irem" },
		{ ireturn, 0, "ireturn" },
		{ ishl , 0, "ishl" },
		{ ishr, 0, "ishr" },
		{ istore, 1, "istore", UnsignedBytePrinter },
		{ istore_0, 0, "istore_0" },
		{ istore_1, 0, "istore_1" },
		{ istore_2, 0, "istore_2" },
		{ istore_3, 0, "istore_3" },
		{ isub, 0, "isub" },
		{ iushr, 0, "iushr" },
		{ ixor, 0, "ixor" },
		{ jsr, 2, "jsr", ShortIndices }, // SIGNED 16 bit address
		{ jsr_w, 4, "jsr_w", ShortIndices }, // SIGNED 32 bit address
		{ l2d, 0, "l2d" },
		{ l2f, 0, "l2f" },
		{ l2i, 0, "l2i" },
		{ ladd, 0, "ladd" },
		{ laload, 0, "laload" },
		{ land, 0, "land", },
		{ lastore, 0, "lastore" },
		{ lcmp, 0, "lcmp" },
		{ lconst_0, 0, "lconst_0" },
		{ lconst_1, 0, "lconst_1" },
		{ i_ldiv, 0, "ldiv" },
		{ lload, 1, "lload", UnsignedBytePrinter },
		{ lload_0, 0, "lload_0" },
		{ lload_1, 0, "lload_1" },
		{ lload_2, 0, "lload_2" },
		{ lload_3, 0, "lload_3" },
		{ lmul, 0, "lmul" },
		{ lneg, 0, "lneg" },
		{ lor, 0, "lor" },
		{ lrem, 0, "lrem" },
		{ lreturn, 0, "lreturn" },
		{ lshl, 0, "lshl" },
		{ lshr, 0, "lshr" },
		{ lstore, 1, "lstore", UnsignedBytePrinter },
		{ lstore_0, 0, "lstore_0" },
		{ lstore_1, 0, "lstore_1" },
		{ lstore_2, 0, "lstore_2" },
		{ lstore_3, 0, "lstore_3" },
		{ lsub, 0, "lsub" },
		{ lushr, 0, "lushr" },
		{ lxor, 0, "lxor" },
		{ monitorenter, 0, "monitorenter" },
		{ monitorexit, 0, "monitorexit" },
		{ multianewarray, 3, "multianewarray", MultiArrayPrinter },
		{ newarray, 1, "newarray", ArrayTypePrinter },
		{ nop, 0, "nop" },
		{ pop, 0, "pop" },
		{ pop2, 0, "pop2" },
		{ putstatic, 2, "putstatic", ShortIndices },
		{ ret, 1, "ret", SignedBytePrinter },
		{ saload, 0, "saload" },
		{ sastore, 0, "sastore" },
		{ sipush, 2, "sipush", SignedShort },
		{ swap, 0, "swap" },
		// TODO: Add custom printer for tableswitch
		// TODO: Add custom printer for wide
		{ putfield, 2, "putfield", ShortIndices }
	};
	const char* Unknown = "(unknown)";
	const char* ACC_PUBLIC_STR = "ACC_PUBLIC";
	const char* ACC_PRIVATE_STR = "ACC_PRIVATE";
	const char* ACC_PROTECTED_STR = "ACC_PROTECTED";
	const char* ACC_STATIC_STR = "ACC_STATIC";
	const char* ACC_FINAL_STR = "ACC_FINAL";
	const char* ACC_SUPER_STR = "ACC_SUPER";
	const char* ACC_BRIDGE_STR = "ACC_BRIDGE";
	const char* ACC_VARARGS_STR = "ACC_VARARGS";
	const char* ACC_NATIVE_STR = "ACC_NATIVE";
	const char* ACC_INTERFACE_STR = "ACC_INTERFACE";
	const char* ACC_ABSTRACT_STR = "ACC_ABSTRACT";
	const char* ACC_STRICT_STR = "ACC_STRICT";
	const char* ACC_SYNTHETIC_STR = "ACC_SYNTHETIC";
	const char* ACC_ANNOTATION_STR = "ACC_ANNOTATION";
	const char* ACC_ENUM_STR = "ACC_ENUM";
	const char* ACC_SYNCHRONIZED_STR = "ACC_SYNCHRONIZED";
	const char* ACC_VOLATILE_STR = "ACC_VOLATILE";
	const char* ACC_TRANSIENT_STR = "ACC_TRANSIENT";

	const char* ACC_PUBLIC_KEYWORD = "public";
	const char* ACC_PRIVATE_KEYWORD = "private";
	const char* ACC_PROTECTED_KEYWORD = "protected";
	const char* ACC_STATIC_KEYWORD = "static";
	const char* ACC_FINAL_KEYWORD = "final";
	const char* ACC_NATIVE_KEYWORD = "native";
	const char* ACC_INTERFACE_KEYWORD = "interface";
	const char* ACC_ABSTRACT_KEYWORD = "abstract";
	const char* ACC_SYNCHRONIZED_KEYWORD = "synchronized";
	const char* ACC_VOLATILE_KEYWORD = "volatile";
	const char* ACC_TRANSIENT_KEYWORD = "transient";

	enum AccessFlagType {
		CLASS, METHOD, FIELD
	};
	
	const std::map<ConstantType, const char*> constantTypes =
	{
		{CT_UTF8,    "Utf8"},
		{CT_INTEGER, "Integer"},
		{CT_FLOAT,   "Float"},
		{CT_LONG,    "Long"},
		{CT_DOUBLE,  "Double"},
		{CT_CLASS,   "Class"},
		{CT_STRING,  "String"},
		{CT_FIELDREF, "Fieldref"},
		{CT_METHODREF,"Methodref"},
		{CT_INTERFACEMETHOD, "Interfacemethod"},
		{CT_NAMEANDTYPE,  "NameAndType"},
		{CT_METHODHANDLE, "MethodHandle"},
		{CT_METHODTYPE, "Methodtype"},
		{CT_INVOKEDYNAMIC, "InvokeDynamic"}
	};
	const std::map<AccessFlag, const char*> accessFlagsField = {
		{ACC_PUBLIC, ACC_PUBLIC_STR},
		{ACC_PRIVATE, ACC_PRIVATE_STR},
		{ACC_PROTECTED, ACC_PROTECTED_STR},
		{ACC_STATIC, ACC_STATIC_STR},
		{ACC_FINAL, ACC_FINAL_STR},
		{ACC_VOLATILE, ACC_VOLATILE_STR},
		{ACC_TRANSIENT, ACC_TRANSIENT_STR},
		{ACC_SYNTHETIC, ACC_SYNTHETIC_STR},
		{ACC_ENUM, ACC_ENUM_STR}
	};

	const std::map<AccessFlag, const char*> keywords = {
		{ACC_PUBLIC, ACC_PUBLIC_KEYWORD},
		{ACC_PRIVATE, ACC_PRIVATE_KEYWORD},
		{ACC_PROTECTED, ACC_PROTECTED_KEYWORD},
		{ACC_STATIC, ACC_STATIC_KEYWORD},
		{ACC_FINAL, ACC_FINAL_KEYWORD},
		{ACC_NATIVE, ACC_NATIVE_KEYWORD},
		{ACC_ABSTRACT, ACC_ABSTRACT_KEYWORD},
		{ACC_SYNCHRONIZED, ACC_SYNCHRONIZED_KEYWORD},
		{ACC_VOLATILE, ACC_VOLATILE_KEYWORD},
		{ACC_TRANSIENT, ACC_TRANSIENT_KEYWORD}
	};

	const std::map<AccessFlag, const char*> keywordsClass = {
		{ACC_PUBLIC, ACC_PUBLIC_KEYWORD},
		{ACC_PRIVATE, ACC_PRIVATE_KEYWORD},
		{ACC_PROTECTED, ACC_PROTECTED_KEYWORD},
		{ACC_STATIC, ACC_STATIC_KEYWORD},
		{ACC_FINAL, ACC_FINAL_KEYWORD},
		{ACC_ABSTRACT, ACC_ABSTRACT_KEYWORD}
	};

	const std::map<AccessFlag, const char*> accessFlagsMethod = {
		{ACC_PUBLIC, ACC_PUBLIC_STR},
		{ACC_PRIVATE, ACC_PRIVATE_STR},
		{ACC_PROTECTED, ACC_PROTECTED_STR},
		{ACC_STATIC, ACC_STATIC_STR},
		{ACC_FINAL, ACC_FINAL_STR},
		{ACC_SYNCHRONIZED, ACC_SYNCHRONIZED_STR},
		{ACC_BRIDGE, ACC_BRIDGE_STR},
		{ACC_VARARGS, ACC_VARARGS_STR},
		{ACC_NATIVE, ACC_NATIVE_STR},
		{ACC_ABSTRACT, ACC_ABSTRACT_STR},
		{ACC_STRICT, ACC_STRICT_STR},
		{ACC_SYNTHETIC, ACC_SYNTHETIC_STR}
	};

	const std::map<AccessFlag, const char*> accessFlagsClass = {
		{ACC_PUBLIC, ACC_PUBLIC_STR},
		{ACC_FINAL, ACC_FINAL_STR},
		{ACC_SUPER, ACC_SUPER_STR},
		{ACC_INTERFACE, ACC_INTERFACE_STR},
		{ACC_ABSTRACT, ACC_ABSTRACT_STR},
		{ACC_SYNTHETIC, ACC_SYNTHETIC_STR},
		{ACC_ANNOTATION, ACC_ANNOTATION_STR},
		{ACC_ENUM, ACC_ENUM_STR}
	};
	const std::map<char, const char*> types = {
		{'V', T_VOID},
		{'B', T_BYTE},
		{'C', T_CHAR},
		{'D', T_DOUBLE},
		{'F', T_FLOAT},
		{'I', T_INT},
		{'J', T_LONG},
		{'S', T_SHORT},
		{'Z', T_BOOLEAN},
		{'D', T_DOUBLE}
	};
	const char* getTypeAsString(ConstantType type) const;
	const char* getTypeAsString(AccessFlag flag, AccessFlagType type) const;
	std::string getAsExternalReturnType(std::string returnType);
	void printField(const FieldInfo* fieldInfo, const ConstantPool* cp, Memory* memory);
	void printMethodSignature(const MethodInfo* methodInfo, 
		const ClassInfo& classInfo, 
		const char* className, 
		const ConstantPool* cp,
		Memory* memory);
	void printMethod(const MethodInfo* methodInfo, const ClassInfo& classInfo, const char* className, const ConstantPool* cp, Memory* memory);
	static std::string toString(const ConstantPoolItem* item, const ConstantPool* cp);
	static std::string toExpandedString(const ConstantPoolItem* item, const ConstantPool* cp);
	void printCode(const AttributeCode* code, const MethodInfo* methodInfo, const ConstantPool* cp, Memory* memory);
	void printConstantPoolItem(uint16_t currentIndex, const ConstantPool* cp);
public:
	void printClass(const ClassInfo& classInfo, Memory* memory);
	static std::string toStringInline(const ConstantPoolItem* item, const ConstantPool* cp);
};