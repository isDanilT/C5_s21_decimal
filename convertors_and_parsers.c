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

/*
Return value - code error:

0 - OK
1 - convertation error
*/

// int s21_from_int_to_decimal(int src, s21_decimal *dst);

// int s21_from_float_to_decimal(float src, s21_decimal *dst);

// int s21_from_decimal_to_int(s21_decimal src, int *dst);

// int s21_from_decimal_to_float(s21_decimal src, float *dst);