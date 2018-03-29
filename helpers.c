#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "helpers.h"

char *reg2bin(char *reg) {
  int i, reg_num, len = strlen(reg), buf_size = (2 * len) + 1;
  char *bin, *buf = (char * ) malloc(buf_size);
  for(i = 1; i < len; i++) {
    reg_num = reg[i] - '0';
    bin = int2bin(reg_num, 2);
    strncat(buf, bin, 2);
  }
  // Pad if 8bit register
  if(len == 2) {
    strncat(buf, "00", 2);
  }
  return buf;
}

char *imm2bin(char *imm) {
  int val = hex2int(&imm[1]);
  return int2bin(val, 8);
}

// TODO: Calculate offsets
char *ref2bin(char *ref) {
  int len = strlen(ref) - 2;
  char *hex = (char *) malloc(len);
  strncpy(hex, ++ref, len);
  return imm2bin(hex);
}

bool is_register(char *param) {
  if (param[0] == 'R') {
    if (!isdigit(param[1]) && param[1] != '0') {
      return false;
    } else {
      return true;
    }
  }

  return false;
}

bool is_immediate(char *param) {
  int i, len = strlen(param);
  if (param[0] == '$') {
    for(i = 1; i < len; i++) {
      if((param[i] >= '0' && param[i] <= '9') || 
         (param[i] >= 'A' && param[i] <= 'F')) {
          continue;
      } else {
        return false;
      }
    }
    return true;
  }
  return false;
}

bool is_reference(char *param) {
  int i, len = strlen(param);
  if (param[0] == '[') {
    for (i = 1; i < len - 1; i++) {
      if((param[i] >= '0' && param[i] <= '9') || 
         (param[i] >= 'A' && param[i] <= 'F')) {
          continue;
      } else {
        return false;
      }
    }
    if(param[i] == ']') {
      return true;
    }
  }
  return false;
}
