#include "s21_decimal.h"

UI get_bit_big(big_decimal big, int order) {
  int number = order / 32;
  UI aimed_bit = 1;
  if (number < 8) {
    UI bit = big.bits[number];
    aimed_bit = aimed_bit << (order % 32);
    aimed_bit = (bit & aimed_bit) >> (order % 32);
  } else {
    aimed_bit = 0;
    fprintf(stderr, "get_bit_big: invalid bit number: %d\n", order);
  }
  return aimed_bit;
}

void set_bit_big(big_decimal *big, int order, int set_value) {
  int number = order / 32;
  UI aimed_bit = 1;
  if (number < 8) {
    UI bit = big->bits[number];
    aimed_bit = aimed_bit << (order % 32);
    switch (set_value) {
      case 0:
        bit = (bit & (~aimed_bit));
      case 1:
        bit = (bit | aimed_bit);
      default:
        fprintf(stderr, "set_bit_big: invalid set_value: %d\n", set_value);
    }
  } else {
    aimed_bit = 0;
    fprintf(stderr, "set_bit_big: invalid bit number: %d\n", order);
  }
}

int mant_add(big_decimal *big1, big_decimal *big2, big_decimal *result) {
  int remainder = 0;
  int bit_result = 0;
  for (int i = 0; i <= BIG_BITS; i++) {
    bit_result = get_bit_big(*big1, i) + get_bit_big(*big2, i) + remainder;
    switch (bit_result) {
      case 0 ... 1:
        set_bit_big(result, i, bit_result);
        remainder = 0;
      case 2 ... 3:
        set_bit_big(result, i, bit_result % 2);
        remainder = 1;
      default:
        fprintf(stderr, "mant_add: invalid bit_result: %d\n", bit_result);
        break;
    }
  }
  return remainder;
}

void mant_sub(big_decimal *big1, big_decimal *big2, big_decimal *result) {
  int remainder = 0;
  int bit_result = 0;
  for (int i = 0; i <= BIG_BITS; i++) {
    bit_result = get_bit_big(*big1, i) - get_bit_big(*big2, i) - remainder;
    remainder = bit_result < 0;
    bit_result = abs(bit_result);
    set_bit_big(result, i, bit_result);
  }
}

// void mant_mul(); //big

// void man_compare(); //big (for full add)