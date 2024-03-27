#include "../main_app.h"

int is_string_valid(const char* str_to_check) {
  int res = 1;
  int num_of_dots = 0;
  for (; *str_to_check; str_to_check++) {
    if (*str_to_check < 48 || *str_to_check > 57) {
      if (*str_to_check == '.') {
        num_of_dots += 1;
        if (num_of_dots > 1) {
          res = 0;
          break;
        }
      } else {
        res = 0;
        break;
      }
    }
  }
  return res;
}

credit* process_data(char* str_to_check[3], enum credit_type type) {
  credit* credit_data;
  int is_input_valid = 1;
  for (int i = 0; i < 3; i++) {
    if (!is_string_valid(str_to_check[i]) || strcmp(str_to_check[i], "") == 0) {
      is_input_valid = 0;
      str_to_check[i] = "";
    }
  }
  if (is_input_valid)
    credit_data = calc_credit(str_to_check, type);
  else
    credit_data = NULL;
  return credit_data;
}

int is_date_valid(char* str) {
  int res = 1;
  if (strlen(str) != 10)
    res = 0;
  else {
    for (int i = 0; i < 10 && res; i++) {
      if (i == 2 || i == 5) {
        if (str[i] != '.') res = 0;
      } else if (str[i] > 57 || str[i] < 48)
        res = 0;
    }
  }
  return res;
}

deposit* process_deposit_data(char* inputs[4], enum deposit_type type,
                              enum periodicity dep_periodicity,
                              char*** replanishments, int num_of_replanishments,
                              char*** withdrawals, int num_of_withdrawals) {
  deposit* deposit_data;
  int is_input_valid = 1;
  // Checking input
  for (int i = 0; i < 4; i++) {
    if (!is_string_valid(inputs[i]) || strcmp(inputs[i], "") == 0) {
      is_input_valid = 0;
      inputs[i] = "";
    }
  }
  if (num_of_replanishments) {
    for (int i = 0; i < num_of_replanishments; i++) {
      if (!is_string_valid(replanishments[i][1]) ||
          strcmp(replanishments[i][1], "") == 0) {
        is_input_valid = 0;
        replanishments[i][1] = "";
      }
      if (!is_date_valid(replanishments[i][0])) {
        is_input_valid = 0;
        replanishments[i][0] = "";
      }
    }
  }
  if (num_of_withdrawals) {
    for (int i = 0; i < num_of_withdrawals; i++) {
      if (!is_string_valid(withdrawals[i][1]) ||
          strcmp(withdrawals[i][1], "") == 0) {
        is_input_valid = 0;
        withdrawals[i][1] = "";
      }
      if (!is_date_valid(withdrawals[i][0])) {
        is_input_valid = 0;
        withdrawals[i][0] = "";
      }
    }
  }
  if (is_input_valid)
    deposit_data =
        calc_deposit(inputs, type, dep_periodicity, replanishments,
                     num_of_replanishments, withdrawals, num_of_withdrawals);
  else
    deposit_data = NULL;
  return deposit_data;
}

points* process_plot_data(char* x_min_str, char* x_max_str, char* y_min_str,
                          char* y_max_str, char* expression) {
  points* points_to_calculate = NULL;
  char* ptr[4] = {NULL, NULL, NULL, NULL};
  double x_min = strtod(x_min_str, &ptr[0]);
  double x_max = strtod(x_max_str, &ptr[1]);
  double y_min = strtod(y_min_str, &ptr[2]);
  double y_max = strtod(y_max_str, &ptr[3]);
  if (!strcmp(ptr[0], "") && !strcmp(ptr[1], "") && !strcmp(ptr[2], "") &&
      !strcmp(ptr[3], "") && x_min < x_max && y_min < y_max)
    points_to_calculate = calc_points(x_min, x_max, y_min, y_max, expression);
  return points_to_calculate;
}
