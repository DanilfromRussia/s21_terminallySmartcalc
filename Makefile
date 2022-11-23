CC := gcc
CFLAGS := -std=c11 -Wall -Werror -Wextra -lm -lsubunit
DBG := -g -lm
LIB := -pthread -lcheck_pic -lrt -lm -lsubunit

all: clean s21_smartcalc run

clean:
	@rm -rf *.a *.o s21_smartcalc *.out s21_debugcalc

s21_smartcalc: s21_smartcalc.a
	@$(CC) -g s21_main.c s21_smartcalc.a -o s21_smartcalc $(LIB)

s21_smartcalc.a: s21_smartcalc.o
	ar rc s21_smartcalc.a s21_smartcalc.o

s21_smartcalc.o: s21_smartcalc.c s21_smartcalc.h
	@$(CC) -c s21_smartcalc.c

run:
	./s21_smartcalc

debug:
	@$(CC) s21_smartcalc.c $(DBG) -o ./a.out

test: clean s21_smartcalc.a
	@$(CC) -g s21_tests.c s21_smartcalc.a -o unit_tests.out $(LIB)
	./unit_tests.out

check:
	@cppcheck *.c *.h

valgrind:
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./test

leaks: test
	CK_FORK=no leaks --atExit -- ./test
	@make clean
