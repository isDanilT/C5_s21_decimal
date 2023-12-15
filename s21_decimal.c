#include "s21_decimal.h"

void from_s21_to_big_decimal(s21_decimal s21, big_decimal *big) {
  big->exp = get_exp(s21);
  big->sign = get_sign(s21);
  big->bits[0] = s21.bits[0];
  big->bits[1] = s21.bits[1];
  big->bits[2] = s21.bits[2];
  set_zeros_ones_parameters(big);
}

void from_big_to_s21_decimal(big_decimal big, s21_decimal *s21) {}

// void s21_print(); //s21

void big_print(big_decimal *big) {
  set_zeros_ones_parameters(big);
  printf("sign: %d,", big->sign);
  printf("exponent: %d,", big->exp);
  printf("first bit: %d,", big->first_left_one);
  printf("count left zeros: %d,", big->left_zeros);
  printf("last bit: %d,", big->first_right_one);
  printf("mantissa:");
  for (int i = 7; i >= 0; i--) {
    for (int j = 31; j >= 0; j--) {
      printf("%d", get_bit_big(*big, i * 32 + j));
    }
  }
}

void reset_big_to_zero(big_decimal *big) {
  reset_big_mant_to_zero(big);
  big->exp = 0;
  big->sign = 0;
  big->left_zeros = 0;
  big->first_left_one = 0;
  big->first_right_one = 0;
}

// void multiply_with_10();

// void normalize();