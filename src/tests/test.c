#include <check.h>
#include <stdio.h>

#include "../main_app.h"

START_TEST(cos_test) {
  char* expression = "cos(2*x)-x-12/x";
  char* x = "3.14";
  char* res_str = calculator(expression, x);
  double res = strtold(res_str, NULL);
  ck_assert_ldouble_eq_tol(res, cosl(2 * 3.14) - 3.14 - 12 / 3.14, 1e-6);
  free(res_str);
}
END_TEST

Suite* create_cos_suite() {
  Suite* s = suite_create("cos_suite");
  TCase* tc = tcase_create("tc_cos");
  tcase_add_test(tc, cos_test);
  suite_add_tcase(s, tc);
  return s;
}

START_TEST(sin_test) {
  char* expression = "sin(x)+10+5modx";
  char* x = "3.14";
  char* res_str = calculator(expression, x);
  long double res = strtold(res_str, NULL);
  free(res_str);
  ck_assert_ldouble_eq_tol(res, sinl(3.14) + 10 + fmodl(5, 3.14), 1e-6);
}
END_TEST

Suite* create_sin_suite() {
  Suite* s = suite_create("sin_suite");
  TCase* tc = tcase_create("tc_sin");
  tcase_add_test(tc, sin_test);
  suite_add_tcase(s, tc);
  return s;
}

START_TEST(tan_atan_test) {
  char* expression = "tan(x)+atan(x)/x";
  char* x = "-1.1";
  char* res_str = calculator(expression, x);
  long double res = strtold(res_str, NULL);
  free(res_str);
  ck_assert_double_eq_tol(res, tanl(-1.1) + atanl(-1.1) / -1.1, 1e-6);
}
END_TEST

Suite* create_tan_atan_suite() {
  Suite* s = suite_create("tan_atan_suite");
  TCase* tc = tcase_create("tc_tan_atan");
  tcase_add_test(tc, tan_atan_test);
  suite_add_tcase(s, tc);
  return s;
}

START_TEST(mixed_test) {
  char* expression = "ln(x)+log(x)/sqrt(x)+x^x";
  char* x = "0.376";
  char* res_str = calculator(expression, x);
  long double res = strtold(res_str, NULL);
  free(res_str);
  ck_assert_double_eq_tol(
      res, log(0.376) + log10(0.376) / sqrt(0.376) + pow(0.376, 0.376), 1e-6);
}
END_TEST

Suite* create_mixed_suite() {
  Suite* s = suite_create("mixed_suite_ss");
  TCase* tc = tcase_create("tc_mixed");
  tcase_add_test(tc, mixed_test);
  suite_add_tcase(s, tc);
  return s;
}

START_TEST(credit_test_annuity) {
  char* terms[3];
  terms[0] = "20000";
  terms[1] = "2";
  terms[2] = "3.75";
  credit* credit_data = calc_credit(terms, ANNUITY);
  ck_assert_double_eq_tol(credit_data->payments[0], 866.27, 1e-2);
  ck_assert_double_eq_tol(credit_data->total_payment, 20790.48, 1);
  ck_assert_double_eq_tol(credit_data->overpayment, 790.48, 1);
  free_credit_data(credit_data);
}

Suite* create_annuity() {
  Suite* s = suite_create("annuity");
  TCase* tc = tcase_create("tc_annuity");
  tcase_add_test(tc, credit_test_annuity);
  suite_add_tcase(s, tc);
  return s;
}

START_TEST(credit_test_diff) {
  char* terms[3];
  terms[0] = "20000";
  terms[1] = "2";
  terms[2] = "3.75";
  credit* credit_data = calc_credit(terms, DIFFERENTIATED);
  ck_assert_double_eq_tol(credit_data->total_payment, 20781.25, 1e-2);
  ck_assert_double_eq_tol(credit_data->overpayment, 781.25, 1e-2);
  free_credit_data(credit_data);
}

Suite* create_diff() {
  Suite* s = suite_create("diff");
  TCase* tc = tcase_create("tc_diff");
  tcase_add_test(tc, credit_test_diff);
  suite_add_tcase(s, tc);
  return s;
}

START_TEST(deposit_test) {
  char* inputs[4];
  inputs[0] = "1000000";
  inputs[1] = "2";
  inputs[2] = "7.2";
  inputs[3] = "3";
  char*** replanishments = (char***)malloc(sizeof(char***));
  replanishments[0] = (char**)malloc(sizeof(char**) * 2);
  replanishments[0][0] = "05.01.2024";
  replanishments[0][1] = "900000";
  char*** withdrawals = (char***)malloc(sizeof(char***));
  withdrawals[0] = (char**)malloc(sizeof(char**) * 2);
  withdrawals[0][0] = "08.08.2024";
  withdrawals[0][1] = "1500000";
  deposit* deposit_data = calc_deposit(inputs, INTEREST_CAP, DAILY,
                                       replanishments, 1, withdrawals, 1);
  free(replanishments[0]);
  free(withdrawals[0]);
  free(replanishments);
  free(withdrawals);
  ck_assert_double_eq_tol(deposit_data->accrued_interest, 138762.6, 1);
  ck_assert_double_eq_tol(deposit_data->deposit_amount, 538762.6, 1);
  ck_assert_double_eq_tol(deposit_data->tax, 138762.6 * 0.03, 1);
  free_deposit_data(deposit_data);
}

Suite* create_deposit_1() {
  Suite* s = suite_create("dp1");
  TCase* tc = tcase_create("tc_dp1");
  tcase_add_test(tc, deposit_test);
  suite_add_tcase(s, tc);
  return s;
}

void run_test(Suite* s, const char* test_kind, int* failed) {
  SRunner* sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);
  printf("\033[95m%s\n\033[92m", test_kind);
  srunner_run_all(sr, CK_NORMAL);
  (*failed) += srunner_ntests_failed(sr);
  srunner_free(sr);
  printf("\n\033[39m");
}

int main() {
  int num_of_failed = 0;
  // Suites
  Suite* cos_suite = create_cos_suite();
  Suite* sin_suite = create_sin_suite();
  Suite* tan_suite = create_tan_atan_suite();
  Suite* mixed_suite = create_mixed_suite();
  Suite* annuity_suite = create_annuity();
  Suite* diff_suite = create_diff();
  Suite* deposit_suite = create_deposit_1();
  //  running all tests
  run_test(cos_suite, "Cos tests", &num_of_failed);
  run_test(sin_suite, "Sin tets", &num_of_failed);
  run_test(tan_suite, "Тангенс и арктангенс", &num_of_failed);
  run_test(mixed_suite, "Mixed", &num_of_failed);
  run_test(annuity_suite, "Annuity", &num_of_failed);
  run_test(diff_suite, "Differentiated", &num_of_failed);
  run_test(deposit_suite, "Deposit_test_1", &num_of_failed);
  return (num_of_failed == 0) ? 0 : 1;
}
