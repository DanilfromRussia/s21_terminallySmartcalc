#include "s21_smartcalc.h"

// Read array from console
void start_iteration() {
  char input[257] = "";
  char c;
  int i = 0;
  while ((c = getchar()) != '\n' && (i <= 255)) {
    input[i++] = c;
  }

  if (i > 255) {
    printf("Error, too much symbols to calculate!\n");
  } else if (checker(input) == CORRECT) {
    printf("%f\n", parse_staff(input));
  } else {
    printf("Err in validate input line!\n");
  }
}

// Validate input
int checker(char *input) {
  int returnable = CORRECT;

  int length = strlen(input);
  bool brake = false;  // "true" say, that found crush
  int bracket_cnt = 0;  // "(" increase, "(" reduce
  bool lastWasDigit = false;
  bool lastWasOperat = false;
  int digit_cnt = 0;  // for ex. "7" is 1 digit, "345" is 3
  int dot_cnt = 0;  // digit analogy

  for (int elem = 0; elem < length; elem++) {
    // Bracket protection starting
    if (input[elem] == '(') {
      if (lastWasOperat == false && elem != 0) {
        brake = true;
        break;
      }
      lastWasDigit = false;
      lastWasOperat = false;
      digit_cnt = 0;
      bracket_cnt++;
      continue;
    }
    if (input[elem] == ')') {
      if (bracket_cnt < 1) {
        brake = true;
        break;
      }
      bracket_cnt--;
      lastWasDigit = false;
      lastWasOperat = false;
      digit_cnt = 0;
      continue;
    }
    // Dot protection starting
    if (input[elem] == '.') {
      // Dot without num ".34" -> crush
      if ((digit_cnt == 0) || (dot_cnt > 0)) {
        brake = true;
        break;
      }
      // Last array elem is "." -> crush
      if (elem + 1 == length) {
        brake = true;
        break;
      }
      lastWasDigit = false;
      lastWasOperat = false;
      digit_cnt = 0;
      dot_cnt++;
      continue;
    }
    if (dot_cnt > 0) {  // If last - dot, prelast - digit
      if (input[elem] > 47 && input[elem] < 58) {
        digit_cnt++;
        lastWasDigit = true;
        lastWasOperat = false;
        continue;
      }
    }
    // Num protection starting
    if (input[elem] > 47 && input[elem] < 58) {
      // "05" is not num -> crush
      if (digit_cnt == 1 && lastWasDigit == true && input[elem - 1] == '0') {
        brake = true;
        break;
      }
      // Division by zero
      if (lastWasOperat == true && input[elem - 1] == '/' &&
          input[elem] == '0') {
        brake = true;
        break;
      }
      digit_cnt++;
      lastWasDigit = true;
      lastWasOperat = false;
      continue;
    }
    //  Math Functional protection starting
    if (ACTUAL == 'm' || ACTUAL == 'c' || ACTUAL == 's' || ACTUAL == 't' ||
        ACTUAL == 'l') {
      functional_protection(&brake, elem, &lastWasDigit, &lastWasOperat, input);
      if (brake == true) break;
      findEndOfWord(&elem, input);
      continue;
    }
    // Arithmetic protection starting
    if (input[elem] == '^' || input[elem] == '*' || input[elem] == '/' ||
        input[elem] == '-' || input[elem] == '+') {
    // Make unary sign in stack different from default: "-" turn "_" && "+" turn "#"
      if ((ACTUAL == '-' || ACTUAL == '+') && dot_cnt == 0 &&
          (input[elem - 1] == '(' || elem == 0)) {
        if (ACTUAL == '-') ACTUAL = '_';
        if (ACTUAL == '+') ACTUAL = '#';
        lastWasDigit = true;
      }
      if (lastWasOperat == true) {
        brake = true;
        break;
      }
      if (dot_cnt && lastWasOperat == false) dot_cnt--;
      if (lastWasDigit == false && input[elem - 1] != ')') {
        brake = true;
        break;
      }
      if (elem + 1 == length) {
        brake = true;
        break;
      }
      digit_cnt = 0;
      lastWasOperat = true;
      lastWasDigit = false;
      continue;
    }
  }
  if ((bracket_finalProtect(bracket_cnt, &brake)) == 0) {
    if (brake == true) returnable = ERR;
  } else {
    returnable = ERR;
  }
  return returnable;
}

