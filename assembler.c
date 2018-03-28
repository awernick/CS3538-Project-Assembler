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


char *assemble(char **tokens, int count) {
  char *line = (char *) malloc(INSTR_SIZE);
  char *params = parse_params(tokens, count);

  if (strcmp(tokens[0], OR_INSTR) == 0) {
    return parse_OR(line, tokens, count);
  } 
  else if (strcmp(tokens[0], AND_INSTR) == 0) {
    return parse_AND(line, tokens, count);
  }
  else if (strcmp(tokens[0], CMP_INSTR) == 0) {
    return parse_CMP(line, tokens, count);
  }
  else if (strcmp(tokens[0], MOV_INSTR) == 0) {
    return assemble_MOV(line, tokens, count);
  }
  else if (strcmp(tokens[0], NOT_INSTR) == 0) {
    return assemble_NOT(line, tokens, count);
  }
  else if (strcmp(tokens[0], XOR_INSTR) == 0) {
    return assemble_XOR(line, tokens, count);
  }
  else if (strcmp(tokens[0], ADDC_INSTR) == 0) {
    return assemble_ADDC(line, tokens, count);
  }
  else if (strcmp(tokens[0], SUBB_INSTR) == 0) {
    return assemble_SUBB(line, tokens, count);
  }
  else {
    return NULL;
  }
}

char *parse_params(char **tokens, int count) {
  int i;
  for(i = 1; i < count; i++) {
    if(is_register(tokens[i])) {
      reg2bin(tokens[i]);
    }
    else if(is_absolute(tokens[i])) {
      abs2bin(tokens[i]);
    }
    else if(is_relative(tokens[i])) {
      rel2bin(tokens[i]);
    }
    else {
      return NULL;
    }
  }
}


char *assemble_OR(char *line, char **tokens, int count) {
  int i;
  char dst; 
  for (i = 0; i < count; i++) {
    tokens[1];
  }
}
