#ifndef __HELPERS__
#define __HELPERS__

#include <stdbool.h>

char *reg2bin(char *reg);
char *abs2bin(char *abs);
char *rel2bin(char *rel);

bool is_register(char *param);
bool is_absolute(char *param);
bool is_relative(char *param);

#endif
