#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int hex2int(char *hex) {
  int i, val = 0, curr, pos, len = strlen(hex);
  for (i = 0; i < len; i++) {
    if (hex[i] >= '0' && hex[i] <= '9') {
      curr = (hex[i] - '0');
    }
    else if(hex[i] >= 'A' & hex[i] <= 'F') {
      curr = (hex[i] - 'A') + 10;
    }
    else {
      return -1;
    }

    // multiplier
    pos = (len - 1) - i;
    if (pos != 0) curr *= (pow(16.0, pos));
    val += curr;
  }

  return val;
}

// int to binary. Output is not padded.
char *int2bin(int val, int pad) {
  int i, j, offset, word_size, bin_size, size;
  char *buff;

  word_size = sizeof(int) * 8;
  bin_size = word_size  -  __builtin_clz(val);
  offset = pad - bin_size;
  size = bin_size + offset;

  buff = (char *) malloc(size + 1);
  buff[size] = '\0';

  for (i = size - 1; i >= 0; i--) {
    buff[i] = (val & 1) + '0';
    val >>= 1;
  }

  for (j = 0; j < offset; j++) {
    buff[j] = '0';
  }

  return buff; 
}

char **tokenizer(char *buff, int size, int *tcount) {
  char *token, *s, **tokens, delim[2] = " ";
  *tcount = 0;

  s = (char *) malloc(size);
  strncpy(s, buff, size);
  strtok(s, "\n");
  tokens = (char **) malloc(sizeof(char *));

  token = strtok(s, delim);
  while (token != NULL) {
    tokens[*tcount] = token;
    tokens = (char **) realloc(tokens, sizeof(char *) * ++(*tcount));
    token = strtok(NULL, delim);
  }

	if(*tcount == 0) {
    return NULL;
	}

  return tokens;
}

void printerr(const char msg[]) {
  fprintf(stderr, "%s", msg);
}
