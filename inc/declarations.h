#include <stdbool.h>

#ifndef DECLARATIONS_H
#define DECLARATIONS_H


//register declarations

#define zero    0
#define at      1
#define v0      2
#define v1      3
#define a0      4
#define a1      5
#define a2      6
#define a3      7
#define t0      8
#define t1      9
#define t2      10
#define t3      11
#define t4      12
#define t5      13
#define t6      14
#define t7      15
#define s0      16
#define s1      17
#define s2      18
#define s3      19
#define s4      20
#define s5      21
#define s6      22
#define s7      23
#define t8      24
#define t9      25
#define k0      26
#define k1      27
#define gp      28
#define sp      29
#define fp      30
#define ra      31


//instruction set

/* need


sh

*/

#define j       0x0002      //j
#define jal     0x0003      //j

#define beq     0x0004      //i
#define bne     0x0005      //i
#define addi    0x0008      //i
#define addiu   0x0009      //i
#define slti    0x000A      //i
#define sltiu   0x000B      //i
#define andi    0x000C      //i
#define ori     0x000D      //i
#define xori    0x000E      //i
#define lui     0x000F      //i
#define lb      0x0020      //i
#define lh      0x0021      //i
#define lw      0x0023      //i
#define lbu     0x0024      //i
#define lhu     0x0025      //i
#define sb      0x0028      //i
#define sh      0x0029      //i
#define sw      0x002B      //i
#define bltz    0x0001      //i
#define bgtz    0x0007      //i
#define blez    0x0006      //i

//seb sign extend byte
#define sll     0x0000      //r
#define srl     0x0002      //r
#define jr      0x0008      //r
#define add     0x0020      //r
#define addu    0x0021      //r
#define sub     0x0022      //r
#define subu    0x0023      //r
#define and     0x0024      //r
#define or      0x0025      //r
#define xor     0x0026      //r
#define nor     0x0027      //r
#define slt     0x002A      //r
#define sltu    0x002B      //r

//need some help on these two
#define movn    0x000B      //r
#define movz    0x000A      //r





typedef struct{
    unsigned int Opcode;
    unsigned int Rs;
    unsigned int Rt;
    unsigned int Rd;
    unsigned int shamt;
    unsigned int funct;
    short int immediate;
    unsigned long jumpaddress;
    unsigned int PCinc;
    unsigned int type;
} IFID_Reg;


typedef struct{
    bool regWrite;//regWrite
    bool memread;
    bool memtoreg;
    bool memwrite;
    unsigned int ALUop;
    unsigned int Rd; //destination reg
    unsigned int Rs; //regRs
    unsigned int Rt; //regRt
    unsigned long RsValue; //regRsValue
    unsigned long RtValue; //regRtValue
    unsigned int shamt;
    short int immediate;
    unsigned long jumpaddress;
    unsigned int PCinc;
    unsigned int type;

} IDEX_Reg;

typedef struct{
    bool regWrite;//regWrite
    bool memread;
    bool memtoreg;
    bool memwrite;
    unsigned int ALUop;
    unsigned long ALUresult;
    unsigned long jumpaddress;
    unsigned int shamt;
    unsigned int Rd; //destination reg r type
    unsigned int Rt; //destination reg i type
    unsigned long RsValue; //regRsValue
    unsigned long RtValue; //regRtValue
    unsigned long WBRegister;
    short int immediate;
    unsigned int PCinc;
    unsigned int type;

} EXMEM_Reg;

typedef struct{
    bool regWrite;
    bool memtoreg;
    bool memwrite;
    unsigned int DataMemResult;
    unsigned long ALUresult;
    unsigned long WBRegister;
    unsigned int type;

} MEMWB_Reg;




#endif //DECLARATIONS_H
