#ifndef __ASSEMBLER__
#define __ASSEMBLER__

#define INSTR_SIZE 16

char  *assemble(char **tokens, int count);
char  *parse_params(char **tokens, int count);
char  *parse_OR(char *line, char **tokens, int count);
char  *parse_AND(char *line, char **tokens, int count);
char  *parse_CMP(char *line, char **tokens, int count);
char  *parse_MOV(char *line, char **tokens, int count);
char  *parse_NOT(char *line, char **tokens, int count);
char  *parse_NOP(char *line, char **tokens, int count);
char  *parse_XOR(char *line, char **tokens, int count);
char  *parse_ADDC(char *line, char **tokens, int count);
char  *parse_SUBB(char *line, char **tokens, int count);

const char   OR_INSTR[] =   "OR";
const char  AND_INSTR[] =  "AND";
const char  CMP_INSTR[] =  "CMP";
const char  MOV_INSTR[] =  "MOV";
const char  NOT_INSTR[] =  "NOT";
const char  NOP_INSTR[] =  "NOP";
const char  XOR_INSTR[] =  "XOR";
const char ADDC_INSTR[] = "ADDC";
const char SUBB_INSTR[] = "SUBB";

// Jumps
const char  JMP_INSTR[] = "JMP";
const char  JLO_INSTR[] = "JLO";
const char  JHS_INSTR[] = "JHS";
const char  JEQ_INSTR[] = "JEQ";
const char  JNE_INSTR[] = "JNE";
const char  JMI_INSTR[] = "JMI";
const char  JPL_INSTR[] = "JPL";

#endif
