#include "s21_decimal.h"


void from_s21_to_big_decimal(s21_decimal s21, big_decimal *big) {
    big->exp  = get_exp(s21);
    big->sign = get_sign(s21);
    big->bits[0] = s21.bits[0];
    big->bits[1] = s21.bits[1];
    big->bits[2] = s21.bits[2];
    set_zeros_ones_parameters(big);
}

void from_big_to_s21_decimal(big_decimal big, s21_decimal *s21){
    
}

//void s21_print(); //s21

//void big_print(); //big

//void multiply_with_10();

//void normalize();