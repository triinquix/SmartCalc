#include <time.h>

#include "../main_app.h"

void calc_annuity(double total_amount, double rate, int number_of_periods,
                  double* payments, double* payments_for_principal_loan,
                  double* payments_for_interest, double* residuals) {
  double payment =
      total_amount * rate / (1 - pow(1 + rate, -number_of_periods));
  payments[0] = payment;
  payments_for_interest[0] = total_amount * rate;
  payments_for_principal_loan[0] = payment - payments_for_interest[0];
  residuals[0] = total_amount * (1 + rate) - payment;
  for (int i = 1; i < number_of_periods; i++) {
    payments[i] = payment;
    payments_for_interest[i] = residuals[i - 1] * rate;
    payments_for_principal_loan[i] = payments[i] - payments_for_interest[i];
    residuals[i] = residuals[i - 1] + payments_for_interest[i] - payments[i];
  }
}

void calc_differentiated(double total_amount, double rate,
                         int number_of_periods, double* payments,
                         double* payments_for_principal_loan,
                         double* payments_for_interest, double* residuals) {
  double payment_for_principal_loan = total_amount / number_of_periods;
  payments[0] = payment_for_principal_loan + total_amount * rate;
  payments_for_principal_loan[0] = payment_for_principal_loan;
  payments_for_interest[0] = payments[0] - payments_for_principal_loan[0];
  residuals[0] = total_amount * (1 + rate) - payments[0];
  for (int i = 1; i < number_of_periods; i++) {
    payments_for_principal_loan[i] = payment_for_principal_loan;
    payments[i] = payment_for_principal_loan + residuals[i - 1] * rate;
    payments_for_interest[i] = payments[i] - payments_for_principal_loan[i];
    residuals[i] = residuals[i - 1] + payments_for_interest[i] - payments[i];
  }
}

char* get_year_and_month(int month_shift) {
  time_t current_time;
  time(&current_time);
  struct tm* data = localtime(&current_time);
  data->tm_mon += month_shift;
  if (data->tm_mon > 11) {
    int year_shift = data->tm_mon / 12;
    data->tm_mon %= 12;
    data->tm_year += year_shift;
  }
  char tmp_str[20];
  strftime(tmp_str, 20, "%B %Y", data);
  char* res = (char*)malloc(sizeof(char) * strlen(tmp_str) + 1);
  strcpy(res, tmp_str);
  return res;
}
void free_credit_data(credit* credit_data) {
  for (int i = 0; i < credit_data->number_of_periods; i++) {
    free(credit_data->dates[i]);
    credit_data->dates[i] = NULL;
  }
  free(credit_data->dates);
  credit_data->dates = NULL;
  free(credit_data->payments);
  credit_data->payments = NULL;
  free(credit_data->payments_for_principal_loan);
  credit_data->payments_for_principal_loan = NULL;
  free(credit_data->payments_for_interest);
  credit_data->payments_for_interest = NULL;
  free(credit_data->residuals);
  credit_data->residuals = NULL;
  free(credit_data);
  credit_data = NULL;
}

credit* calc_credit(char* terms[3], enum credit_type type) {
  credit* credit_data = (credit*)malloc(sizeof(credit));
  double total_amount = strtod(terms[0], NULL);
  double term = strtod(terms[1], NULL);
  double rate = strtod(terms[2], NULL) / 12 / 100;  // monthly rate
  int number_of_periods = term * 12;
  // Allocating memory for arrays
  credit_data->dates = (char**)malloc(sizeof(char*) * number_of_periods);
  credit_data->payments = (double*)malloc(sizeof(double) * number_of_periods);
  credit_data->payments_for_principal_loan =
      (double*)malloc(sizeof(double) * number_of_periods);
  credit_data->payments_for_interest =
      (double*)malloc(sizeof(double) * number_of_periods);
  credit_data->residuals = (double*)malloc(sizeof(double) * number_of_periods);
  // Initializing arrays
  for (int i = 0; i < number_of_periods; i++)
    credit_data->dates[i] = get_year_and_month(i);
  if (type == ANNUITY)
    calc_annuity(total_amount, rate, number_of_periods, credit_data->payments,
                 credit_data->payments_for_principal_loan,
                 credit_data->payments_for_interest, credit_data->residuals);
  else
    calc_differentiated(
        total_amount, rate, number_of_periods, credit_data->payments,
        credit_data->payments_for_principal_loan,
        credit_data->payments_for_interest, credit_data->residuals);
  credit_data->total_payment = 0;
  for (int i = 0; i < number_of_periods; i++)
    credit_data->total_payment += credit_data->payments[i];
  credit_data->overpayment = credit_data->total_payment - total_amount;
  credit_data->number_of_periods = number_of_periods;
  return credit_data;
}
