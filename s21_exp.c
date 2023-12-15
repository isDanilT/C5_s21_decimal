#include "s21_decimal.h"

UI get_exp(s21_decimal s21) {
  UI exp = 0b11111111;
  exp <<= EXP_SHIFT;
  exp = s21.bits[3] & exp;
  exp >>= EXP_SHIFT;
  return exp;
}

void set_exp(s21_decimal *s21, int exp) {
  if (exp < 256 && exp >= 0) {
    for (int i = 0; i < 8; i++) {
      set_bit_s21(s21, i + 16 + 96, exp % 2);
      exp >>= 1;
    }
  } else {
    fprintf(stderr, "set_exp: invalid exponent: %d\n", exp);
  }
}

UI get_sign(s21_decimal s21) { 
  return get_bit(s21, 127); 
}


