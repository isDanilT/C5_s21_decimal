#include "s21_decimal.h"

/*
The functions return the error code:

0 - OK
1 - the number is too large or equal to infinity
2 - the number is too small or equal to negative infinity
3 - division by 0
*/

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);