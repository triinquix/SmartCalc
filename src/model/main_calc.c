#include "../main_app.h"
#include "stack.h"

void append_ouput(char* output, char* str_to_add) {
  strcat(output, str_to_add);
  strcat(output, " ");
}

int char_is_num(char char_to_check) {
  int res = 0;
  if (char_to_check >= 48 && char_to_check <= 57) res = 1;
  return res;
}

char* get_first_num(char* str) {
  //   // Finding length of num_str
  int length = 1;
  int num_of_dots = 0;
  int stop = 0;
  while (*(str + length) && !stop) {
    if (char_is_num(*(str + length)))
      length++;
    else if (*(str + length) == 46) {
      if (num_of_dots == 0) {
        num_of_dots++;
        length++;
      } else
        stop = 1;
    } else
      stop = 1;
  }
  // Copying necessary chars
  char* res_str = (char*)malloc(sizeof(char) * length + 1);
  strncpy(res_str, str, length);
  res_str[length] = '\0';
  return res_str;
}

void num_to_output(char* str, char* output, int* length) {
  char* number = get_first_num(str);
  append_ouput(output, number);
  *length += strlen(number);
  free(number);
}

int is_function(char* str) {
  char* functions[9] = {"cos",  "sin",  "tan", "acos", "asin",
                        "atan", "sqrt", "ln",  "log"};
  int res = 0;
  for (int i = 0; i < 9 && !res; i++) {
    size_t length = strlen(functions[i]);
    if (strncmp(functions[i], str, length) == 0) res = 1;
  }
  return res;
}

void func_to_stack(char* str, char* stack, int* length) {
  char* functions[9] = {"cos",  "sin",  "tan", "acos", "asin",
                        "atan", "sqrt", "ln",  "log"};
  int to_stop = 0;
  for (int i = 0; i < 9 && !to_stop; i++) {
    size_t str_length = strlen(functions[i]);
    if (strncmp(functions[i], str, str_length) == 0) {
      strcat(stack, functions[i]);
      to_stop = 1;
      *length += str_length;
    }
  }
}

int is_operator(char* str) {
  char* operators[6] = {"+", "*", "-", "/", "mod", "^"};
  int res = 0;
  for (int i = 0; i < 6 && !res; i++) {
    size_t length = strlen(operators[i]);
    if (strncmp(str, operators[i], length) == 0) res = 1;
  }
  return res;
}

char* get_last_operator_from_stack(char* stack) {
  char* res_str;
  size_t stack_length = strlen(stack);
  if (stack_length) {
    switch (stack[stack_length - 1]) {
      case '+':
        res_str = "+";
        break;
      case '*':
        res_str = "*";
        break;
      case '(':
        res_str = "(";
        break;
      case '-':
        res_str = "-";
        break;
      case '/':
        res_str = "/";
        break;
      case '^':
        res_str = "^";
        break;
      case 'd':
        if (stack[stack_length - 2] && stack[stack_length - 3] &&
            stack[stack_length - 2] == 'o' && stack[stack_length - 3] == 'm')
          res_str = "mod";
        else
          res_str = NULL;
        break;
      default:
        res_str = NULL;
        break;
    }
  } else
    res_str = NULL;
  return res_str;
}

int get_precedence(char* operator) {
  int res = 0;
  switch (operator[0]) {
    case '^':
      res = 2;
      break;
    case '*':
    case '/':
    case 'm':
      res = 3;
      break;
    case '+':
    case '-':
      res = 4;
      break;
    case '(':
      res = 1;
      break;
  }
  return res;
}

char* get_operator_from_str(char* str) {
  char* operators[6] = {"+", "*", "-", "/", "mod", "^"};
  char* res_str = NULL;
  for (int i = 0; i < 6 && !res_str; i++) {
    size_t str_length = strlen(operators[i]);
    if (strncmp(operators[i], str, str_length) == 0) {
      res_str = (char*)malloc(sizeof(char) * str_length + 1);
      strcpy(res_str, operators[i]);
      res_str[str_length] = '\0';
    }
  }
  return res_str;
}

void pop_from_stack_to_output(char* output, char* stack, char* last_operator) {
  append_ouput(output, last_operator);
  size_t stack_length = strlen(stack);
  size_t oper_length = strlen(last_operator);
  for (size_t i = 0; i < oper_length; i++) stack[stack_length - i - 1] = '\0';
}

char* get_last_function_from_stack(char* stack) {
  char* res_str = NULL;
  char* functions[9] = {"acos", "asin", "atan", "sqrt", "ln",
                        "log",  "cos",  "sin",  "tan"};
  size_t stack_length = strlen(stack);
  for (int i = 0; i < 9 && !res_str; i++) {
    size_t func_length = strlen(functions[i]);
    if (stack_length >= func_length)
      if (strcmp(functions[i], stack + stack_length - func_length) == 0)
        res_str = functions[i];
  }
  return res_str;
}

