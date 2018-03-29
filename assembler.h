#ifndef __ASSEMBLER__
#define __ASSEMBLER__

char *assemble(char *instr, char **params, int pcount);
char **parse_params(char **params, int pcount);

int parse_OR(char **params, int pcount);
int parse_AND(char **params, int pcount);
int parse_CMP(char **params, int pcount);
int parse_JMP(char **params, int pcount);
int parse_MOV(char **params, int pcount);
int parse_NOT(char **params, int pcount);
int parse_XOR(char **params, int pcount);

int parse_ADDC(char **params, int pcount);
int parse_SUBB(char **params, int pcount);

int parse_CJMP(char *instr, char **params, int pcount);


// Instructions
const char  OR_INSTR[] =   "OR";
const char AND_INSTR[] =  "AND";
const char CMP_INSTR[] =  "CMP";
const char JMP_INSTR[] =  "JMP";
const char MOV_INSTR[] =  "MOV";
const char NOT_INSTR[] =  "NOT";
const char NOP_INSTR[] =  "NOP";
const char XOR_INSTR[] =  "XOR";

const char JLO_INSTR[] = "JLO";
const char JHS_INSTR[] = "JHS";
const char JEQ_INSTR[] = "JEQ";
const char JNE_INSTR[] = "JNE";
const char JMI_INSTR[] = "JMI";
const char JPL_INSTR[] = "JPL";

const char ADDC_INSTR[] = "ADDC";
const char SUBB_INSTR[] = "SUBB";


// Opcodes
const int  OR_OPCODES[4] = {0x60, 0x61, 0x62, 0x63};
const int AND_OPCODES[4] = {0x50, 0x51, 0x52, 0x53};
const int CMP_OPCODES[4] = {0x30, 0x31, 0x32, 0x33};
const int JMP_OPCODES[2] = {0xB8, 0xB9};
const int MOV_OPCODES[4] = {0x80, 0x81, 0x82, 0x83};
const int NOT_OPCODES[2] = {0x40, 0x43};
const int XOR_OPCODES[4] = {0x70, 0x71, 0x72, 0x73};

const int ADDC_OPCODES[4] = {0x10, 0x11, 0x12, 0x13};
const int SUBB_OPCODES[4] = {0x20, 0x21, 0x22, 0x23};

const int NOP_OPCODE = 0xE0;
const int JLO_OPCODE = 0xD6;
const int JHS_OPCODE = 0xD7;
const int JEQ_OPCODE = 0xD8;
const int JNE_OPCODE = 0xD9;
const int JMI_OPCODE = 0xDA;
const int JPL_OPCODE = 0xDB;

#endif
