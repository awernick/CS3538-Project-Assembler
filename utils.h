#ifndef __UTILS__
#define __UTILS__

int   hex2int(char *hex);
char *hex2bin(char *hex);
char *int2bin(int   val);
char *int2bin(int   val, int pad);
char **tokenizer(char *buff, int lsize, int *tcount);

#endif
