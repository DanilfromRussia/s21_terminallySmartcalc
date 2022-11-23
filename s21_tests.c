#include <check.h>
#include "s21_smartcalc.h"

START_TEST(plus_t) {
  double my = parse_staff("(-2)+2");
  ck_assert_float_eq(my, 0);
}
END_TEST

START_TEST(minus_t) {
  double my = parse_staff("(-2.2)-2.2");
  ck_assert_float_eq(my, -4.4);
}
END_TEST

START_TEST(mult_t) {
  double my = parse_staff("(-2.2)*(+2.2)");
  ck_assert_float_eq(my, -4.84);
}
END_TEST

START_TEST(division_t) {
  double my = parse_staff("(-2.2)/2.2");
  ck_assert_float_eq(my, -1);
}
END_TEST

START_TEST(degr_t) {
  double my = parse_staff("(2.2)^(-2.2)");
  ck_assert_double_eq_tol(my, 0.17647, 0.17647);
}
END_TEST

START_TEST(sin_t) {
  double my = parse_staff("sin(+2)");
  ck_assert_double_eq(my, sinl(2));
}
END_TEST

START_TEST(cos_t) {
  long double my = parse_staff("cos(2)");
  ck_assert_ldouble_eq(my, cosl(2));
}
END_TEST

START_TEST(tan_t) {
  long double my = parse_staff("tan(2)");
  ck_assert_ldouble_eq(my, tanl(2));
}
END_TEST

START_TEST(asin_t) {
  long double my = parse_staff("asin(1)");
  ck_assert_ldouble_eq(my, asinl(1));
}
END_TEST

START_TEST(acos_t) {
  long double my = parse_staff("acos(1)");
  ck_assert_ldouble_eq(my, acosl(1));
}
END_TEST

START_TEST(atan_t) {
  long double my = parse_staff("atan(1)");
  ck_assert_ldouble_eq(my, atanl(1));
}
END_TEST

START_TEST(ln_t) {
  long double my = parse_staff("log(1)");
  ck_assert_ldouble_eq(my, logf(1));
}
END_TEST

START_TEST(log_t) {
  long double my = parse_staff("log(1)");
  ck_assert_ldouble_eq(my, log10l(1));
}
END_TEST

START_TEST(sqrt_t) {
  long double my = parse_staff("sqrt(1)");
  ck_assert_ldouble_eq(my, sqrtl(1));
}
END_TEST

START_TEST(mod_t) {
  long double my = parse_staff("4mod8");
  ck_assert_ldouble_eq(my, 4);
}
END_TEST

Suite *lib_suite(void) {
  Suite *s;
  s = suite_create("Num_Check");
  TCase *t_1;
  TCase *t_2;
  TCase *t_3;
  TCase *t_4;
  TCase *t_5;
  TCase *t_6;
  TCase *t_7;
  TCase *t_8;
  TCase *t_9;
  TCase *t_10;
  TCase *t_11;
  TCase *t_12;
  TCase *t_13;
  TCase *t_14;
  TCase *t_15;

  t_1 = tcase_create("t_1");
  suite_add_tcase(s, t_1);
  tcase_add_test(t_1, plus_t);

  t_2 = tcase_create("t_2");
  suite_add_tcase(s, t_2);
  tcase_add_test(t_2, minus_t);

  t_3 = tcase_create("t_3");
  suite_add_tcase(s, t_3);
  tcase_add_test(t_3, division_t);

  t_4 = tcase_create("t_4");
  suite_add_tcase(s, t_4);
  tcase_add_test(t_4, mult_t);

  t_5 = tcase_create("t_5");
  suite_add_tcase(s, t_5);
  tcase_add_test(t_5, degr_t);

  t_6 = tcase_create("t_6");
  suite_add_tcase(s, t_6);
  tcase_add_test(t_6, sin_t);

  t_7 = tcase_create("t_7");
  suite_add_tcase(s, t_7);
  tcase_add_test(t_7, cos_t);

  t_8 = tcase_create("t_8");
  suite_add_tcase(s, t_8);
  tcase_add_test(t_8, tan_t);

  t_9 = tcase_create("t_9");
  suite_add_tcase(s, t_9);
  tcase_add_test(t_9, asin_t);

  t_10 = tcase_create("t_10");
  suite_add_tcase(s, t_10);
  tcase_add_test(t_10, acos_t);

  t_11 = tcase_create("t_11");
  suite_add_tcase(s, t_11);
  tcase_add_test(t_11, atan_t);

  t_12 = tcase_create("t_12");
  suite_add_tcase(s, t_12);
  tcase_add_test(t_12, ln_t);

  t_13 = tcase_create("t_13");
  suite_add_tcase(s, t_13);
  tcase_add_test(t_13, log_t);

  t_14 = tcase_create("t_14");
  suite_add_tcase(s, t_14);
  tcase_add_test(t_14, sqrt_t);

  t_15 = tcase_create("t_15");
  suite_add_tcase(s, t_15);
  tcase_add_test(t_15, mod_t);

  return s;
}

int main(void) {
  Suite *s;
  SRunner *sr;
  s = lib_suite();
  sr = srunner_create(s);
  srunner_run_all(sr, CK_VERBOSE);
  srunner_free(sr);
  return 0;
}
