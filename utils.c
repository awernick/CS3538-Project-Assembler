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
char *int2bin(int val) {
  int i, word_size, num_bits;
  char *buff;
  word_size = sizeof(int) * 8;
  num_bits = word_size  -  __builtin_clz(val);
  buff = (char *) malloc(num_bits + 1);
  buff[num_bits] = '\0';
  for (i = num_bits - 1; i >= 0; i--) {
    buff[i] = (val & 1) + '0';
    val >>= 1;
  }
  return buff; 
}

char *hex2bin(char *hex) {
  int val = hex2int(hex);
  return int2bin(val);
}

char **tokenizer(char *buff, int size, int *tcount) {
  char *token, *s, **tokens, delim[2] = " ";
  *tcount = 0;

  s = (char *) malloc(size);
  strncpy(s, buff, size);
  tokens = (char **) malloc(sizeof(char *));

  token = strtok(s, delim);
  while (token != NULL) {
    tokens[*tcount] = token;
    tokens = (char **) realloc(tokens, sizeof(char *) * ++(*tcount));
    token = strtok(NULL, delim);
  }

  free(s);
  return tokens;
}
