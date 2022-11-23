#ifndef SRC_S21_SMARTCALC_H_
#define SRC_S21_SMARTCALC_H_

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ACTUAL input[elem]
#define ACTUALP input[*elem]
#define STACK_MAX_SIZE 256
#define ERR 1
#define CORRECT 0

enum priority {
  zero = 0,
  plus = 1,
  minus = 1,
  ost = 2,
  mlt = 3,
  divv = 3,
  powr = 4,
  ln_ = 5,
  log_ = 5,
  cos_ = 5,
  acos_ = 5,
  sin_ = 5,
  tan_ = 5,
  atan_ = 5,
  sqrt_ = 5
};

typedef struct num {
  double num_arr[STACK_MAX_SIZE];
  size_t num_count;
} num_t;

typedef struct calc {
  char calc_arr[STACK_MAX_SIZE];
  size_t oper_count;
} calc_t;

int checker(char *input);
void start_iteration(void);
int bracket_finalProtect(int bracket_cnt, bool *brake);
double parse_staff(char *input);
void functional_protection(bool *brake, int elem, bool *lastWasDigit,
                           bool *lastWasOperat, char *input);
int getPriority(char elem);
void sum_to_1(int *elem);
void sum_to_2(int *elem);

void pushNum(num_t *stack, double value);
double popNum(num_t *stack);
void pushOper(calc_t *stack, char value);
char popOper(calc_t *stack);
char checkLastOper(calc_t *stack);
void findEndOfNum(num_t *stack, int *elem, char *input);
void findEndOfWord(int *elem, char *input);
void operation_compl(double up, num_t *n_stack, char elem);
void comparisonPriority(calc_t *c_stack, char *str, int elem, num_t *n_stack);
void whatPush(calc_t *c_stack, char *str, int elem);
void result(num_t *n_stack, calc_t *c_stack);
double visualise(num_t *stack);

#endif  // SRC_S21_SMARTCALC_H_
