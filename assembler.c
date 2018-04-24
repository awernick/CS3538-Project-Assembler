#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "assembler.h"
#include "helpers.h"
#include "utils.h"
#include "sizes.h"

int main(int argc, char *argv[]) {
  char **tokens, *instr_line, *buff = NULL;
  FILE *rfile, *wfile;
  int tcount;
  size_t lsize = 0;

  if (argc < 3) {
    printerr("Incorrect number of params.\n");
    print_usage();
    return 1;
  }

  if ((rfile = fopen(argv[1], "r")) ==  NULL) {
    perror("open");
    return 1;
  }

  if ((wfile = fopen(argv[2], "w+")) == NULL ) {
    perror("open");
    return 1;
  }

  while (getline(&buff, &lsize, rfile) > 0) {
    tokens = tokenizer(buff, strlen(buff), &tcount);
    if(tcount < 1) { 
      printerr("Incorrect input: "); 
      printerr(buff); 
      printerr(". Exiting..."); 
      free(buff);
      return 1;
    }
    if((instr_line = assemble(tokens[0], &tokens[1], tcount - 1)) == NULL) {
      printerr("Incorrect instruction. Skipping\n"); 
      continue;
    }
    printf("%s\n", instr_line);
    fprintf(wfile, "%s\n", instr_line);
    free(instr_line);
    free(tokens[0]);
    free(tokens);
  }

  free(buff);
  fclose(wfile);
  fclose(rfile);
  return 0;
}


char *assemble(char *instr, char **params, int pcount) {
  int opcode, param0, param1, buf_size, i;
  char *buf = (char *) malloc(256);

  if (strcmp(instr, OR_INSTR) == 0) {
    opcode = parse_OR(params, pcount);
  } 
  else if (strcmp(instr, AND_INSTR) == 0) {
    opcode = parse_AND(params, pcount);
  }
  else if (strcmp(instr, CMP_INSTR) == 0) {
    opcode = parse_CMP( params, pcount);
  }
  else if (strcmp(instr, JMP_INSTR) == 0) {
    opcode = parse_JMP( params, pcount);
  }
  else if (strcmp(instr, MOV_INSTR) == 0) {
    opcode = parse_MOV(params, pcount);
  }
  else if (strcmp(instr, NOT_INSTR) == 0) {
    opcode = parse_NOT(params, pcount);
  }
  else if (strcmp(instr, XOR_INSTR) == 0) {
    opcode = parse_XOR(params, pcount);
  }
  else if (strcmp(instr, ADDC_INSTR) == 0) {
    opcode = parse_ADDC(params, pcount);
  }
  else if (strcmp(instr, SUBB_INSTR) == 0) {
    opcode = parse_SUBB(params, pcount);
  }
  else if (strcmp(instr, NOP_INSTR) == 0) {
    opcode = NOP_OPCODE;
  }
  else {
    opcode = parse_CJMP(instr, params, pcount);
  }

#ifdef DEBUG
  printf("=================================================\n");
  printf("Instr: %s, Opcode: %02x\n", instr, opcode); 
  printf("%d\n", pcount);
  if(pcount > 0) {
    printf("Param: %s, Register: %d, Immediate: %d, Reference: %d\n", params[0],
        is_register(params[0]), is_immediate(params[0]), is_reference(params[0]));
  }

  if(pcount > 1) {
    printf("Param: %s, Register: %d, Immediate: %d, Reference: %d\n", params[1],
        is_register(params[1]), is_immediate(params[1]), is_reference(params[1]));
  }
  printf("==================================================\n");
#endif

  if(opcode == -1) {
    return NULL;
  }

  if(pcount > 0) {
    param0 = parse_param(params[0]);
    param1 = parse_param(params[1]);

#ifdef DEBUG
    printf("=================================================\n");
    printf("Params: %02x %02x\n", param0, param1);
    printf("Opcode: %x\n", opcode);
    printf("==================================================\n");
#endif

    if(is_register(params[0])) {
      param0 = param0 << 4;
    }
  }

  if(pcount == 0) {
    sprintf(buf, "%02X", opcode);
  }
  // 2 byte instruction e.g MOV R2 R1
  else if(is_register(params[0]) && is_register(params[1])) {
    param0 |= param1;
    sprintf(buf, "%02X %02X", opcode, param0);
  } 
  // 4 byte instructions e.g MOV [$0000] R2
  else if(is_reference(params[0])) {
    int msn = (param0 & 0xFF00) >> 8;
    int lsn = param0 & 0xFF;
    sprintf(buf, "%02X %02X %02X %02X", 
      opcode, msn, lsn, param1);
  }
  // 4 byte instructions e.g MOV R2 [$4000]
  else if(is_reference(params[1])) {
    int msn = (param1 & 0xFF00) >> 8;
    int lsn = param1 & 0xFF ;
    sprintf(buf, "%02X %02X %02X %02X", 
      opcode, param0, msn, lsn);
  }
  else if(is_immediate(params[0])) {
    int msn = (param0 & 0xFF00) >> 8;
    int lsn = param0 & 0xFF ;
    sprintf(buf, "%02X %02X %02X", opcode, msn, lsn);
  }
  // 3 byte instructions
  else {
    if(pcount == 1) {
      sprintf(buf, "%02X %02X", opcode, param0);
    } else {
      sprintf(buf, "%02X %02X %02X", opcode, param0, param1);
    }
  }

  return buf;
}

int parse_param(char *tok_param) {
  int i;

  if(is_register(tok_param)) {
    return reg2int(tok_param);
  }
  else if(is_immediate(tok_param)) {
    return imm2int(tok_param);
  }
  else if(is_reference(tok_param)) {
    return ref2int(tok_param);
  }
  else {
    return 0;
  }
}

