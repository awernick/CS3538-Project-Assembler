#ifndef __HELPERS__
#define __HELPERS__

#include <stdbool.h>

char *reg2bin(char *reg);
char *imm2bin(char *imm);
char *ref2bin(char *ref);

int reg2int(char *reg);
int imm2int(char *imm);
int ref2int(char *ref);

bool is_register(char *param);
bool is_immediate(char *param);
bool is_reference(char *param);
bool is_empty(char *param);
#endif
