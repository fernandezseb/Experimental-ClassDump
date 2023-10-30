#pragma once

#include "ClassLoader/ClassLoader.h"

enum Opcode : uint8_t {
	// CONSTANTS
	nop = 0x00,
	aconst_null = 0x01, 
	iconst_m1 = 0x02,
	iconst_0 = 0x03,
	iconst_1 = 0x04,
	iconst_2 = 0x05,
	iconst_3 = 0x06,
	iconst_4 = 0x07,
	iconst_5 = 0x08,
	lconst_0 = 0x09,
	lconst_1 = 0x0a,
	fconst_0 = 0x0b,
	fconst_1 = 0x0c,
	fconst_2 = 0x0d,
	dconst_0 = 0x0e,
	dconst_1 = 0x0f,
	bipush = 0x10,
	sipush = 0x11,
	ldc = 0x12,
	ldc_w = 0x13,
	ldc2_w = 0x14,
	// LOADS
	iload = 0x15,
	lload = 0x16,
	fload = 0x17,
	dload = 0x18,
	aload = 0x19,
	iload_0 = 0x1a,
	iload_1 = 0x1b,
	iload_2 = 0x1c,
	iload_3 = 0x1d,
	lload_0 = 0x1e,
	lload_1 = 0x1f,
	lload_2 = 0x20,
	lload_3 = 0x21,
	fload_0 = 0x22,
	fload_1 = 0x23,
	fload_2 = 0x24,
	fload_3 = 0x25,
	dload_0 = 0x26,
	dload_1 = 0x27,
	dload_2 = 0x28,
	dload_3 = 0x29,
	aload_0 = 0x2a,
	aload_1 = 0x2b,
	aload_2 = 0x2c,
	aload_3 = 0x2d,
	iaload = 0x2e,
	laload = 0x2f,
	faload = 0x30,
	daload = 0x31,
	aaload = 0x32,
	baload = 0x33,
	caload = 0x34,
	saload = 0x35,
	// STORES
	istore = 0x36,
	lstore = 0x37,
	fstore = 0x38,
	dstore = 0x39,
	astore = 0x3a,
	istore_0 = 0x3b,
	istore_1 = 0x3c,
	istore_2 = 0x3d,
	istore_3 = 0x3e,
	lstore_0 = 0x3f,
	lstore_1 = 0x40,
	lstore_2 = 0x41,
	lstore_3 = 0x42,
	fstore_0 = 0x43,
	fstore_1 = 0x44,
	fstore_2 = 0x45,
	fstore_3 = 0x46,
	dstore_0 = 0x47,
	dstore_1 = 0x48,
	dstore_2 = 0x49,
	dstore_3 = 0x4a,
	astore_0 = 0x4b,
	astore_1 = 0x4c,
	astore_2 = 0x4d,
	astore_3 = 0x4e,
	iastore = 0x4f,
	lastore = 0x50,
	fastore = 0x51,
	dastore = 0x52,
	aastore = 0x53,
	bastore = 0x54,
	castore = 0x55,
	sastore = 0x56,
	// STACK
	pop = 0x57,
	pop2 = 0x58,
	dup = 0x59,
	dup_x1 = 0x5a,
	dup_x2 = 0x5b,
	dup2 = 0x5c,
	dup2_x1 = 0x5d,
	dup2_x2 = 0x5e,
	swap = 0x5f,
	// MATH
	iadd = 0x60,
	ladd = 0x61,
	fadd = 0x62,
	dadd = 0x63,
	isub = 0x64,
	lsub = 0x65,
	fsub = 0x66,
	dsub = 0x67,
	imul = 0x68,
	lmul = 0x69,
	fmul = 0x6a,
	dmul = 0x6b,
	idiv = 0x6c,
	i_ldiv = 0x6d,
	fdiv = 0x6e,
	ddiv = 0x6f,
	irem = 0x70,
	lrem = 0x71,
	frem = 0x72,
	drem = 0x73,
	ineg = 0x74,
	lneg = 0x75,
	fneg = 0x76,
	dneg = 0x77,
	ishl = 0x78,
	lshl = 0x79,
	ishr = 0x7a,
	lshr = 0x7b,
	iushr = 0x7c,
	lushr = 0x7d,
	iand = 0x7e,
	land = 0x7f,
	ior = 0x80,
	lor = 0x81,
	ixor = 0x82,
	lxor = 0x83,
	iinc = 0x84,
	// CONVERSIONS
	i2l = 0x85,
	i2f = 0x86,
	i2d = 0x87,
	l2i = 0x88,
	l2f = 0x89,
	l2d = 0x8a,
	f2i = 0x8b,
	f2l = 0x8c,
	f2d = 0x8d,
	d2i = 0x8e,
	d2l = 0x8f,
	d2f = 0x90,
	i2b = 0x91,
	i2c = 0x92,
	i2s = 0x93,
	lcmp = 0x94,
	fcmpl = 0x95,
	fcmpg = 0x96,
	dcmpl = 0x97,
	dcmpg = 0x98,
	// CONTROL
	ifeq = 0x99,
	ifne = 0x9a,
	iflt = 0x9b,
	ifge = 0x9c,
	ifgt = 0x9d,
	ifle = 0x9e,
	if_icmpeq = 0x9f,
	if_icmpne = 0xa0,
	if_icmplt = 0xa1,
	if_icmpge = 0xa2,
	if_icmpgt = 0xa3,
	if_icmple = 0xa4,
	if_acmpeq = 0xa5,
	if_acmpne = 0xa6,
	i_goto = 0xa7,
	jsr = 0xa8,
	ret = 0xa9,
	tableswitch = 0xaa,
	lookupswitch = 0xab,
	ireturn = 0xac,
	lreturn = 0xad,
	freturn = 0xae,
	dreturn = 0xaf,
	areturn = 0xb0,
	i_return = 0xb1,
	// REFERENCES
	getstatic = 0xb2,
	putstatic = 0xb3,
	getfield = 0xb4,
	putfield = 0xb5,
	invokevirtual = 0xb6,
	invokespecial = 0xb7,
	invokestatic = 0xb8,
	invokeinterface = 0xb9,
	invokedynamic = 0xba,
	i_new = 0xbb,
	newarray = 0xbc,
	anewarray = 0xbd,
	arraylength = 0xbe,
	athrow = 0xbf,
	checkcast = 0xc0,
	instanceof = 0xc1,
	monitorenter = 0xc2,
	monitorexit = 0xc3,
	// EXTENDED
	wide = 0xc4,
	multianewarray = 0xc5,
	ifnull = 0xc6,
	ifnonnull = 0xc7,
	goto_w = 0xc8,
	jsr_w = 0xc9,
	// RESERVED
	breakpoint = 0xca,
	impdep1 = 0xfe,
	impdep2 = 0xff
};

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

