#ifndef __HELPERS__
#define __HELPERS__

#include <stdbool.h>

char *reg2bin(char *reg);
char *imm2bin(char *imm);
char *ref2bin(char *ref);

bool is_register(char *param);
bool is_immediate(char *param);
bool is_reference(char *param);

#endif
