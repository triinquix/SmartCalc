SYSSHELL:=$(shell uname)
CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11 -g
ifeq ($(SYSSHELL), Linux)
TFLAGS=-lcheck -lpthread -lrt -lsubunit --coverage -lm
else
TFLAGS=$(shell pkg-config --cflags --libs check) --coverage
endif

prefix=../../

all:
	$(CC) $(CFLAGS) $(shell pkg-config --cflags gtk4) -o smart_calc view/*.c controller/*.c model/*.c $(shell pkg-config --libs gtk4) -lm

install: all
	mv smart_calc $(prefix)
	
uninstall:
	rm $(prefix)smart_calc

main_calc.a:
	$(CC) $(CFLAGS) model/*.c -c $(shell pkg-config --cflags gtk4)
	ar rc main_calc.a *.o
	rm -rf *.o

test: clean  main_calc.a
	$(CC) $(CFLAGS) tests/test.c -c $(shell pkg-config --cflags gtk4)
	$(CC) $(CFLAGS) tests/test.c model/*.c $(shell pkg-config --cflags gtk4) $(TFLAGS) -o test -L. main_calc.a
	./test

valgrind: clean main_calc.a 
	$(CC) $(CFLAGS) tests/test.c -c $(shell pkg-config --cflags gtk4)
	$(CC) $(CFLAGS) tests/test.c model/*.c $(shell pkg-config --cflags gtk4) $(TFLAGS) -o test -L. main_calc.a
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./test

gcov_report: test
	lcov -t "s21_smart_calc" -o s21_smart_calc.info -c -d . 
	genhtml -o report s21_smart_calc.info


clean:
	rm -rf *.gcno *.info *.gcda *.o *.a test report

dist:
	mkdir tmp_dir
	cp -r  controller/ tmp_dir/
	cp -r  model/ tmp_dir/
	cp -r  view/ tmp_dir/
	cp Makefile tmp_dir/
	cp -r tests/ tmp_dir/
	cp main_app.h tmp_dir/
	tar cvzf calc-1.0.tar.gz tmp_dir
	rm -rf tmp_dir

div:
	open README.md

.PHONY: all main_calc.a test gcov_report clean prefix
