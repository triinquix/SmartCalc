#include <time.h>

#include "../main_app.h"

char* date_to_str(struct tm* date) {
  char* res;
  char tmp_str[20];
  mktime(date);
  strftime(tmp_str, 20, "%d.%m.%Y", date);
  res = (char*)malloc(sizeof(char) * strlen(tmp_str) + 1);
  strcpy(res, tmp_str);
  return res;
}

int calc_num_of_days(double term) {
  time_t current_time;
  time(&current_time);
  struct tm* date = localtime(&current_time);
  char* start_date = date_to_str(date);
  char* tmp_str = (char*)malloc(sizeof(char) * strlen(start_date) + 1);
  strcpy(tmp_str, start_date);
  char* token;
  token = strtok(tmp_str, ".");
  token = strtok(NULL, ".");
  int new_month = atoi(token) + 12 * term;
  free(tmp_str);
  char end_date[10];
  sprintf(end_date, "%c%c.%d.%c%c%c%c", start_date[0], start_date[1], new_month,
          start_date[6], start_date[7], start_date[8], start_date[9]);
  int res = diff_in_days(start_date, end_date);
  free(start_date);
  return res;
}

int num_of_main_rows(double term, enum periodicity dep_per) {
  int res;
  switch (dep_per) {
    case END:
      res = 1;
      break;
    case DAILY:
      res = calc_num_of_days(term);
      break;
    case MONTHLY:
      res = 12 * term;
      break;
    case QUARTERLY:
      res = 4 * term;
      break;
    case HALF:
      res = 2 * term;
      break;
    case ANNUALLY:
      res = (int)term;
      break;
  }
  return res;
}

deposit* declare_deposit(double term, enum periodicity per,
                         int num_of_replanishments, int num_of_withdrawals) {
  deposit* deposit_data = (deposit*)malloc(sizeof(deposit));
  deposit_data->number_of_rows = num_of_main_rows(term, per) +
                                 num_of_replanishments + num_of_withdrawals + 1;
  deposit_data->dates =
      (char**)malloc(sizeof(char*) * deposit_data->number_of_rows);
  deposit_data->interest =
      (double*)malloc(sizeof(double) * deposit_data->number_of_rows);
  deposit_data->balance_change =
      (double*)malloc(sizeof(double) * deposit_data->number_of_rows);
  deposit_data->payments =
      (double*)malloc(sizeof(double) * deposit_data->number_of_rows);
  deposit_data->balance =
      (double*)malloc(sizeof(double) * deposit_data->number_of_rows);
  return deposit_data;
}

void init_first_row(deposit* deposit_data, double amount) {
  time_t current_time;
  time(&current_time);
  struct tm* current_date = localtime(&current_time);
  deposit_data->dates[0] = date_to_str(current_date);
  deposit_data->interest[0] = 0;
  deposit_data->balance_change[0] = amount;
  deposit_data->payments[0] = 0;
  deposit_data->balance[0] = amount;
}

struct tm* get_next_date(int n, enum periodicity per, double term) {
  time_t current_time;
  time(&current_time);
  struct tm* current_date = localtime(&current_time);
  current_date->tm_sec = 0;
  current_date->tm_min = 0;
  current_date->tm_hour = 0;
  switch (per) {
    case END:
      current_date->tm_mon += 12 * term;
      break;
    case DAILY:
      current_date->tm_mday += n;
      break;
    case MONTHLY:
      current_date->tm_mon += n;
      break;
    case QUARTERLY:
      current_date->tm_mon += 3 * n;
      break;
    case HALF:
      current_date->tm_mon += 6 * n;
      break;
    case ANNUALLY:
      current_date->tm_year += n;
      break;
  }
  return current_date;
}

struct tm* str_to_date(char* str) {
  struct tm* date = (struct tm*)malloc(sizeof(struct tm));
  date->tm_sec = 0;
  date->tm_min = 0;
  date->tm_hour = 0;
  date->tm_isdst = -1;
  char str_cpy[15];
  strcpy(str_cpy, str);
  char* token;
  char* delim = ".";
  token = strtok(str_cpy, delim);
  date->tm_mday = atoi(token);
  token = strtok(NULL, delim);
  date->tm_mon = atoi(token) - 1;
  token = strtok(NULL, delim);
  date->tm_year = atoi(token) - 1900;
  return date;
}

