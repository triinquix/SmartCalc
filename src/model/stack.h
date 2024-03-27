#ifndef STACK_H
#define STACK_H
#include <stdlib.h>

typedef struct stack_struct {
  long double* stack;
  int stack_size;
} stack;

stack create_stack();

void push(stack* a, long double number);

void pop(stack* a);

void free_stack(stack* a);

#endif