#include "stack.h"

stack create_stack() {
  stack res_stack = {NULL, 0};
  return res_stack;
}

void push(stack* a, long double number) {
  a->stack_size++;
  if (a->stack_size)
    a->stack =
        (long double*)realloc(a->stack, sizeof(long double) * a->stack_size);
  else
    a->stack = (long double*)malloc(sizeof(long double));
  a->stack[a->stack_size - 1] = number;
}

void pop(stack* a) {
  if (a->stack) {
    if (a->stack_size > 1)
      a->stack =
          (long double*)realloc(a->stack, sizeof(long double) * a->stack_size);
    else {
      free(a->stack);
      a->stack = NULL;
    }
    a->stack_size--;
  }
}

void free_stack(stack* a) {
  if (a->stack) {
    free(a->stack);
    a->stack = NULL;
  }
  a->stack_size = 0;
}