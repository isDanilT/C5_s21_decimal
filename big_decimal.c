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

void reset_big_to_zero(big_decimal *big) {
  reset_big_mant_to_zero(big);
  big->exp = 0;
  big->sign = 0;
  big->left_zeros = 0;
  big->first_left_one = 0;
  big->first_right_one = 0;
}

// сдвигаем big_decimal влево по битам. 1 - вылезли за пределы, 0 - ок
int shift_left_big(big_decimal *big, int shift) {
  int status = 0;
  set_zeros_ones_parameters(big);
  if (255 - big->first_left_one < shift) {
    status = 1;
  }
  for (int i = big->first_left_one; i >= 0; i--) {
    // для того, чтобы не выходить за пределы массива
    if ((i + shift) <= 255) {
      set_bit_big(big, i + shift, get_bit_big(*big, i));
    }
  }
  // доставляем нули справа
  for (int i = 0; i < shift; i++) {
    set_bit_big(big, i, 0);
  }
  set_zeros_ones_parameters(big);  // устанавливаем нули слева и позицию первой 1

  return status;
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
  set_zeros_ones_parameters(&big1);
  set_zeros_ones_parameters(big2);
  for (int i = 0; i <= big2->first_left_one; i++) {
    if (!(i)) {
      if (shift_left_big(&big1, 1) == 1) {
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


int mant_compare(big_decimal *big1, big_decimal *big2) {
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


// переводим из биг ту с21 со старшим битом не больше 95
void big_to_s21_decimal_case_95(big_decimal *big, s21_decimal *s21) {
  for (int i = 0; i < 3; i++) {
    s21->bits[i] = big->bits[i];
  }
}










//ДОФИКСИТЬ:

// переводим из бига в s21
int big_to_s21_decimal(big_decimal *big, s21_decimal *s21) {
  int status = 0;
  // сколько нулей слева, если занулеванное, то one_position_left = -1
  set_zeros_ones_parameters(big);
  big_decimal ten = {0};
  ten.bits[0] = 10;
  int diff = 0;
  big_decimal copy_result_big_x =
      *big;  // x
                    // для очень маленьких чисел, если экспонента очень большая
  while (big->exp > 28) {
    //
    if (mant_compare(big, &ten) >= 0) {
      division_with_rest_for10(*big, big);  // y
      diff++;
    } else {
      status = 2;
      break;
    }
  }
  if (diff > 0) bank_round(copy_result_big_x, big, diff);

  diff = 0;
  // если мантиса бига выходит за пределы обычной, то
  if (go_beyond_big_decimal_s21(big)) {
    if (big->exp < 1) {
      status = 1;
    } else {
      copy_result_big_x = *big;
      // делим резалт на 10 до тех пор пока
      while (go_beyond_big_decimal_s21(big) &&
             (big->exp > 0)) {
        division_with_rest_for10(*big, big);
        diff++;
      }
      if (diff > 0) bank_round(copy_result_big_x, big, diff);
    }
  }

  // если все равно выходит за пределы мантисы
  if (go_beyond_big_decimal_s21(big)) status = 1;

  if ((status == 1) && big->sign) status = 2;

  if (!status) {
    if (big->sign) set_sign(s21, 1);
    // если нет переполнения, то запихиваем биг децимал в s21_dec
    big_to_s21_decimal_case_95(big, s21);
    set_exp(s21, big->exp);
  }
  // print_big_decimal(big);
  return status;
}




// приводим big_decimal к одной экспоненте
void normalize_big(big_decimal *big1, big_decimal *big2) {
  int def = big1->exp - big2->exp;
  if (def > 0) {
    multiply_10_mantis_big(big2, def);
    set_zeros_ones_parameters(big2);
  } else if (def < 0) {
    multiply_10_mantis_big(big1, -def);
    set_zeros_ones_parameters(big1);
  }
}