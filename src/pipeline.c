#include "declarations.h"
#include "Load_Program.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


//create the 4 register structs
IFID_Reg IFID;
IDEX_Reg IDEX;
EXMEM_Reg EXMEM;
MEMWB_Reg MEMWB;

//intialize program counter
unsigned long pc = 0;

//DEFINE REGISTERS
unsigned long reg[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//tool used for debugging
void printInBinary(uint32_t num, int bit){
    if ( bit >= 8*sizeof(num) )
    {
        return;
    }

    printInBinary(num/2, bit+1);
    printf("%u", num%2);

    if ( bit%8 == 0 )
    {
        printf(" ");
    }
    if ( bit == 0 )
    {
        printf("\n");
    }
}

/*******************************************************************************
 █████╗  ██╗      ██╗   ██╗
██╔══██╗ ██║      ██║   ██║
███████║ ██║      ██║   ██║
██╔══██║ ██║      ██║   ██║
██║  ██║ ███████╗ ╚██████╔╝
╚═╝  ╚═╝ ╚══════╝  ╚═════╝
*******************************************************************************/

/********************J_TYPE_FUNCTIONS*******************/

void jump(unsigned long jumpAddr){
    //printf("jump\n");
    pc = pc & 0xf0000000;
    pc = (pc >> 2) | (jumpAddr);
}

void jumpAligned(unsigned long jumpAddr){
    //printf("jal\n");
    reg[ra] = pc + 2;
    pc = pc & 0xf0000000;
    pc = (pc >> 2) | (jumpAddr);
}

/********************I_TYPE_FUNCTIONS*******************/

void branchEqual(unsigned long rs, unsigned long rt, short int immed){
    //printf("beq\n");
    if(rs == rt){
        pc = pc + immed;
    } else {
        return;
    }
    return;
}


void branchNotEqual(unsigned long rs, unsigned long rt, short int immed){
    //printf("bne\n");
    //printf("rs: %lu\n",rs);
    //printf("rt: %lu\n",rt);
    //printf("immed: %d\n",immed);
    if(rs != rt){
        pc = pc + immed;
    } else {
        return;
    }
    //printf("pc: %lu\n", pc);
    return;
}

unsigned long addImmed(unsigned int rs, short int immed){
    //printf("addi\n");
    unsigned long result;
    result = rs + immed;
    return result;
}

unsigned long addImmediateUnsigned(unsigned int rs, short int immed){
    //printf("addiu\n");
    //printf("rs: %u\n",rs);
    //printf("immed: %d\n",immed);
    unsigned long result;
    result = rs + immed;
    //printf("result: %d\n",result);

    return result;
}

unsigned long setLessThanImmed(unsigned long rs, short int immed){
    //printf("slti\n");
    unsigned long result;
    result = (rs < immed)? 1 : 0;
    return result;
}

unsigned long setLessThanImmedUnsigned(unsigned int rs, short int immed){
    //printf("sltiu\n");
    unsigned short int uImmed = immed;
    unsigned long result;
    result = (rs < uImmed)? 1 : 0;
    return result;
}

unsigned long andImmed(unsigned int rs, short int immed){
    //printf("andi\n");
    unsigned short int uImmed = immed;
    unsigned long result;
    result = rs & (uint32_t)uImmed;
    return result;
}

unsigned long orImmed(unsigned int rs, short int immed){
    //printf("ori\n");
    unsigned short int uImmed = immed;
    unsigned long result;
    result = rs | (uint32_t)uImmed;
    return result;
}

unsigned long xorImmed(unsigned int rs, short int immed){
    //printf("xori\n");
    unsigned short int uImmed = immed;
    unsigned long result;
    result = rs ^ (uint32_t)uImmed;
    return result;
}

unsigned long loadUpperImmed(short int immed){
    //printf("lui\n");
    unsigned short int uImmed = immed;
    unsigned long result;
    result = (uImmed << 16);
    return result;
}

unsigned long loadWord(unsigned int rs, short int immed){
    //printf("lw\n");
    //printf("rs: %u\n",rs);
    //printf("immed: %d\n",immed);
    unsigned long result;
    result = memory[rs + immed];
    //printf("result: %d\n",result);
    return result;
}

/*
unsigned long loadByte(unsigned int rs, short int immed){

    unsigned long result;

    return result;
}

unsigned long loadHalfWord(unsigned int rs, short int immed){

    unsigned long result;

    return result;
}

unsigned long loadByteUnsigned(unsigned int rs, short int immed){
    unsigned long result;

    return result;
}

unsigned long loadHalfWordUnsigned(unsigned int rs, short int immed){
    unsigned long result;

    return result;
}

void storeByte(unsigned int rs, short int immed){
    unsigned long result;

    return;
}

void storeHalfWord(unsigned int rs, short int immed){
    unsigned long result;

    return;
}
*/

unsigned long storeWord(unsigned int rs, short int immed){
    //printf("sw\n");
    //printf("rs: %u\n",rs);
    //printf("immed: %d\n",immed);
    unsigned long result;
    result = rs + immed;
    return result;
}

void branchLessThanZero(long rs, short int immed){
    //printf("bltz\n");
    if(rs < 0){
        pc = pc + immed;
    } else {
        return;
    }
    return;
}

void branchGreaterThanZero(unsigned int rs, short int immed){
    //printf("bgtz\n");
    if(rs > 0){
        pc = pc + immed;
    } else {
        return;
    }
    return;
}

void branchLessThanEqualZero(unsigned int rs, short int immed){
    //printf("blez\n");
    if(rs <= 0){
        pc = pc + immed;
    } else {
        return;
    }
    return;
}


/********************R_TYPE_FUNCTIONS*******************/

unsigned long shiftLeftLogical(unsigned int rt, unsigned int shamt){
    //printf("sll\n");
    unsigned long result;
    result = rt << shamt;
    return result;
}

unsigned long shiftRightLogical(unsigned int rt, unsigned int shamt){
    //printf("srl\n");
    unsigned long result;
    result = rt >> shamt;
    return result;
}

//DONT FORGET TO TEST IF RS NEED TO BE SHIFTED RIGHT 2 TO ACCOUNT FOR WORD memory
void jumpRegister(unsigned long rs){
    //printf("jr\n");
    pc = rs;
    pc--;
    return;
}


unsigned long addition(unsigned int rs, unsigned int rt){
    //printf("add\n");
    unsigned long result;
    result = rs + rt;
    return result;
}

unsigned long addUnsigned(unsigned int rs, unsigned int rt){
    //printf("addu\n");
    unsigned long result;
    result = rs + rt;
    return result;
}

unsigned long subtract(unsigned int rs, unsigned int rt){
    //printf("sub\n");
    unsigned long result;
    result = rs - rt;
    return result;
}

unsigned long subtractUnsigned(unsigned int rs, unsigned int rt){
    //printf("subu\n");
    unsigned long result;
    result = rs - rt;
    return result;
}

//cant use just the word and() because it gets mad about the predefined variable and
unsigned long andOperation(unsigned int rs, unsigned int rt){
    //printf("and\n");
    unsigned long result;
    result = rs & rt;
    return result;
}

unsigned long orOperation(unsigned int rs, unsigned int rt){
    //printf("or\n");
    unsigned long result;
    result = rs | rt;
    return result;
}

unsigned long xorOperation(unsigned int rs, unsigned int rt){
    //printf("xor\n");
    unsigned long result;
    result = rs ^ rt;
    return result;
}

unsigned long norOperation(unsigned int rs, unsigned int rt){
    //printf("nor\n");
    unsigned long result;
    result = !(rs | rt);
    return result;
}

unsigned long setLessThan(unsigned int rs, unsigned int rt){
    //printf("slt\n");
    unsigned long result;
    result = (rs < rt)? 1:0;
    return result;
}

unsigned long setLessThanUnsigned(unsigned int rs, unsigned int rt){
    //printf("sltu\n");
    unsigned long result;
    result = (rs < rt)? 1:0;
    return result;
}

unsigned long moveNonZero(unsigned int rs, unsigned int rt, unsigned int rd){
    //printf("movn\n");
    unsigned long result;
    if(rt != 0){
        result = rs;
    } else {
        result = rd;
    }
    return result;
}

unsigned long moveZero(unsigned int rs, unsigned int rt, unsigned int rd){
    //printf("movz\n");
    unsigned long result;
    if(rt == 0){
        result = rs;
    } else {
        result = rd;
    }
    return result;
}

/*******************************************************************************
██╗  ██╗ ███████╗ ██╗      ██████╗  ███████╗ ██████╗  ███████╗
██║  ██║ ██╔════╝ ██║      ██╔══██╗ ██╔════╝ ██╔══██╗ ██╔════╝
███████║ █████╗   ██║      ██████╔╝ █████╗   ██████╔╝ ███████╗
██╔══██║ ██╔══╝   ██║      ██╔═══╝  ██╔══╝   ██╔══██╗ ╚════██║
██║  ██║ ███████╗ ███████╗ ██║      ███████╗ ██║  ██║ ███████║
╚═╝  ╚═╝ ╚══════╝ ╚══════╝ ╚═╝      ╚══════╝ ╚═╝  ╚═╝ ╚══════╝
*******************************************************************************/

//sections up r type data
void rtype(unsigned long rtypeData){
    //printf("rtype\n");
    //bit masking and shifting for each part of the r type
    IFID.Opcode = (rtypeData & 0xFC000000) >> 26;
    IFID.Rs = (rtypeData & 0x03E00000) >> 21;
    IFID.Rt = (rtypeData & 0x001F0000) >> 16;
    IFID.Rd = (rtypeData & 0x0000F800) >> 11;
    IFID.shamt = (rtypeData & 0x000007C0) >> 6;
    IFID.funct = (rtypeData & 0x0000003F) >> 0;
    return;
}

//sections up i type data
void itype(unsigned long itypeData){
    //printf("itype\n");
    //bit masking and shifting for each part of the i type
    IFID.Opcode = (itypeData & 0xFC000000) >> 26;
    IFID.Rs = (itypeData & 0x03E00000) >> 21;
    IFID.Rt = (itypeData & 0x001F0000) >> 16;
    IFID.immediate = (itypeData & 0x0000FFFF) >> 0;
    return;
}

//sections up j type data
void jtype(unsigned long jtypeData){
    //printf("jtype\n");
    //bit masking and shifting for each part of the i type
    IFID.Opcode = (jtypeData & 0xFC000000) >> 26;
    IFID.jumpaddress = (jtypeData & 0x03FFFFFF) >> 0;
    return;
}

//determine where to send the data based on the opcode
void typeSelect(unsigned long machCode){
    IFID.Opcode = (machCode & 0xFC000000) >> 26;

    switch(IFID.Opcode) {
        //every r type has a 0 for the opcode
        case 0x0000 :
            rtype(machCode);
            IFID.type = 3;
            break;
        //only two functions use j type
        case 0x0002 : //jump
            jtype(machCode);
            IFID.type = 1;
            break;
        case 0x0003 : //jump and link
            jtype(machCode);
            IFID.type = 1;
            break;
        default :   //everything else will be an immidiate
            itype(machCode);
            IFID.type = 2;
            break;
    }

    return;
}

//determine rs value
void rsRegDetermination(unsigned int rs){
    IDEX.Rs = IFID.Rs;
    IDEX.RsValue = reg[rs];
}

//determine rt values
void rtRegDetermination(unsigned int rt){
    IDEX.Rt = IFID.Rt;
    IDEX.RtValue = reg[rt];
}

void destinationReg(unsigned int type, bool regWrite, bool memtoreg, bool memwrite){
    //i type = 2
    //r type = 3

    if((type == 2) && (regWrite)){
        EXMEM.WBRegister = EXMEM.Rt;
    } else if((type == 3) && (regWrite)){
        EXMEM.WBRegister = EXMEM.Rd;
    } else if((type == 2) && (memtoreg)){
        EXMEM.WBRegister = EXMEM.Rt;
    } else if((type == 2) && (memwrite)){
        EXMEM.WBRegister = EXMEM.Rt;
    } else {
        return;
    }
    return;
}

//takes the information on I and R type instructions and figures out what its supposed to do
void executeDetermination(){
    //printf("executeDetermination\n");
    //R Type
    if(IFID.type == 3){
        //decide what to do with r type inst based on its funct
        switch(IFID.funct){
            //add
            case add :
                IDEX.regWrite = true;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.Rd = IFID.Rd;
                IDEX.shamt = IFID.shamt;
                IDEX.ALUop = add;

                break;
            //add unsigned
            case addu :
                IDEX.regWrite = true;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.Rd = IFID.Rd;
                IDEX.shamt = IFID.shamt;
                IDEX.ALUop = addu;

                break;
            //and
            case and :
                IDEX.regWrite = true;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.Rd = IFID.Rd;
                IDEX.shamt = IFID.shamt;
                IDEX.ALUop = and;

                break;
            //jr jump register
            case jr :
                IDEX.regWrite = false;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                IDEX.Rd = IFID.Rd;
                IDEX.shamt = IFID.shamt;
                IDEX.ALUop = jr;
                break;

            //nor
            case nor :
                IDEX.regWrite = true;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.Rd = IFID.Rd;
                IDEX.shamt = IFID.shamt;
                IDEX.ALUop = nor;

                break;
            //or
            case or :
                IDEX.regWrite = true;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.Rd = IFID.Rd;
                IDEX.shamt = IFID.shamt;
                IDEX.ALUop = or;

                break;
            //slt - set less than
            case slt :
                IDEX.regWrite = true;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.Rd = IFID.Rd;
                IDEX.shamt = IFID.shamt;
                IDEX.ALUop = slt;

                break;
            //sltu - set less than unsigned
            case sltu :
                IDEX.regWrite = true;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.Rd = IFID.Rd;
                IDEX.shamt = IFID.shamt;
                IDEX.ALUop = sltu;

                break;
            //sll - shift left logical
            case sll :
                IDEX.regWrite = true;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.Rd = IFID.Rd;
                IDEX.shamt = IFID.shamt;
                IDEX.ALUop = sll;


                break;

            //srl - shift right logical
            case srl :
                IDEX.regWrite = true;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.Rd = IFID.Rd;
                IDEX.shamt = IFID.shamt;
                IDEX.ALUop = srl;


                break;

            //sub - subtract
            case sub :
                IDEX.regWrite = true;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.Rd = IFID.Rd;
                IDEX.shamt = IFID.shamt;
                IDEX.ALUop = sub;

                break;

            //subu - subtract unsigned
            case subu :
                IDEX.regWrite = true;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.Rd = IFID.Rd;
                IDEX.shamt = IFID.shamt;
                IDEX.ALUop = subu;

                break;

            //movn - move conditional on not zero
            case movn :
                IDEX.regWrite = true;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.Rd = IFID.Rd;
                IDEX.shamt = IFID.shamt;
                IDEX.ALUop = movn;

                break;

            //movz - move conditional on zero
            case movz :
                IDEX.regWrite = true;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.Rd = IFID.Rd;
                IDEX.shamt = IFID.shamt;
                IDEX.ALUop = movz;

                break;

        }

    }
    //J Type
    else if(IFID.type == 1){
        //decide what to do with the J type inst based on its opcode
        switch(IFID.Opcode){
            //jump
            case j :
                IDEX.regWrite = false;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                IDEX.jumpaddress = IFID.jumpaddress;
                IDEX.ALUop = j;
                break;

            //jump aligned
            case jal :
                IDEX.regWrite = false;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                IDEX.jumpaddress = IFID.jumpaddress;
                IDEX.ALUop = jal;
                break;

        }

    }
    //I Type
    else if(IFID.type == 2){
        //decide what to do with the I type inst based on its opcode
        switch(IFID.Opcode){
            //beq
            case beq :
                IDEX.regWrite = false;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.ALUop = beq;
                IDEX.immediate = IFID.immediate;

                break;
            //bne
            case bne :
                IDEX.regWrite = false;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.ALUop = bne;
                IDEX.immediate = IFID.immediate;

                break;
            //addi
            case addi :
                IDEX.regWrite = true;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.ALUop = addi;
                IDEX.immediate = IFID.immediate;

                break;
            //addiu
            case addiu :
                IDEX.regWrite = true;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.ALUop = addiu;
                IDEX.immediate = IFID.immediate;

                break;
            //slti
            case slti :
                IDEX.regWrite = true;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.ALUop = slti;
                IDEX.immediate = IFID.immediate;

                break;
                //sltiu
            case sltiu :
                IDEX.regWrite = true;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.ALUop = sltiu;
                IDEX.immediate = IFID.immediate;

                break;
                //andi
            case andi :
                IDEX.regWrite = true;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.ALUop = andi;
                IDEX.immediate = IFID.immediate;

                break;
            //ori
            case ori :
                IDEX.regWrite = true;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.ALUop = ori;
                IDEX.immediate = IFID.immediate;

                break;
            //xori
            case xori :
                IDEX.regWrite = true;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.ALUop = xori;
                IDEX.immediate = IFID.immediate;

                break;
            //lui
            case lui :
                IDEX.regWrite = true;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.ALUop = lui;
                IDEX.immediate = IFID.immediate;

                break;
            //lb
            case lb :
                IDEX.regWrite = false;
                IDEX.memread = true;
                IDEX.memtoreg = true;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.ALUop = lb;
                IDEX.immediate = IFID.immediate;

                break;
            //lh
            case lh :
                IDEX.regWrite = false;
                IDEX.memread = true;
                IDEX.memtoreg = true;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.ALUop = lh;
                IDEX.immediate = IFID.immediate;

                break;
            //lw
            case lw :
                IDEX.regWrite = false;
                IDEX.memread = true;
                IDEX.memtoreg = true;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.ALUop = lw;
                IDEX.immediate = IFID.immediate;

                break;
            //lbu
            case lbu :
                IDEX.regWrite = false;
                IDEX.memread = true;
                IDEX.memtoreg = true;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.ALUop = lbu;
                IDEX.immediate = IFID.immediate;

                break;
            //lhu
            case lhu :
                IDEX.regWrite = false;
                IDEX.memread = true;
                IDEX.memtoreg = true;
                IDEX.memwrite = false;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.ALUop = lhu;
                IDEX.immediate = IFID.immediate;

                break;
            //sb
            case sb :
                IDEX.regWrite = false;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = true;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.ALUop = sb;
                IDEX.immediate = IFID.immediate;

                break;
            //sh
            case sh :
                IDEX.regWrite = false;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = true;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.ALUop = sh;
                IDEX.immediate = IFID.immediate;

                break;

            //sw
            case sw :
                IDEX.regWrite = false;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = true;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.ALUop = sw;
                IDEX.immediate = IFID.immediate;

                break;

            //bgtz
            case bgtz :
                IDEX.regWrite = false;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = true;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.ALUop = bgtz;
                IDEX.immediate = IFID.immediate;

                break;

            //bltz
            case bltz :
                IDEX.regWrite = false;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = true;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.ALUop = bltz;
                IDEX.immediate = IFID.immediate;

                break;

            //blez
            case blez :
                IDEX.regWrite = false;
                IDEX.memread = false;
                IDEX.memtoreg = false;
                IDEX.memwrite = true;
                rsRegDetermination(IFID.Rs);
                rtRegDetermination(IFID.Rt);
                IDEX.ALUop = blez;
                IDEX.immediate = IFID.immediate;

                break;

        }
    }

    return;
}

void execute(){
    //printf("execute\n");

    //switch on type
    switch(EXMEM.type){
        //j type instuction set
        case 1:

            //determine the operation
            switch(EXMEM.ALUop){

                case j :
                    jump(EXMEM.jumpaddress);
                    break;

                case jal:
                    jumpAligned(EXMEM.jumpaddress);
                    break;
            }
            break;

        //i type
        case 2:

            //determine the operation
            switch(EXMEM.ALUop){

                case beq:
                    branchEqual(EXMEM.RsValue, EXMEM.RtValue, EXMEM.immediate);
                    break;

                case bne:
                    branchNotEqual(EXMEM.RsValue, EXMEM.RtValue, EXMEM.immediate);
                    break;

                case addi:
                    EXMEM.ALUresult = addImmed(EXMEM.RsValue, EXMEM.immediate);
                    break;

                case addiu:
                    EXMEM.ALUresult = addImmediateUnsigned(EXMEM.RsValue, EXMEM.immediate);
                    break;

                case slti:
                    EXMEM.ALUresult = setLessThanImmed(EXMEM.RsValue, EXMEM.immediate);
                    break;

                case sltiu:
                    EXMEM.ALUresult = setLessThanImmedUnsigned(EXMEM.RsValue, EXMEM.immediate);
                    break;

                case andi:
                    EXMEM.ALUresult = andImmed(EXMEM.RsValue, EXMEM.immediate);
                    break;

                case ori:
                    EXMEM.ALUresult = orImmed(EXMEM.RsValue, EXMEM.immediate);
                    break;

                case xori:
                    EXMEM.ALUresult = xorImmed(EXMEM.RsValue, EXMEM.immediate);
                    break;

                case lui:
                    EXMEM.ALUresult = loadUpperImmed(EXMEM.immediate);
                    break;

                case lw:
                    EXMEM.ALUresult = loadWord(EXMEM.RsValue, EXMEM.immediate);
                    break;

                case sw:
                    EXMEM.DataMemResult = storeWord(EXMEM.RsValue, EXMEM.immediate);
                    break;

                case bltz:
                    branchLessThanZero(EXMEM.RsValue, EXMEM.immediate);
                    break;

                case bgtz:
                    branchGreaterThanZero(EXMEM.RsValue, EXMEM.immediate);
                    break;

                case blez:
                    branchLessThanEqualZero(EXMEM.RsValue, EXMEM.immediate);
                    break;

            }
            break;


        //r type
        case 3:

            //determine the operation
            switch(EXMEM.ALUop){
                //shift left logical
                case sll :
                    EXMEM.ALUresult = shiftLeftLogical(EXMEM.RtValue, EXMEM.shamt);
                    break;

                //shift right logical
                case srl :
                    EXMEM.ALUresult = shiftRightLogical(EXMEM.RtValue, EXMEM.shamt);
                    break;

                //jr
                case jr :
                    jumpRegister(EXMEM.RsValue);
                    break;

                //add
                case add :
                    EXMEM.ALUresult = addition(EXMEM.RsValue, EXMEM.RtValue);
                    break;

                //add unsigned
                case addu :
                    EXMEM.ALUresult = addUnsigned(EXMEM.RsValue, EXMEM.RtValue);
                    break;

                //subtract
                case sub :
                    EXMEM.ALUresult = subtract(EXMEM.RsValue, EXMEM.RtValue);
                    break;

                //subtract unsigned
                case subu :
                    EXMEM.ALUresult = subtractUnsigned(EXMEM.RsValue, EXMEM.RtValue);
                    break;

                //and
                case and :
                    EXMEM.ALUresult = andOperation(EXMEM.RsValue, EXMEM.RtValue);
                    break;

                //or
                case or :
                    EXMEM.ALUresult = orOperation(EXMEM.RsValue, EXMEM.RtValue);
                    break;

                //xor
                case xor :
                    EXMEM.ALUresult = xorOperation(EXMEM.RsValue, EXMEM.RtValue);
                    break;

                //nor
                case nor :
                    EXMEM.ALUresult = norOperation(EXMEM.RsValue, EXMEM.RtValue);
                    break;

                //set less than
                case slt :
                    EXMEM.ALUresult = setLessThan(EXMEM.RsValue, EXMEM.RtValue);
                    break;

                //set less than unsigned
                case sltu :
                    EXMEM.ALUresult = setLessThanUnsigned(EXMEM.RsValue, EXMEM.RtValue);
                    break;

                //move non zero
                case movn :
                    EXMEM.ALUresult = moveNonZero(EXMEM.RsValue, EXMEM.RtValue, EXMEM.Rd);
                    break;

                //move zero
                case movz :
                    EXMEM.ALUresult = moveZero(EXMEM.RsValue, EXMEM.RtValue, EXMEM.Rd);
                    break;
            }
            break;

    }

    return;
}

void memoryHelp(){
    //printf("memoryHelp\n");

    if(MEMWB.regWrite){
        reg[MEMWB.WBRegister] = MEMWB.ALUresult;
    } else if(MEMWB.memtoreg){        //lb,lh,lw,lbu,lhu,lwu
        reg[MEMWB.WBRegister] = MEMWB.ALUresult;
    } else if(MEMWB.memwrite){
        memory[MEMWB.DataMemResult] = reg[MEMWB.WBRegister];
        printf("DataMemResult: %ld\n",MEMWB.DataMemResult);
        printf("WBRegister: %lu\n",MEMWB.WBRegister);
        printf("Memory: %u\n",memory[MEMWB.DataMemResult]);
        printf("reg: %lu\n",reg[MEMWB.WBRegister]);
    }
    return;
}

/*******************************************************************************
██████╗  ██╗ ██████╗  ███████╗ ██╗      ██╗ ███╗   ██╗ ███████╗
██╔══██╗ ██║ ██╔══██╗ ██╔════╝ ██║      ██║ ████╗  ██║ ██╔════╝
██████╔╝ ██║ ██████╔╝ █████╗   ██║      ██║ ██╔██╗ ██║ █████╗
██╔═══╝  ██║ ██╔═══╝  ██╔══╝   ██║      ██║ ██║╚██╗██║ ██╔══╝
██║      ██║ ██║      ███████╗ ███████╗ ██║ ██║ ╚████║ ███████╗
╚═╝      ╚═╝ ╚═╝      ╚══════╝ ╚══════╝ ╚═╝ ╚═╝  ╚═══╝ ╚══════╝
*******************************************************************************/

void IF(unsigned long machCode){
    //find data in memory

    //find out instruction type and decode that type
    typeSelect(machCode);

    //HAND OFF
    IDEX.type = IFID.type;
    return;
}

void ID(){
    //find out what we're supposed to do with the instruction
    executeDetermination();

    //HAND OFF
    EXMEM.regWrite = IDEX.regWrite;
    EXMEM.memread = IDEX.memread;
    EXMEM.memtoreg = IDEX.memtoreg;
    EXMEM.memwrite = IDEX.memwrite;
    EXMEM.ALUop = IDEX.ALUop;
    EXMEM.shamt = IDEX.shamt;
    EXMEM.Rd = IDEX.Rd;
    EXMEM.Rt = IDEX.Rt;
    EXMEM.RsValue = IDEX.RsValue;
    EXMEM.RtValue = IDEX.RtValue;
    EXMEM.jumpaddress = IDEX.jumpaddress;
    EXMEM.immediate = IDEX.immediate;
    EXMEM.PCinc = IDEX.PCinc;
    EXMEM.type = IDEX.type;

    //determine destinationReg
    destinationReg(EXMEM.type, EXMEM.regWrite, EXMEM.memtoreg, EXMEM.memwrite);
    return;
}

void EX(){
    //do the ALU operation
    execute();
    //HAND OFF
    MEMWB.regWrite = EXMEM.regWrite;
    MEMWB.memtoreg = EXMEM.memtoreg;
    MEMWB.memwrite = EXMEM.memwrite;
    MEMWB.DataMemResult = EXMEM.DataMemResult;
    MEMWB.ALUresult = EXMEM.ALUresult;
    MEMWB.WBRegister = EXMEM.WBRegister;
    MEMWB.type = EXMEM.type;
    return;
}

void MEM(){
    memoryHelp();

    return;
}

void WB(){
    if(MEMWB.type != 1){
        pc++;
    } else {
        return;
    }
    return;
}

/******************************************************************************
███╗   ███╗  █████╗  ██╗ ███╗   ██╗
████╗ ████║ ██╔══██╗ ██║ ████╗  ██║
██╔████╔██║ ███████║ ██║ ██╔██╗ ██║
██║╚██╔╝██║ ██╔══██║ ██║ ██║╚██╗██║
██║ ╚═╝ ██║ ██║  ██║ ██║ ██║ ╚████║
╚═╝     ╚═╝ ╚═╝  ╚═╝ ╚═╝ ╚═╝  ╚═══╝
******************************************************************************/


int main(){
    //copy over memory
    Initialize_Simulation_Memory();
    //initialize important regisers
    reg[sp] = memory[0];
    reg[fp] = memory[1];
    pc = memory[5];
    pc--;

    //below this is all used for debugging
    printf("%04x\n\n", memory[pc]);

    char option = '\0';
    while(pc != 0){

        printf("Current INST: %04x\n\n", memory[pc]);

        WB();
        printf("%lu\n",pc);
        IF(memory[pc]);
        ID();
        EX();
        MEM();


        printf("Register at = %lu\n", reg[at]);
        printf("Register v0 = %lu\n", reg[v0]);
        printf("Register v1 = %lu\n", reg[v1]);
        printf("Register a0 = %lu\n", reg[a0]);
        printf("Register a1 = %lu\n", reg[a1]);
        printf("Register a2 = %lu\n", reg[a2]);
        printf("Register a3 = %lu\n", reg[a3]);
        printf("Register t0 = %lu\n", reg[t0]);
        printf("Register t1 = %lu\n", reg[t1]);
        printf("Register t2 = %lu\n", reg[t2]);
        printf("Register t3 = %lu\n", reg[t3]);
        printf("Register t4 = %lu\n", reg[t4]);
        printf("Register t5 = %lu\n", reg[t5]);
        printf("Register t6 = %lu\n", reg[t6]);
        printf("Register t7 = %lu\n", reg[t7]);
        printf("Register s0 = %lu\n", reg[s0]);
        printf("Register s1 = %lu\n", reg[s1]);
        printf("Register s2 = %lu\n", reg[s2]);
        printf("Register s3 = %lu\n", reg[s3]);
        printf("Register s4 = %lu\n", reg[s4]);
        printf("Register s5 = %lu\n", reg[s5]);
        printf("Register s6 = %lu\n", reg[s6]);
        printf("Register s7 = %lu\n", reg[s7]);
        printf("Register t8 = %lu\n", reg[t8]);
        printf("Register t9 = %lu\n", reg[t9]);
        printf("Register k0 = %lu\n", reg[k0]);
        printf("Register k1 = %lu\n", reg[k1]);
        printf("Register gp = %lu\n", reg[gp]);
        printf("Register sp = %lu\n", reg[sp]);
        printf("Register fp = %lu\n", reg[fp]);
        printf("Register ra = %lu\n", reg[ra]);

        if(pc == 180){
            break;
        }

    }

    return 0;
}


/*
int main(){
    //copy over memory
    Initialize_Simulation_Memory();
    //initialize important regisers
    reg[sp] = memory[0];
    reg[fp] = memory[1];
    pc = memory[5];
    pc--;

    //below this is all used for debugging
    //printf("%04x\n\n", memory[pc]);

    char option = '\0';
    while(1){

    //if(option == 'n'){
        //printf("Current INST: %04x\n\n", memory[pc]);

        WB();
        //printf("%lu\n",pc);
        IF(memory[pc]);
        ID();
        EX();
        MEM();

        //printf("Memory 6 = %u\n", memory[6]);
        //printf("Memory 7 = %u\n", memory[7]);
        //printf("Memory 8 = %u\n", memory[8]);
        //printf("Memory 9 = %u\n", memory[9]);

        if(pc == 180){
            break;
        }

    }

    return 0;
}
*/

















//DEBUGGING STUFF



/*
IF(memory[pc]);
printf("\n\n");
printf("type\n");
printInBinary( (uint32_t) IFID.type , 0 );
printf("Opcode\n");
printInBinary( (uint32_t) IFID.Opcode , 0 );
printf("Rs\n");
printInBinary( (uint32_t) IFID.Rs , 0 );
printf("Rt\n");
printInBinary( (uint32_t) IFID.Rt , 0 );
printf("Rd\n");
printInBinary( (uint32_t) IFID.Rd , 0 );
printf("shamt\n");
printInBinary( (uint32_t) IFID.shamt , 0 );
printf("funct\n");
printInBinary( (uint32_t) IFID.funct , 0 );
printf("immediate\n");
printInBinary( (uint32_t) IFID.immediate , 0 );
printf("jumpaddress\n");
printInBinary( (uint32_t) IFID.jumpaddress , 0 );
printf("PCinc\n");
printInBinary( (uint32_t) IFID.PCinc , 0 );
printf("\n\n");

ID();
printf("\n\n");
printf("type\n");
printInBinary( (uint32_t) IDEX.type , 0 );
printf("regWrite\n");
printInBinary( (uint32_t) IDEX.regWrite , 0 );
printf("ALUop\n");
printInBinary( (uint32_t) IDEX.ALUop , 0 );
printf("Rd\n");
printInBinary( (uint32_t) IDEX.Rd , 0 );
printf("Rs\n");
printInBinary( (uint32_t) IDEX.Rs , 0 );
printf("Rt\n");
printInBinary( (uint32_t) IDEX.Rt , 0 );
printf("RsValue\n");
printInBinary( (uint32_t) IDEX.RsValue , 0 );
printf("RtValue\n");
printInBinary( (uint32_t) IDEX.RtValue , 0 );
printf("shamt\n");
printInBinary( (uint32_t) IDEX.shamt , 0 );
printf("immediate\n");
printInBinary( (uint32_t) IDEX.immediate , 0 );
printf("PCinc\n");
printInBinary( (uint32_t) IDEX.PCinc , 0 );
printf("\n\n");

EX();
printf("\n\n");
printf("type\n");
printInBinary( (uint32_t) EXMEM.type , 0 );
printf("regWrite\n");
printInBinary( (uint32_t) EXMEM.regWrite , 0 );
printf("ALUop\n");
printInBinary( (uint32_t) EXMEM.ALUop , 0 );
printf("ALUresult\n");
printInBinary( (uint32_t) EXMEM.ALUresult , 0 );
printf("Rd\n");
printInBinary( (uint32_t) EXMEM.Rd , 0 );
printf("Rt\n");
printInBinary( (uint32_t) EXMEM.Rt , 0 );
printf("Rs Value\n");
printInBinary( (uint32_t) EXMEM.RsValue , 0 );
printf("Rt Value\n");
printInBinary( (uint32_t) EXMEM.RtValue , 0 );
printf("immediate\n");
printInBinary( (uint32_t) EXMEM.immediate , 0 );
printf("PCinc\n");
printInBinary( (uint32_t) EXMEM.PCinc , 0 );
printf("\n\n");

MEM();
printf("\n\n");
printf("regWrite\n");
printInBinary( (uint32_t) MEMWB.regWrite , 0 );
printf("memtoreg\n");
printInBinary( (uint32_t) MEMWB.memtoreg , 0 );
printf("memwrite\n");
printInBinary( (uint32_t) MEMWB.memwrite , 0 );
printf("DataMemResult\n");
printInBinary( (uint32_t) MEMWB.DataMemResult , 0 );
printf("ALUresult\n");
printInBinary( (uint32_t) MEMWB.ALUresult , 0 );
printf("WBRegister\n");
printInBinary( (uint32_t) MEMWB.WBRegister , 0 );
printf("\n\n");

printf("\n\nProgram Counter\n");
printInBinary( (uint32_t) pc, 0);

*/