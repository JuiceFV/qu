/*
    <STACK DATA STRUCTURE>
    The stack data structure is pretty simple but pretty useful
    data structure.
    What is Stack?
    - It is type of linear data structure.
    - It follows LIFO (Last In First Out) property.
    - Insertion and Deletion in stack can only be done from top only.
    - Insertion in stack is also known as a PUSH operation.
    - Deletion from stack is also known as POP operation in stack.
    Stack Implementation
    - Stack implementation using array. (I used)
    - Stack implementation using linked list.
    Stack usage example
    -When function (sub-program) is called
     When a function is called, the function is called last will be completed
     first. It is the property of stack. There is a memory area, specially
     reserved for this stack.

    More information you can find here:
    https://en.wikipedia.org/wiki/Stack_(abstract_data_type)
*/
// Also I would note that I can't (I don't know if I will find a sulution then I
// will fix it) implement some behavior due to MSVC-compiler
#ifndef _STACK
#define _STACK

// Including the header which will be used as common file for the
// all data-structures-implementation.
// Now it contains:
// - Error messages
// - function which writes error logs
// - library for the memory leaks detection (#include <crtdbg.h>)
#include "common.h"
#include "error.h"

// This define will expand as
// struct stack_T{unsigned long size; T values[];}, where T is a type
// The operator "##" using as concatenation operator.
// You can read more about this here:
// https://docs.microsoft.com/en-us/cpp/preprocessor/token-pasting-operator-hash-hash?view=vs-2019
#define stack(T)        \
  struct stack_##T {    \
    unsigned long size; \
    T values[];         \
  }

#define anticipated_error 1

// Freeing memory which were allocated for the stack.
#define stack_destructor(st)                                               \
  {                                                                        \
    return_val_from_macro = 0;                                             \
    if (st && anticipated_error == 1) {                                    \
      free(st);                                                            \
      st = NULL;                                                           \
    } else {                                                               \
      char *specific_dir =                                                 \
          cat_dir_and_num("includes/stack.h:", stack_error_lines[1]);      \
      write_error_log(STACK_INACCESSIBILITY("stack_destructor"), __LINE__, \
                      __FILE__, specific_dir);                             \
      free(specific_dir);                                                  \
      return_val_from_macro = -1;                                          \
    }                                                                      \
  }                                                                        \
  }

// Splitting into two implementations due to the variety of compilers. However,
// the basic logic is the same.
#if defined(_MSC_VER)
// Constructor is allocating memory for the stack and zeroing "size"-field.
// Otherwise it writes the error into error_logs.txt.
// There are two possible errors:
// 1) The stack "st" already exists. It means that the constructor has been used
// somewhere else.
// 2) Malloc can't allocate the memory for the stack "st" for some reason.
#define stack_constructor(T, st)                                          \
  {                                                                       \
    return_val_from_macro = 0;                                            \
    if (st == NULL && anticipated_error == 1) {                           \
      if (!(st = malloc(sizeof(stack(T)))) && anticipated_error == 1) {   \
        char *specific_dir =                                              \
            cat_dir_and_num("includes/stack.h:", stack_error_lines[3]);   \
        write_error_log(MALLOC_ERROR_MESSAGE, __LINE__, __FILE__,         \
                        specific_dir);                                    \
        free(specific_dir);                                               \
        return_val_from_macro = -1;                                       \
      }                                                                   \
      st->size = 0;                                                       \
    } else {                                                              \
      char *specific_dir =                                                \
          cat_dir_and_num("includes/stack.h:", stack_error_lines[2]);     \
      write_error_log(STACK_EXISTANCE, __LINE__, __FILE__, specific_dir); \
      free(specific_dir);                                                 \
      return_val_from_macro = -1;                                         \
      stack_destructor(st);                                               \
    }                                                                     \
  }

