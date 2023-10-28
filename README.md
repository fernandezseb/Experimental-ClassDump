# ClassDump

A simple java classfile parser made for learning purposes.
The output is very similar to the one from the javap tool with all output enabled. (Private members, system info about the class, etc.)


## Example

```bash
./ClassDump test/Main.class
```

```
Starting ClassDump v0.1
Classfile C:\Users\Sebastiaan\source\repos\fernandezseb\ClassDump\test\Main.class
  Last modified Oct 03, 2023; size 570 bytes
  MD5 checksum e3d65bf17469600ec4c904dadaf75c43
public class Main implements java.io.Serializable
  minor version 0
  major version 52
  flags: ACC_PUBLIC, ACC_SUPER
Constant pool:
   #1 = Methodref      #9.#28         // java/lang/Object."<init>":()V
   #2 = Integer        45412
   #3 = Fieldref       #8.#29         // Main.i:I
   #4 = Methodref      #8.#30         // Main.brol:()V
   #5 = Class          #31            // java/lang/IllegalArgumentException
   #6 = String         #32            // Brol
   #7 = Methodref      #5.#33         // java/lang/IllegalArgumentException."<init>":(Ljava/lang/String;)V
   #8 = Class          #34            // Main
   #9 = Class          #35            // java/lang/Object
  #10 = Class          #36            // java/io/Serializable
  #11 = Utf8           i
  #12 = Utf8           I
  #13 = Utf8           <init>
  #14 = Utf8           ()V
  #15 = Utf8           Code
  #16 = Utf8           LineNumberTable
  #17 = Utf8           LocalVariableTable
  #18 = Utf8           this
  #19 = Utf8           LMain;
  #20 = Utf8           brol
  #21 = Utf8           main
  #22 = Utf8           ([Ljava/lang/String;)V
  #23 = Utf8           args
  #24 = Utf8           [Ljava/lang/String;
  #25 = Utf8           j
  #26 = Utf8           SourceFile
  #27 = Utf8           Main.java
  #28 = NameAndType    #13.#14        // "<init>":()V
  #29 = NameAndType    #11.#12        // i:I
  #30 = NameAndType    #20.#14        // brol:()V
  #31 = Utf8           java/lang/IllegalArgumentException
  #32 = Utf8           Brol
  #33 = NameAndType    #13.#37        // "<init>":(Ljava/lang/String;)V
  #34 = Utf8           Main
  #35 = Utf8           java/lang/Object
  #36 = Utf8           java/io/Serializable
  #37 = Utf8           (Ljava/lang/String;)V
{
  public Main();
    descriptor: ()V
    flags: ACC_PUBLIC
    Code:
      stack=2, locals=1, args_size=0
         0: aload_0
         1: invokespecial   #1
         4: aload_0
         5: ldc             #2
         7: putfield        #3
        10: return

  public static native void brol();
    descriptor: ()V
    flags: ACC_PUBLIC, ACC_STATIC, ACC_NATIVE

  public static void main(java.lang.String[]);
    descriptor: ([Ljava/lang/String;)V
    flags: ACC_PUBLIC, ACC_STATIC
    Code:
      stack=3, locals=2, args_size=1
         0: bipush          9
         2: istore_1
         3: invokestatic    #4
         6: new             #5
         9: dup
        10: ldc             #6
        12: invokespecial   #7
        15: athrow

}
```


## Goal and scope

The goal of this project is to be a simple to understand C++ implementation of a Java class file parser, in order to learn more about Java class files and C++. 
(Because I haven't worked professionally with C++ yet.)
In the future I am going to use this one and create a simple to understand basic implementation of a JVM using this parser, also with the aim of being educational.

The goal is **not** to be reliable, fast, or memory efficient, or even feature complete.
Even though it is not set in stone, I will be trying to implement as much features as possible of JDK 8, in the free time that I have.

## Still to do
* Showing the data to which the code is referring inline
* Finish all attribute parsing
* Complete all flag parsing
* The last opcodes that are not implemented yet: lookuptable, wide
* Print StackMapTable (other method/code attributes)
* Use contiguous memory for loaded classes