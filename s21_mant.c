#include "s21_decimal.h"

UI get_bit(s21_decimal s21, int order) {
    int number = order / 32;
    UI aimed_bit = 1;
    if (number < 3) {
        UI bit = s21.bits[number];
        aimed_bit = aimed_bit << (order % 32);
        aimed_bit = (bit & aimed_bit) >> (order % 32);
    } else {
        aimed_bit = 0;
        fprintf(stderr, "get_bit: invalid bit number: %d\n", order);
    }
    return aimed_bit;
}

void set_bit(s21_decimal *s21, int order, int set_value) {
    int number = order / 32;
    UI aimed_bit = 1;
    if (number < 3) {
        UI bit = s21->bits[number];
        aimed_bit = aimed_bit << (order % 32);
        switch (set_value) {
            case 0:
                bit = (bit & (~aimed_bit));
            case 1:
                bit = (bit | aimed_bit);
            default:
                fprintf(stderr, "set_bit: invalid set_value: %d\n", set_value);
        }
    } else {
        aimed_bit = 0;
        fprintf(stderr, "set_bit: invalid bit number: %d\n", order);
    }
}