int is_date_near(struct tm* date_1, struct tm* date_2) {
  int res = 1;
  time_t time_1 = mktime(date_1);
  time_t time_2 = mktime(date_2);
  double seconds = difftime(time_1, time_2);
  if (seconds >= 0) res = 0;
  return res;
}

void iter_through_replanishments(deposit* deposit_data, int* row_counter,
                                 char*** replanishments,
                                 int num_of_replanishments,
                                 struct tm* next_date) {
  for (int i = 0; i < num_of_replanishments; i++) {
    if (replanishments[i]) {
      struct tm* date_of_replanishment = str_to_date(replanishments[i][0]);
      if (is_date_near(date_of_replanishment, next_date)) {
        deposit_data->dates[*row_counter] = date_to_str(date_of_replanishment);
        deposit_data->interest[*row_counter] = 0;
        deposit_data->balance_change[*row_counter] =
            strtod(replanishments[i][1], NULL);
        deposit_data->payments[*row_counter] = 0;
        deposit_data->balance[*row_counter] =
            deposit_data->balance[*row_counter - 1] +
            strtod(replanishments[i][1], NULL);
        (*row_counter)++;
        free(replanishments[i]);
        replanishments[i] = NULL;
      }
      free(date_of_replanishment);
    }
  }
}

void iter_through_withdrawals(deposit* deposit_data, int* row_counter,
                              char*** withdrawals, int num_of_withdrawals,
                              struct tm* next_date) {
  for (int i = 0; i < num_of_withdrawals; i++) {
    if (withdrawals[i]) {
      struct tm* date_of_withdrawal = str_to_date(withdrawals[i][0]);
      if (is_date_near(date_of_withdrawal, next_date)) {
        deposit_data->dates[*row_counter] = date_to_str(date_of_withdrawal);
        deposit_data->interest[*row_counter] = 0;
        deposit_data->balance_change[*row_counter] =
            -strtod(withdrawals[i][1], NULL);
        deposit_data->payments[*row_counter] = 0;
        deposit_data->balance[*row_counter] =
            deposit_data->balance[*row_counter - 1] -
            strtod(withdrawals[i][1], NULL);
        (*row_counter)++;
        free(withdrawals[i]);
        withdrawals[i] = NULL;
      }
      free(date_of_withdrawal);
    }
  }
}

int is_year_leap(char* date) {
  int res = 0;
  struct tm* date_struct = str_to_date(date);
  int year = date_struct->tm_year + 1900;
  if (year % 4 == 0) {
    if (year % 100 == 0) {
      if (year % 400 == 0) res = 1;
    } else
      res = 1;
  }
  free(date_struct);
  return res;
}

int year_passed(char* date_1, char* date_2) {
  int res;
  struct tm* st_date_1 = str_to_date(date_1);
  struct tm* st_date_2 = str_to_date(date_2);
  if (st_date_1->tm_year != st_date_2->tm_year)
    res = 1;
  else
    res = 0;
  free(st_date_1);
  free(st_date_2);
  return res;
}

int diff_in_days(char* date_1, char* date_2) {
  struct tm* st_date_1 = str_to_date(date_1);
  struct tm* st_date_2 = str_to_date(date_2);
  time_t time_1 = mktime(st_date_1);
  time_t time_2 = mktime(st_date_2);
  free(st_date_1);
  free(st_date_2);
  return difftime(time_2, time_1) / 86400;
}

double calc_interest(int prev_main_row, int row_counter, deposit* deposit_data,
                     double rate, int* num_of_days_in_year) {
  double cum_interest = 0;
  for (int i = prev_main_row + 1; i <= row_counter; i++) {
    if (year_passed(deposit_data->dates[i - 1], deposit_data->dates[i])) {
      struct tm* date = str_to_date(deposit_data->dates[i]);
      char start_date[20];
      sprintf(start_date, "01.01.%d", date->tm_year + 1900);
      free(date);
      cum_interest += deposit_data->balance[i - 1] * rate *
                      diff_in_days(deposit_data->dates[i - 1], start_date) /
                      *num_of_days_in_year / 100;
      if (is_year_leap(start_date))
        *num_of_days_in_year = 366;
      else
        *num_of_days_in_year = 365;
      cum_interest += deposit_data->balance[i - 1] * rate *
                      diff_in_days(start_date, deposit_data->dates[i]) /
                      *num_of_days_in_year / 100;
    } else
      cum_interest +=
          deposit_data->balance[i - 1] * rate *
          diff_in_days(deposit_data->dates[i - 1], deposit_data->dates[i]) /
          *num_of_days_in_year / 100;
  }
  return cum_interest;
}

