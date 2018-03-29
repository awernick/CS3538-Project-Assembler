#ifndef __UTILS__
#define __UTILS__

int   hex2int(char *hex);
char *int2bin(int val, int pad);

char **tokenizer(char *buff, int lsize, int *tcount);

void printerr(const char *str);

#endif