void SignedBytePrinter(std::vector<uint8_t> args, const ConstantPool* cp);
void UnsignedBytePrinter(std::vector<uint8_t> args, const ConstantPool* cp);
void ShortIndices(std::vector<uint8_t> args, const ConstantPool* cp);
void ArrayTypePrinter(std::vector<uint8_t> args, const ConstantPool* cp);
void MultiArrayPrinter(std::vector<uint8_t> args, const ConstantPool* cp);
void SignedShort(std::vector<uint8_t> args, const ConstantPool* cp);

struct Instruction {
	Opcode opcode;
	uint8_t args;
	const char* name;
	void(*printFunction)(std::vector<uint8_t> args, const ConstantPool* cp) = NULL;
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
	static std::string getAsExternalClassName(std::string className);
	void printField(const FieldInfo* fieldInfo, const ConstantPool* cp, Memory* memory);
	void printMethodSignature(const MethodInfo* methodInfo, const ClassInfo& classInfo, const ConstantPool* cp);
	void printMethod(const MethodInfo* methodInfo, const ClassInfo& classInfo, const ConstantPool* cp, Memory* memory);
	static std::string toString(const ConstantPoolItem* item, const ConstantPool* cp);
	static std::string toExpandedString(const ConstantPoolItem* item, const ConstantPool* cp);
	void printCode(const AttributeCode* code, const MethodInfo* methodInfo, const ConstantPool* cp);
public:
	ClassPrinter();
	void printClass(const ClassInfo& classInfo, Memory* memory);
	static std::string toStringInline(const ConstantPoolItem* item, const ConstantPool* cp);
};