int parse_OR(char **params, int pcount) {
  if(pcount != 2) {
    return -1;
  }
  if(is_register(params[0]) && is_register(params[1])) {
    return OR_OPCODES[0];
  }
  else if(is_register(params[0]) && is_immediate(params[1])) {
    return OR_OPCODES[1];
  }
  else if(is_register(params[0]) && is_reference(params[1])) {
    return OR_OPCODES[2];
  }
  else if(is_reference(params[0]) && is_register(params[1])) {
    return OR_OPCODES[4];
  }
  else {
    return -1;
  }
}

int parse_AND(char **params, int pcount) {
  if(pcount != 2) {
    return -1;
  }
  if(is_register(params[0]) && is_register(params[1])) {
    return AND_OPCODES[0];
  }
  else if(is_register(params[0]) && is_immediate(params[1])) {
    return AND_OPCODES[1];
  }
  else if(is_register(params[0]) && is_reference(params[1])) {
    return AND_OPCODES[2];
  }
  else if(is_reference(params[0]) && is_register(params[1])) {
    return AND_OPCODES[3];
  }
  else {
    return -1;
  }
}

int parse_CMP(char **params, int pcount) {
  if(pcount != 2) {
    return -1;
  }
  if(is_register(params[0]) && is_register(params[1])) {
    return CMP_OPCODES[0];
  }
  else if(is_register(params[0]) && is_immediate(params[1])) {
    return CMP_OPCODES[1];
  }
  else if(is_register(params[0]) && is_reference(params[1])) {
    return CMP_OPCODES[2];
  }
  else if(is_reference(params[0]) && is_register(params[1])) {
    return CMP_OPCODES[3];
  }
  else {
    return -1;
  }
}

int parse_JMP(char **params, int pcount) {
  if(pcount != 1) {
    return -1;
  }
  if(is_register(params[0])) {
    return JMP_OPCODES[0];
  }
  else if(is_immediate(params[0])) {
    return JMP_OPCODES[1];
  }
  else {
    return -1;
  }
}

int parse_MOV(char **params, int pcount) {
  if(pcount != 2) {
    return -1;
  }
  if(is_register(params[0]) && is_register(params[1])) {
    return MOV_OPCODES[0];
  }
  else if(is_register(params[0]) && is_immediate(params[1])) {
    return MOV_OPCODES[1];
  }
  else if(is_register(params[0]) && is_reference(params[1])) {
    return MOV_OPCODES[2];
  }
  else if(is_reference(params[0]) && is_register(params[1])) {
    return MOV_OPCODES[3];
  }
  else {
    return -1;
  }
}

int parse_NOT(char **params, int pcount) {
  if(pcount != 1) {
    return -1;
  }
  if(is_register(params[0])) {
    return NOT_OPCODES[0];
  }
  else if(is_reference(params[0])) {
    return NOT_OPCODES[1];
  }
  else {
    return -1;
  }
}

int parse_XOR(char **params, int pcount) {
  if(pcount != 2) {
    return -1;
  }
  if(is_register(params[0]) && is_register(params[1])) {
    return XOR_OPCODES[0];
  }
  else if(is_register(params[0]) && is_immediate(params[1])) {
    return XOR_OPCODES[1];
  }
  else if(is_register(params[0]) && is_reference(params[1])) {
    return XOR_OPCODES[2];
  }
  else if(is_reference(params[0]) && is_register(params[1])) {
    return XOR_OPCODES[3];
  }
  else {
    return -1;
  }
}

int parse_ADDC(char **params, int pcount) {
  if(pcount != 2) {
    return -1;
  }
  if(is_register(params[0]) && is_register(params[1])) {
    return ADDC_OPCODES[0];
  }
  else if(is_register(params[0]) && is_immediate(params[1])) {
    return ADDC_OPCODES[1];
  }
  else if(is_register(params[0]) && is_reference(params[1])) {
    return ADDC_OPCODES[2];
  }
  else if(is_reference(params[0]) && is_register(params[1])) {
    return ADDC_OPCODES[3];
  }
  else {
    return -1;
  }
}

int parse_SUBB(char **params, int pcount) {
  if(pcount != 2) {
    return -1;
  }
  if(is_register(params[0]) && is_register(params[1])) {
    return SUBB_OPCODES[0];
  }
  else if(is_register(params[0]) && is_immediate(params[1])) {
    return SUBB_OPCODES[1];
  }
  else if(is_register(params[0]) && is_reference(params[1])) {
    return SUBB_OPCODES[2];
  }
  else if(is_reference(params[0]) && is_register(params[1])) {
    return SUBB_OPCODES[3];
  }
  else {
    return -1;
  }
}

int parse_CJMP(char *instr, char **params, int pcount) {
  if(pcount != 1) {
    return -1;
  }
  else if(!is_immediate(params[0])) {
    return -1;
  }
  if (strcmp(instr, JLO_INSTR) == 0) {
    return JLO_OPCODE;
  } 
  else if (strcmp(instr, JHS_INSTR) == 0) {
    return JHS_OPCODE;
  }
  else if (strcmp(instr, JEQ_INSTR) == 0) {
    return JEQ_OPCODE;
  }
  else if (strcmp(instr, JNE_INSTR) == 0) {
    return JNE_OPCODE;
  }
  else if (strcmp(instr, JMI_INSTR) == 0) {
    return JMI_OPCODE;
  }
  else if (strcmp(instr, JPL_INSTR) == 0) {
    return JPL_OPCODE;
  }
  else {
    return -1;
  }
}
