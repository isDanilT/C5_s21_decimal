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
  for (int i = 0; i <= 255; i++) {
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
  for (int i = 0; i <= 255; i++) {
    bit_result = get_bit_big(*big1, i) - get_bit_big(*big2, i) - remainder;
    remainder = bit_result < 0;
    bit_result = abs(bit_result);
    set_bit_big(result, i, bit_result);
  }
}


//?????????????????????????????????????????????????
int mant_mul(big_decimal big1, big_decimal *big2, big_decimal *result) {
  int status = 0;
  reset_big_to_zero(result);

  for (int i = 0; i <= big2->first_left_one; i++) {
    if (!(i)) {
      if (left_shift_big(&big1, 1) == 1) {
        status = 1;
        break;
      }
    }
    if (get_bit_big(*big2, i)) {
      if (mant_add(result, &big2, result)) {
        status = 1;
      }
    }
  }

  int is_signs_match = (big1.sign == big2->sign);
  if (!is_signs_match) {
    result->sign =  1;
  }

  result->exp = big1.exp + big2->exp;
  return status;
}

void reset_big_mant_to_zero(big_decimal *big) {
  for (int i = 0; i < 255; i++) {
    set_bit_big(big, i, 0);
  }
}


int man_compare(big_decimal *big1, big_decimal *big2) {
  int result = 0;
  for (int i = 255; i >= 0; i--) {
    int bit1 = get_bit_big(*big1, i);
    int bit2 = get_bit_big(*big2, i);
    if (bit1 != bit2) {
      result = bit1 - bit2;
      break;
    }
  }
  return result;
}