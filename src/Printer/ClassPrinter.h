#pragma once

#include "ClassLoader/ClassLoader.h"

enum Opcode : uint8_t {
	// CONSTANTS
	nop         = 0x00,
	aconst_null = 0x01,
	iconst_m1   = 0x02,
	iconst_0    = 0x03,
	iconst_1    = 0x04,
	iconst_2    = 0x05,
	iconst_3    = 0x06,
	iconst_4    = 0x07,
	iconst_5    = 0x08,
	lconst_0    = 0x09,
	lconst_1    = 0x0a,
	fconst_0    = 0x0b,
	fconst_1    = 0x0c,
	fconst_2    = 0x0d,
	dconst_0    = 0x0e,
	dconst_1    = 0x0f,
	bipush      = 0x10,
	sipush      = 0x11,
	ldc         = 0x12,
	ldc_w       = 0x13,
	ldc2_w      = 0x14,
	// LOADS
	iload   = 0x15,
	lload   = 0x16,
	fload   = 0x17,
	dload   = 0x18,
	aload   = 0x19,
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
	iaload  = 0x2e,
	laload  = 0x2f,
	faload  = 0x30,
	daload  = 0x31,
	aaload  = 0x32,
	baload  = 0x33,
	caload  = 0x34,
	saload  = 0x35,
	// STORES
	istore   = 0x36,
	lstore   = 0x37,
	fstore   = 0x38,
	dstore   = 0x39,
	astore   = 0x3a,
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
	iastore  = 0x4f,
	lastore  = 0x50,
	fastore  = 0x51,
	dastore  = 0x52,
	aastore  = 0x53,
	bastore  = 0x54,
	castore  = 0x55,
	sastore  = 0x56,
	// STACK
	pop     = 0x57,
	pop2    = 0x58,
	dup     = 0x59,
	dup_x1  = 0x5a,
	dup_x2  = 0x5b,
	dup2    = 0x5c,
	dup2_x1 = 0x5d,
	dup2_x2 = 0x5e,
	swap    = 0x5f,
	// MATH
	iadd  = 0x60,
	ladd  = 0x61,
	fadd  = 0x62,
	dadd  = 0x63,
	isub  = 0x64,
	lsub  = 0x65,
	fsub  = 0x66,
	dsub  = 0x67,
	imul  = 0x68,
	lmul  = 0x69,
	fmul  = 0x6a,
	dmul  = 0x6b,
	idiv  = 0x6c,
	i_ldiv  = 0x6d,
	fdiv  = 0x6e,
	ddiv  = 0x6f,
	irem  = 0x70,
	lrem  = 0x71,
	frem  = 0x72,
	drem  = 0x73,
	ineg  = 0x74,
	lneg  = 0x75,
	fneg  = 0x76,
	dneg  = 0x77,
	ishl  = 0x78,
	lshl  = 0x79,
	ishr  = 0x7a,
	lshr  = 0x7b,
	iushr = 0x7c,
	lushr = 0x7d,
	iand  = 0x7e,
	land  = 0x7f,
	ior   = 0x80,
	lor   = 0x81,
	ixor  = 0x82,
	lxor  = 0x83,
	iinc  = 0x84,
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
	lcmp  = 0x94,
	fcmpl = 0x95,
	fcmpg = 0x96,
	dcmpl = 0x97,
	dcmpg = 0x98,
	// CONTROL
	ifeq         = 0x99,
	ifne         = 0x9a,
	iflt         = 0x9b,
	ifge         = 0x9c,
	ifgt         = 0x9d,
	ifle         = 0x9e,
	if_icmpeq    = 0x9f,
	if_icmpne    = 0xa0,
	if_icmplt    = 0xa1,
	if_icmpge    = 0xa2,
	if_icmpgt    = 0xa3,
	if_icmple    = 0xa4,
	if_acmpeq    = 0xa5,
	if_acmpne    = 0xa6,
	i_goto       = 0xa7,
	jsr          = 0xa8,
	ret          = 0xa9,
	tableswitch  = 0xaa,
	lookupswitch = 0xab,
	ireturn      = 0xac,
	lreturn      = 0xad,
	freturn      = 0xae,
	dreturn      = 0xaf,
	areturn      = 0xb0,
	i_return     = 0xb1,
	// REFERENCES
	getstatic       = 0xb2,
	putstatic       = 0xb3,
	getfield        = 0xb4,
	putfield        = 0xb5,
	invokevirtual   = 0xb6,
	invokespecial   = 0xb7,
	invokestatic    = 0xb8,
	invokeinterface = 0xb9,
	invokedynamic   = 0xba,
	i_new           = 0xbb,
	newarray        = 0xbc,
	anewarray       = 0xbd,
	arraylength     = 0xbe,
	athrow          = 0xbf,
	checkcast       = 0xc0,
	instanceof      = 0xc1,
	monitorenter    = 0xc2,
	monitorexit     = 0xc3,
	// EXTENDED
	wide           = 0xc4,
	multianewarray = 0xc5,
	ifnull         = 0xc6,
	ifnonnull      = 0xc7,
	goto_w         = 0xc8,
	jsr_w          = 0xc9,
	// RESERVED
	breakpoint = 0xca,
	impdep1    = 0xfe,
	impdep2    = 0xff
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

struct Instruction {
	Opcode opcode;
	uint8_t args;
	const char* name;
	void(*printFunction)(std::vector<uint8_t> args, const ConstantPool* cp) = NULL;
};

class ClassPrinter {
private:

	std::vector<Instruction> instructions;
	const char* Unknown        = "(unknown)";
	const char* ACC_PUBLIC_STR       = "ACC_PUBLIC";
	const char* ACC_PRIVATE_STR      = "ACC_PRIVATE";
	const char* ACC_PROTECTED_STR    = "ACC_PROTECTED";
	const char* ACC_STATIC_STR       = "ACC_STATIC";
	const char* ACC_FINAL_STR        = "ACC_FINAL";
	const char* ACC_SUPER_STR        = "ACC_SUPER";
	const char* ACC_BRIDGE_STR       = "ACC_BRIDGE";
	const char* ACC_VARARGS_STR      = "ACC_VARARGS";
	const char* ACC_NATIVE_STR       = "ACC_NATIVE";
	const char* ACC_INTERFACE_STR    = "ACC_INTERFACE";
	const char* ACC_ABSTRACT_STR     = "ACC_ABSTRACT";
	const char* ACC_STRICT_STR       = "ACC_STRICT";
	const char* ACC_SYNTHETIC_STR    = "ACC_SYNTHETIC";
	const char* ACC_ANNOTATION_STR   = "ACC_ANNOTATION";
	const char* ACC_ENUM_STR         = "ACC_ENUM";
	const char* ACC_SYNCHRONIZED_STR = "ACC_SYNCHRONIZED";
	const char* ACC_VOLATILE_STR     = "ACC_VOLATILE";
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
	void printField(const FieldInfo* fieldInfo, const ConstantPool* cp);
	void printMethodSignature(const MethodInfo* methodInfo, const ClassInfo& classInfo, const ConstantPool* cp);
	void printMethod(const MethodInfo* methodInfo, const ClassInfo& classInfo, const ConstantPool* cp);
	static std::string toString(const ConstantPoolItem* item, const ConstantPool* cp);
	static std::string toExpandedString(const ConstantPoolItem* item, const ConstantPool* cp);
	void printCode(const AttributeCode* code, const MethodInfo* methodInfo, const ConstantPool* cp);
public:
	ClassPrinter();
	void printClass(const ClassInfo& classInfo);
	static std::string toStringInline(const ConstantPoolItem* item, const ConstantPool* cp);
};