#ifndef MAIN_APP_H
#define MAIN_APP_H

#include <gtk/gtk.h>

typedef struct Points {
  double x[1000];
  double y[1000];
  double y_min;
  double y_max;
} points;

typedef struct credit_output {
  char** dates;
  double* payments;
  double* payments_for_principal_loan;
  double* payments_for_interest;
  double* residuals;
  double overpayment;
  double total_payment;
  int number_of_periods;
} credit;

typedef struct deposit_output {
  char** dates;
  double* interest;
  double* balance_change;
  double* payments;
  double* balance;
  double accrued_interest;
  double tax;
  double deposit_amount;
  int number_of_rows;
} deposit;

enum periodicity { END, DAILY, MONTHLY, QUARTERLY, HALF, ANNUALLY };

enum credit_type { ANNUITY, DIFFERENTIATED };

enum deposit_type { INTEREST_CAP, NO_CAP };

// Creates main menu with widgets
void create_main_menu(GtkWidget* window);

// Calculates expression given in str
char* calculator(char* str, char* x);

// Activates the application
void activate_app(GtkApplication* app);

// Creates main menu
void create_main_menu(GtkWidget* window);

// Opens credit calculator
void open_credit_calc(GtkWidget* window);

// Opens calculator
void open_calc(GtkWidget* window);

// Opens deposit calculator
void open_deposit_calc(GtkWidget* window);

// Initializes grid for credit and deposit calculators
GtkWidget* init_grid(GtkWidget* scrolled_window);

// Processes the given data for credit calculator
credit* process_data(char* str_to_check[3], enum credit_type type);

// processes plot data
points* process_plot_data(char* x_min, char* x_max, char* y_min, char* y_max,
                          char* expression);
// Calculates points
points* calc_points(double x_min, double x_max, double y_min, double y_max,
                    char* expression);

// Processes the given data for deposit calculator
deposit* process_deposit_data(char* inputs[4], enum deposit_type type,
                              enum periodicity dep_periodicity,
                              char*** replanishments, int num_of_replanishments,
                              char*** withdrawals, int num_of_withdrawals);

// Calculates credit data
credit* calc_credit(char* terms[3], enum credit_type type);

// Calculates depost data
deposit* calc_deposit(char* inputs[4], enum deposit_type type,
                      enum periodicity dep_periodicity, char*** replanishments,
                      int num_of_replanishments, char*** withdrawals,
                      int num_of_withdrawals);

// Shows error
void show_error_msg(GtkWidget* grid, int row, int column);

// Frees the memoty allocated for deposit data
void free_deposit_data(deposit* deposit_data);

// Frees the memoty allocated for credit data
void free_credit_data(credit* credit_data);

// Calculates difference in days between two dates
int diff_in_days(char* date_1, char* date_2);

#endif