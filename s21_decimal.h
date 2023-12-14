#ifndef S21_DECIMAL
#define S21_DECIMAL

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define EXP_SHIFT 16;
#define BIG_BITS 255;
#define S21_BITS 95;

#define MINUS 0b10000000000000000000000000000000
#define PLUS 0b00000000000000000000000000000000

typedef unsigned int UI;

typedef struct {
    UI bits[4];
} s21_decimal;

typedef struct {
    UI bits[8];
    int exp;
    int sign;
    int left_zeros;
    int first_left_one;
    int first_right_one;
} big_decimal;



////////////////////////////////////////////////////////////////
/*
The functions return the error code:

0 - OK
1 - the number is too large or equal to infinity
2 - the number is too small or equal to negative infinity
3 - division by 0
*/

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

//int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

//int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

//int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);


////////////////////////////////////////////////////////////////
/*
Return value:

0 - FALSE
1 - TRUE
*/

//int s21_is_less(s21_decimal, s21_decimal);

//int s21_is_less_or_equal(s21_decimal, s21_decimal);

//int s21_is_greater(s21_decimal, s21_decimal);

//int s21_is_greater_or_equal(s21_decimal, s21_decimal);

//int s21_is_equal(s21_decimal, s21_decimal);

//int s21_is_not_equal(s21_decimal, s21_decimal);


////////////////////////////////////////////////////////////////
/*
Return value - code error:

0 - OK
1 - convertation error
*/

//int s21_from_int_to_decimal(int src, s21_decimal *dst);

//int s21_from_float_to_decimal(float src, s21_decimal *dst);

//int s21_from_decimal_to_int(s21_decimal src, int *dst);

//int s21_from_decimal_to_float(s21_decimal src, float *dst);


////////////////////////////////////////////////////////////////
/*
Return value - code error:

0 - OK
1 - calculation error
*/

//int s21_floor(s21_decimal value, s21_decimal *result);

//int s21_round(s21_decimal value, s21_decimal *result);

//int s21_truncate(s21_decimal value, s21_decimal *result);

//int s21_negate(s21_decimal value, s21_decimal *result);


////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////

//S21: MANT

UI get_bit_s21(s21_decimal s21, int order); //s21

void set_bit_s21(s21_decimal *s21, int order, int set_value); //s21


//S21: EXP

UI get_exp(s21_decimal s21); //s21

//void set_exp(s21_decimal s21); //s21

UI get_sign(s21_decimal s21); //s21

void set_zeros_ones_parameters(big_decimal *big); //s21


////////////////////////////////////////////////////////////////
//BIG: MANT

//void mant_add(); //big

//void mant_div(); //big

//void mant_mul(); //big

//void man_compare(); //big (for full add)

UI get_bit_big(big_decimal big, int order);

void set_bit_big(big_decimal *big, int order, int set_value);


////////////////////////////////////////////////////////////////
//S21 AND BIG (MAIN)

void from_s21_to_big_decimal(s21_decimal s21, big_decimal *big); //s21->big

void from_big_to_s21_decimal(big_decimal big, s21_decimal *s21); //big->s21

//void s21_print(); //s21

//void big_print(); //big

//void multiply_with_10();

//void normalize();



#endif