// GNUC-compiler implementation begins
#elif defined(__GNUC__)
#define stack_constructor(T)                                                   \
  ({                                                                           \
    struct stack_##T *st;                                                      \
    return_val_from_macro = 0;                                                 \
    if (!(st = malloc(sizeof(stack(T)))) && anticipated_error == 1) {          \
      char *specific_dir =                                                     \
          cat_dir_and_num("includes/stack.h:", stack_error_lines[4]);          \
      write_error_log(MALLOC_ERROR_MESSAGE, __LINE__, __FILE__, specific_dir); \
      free(specific_dir);                                                      \
      return_val_from_macro = -1;                                              \
    }                                                                          \
    st->size = 0;                                                              \
    st;                                                                        \
  })

#endif  // end compiler-split implimentation

// Due to in the stack_push a type doesn't available we shall use another method
// of memory allocating. This method is fully described here:
// https://en.wikipedia.org/wiki/Flexible_array_member
#define stack_push(st, val)                                                    \
  {                                                                            \
    if (st && anticipated_error == 1) {                                        \
      if (st->size < MAX_SIZE && anticipated_error == 1) {                     \
        st = realloc(st,                                                       \
                     sizeof(st->size) + (st->size + 1) * sizeof(*st->values)); \
        st->values[st->size++] = (val);                                        \
      } else {                                                                 \
        char *specific_dir =                                                   \
            cat_dir_and_num("includes/stack.h:", stack_error_lines[6]);        \
        write_error_log(STACK_OVERFLOW, __LINE__, __FILE__, specific_dir);     \
        free(specific_dir);                                                    \
        return_val_from_macro = -1;                                            \
        stack_destructor(st);                                                  \
      }                                                                        \
    } else {                                                                   \
      char *specific_dir =                                                     \
          cat_dir_and_num("includes/stack.h:", stack_error_lines[5]);          \
      write_error_log(STACK_INACCESSIBILITY("stack_push(st, val)"), __LINE__,  \
                      __FILE__, specific_dir);                                 \
      free(specific_dir);                                                      \
      return_val_from_macro = -1;                                              \
    }                                                                          \
  }

// Stack-pop operation removes and returns the last element. I the
// array-implementation it reduces the stack-size.
#define stack_pop(st) st->values[--st->size]

// Check for the stack emptiness
#define stack_empty(st) st->size == 0 ? 1 : 0

// Stack-clear is the method which clears values but it doesn't clears
// whole stack. Check for stack and first element availability. If both true
// then removes values.
#define stack_clear(st)                                                      \
  do {                                                                       \
    if (st && anticipated_error == 1) {                                      \
      if (st->size == 0 && anticipated_error == 1) {                         \
        char *specific_dir =                                                 \
            cat_dir_and_num("includes/stack.h:", stack_error_lines[7]);      \
        write_error_log(STACK_EMPTINESS, __LINE__, __FILE__, specific_dir);  \
        free(specific_dir);                                                  \
        printf(                                                              \
            "Error has occured! Check error_log.txt for the more details.\n" \
            "Press any key");                                                \
        stack_destructor(st);                                                \
        return (getchar());                                                  \
      }                                                                      \
      st = realloc(st, sizeof(st->size));                                    \
      st->size = 0;                                                          \
    } else {                                                                 \
      char *specific_dir =                                                   \
          cat_dir_and_num("includes/stack.h:", stack_error_lines[8]);        \
      write_error_log(STACK_INACCESSIBILITY("stack_clear(st)"), __LINE__,    \
                      __FILE__, specific_dir);                               \
      free(specific_dir);                                                    \
      printf(                                                                \
          "Error has occured! Check error_log.txt for the more details.\n"   \
          "Press any key");                                                  \
      return (getchar());                                                    \
    }                                                                        \
  } while (0)

// Stack-peek returns the last value of the stack but it doesn't reduces the
// size (in other words it doesn't remove the last element)
#define stack_peek(st) st->values[st->size - 1]

#endif  // end of stack.h