void handle_operator(char* str, char* stack, int* length, char* output) {
  char* operator_from_str = get_operator_from_str(str);
  while (1) {
    char* last_operator = get_last_operator_from_stack(stack);
    if (last_operator && strcmp(last_operator, "(")) {
      int last_oper_prec = get_precedence(last_operator);
      int current_oper_str = get_precedence(operator_from_str);
      if (last_oper_prec < current_oper_str ||
          (last_oper_prec == current_oper_str && strcmp(last_operator, "^")))
        pop_from_stack_to_output(output, stack, last_operator);
      else
        break;
    } else
      break;
  }
  strcat(stack, operator_from_str);
  *length += strlen(operator_from_str);
  free(operator_from_str);
}

int handle_brackets(char* stack, char* output) {
  int error = 1;
  while (strlen(stack) && error) {
    char* last_operator = get_last_operator_from_stack(stack);
    if (strcmp(last_operator, "(") != 0)
      pop_from_stack_to_output(output, stack, last_operator);
    else {
      size_t stack_length = strlen(stack);
      stack[stack_length - 1] = '\0';
      error = 0;
    }
  }
  // If there is no error
  if (error == 0) {
    // Check if there is a function
    char* last_function = get_last_function_from_stack(stack);
    if (last_function) pop_from_stack_to_output(output, stack, last_function);
  }
  return error;
}

char* to_postfix(char* str) {
  int error = 0;
  char* output = (char*)malloc(sizeof(char) * 600);
  char* stack = (char*)malloc(sizeof(char) * 600);
  stack[0] = '\0';
  output[0] = '\0';
  int length = 0;
  while (*(str + length) && !error) {
    if (char_is_num(*(str + length)))
      num_to_output(str + length, output, &length);
    else if (*(str + length) == 40) {
      strcat(stack, "(");
      length++;
    } else if (is_function(str + length))
      func_to_stack(str + length, stack, &length);
    else if (is_operator(str + length))
      handle_operator(str + length, stack, &length, output);
    else if (*(str + length) == 41) {
      error = handle_brackets(stack, output);
      length++;
    } else
      error = 1;
  }
  while (strlen(stack) && !error) {
    char* operator= get_last_operator_from_stack(stack);
    if (operator) {
      if (operator&&(strcmp(operator, "(") == 0 || strcmp(operator, ")") == 0))
        error = 1;
      else
        pop_from_stack_to_output(output, stack, operator);
    } else
      error = 1;
  }
  free(stack);
  if (error) strcpy(output, "Calculation error");
  return output;
}

int char_is_sign(char c) {
  int res = 0;
  if (c == '-' || c == '+') res = 1;
  return res;
}

// 0 not lonely sign,  1 -lonely sign, 2 - error
int check_if_lonely_sign(char* str, int length) {
  int res = 0;
  if (*(str + length) == '-' || (*(str + length)) == '+') {
    if ((str + length + 1) && !char_is_sign(*(str + length + 1))) {
      if (length == 0 || (*(str + length - 1)) == '(') res = 1;
    } else
      res = 2;
  }
  return res;
}

int check_string_has_number(char* str) {
  int length = 0;
  int num_found = 0;
  while (*(str + length) && !num_found) {
    if (char_is_num(*(str + length)) || (*(str + length)) == 'x')
      num_found = 1;
    else
      length++;
  }
  return num_found;
}

char* get_result_string(char* str, char* x) {
  char* res_str = NULL;
  if (check_string_has_number(str)) {
    int length = 0;
    int amount_of_x = 0;
    int number_of_lonely_signs = 0;
    int error = 0;
    if (strlen(x) == 0) x = "0";
    // count amount of xs
    char* new_x = (char*)malloc(sizeof(char) * 200);
    new_x[0] = '\0';
    if (x[0] == '-') {
      strcat(new_x, "(0");
      strcat(new_x, x);
      strcat(new_x, ")");
    } else
      strcat(new_x, x);
    while (*(str + length) && error != 2) {
      if (*(str + length) == 'x')
        amount_of_x++;
      else {
        error = check_if_lonely_sign(str, length);
        if (error == 1) number_of_lonely_signs++;
      }
      length++;
    }
    length = 0;
    if (error != 2) {
      int res_length = strlen(str) + amount_of_x * (strlen(new_x) - 1) +
                       number_of_lonely_signs;
      res_str = (char*)malloc(sizeof(char) * res_length + 1);
      res_str[0] = '\0';
      while (*(str + length)) {
        if (*(str + length) == 'x') {
          if (strlen(new_x)) strcat(res_str, new_x);
        } else if (check_if_lonely_sign(str, length)) {
          char tmp_str[3];
          tmp_str[0] = '0';
          tmp_str[1] = *(str + length);
          tmp_str[2] = '\0';
          strcat(res_str, tmp_str);
        } else {
          char tmp_str[2];
          tmp_str[0] = *(str + length);
          tmp_str[1] = '\0';
          strcat(res_str, tmp_str);
        }
        length++;
      }
      res_str[res_length] = '\0';
    }
    free(new_x);
  }
  return res_str;
}

