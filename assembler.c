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
    perror("Incorrect usage. Exiting...\n");
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
    instr_line = assemble(tokens, tcount);
    fprintf(wfile, "%s\n", instr_line);
    free(buff);
  }

  fclose(wfile);
  fclose(rfile);
  free(buff);
  return 0;
}


char *assemble(char *instr, char **params, int pcount) {
  int opcode, buf_size, i;
  char *buf, *bin_opcode, **bin_params;

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

  if(opcode == -1) {
    return NULL;
  }

  bin_params = parse_params(params, pcount);
  bin_opcode = int2bin(opcode);

  buf_size = strlen(bin_opcode);
  for(i = 0; i < pcount; i++) {
    buf_size += strlen(bin_params[i]);
  }
  buf_size++;

  buf = (char *) malloc(buf_size);
  strcpy(buf, bin_opcode);
  for(i = 0; i < pcount; i++) {
    strcat(buf, bin_params[i]);
  }
  buf[buf_size -1] = '\0';
  return buf;
}

char **parse_params(char **tok_params, int count) {
  int i;
  char **bin_params = (char **) malloc(sizeof(char *) * count);

  for(i = 0; i < count; i++) {
    if(is_register(tok_params[i])) {
      bin_params[i] = reg2bin(tok_params[i]);
    }
    else if(is_immediate(tok_params[i])) {
      bin_params[i] = imm2bin(tok_params[i]);
    }
    else if(is_reference(tok_params[i])) {
      bin_params[i] = ref2bin(tok_params[i]);
    }
    else {
      return NULL;
    }
  }

  return bin_params;
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
    return OR_OPCODES[3];
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
