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

char *ref2bin(char *ref) {
  int len = strlen(ref) - 2;
  char *hex = (char *) malloc(len);
  strncpy(hex, ++ref, len);
  return imm2bin(hex);
}

int reg2int(char *reg) {
  int i, reg_num, len = strlen(reg);
  for(i = 1; i < len; i++) {
    reg_num = reg[i] - '0';
    reg_num = reg_num << (2 * (len - i)) ;
  }
  return reg_num;
}

int imm2int(char *imm) {
  return hex2int(&imm[1]);
}

int ref2int(char *ref) {
  int len = strlen(ref) - 2;
  char *hex = (char *) malloc(len);
  strncpy(hex, ++ref, len);
  return imm2int(hex);
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
  char *buf = (char *) malloc(len - 1);
  if (param[0] == '[') {
    strncpy(buf, param + 1, len - 2) ;
    if ((is_immediate(buf) || is_register(buf)) && param[len-1] == ']') {
        return true;
    }
  }
  return false;
}

bool is_empty(char *param) {
  return !is_reference(param) && !is_register(param) && !is_immediate(param);
}