long double perform_operation(long double num_1, long double num_2, char* str) {
  long double res = 0;
  switch (str[0]) {
    case '+':
      res = num_1 + num_2;
      break;
    case '-':
      res = num_1 - num_2;
      break;
    case 'm':
      res = fmod(num_1, num_2);
      break;
    case '*':
      res = num_1 * num_2;
      break;
    case '/':
      res = num_1 / num_2;
      break;
    case '^':
      res = pow(num_1, num_2);
      break;
  }
  return res;
}

long double execute_function(long double number, char* str) {
  long double res = 0;
  if (strcmp(str, "cos") == 0)
    res = cosl(number);
  else if (strcmp(str, "sin") == 0)
    res = sinl(number);
  else if (strcmp(str, "tan") == 0)
    res = tanl(number);
  else if (strcmp(str, "acos") == 0)
    res = acosl(number);
  else if (strcmp(str, "asin") == 0)
    res = asinl(number);
  else if (strcmp(str, "atan") == 0)
    res = atanl(number);
  else if (strcmp(str, "sqrt") == 0)
    res = sqrtl(number);
  else if (strcmp(str, "ln") == 0)
    res = logl(number);
  else if (strcmp(str, "log") == 0)
    res = log10l(number);
  return res;
}

long double calc_expression(char* str) {
  stack tmp_stack = create_stack();
  char* token = strtok(str, " ");
  while (token) {
    if (char_is_num(token[0])) {
      long double number = strtold(token, NULL);
      push(&tmp_stack, number);
    } else if (is_operator(token)) {
      // operator
      long double num_1 = tmp_stack.stack[tmp_stack.stack_size - 2];
      long double num_2 = tmp_stack.stack[tmp_stack.stack_size - 1];
      pop(&tmp_stack);
      pop(&tmp_stack);
      push(&tmp_stack, perform_operation(num_1, num_2, token));
    } else {
      // function
      long double number = tmp_stack.stack[tmp_stack.stack_size - 1];
      pop(&tmp_stack);
      push(&tmp_stack, execute_function(number, token));
    }
    token = strtok(NULL, " ");
  }
  long double res = tmp_stack.stack[0];
  free_stack(&tmp_stack);
  return res;
}

char* calculator(char* str, char* x) {
  char* res = (char*)malloc(sizeof(char) * 600);
  res[0] = '\0';
  char* result_string = get_result_string(str, x);
  if (result_string) {
    char* postfix = to_postfix(result_string);
    free(result_string);
    if (strcmp(postfix, "Calculation error")) {
      char tmp_str[600];
      sprintf(tmp_str, "%Lf", calc_expression(postfix));
      strcpy(res, tmp_str);
      free(postfix);
    } else
      strcpy(res, "Calculation error");
  } else
    strcpy(res, "Calculation error");
  return res;
}

void linspace(double x_min, double x_max, double x[1000]) {
  double delta = (x_max - x_min) / 1000;
  x[0] = x_min;
  for (int i = 1; i < 1000; i++) x[i] = x[i - 1] + delta;
}

points* calc_points(double x_min, double x_max, double y_min, double y_max,
                    char* expression) {
  points* points_to_plot = (points*)malloc(sizeof(points));
  points_to_plot->y_min = y_min;
  points_to_plot->y_max = y_max;
  linspace(x_min, x_max, points_to_plot->x);
  int stop = 0;
  for (int i = 0; i < 1000 && !stop; i++) {
    char tmp_str[600];
    sprintf(tmp_str, "%lf", points_to_plot->x[i]);
    char* result_string = get_result_string(expression, tmp_str);
    if (result_string) {
      char* postfix = to_postfix(result_string);
      free(result_string);
      if (strcmp(postfix, "Calculation error")) {
        double y_value = calc_expression(postfix);
        if (y_value >= y_min && y_value <= y_max)
          points_to_plot->y[i] = y_value;
        else
          points_to_plot->y[i] = NAN;
      } else {
        free(points_to_plot);
        points_to_plot = NULL;
        stop = 1;
      }
      free(postfix);
    } else {
      free(points_to_plot);
      points_to_plot = NULL;
      stop = 1;
    }
  }
  return points_to_plot;
}