void calc_main_rows(deposit* deposit_data, enum periodicity dep_periodicity,
                    enum deposit_type type, double term, double rate,
                    char*** replanishments, int num_of_replanishments,
                    char*** withdrawals, int num_of_withdrawals) {
  int num_of_days_in_year;
  if (is_year_leap(deposit_data->dates[0]))
    num_of_days_in_year = 366;
  else
    num_of_days_in_year = 365;
  int row_counter = 1;
  int period_counter = 1;
  int prev_main_row = 0;
  while (row_counter < deposit_data->number_of_rows) {
    struct tm* next_date = get_next_date(period_counter, dep_periodicity, term);
    iter_through_replanishments(deposit_data, &row_counter, replanishments,
                                num_of_replanishments, next_date);
    iter_through_withdrawals(deposit_data, &row_counter, withdrawals,
                             num_of_withdrawals, next_date);
    char* new_date = date_to_str(next_date);
    deposit_data->dates[row_counter] = new_date;
    double cum_interest = calc_interest(
        prev_main_row, row_counter, deposit_data, rate, &num_of_days_in_year);
    deposit_data->interest[row_counter] = cum_interest;
    if (type == NO_CAP) {
      deposit_data->balance_change[row_counter] = 0;
      deposit_data->payments[row_counter] = cum_interest;
    } else {
      deposit_data->balance_change[row_counter] = cum_interest;
      deposit_data->payments[row_counter] = 0;
    }
    deposit_data->balance[row_counter] =
        deposit_data->balance[row_counter - 1] +
        deposit_data->balance_change[row_counter];
    prev_main_row = row_counter;
    row_counter++;
    period_counter++;
  }
}

void free_deposit_data(deposit* deposit_data) {
  for (int i = 0; i < deposit_data->number_of_rows; i++) {
    free(deposit_data->dates[i]);
    deposit_data->dates[i] = NULL;
  }
  free(deposit_data->dates);
  deposit_data->dates = NULL;
  free(deposit_data->interest);
  deposit_data->interest = NULL;
  free(deposit_data->balance_change);
  deposit_data->balance_change = NULL;
  free(deposit_data->payments);
  deposit_data->payments = NULL;
  free(deposit_data->balance);
  deposit_data->balance = NULL;
  free(deposit_data);
  deposit_data = NULL;
}

deposit* calc_deposit(char* inputs[4], enum deposit_type type,
                      enum periodicity dep_periodicity, char*** replanishments,
                      int num_of_replanishments, char*** withdrawals,
                      int num_of_withdrawals) {
  double amount = strtod(inputs[0], NULL);
  double term = strtod(inputs[1], NULL);
  double rate = strtod(inputs[2], NULL);
  double tax_rate = strtod(inputs[3], NULL);
  deposit* deposit_data = declare_deposit(
      term, dep_periodicity, num_of_replanishments, num_of_withdrawals);
  init_first_row(deposit_data, amount);
  calc_main_rows(deposit_data, dep_periodicity, type, term, rate,
                 replanishments, num_of_replanishments, withdrawals,
                 num_of_withdrawals);
  if (dep_periodicity == END) {
    deposit* tmp_deposit =
        calc_deposit(inputs, type, MONTHLY, replanishments,
                     num_of_replanishments, withdrawals, num_of_withdrawals);
    deposit_data->interest[deposit_data->number_of_rows - 1] =
        tmp_deposit->accrued_interest;
    deposit_data->payments[deposit_data->number_of_rows - 1] =
        tmp_deposit->accrued_interest;
    free_deposit_data(tmp_deposit);
  }
  deposit_data->accrued_interest = 0;
  for (int i = 0; i < deposit_data->number_of_rows; i++)
    deposit_data->accrued_interest += deposit_data->interest[i];
  deposit_data->deposit_amount =
      deposit_data->balance[deposit_data->number_of_rows - 1];
  deposit_data->tax = deposit_data->accrued_interest * tax_rate / 100;
  return deposit_data;
}