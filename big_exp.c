#include "s21_decimal.h"

void set_zeros_ones_parameters(big_decimal *big) {
  int counter = BIG_BITS;
  int flag = 1;
  while (flag) {
    if (get_bit_big(*big, counter) == 1) {
      big->left_zeros = BIG_BITS - counter;
      big->first_left_one = counter;
      flag = 0;
    }
    if (flag != 0) {
      counter--;
    }
    if (counter < 0) {
      big->left_zeros = BIG_BITS;
      big->first_left_one = -1;
      flag = 0;
    }
  }
  flag = 1;
  for (int i = 0; i <= 255 && flag == 1; i++) {
    if (get_bit_big(*big, i) == 1) {
      big->first_right_one = i;
      flag == 0;
    }
  }
}