// START PARSING TO STACK
double parse_staff(char *input) {
  int length = strlen(input);
  calc_t c_stack;
  c_stack.oper_count = 0;
  num_t n_stack = {0};
  n_stack.num_count = 0;
  for (int elem = 0; elem < length; elem++) {
    // Return unary elems to normal view.
    if (ACTUAL == '_' || ACTUAL == '#') {
      if (ACTUAL == '_') ACTUAL = '-';
      if (ACTUAL == '#') ACTUAL = '+';
      findEndOfNum(&n_stack, &elem, input);
    } else if (ACTUAL == '^' || ACTUAL == '*' || ACTUAL == '/' ||
               ACTUAL == '-' || ACTUAL == '+') {
      if (c_stack.oper_count == 0) {
        pushOper(&c_stack, ACTUAL);
      } else {
        comparisonPriority(&c_stack, input, elem, &n_stack);
      }
    } else if (ACTUAL > 47 && ACTUAL < 58) {
      findEndOfNum(&n_stack, &elem, input);
    } else if (ACTUAL == '(') {
      pushOper(&c_stack, ACTUAL);
    } else if (ACTUAL == ')') {
      char lastOper = popOper(&c_stack);
      if (lastOper != '(') {
        for (; lastOper != '('; lastOper = popOper(&c_stack)) {
          double up1 = popNum(&n_stack);
          operation_compl(up1, &n_stack, lastOper);
        }
      } else if (c_stack.oper_count > 0) {
          double up1 = popNum(&n_stack);
          operation_compl(up1, &n_stack, popOper(&c_stack));
      }
    } else if (ACTUAL == 'm' || ACTUAL == 'c' || ACTUAL == 's' ||
               ACTUAL == 't' || ACTUAL == 'a' || ACTUAL == 'l') {
      if (ACTUAL == 'm') {
        comparisonPriority(&c_stack, input, elem, &n_stack);
        findEndOfWord(&elem, input);
      } else if (ACTUAL == 'a') {
        if (input[elem + 1] == 't') {  // atan
          comparisonPriority(&c_stack, input, elem, &n_stack);
          findEndOfWord(&elem, input);
        } else if (input[elem + 1] == 's') {  // asin
          comparisonPriority(&c_stack, input, elem, &n_stack);
          findEndOfWord(&elem, input);
        } else {  // acos
          comparisonPriority(&c_stack, input, elem, &n_stack);
          findEndOfWord(&elem, input);
        }
      } else if (ACTUAL == 's') {
        if (input[elem + 1] == 'i') {  // sin
          comparisonPriority(&c_stack, input, elem, &n_stack);
          findEndOfWord(&elem, input);
        } else if (input[elem + 1] == 'q') {  // sqrt
          comparisonPriority(&c_stack, input, elem, &n_stack);
          findEndOfWord(&elem, input);
        }
      } else if (ACTUAL == 'c') {  // cos
        comparisonPriority(&c_stack, input, elem, &n_stack);
        findEndOfWord(&elem, input);
      } else if (ACTUAL == 'l') {
        if (input[elem + 1] == 'n') {  // ln
          comparisonPriority(&c_stack, input, elem, &n_stack);
          findEndOfWord(&elem, input);
        } else if (input[elem + 1] == 'o') {  // log
          comparisonPriority(&c_stack, input, elem, &n_stack);
          findEndOfWord(&elem, input);
        }
      } else {  // tan
        comparisonPriority(&c_stack, input, elem, &n_stack);
        findEndOfWord(&elem, input);
      }
    }
  }
  if (c_stack.oper_count > 0) {
    while (c_stack.oper_count != 0) {
      double up = popNum(&n_stack);
      operation_compl(up, &n_stack, popOper(&c_stack));
    }
  } if (n_stack.num_count > 1) {
    result(&n_stack, &c_stack);
  }
  double returnable = visualise(&n_stack);
  return returnable;
}

void result(num_t *n_stack, calc_t *c_stack) {
  if (c_stack->oper_count > 0) {
    while (c_stack->oper_count != 0) {
      double up = popNum(n_stack);
      operation_compl(up, n_stack, popOper(c_stack));
    }
  } else {
    double up = popNum(n_stack);
    operation_compl(up, n_stack, popOper(c_stack));
  }
}

// In my calc you must writing fullname function, for ex "asin(0.5)"
void findEndOfWord(int *elem, char *input) {
  while (input[(*elem) + 1] > 96 && input[(*elem) + 1] < 123) {
    (*elem)++;
  }
}

void findEndOfNum(num_t *stack, int *elem, char *input) {
  char str[256] = {'\0'};
  size_t strcnt = 0;
  if (ACTUALP == '+' || ACTUALP == '-') {
    str[strcnt] = ACTUALP;
    strcnt++;
    (*elem)++;
  }
  while ((ACTUALP > 47 && ACTUALP < 58) || ACTUALP == '.') {
    str[strcnt] = ACTUALP;
    strcnt++;
    (*elem)++;
  }
  (*elem)--;
  pushNum(stack, atof(str));
}

void comparisonPriority(calc_t *c_stack, char *str, int elem, num_t *n_stack) {
  if (c_stack->oper_count > 0) {
    char Last = popOper(c_stack);
    if (getPriority(Last) < getPriority(str[elem])) {
      pushOper(c_stack, Last);
      whatPush(c_stack, str, elem);
    } else {
      double up2 = popNum(n_stack);
      operation_compl(up2, n_stack, Last);
      pushOper(c_stack, str[elem]);
    }
  } else {
    whatPush(c_stack, str, elem);
  }
}

