#include "s21_decimal.h"

UI get_bit_s21(s21_decimal s21, int order) {
  int number = order / 32;
  UI aimed_bit = 1;
  if (number < 4) {
    UI bit = s21.bits[number];
    aimed_bit = aimed_bit << (order % 32);
    aimed_bit = (bit & aimed_bit) >> (order % 32);
  } else {
    aimed_bit = 0;
    fprintf(stderr, "get_bit_s21: invalid bit number: %d\n", order);
  }
  return aimed_bit;
}

void set_bit_s21(s21_decimal *s21, int order, int set_value) {
  int number = order / 32;
  UI aimed_bit = 1;
  if (number < 4) {
    UI bit = s21->bits[number];
    aimed_bit = aimed_bit << (order % 32);
    switch (set_value) {
      case 0:
        bit = (bit & (~aimed_bit));
      case 1:
        bit = (bit | aimed_bit);
      default:
        fprintf(stderr, "set_bit_s21: invalid set_value: %d\n", set_value);
    }
  } else {
    aimed_bit = 0;
    fprintf(stderr, "set_bit_s21: invalid bit number: %d\n", order);
  }
}

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

void set_sign(s21_decimal *s21, int sign) {
  set_bit_big(s21, 127, sign);
}

void change_sign(s21_decimal *s21) {
  set_sign(s21, !get_sign(*s21));
}

//inverse element of multiplication (s21 * (-1) = -s21)
int inverse_s21_of_mul(s21_decimal s21, s21_decimal *result) {
  int status = 0;
  if (result) {
    *result = s21;
    change_sign(result);
  } else {
    status = 1;
  }
  return status;
}

void full_decimal(s21_decimal* s21) {
  for (int i = 0; i < 96; i++) {
    set_bit(s21, i, 1);
  }
}


// void s21_print(); //s21

// void multiply_with_10();

// void normalize();