// log L, ln l, atan T, acos C, asin S, sqrt q, sin s, mod m, tan t
void whatPush(calc_t *c_stack, char *str, int elem) {
  if ((str[elem]) == 'a') {
    if ((str[elem + 1]) == 't') {
      pushOper(c_stack, 'T');
    } else if ((str[elem + 1]) == 'c') {
      pushOper(c_stack, 'C');
    } else if ((str[elem + 1]) == 's') {
      pushOper(c_stack, 'S');
    }
  } else if ((str[elem]) == 'l') {
    ((str[elem + 1]) == 'n') ? pushOper(c_stack, 'l') : pushOper(c_stack, 'L');
  } else if ((str[elem]) == 's') {
    ((str[elem + 1]) == 'i') ? pushOper(c_stack, 's') : pushOper(c_stack, 'q');
  } else if ((str[elem]) == 'm') {
    pushOper(c_stack, 'm');
  } else if ((str[elem]) == 'c') {
    pushOper(c_stack, 'c');
  } else if ((str[elem]) == 't') {
    pushOper(c_stack, 't');
  } else {
    pushOper(c_stack, str[elem]);
  }
}

void operation_compl(double up, num_t *n_stack, char elem) {
  double down = 1;
  if (n_stack->num_count > 0 && getPriority(elem) < 5) {
    down = popNum(n_stack);
  }
  if (elem == '+') {
    pushNum(n_stack, (up + down));
  } else if (elem == '-') {
    pushNum(n_stack, (down - up));
  } else if (elem == '/') {
    pushNum(n_stack, (down / up));
  } else if (elem == '*') {
    pushNum(n_stack, (down * up));
  } else if (elem == '^') {
    pushNum(n_stack, (pow(down, up)));
  } else if (elem == 'm') {
    pushNum(n_stack, (fmod(down, up)));
  } else if (elem == 'L') {
    pushNum(n_stack, (log10(up)));
  } else if (elem == 'l') {
    pushNum(n_stack, (log(up)));
  } else if (elem == 'T') {
    pushNum(n_stack, (atan(up)));
  } else if (elem == 'C') {
    pushNum(n_stack, (acos(up)));
  } else if (elem == 'S') {
    pushNum(n_stack, (asin(up)));
  } else if (elem == 'q') {
    pushNum(n_stack, (sqrt(up)));
  } else if (elem == 's') {
    pushNum(n_stack, (sin(up)));
  } else if (elem == 'c') {
    pushNum(n_stack, (cos(up)));
  } else {
    pushNum(n_stack, (tan(up)));
  }
}

double visualise(num_t *stack) { return (popNum(stack)); }

void pushNum(num_t *stack, double value) {
  stack->num_arr[stack->num_count] = value;
  stack->num_count++;
}

double popNum(num_t *stack) {
  stack->num_count--;
  return stack->num_arr[stack->num_count];
}

void pushOper(calc_t *stack, char value) {
  stack->calc_arr[stack->oper_count] = value;
  stack->oper_count++;
}

char popOper(calc_t *stack) {
  stack->oper_count--;
  return stack->calc_arr[stack->oper_count];
}

char checkLastOper(calc_t *stack) { return stack->calc_arr[stack->oper_count]; }

void functional_protection(bool *brake, int elem, bool *lastWasDigit,
                           bool *lastWasOperat, char *input) {
  if (ACTUAL == 'm') {
    if (*lastWasDigit != true || elem > 251) *brake = true;
  } else if (ACTUAL == 'c') {
    if (elem > 0 && input[elem - 1] == 'a') {
      if (elem > 249) *brake = true;
    } else {
      if (elem > 249) *brake = true;
    }
  } else if (ACTUAL == 't') {
    if (elem > 0 && input[elem - 1] == 'a') {
      if (elem > 249) *brake = true;
    } else {
      if (elem > 249) *brake = true;
    }
  } else if (ACTUAL == 's') {
    if (elem > 249) {
      *brake = true;
    } else if (input[elem + 1] == 'q') {
      if (elem > 248 || input[elem + 5] == '-') *brake = true;
    }
  } else if (ACTUAL == 'l') {
    if (input[elem + 1] == 'n') {
      if (elem > 250 || input[elem + 3] == '-') *brake = true;
    } else {
      if (elem > 249 || input[elem + 4] == '-') *brake = true;
    }
  }
  *(lastWasDigit) = false;
  *(lastWasOperat) = true;
}

// Bracket validator
int bracket_finalProtect(int bracket_cnt, bool *brake) {
  int returnable = CORRECT;
  if (bracket_cnt != 0) {
    *(brake) = true;
    returnable = ERR;
  }
  return returnable;
}

// Return priority sign or math func
int getPriority(char elem) {
  int returnable = 0;  // Using 0 because no validation here
  if (elem == '+' || elem == '-') {
    returnable = plus;
  } else if (elem == 'm') {
    returnable = ost;
  } else if (elem == '*' || elem == '/') {
    returnable = divv;
  } else if (elem == '^') {
    returnable = powr;
  } else if (elem == '(' || elem == ')') {
    returnable = zero;
  } else {
    returnable = tan_;  // tan priority == log == cos ...
  }
  return returnable;
}
