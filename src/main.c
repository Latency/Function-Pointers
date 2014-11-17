// ****************************************************************************
// * Project:  Function-Pointers
// * File:     main.c
// * Author:   Latency McLaughlin
// * Date:     08/20/2014
// ****************************************************************************
/*
 * Topic for discussion found here http://lnkd.in/bV6XKX6
 *
 * Use C99 compilation rules for 'inline' types.
 * > gcc -std=c99 -c main.c
 */
#ifndef WIN32
# define _GNU_SOURCE
#endif
#include <string.h>   // strcmp, strlen
#include <stdio.h>    // BUFSIZ
#include <malloc.h>	  // _msize / 
#include <stdlib.h>   // atoi
#include <ctype.h>    // tolower
#include "structs.h"
#include "_free.h"
#include "_malloc.h"
#include "_realloc.h"


//-------------------------------------------------
// External prototypes
//
extern string    getline();
extern size_t    split(string **, const string a);
extern size_t    lowercase(string *, const string);
extern void      classify_tokens(set_t **, const string *);


//-------------------------------------------------
// Integer is 4 bytes and a pointer is 4 bytes in x86_32 (8 bytes in x86_64)
// There is not guarantee that a pointer and an integer have the same size.
// The C standard does not specify this.
//-------------------------------------------------
// Function pointer method signatures &
// connectors for function pointer invocation
// XXX Private Access XXX
FUNC(0) {
  typedef const string(*signature0_t)(void);
  signature0_t signature = (signature0_t)container->address;
  return (void *) signature();
}

FUNC(1) {
  typedef const string(*signature1_t)(const string);
  signature1_t signature = (signature1_t)container->address;
  return (void *) signature(container->type._1.param1);
}

FUNC(2) {
  typedef int(*signature2_t)(int, int);
  signature2_t signature = (signature2_t)container->address;
  return (void *) signature(container->type._2.param1, container->type._2.param2);
}

FUNC(3) {
  typedef const string(*signature3_t)(const string, const string);
  signature3_t signature = (signature3_t)container->address;
  return (void *) signature(container->type._3.param1, container->type._3.param2);
}


//-------------------------------------------------
// Jump Table
//
struct Command commands[] = {
  COMMAND(hello,  &FUNC(0), hello_command),
  COMMAND(hello,  &FUNC(1), hello_again_command),
  COMMAND(add,    &FUNC(2), add_command),
  COMMAND(append, &FUNC(3), append_command)
};


//-------------------------------------------------
// Entry Point
// XXX Public Access XXX
int main(int argc, char *argv[], char *envp[]) {
  size_t cmd_count = ARYSIZE(commands),
         ary_size = 0,
         idx = 0;
  string buf = NULL,
        *ary = NULL;

  for (size_t x = 0; x < cmd_count; x++) {
    short duplicates = 0;

    for (size_t y = 0; y < idx; y++) {
      string ptr = ary[y];
      if (!strcmp(commands[x].name, (!ptr ? "" : ptr))) {
        duplicates++;
        break;
      }
    }

    if (!duplicates) {
      // Dynamically allocate size of string for optomial memory usage
      string tmp;
      _malloc(&tmp,
              (buf != NULL ? strlen(buf) : 0) +
              (buf != NULL ? 3 : 0) /* strlen(" | ") or strlen("") */ +
              strlen(commands[x].name) +
              1 /* '\0' */
            );

#if defined(WIN32) && !defined(_CRT_SECURE_NO_WARNINGS)
      sprintf_s(tmp, _msize(tmp), "%s%s%s", ((buf && *buf) ? buf : ""), ((buf && *buf) ? " | " : ""), commands[x].name);
#else
      sprintf(tmp, "%s%s%s", ((buf && *buf) ? buf : ""), ((buf && *buf) ? " | " : ""), commands[x].name);
#endif
      _free(buf);    // Cleanup old memory location
      buf = tmp;

      // Assign command to list
      // Reallocate and show new size:
      ary = _realloc(ary, ary_size + sizeof(string));
      ary[idx++] = commands[x].name;
#ifdef WIN32
      ary_size = _msize(ary);
#else
      ary_size = malloc_usable_size(ary);
#endif
    }
  }
  printf("Enter in a command:  (%s | exit)\n", buf);

  string p;
  while (1) {
#if _DEBUG
    char buf[BUFSIZ] = { "" };
    for (int y = 1; y < argc; ++y)
#if defined(WIN32) && !defined(_CRT_SECURE_NO_WARNINGS)
      sprintf_s(buf + strlen(buf), BUFSIZ - 1, " %s", argv[y]);
#else
      sprintf(buf + strlen(buf), " %s", argv[y]);
#endif
    p = buf;
#else // Merge the args from the command line into one string
    printf("> ");
    p = getline();
#endif
    // Left trim whitespace
    while (isspace(*p)) p++;

    string tmp = NULL;
    lowercase(&tmp, p);

    // Tokenize string
    string *args = NULL;
    size_t token_count = split(&args, tmp);

    // Cleanup resources since memory copied to 'args'
    _free(tmp);

    // Classify token reference pointers mapping to 'args'
    set_t *tokens = NULL;      // tokens[token_count]
    classify_tokens(&tokens, args);

    if (!strcmp(tokens[0].param, "exit"))
      break;

    short dispose = 0;
    // Parse input
    for (size_t x = 0; x < cmd_count; x++) {
      for (size_t y = 0; y < token_count; y++) {
        if (!strcmp(commands[x].name, (!tokens[y].param ? "" : tokens[y].param))) {
          // Detect signature type from arguments
          struct Signatures signature;
          signature.address = commands[x].address;

          // Determine type for container fill
          if (commands[x].action == FUNC_PTR(0) && !tokens[1].param) {
            ;
          } else if (commands[x].action == FUNC_PTR(1) &&
              tokens[1].param && !tokens[1].is_numeric) {
            signature.type._1.param1 = tokens[1].param;
            dispose = 1;
          } else if (commands[x].action == FUNC_PTR(2) &&
              tokens[1].param && tokens[1].is_numeric &&
              tokens[2].param && tokens[2].is_numeric) {
            signature.type._2.param1 = atoi(tokens[1].param);
            signature.type._2.param2 = atoi(tokens[2].param);
          } else if (commands[x].action == FUNC_PTR(3) &&
              tokens[1].param && !tokens[1].is_numeric &&
              tokens[2].param && !tokens[2].is_numeric) {
            signature.type._3.param1 = tokens[1].param;
            signature.type._3.param2 = tokens[2].param;
            dispose = 1;
          } else
            break;

          fptr_t data = (commands[x].action)(&signature);

          // Output the data
          puts((char *)data);

          // Internal cleanup
          if (dispose) {
            _free(data);
            dispose = 0;
          }

          goto Cleanup;
        }
      }
    }
    // Finalizers
  Cleanup: {
      // Cleanup reference pointers from 'tokens'
      _free(tokens);

      // Cleanup pointer to array of 'args'
      ary_size = 0;
      for (string it = args[0]; it; it = args[++ary_size]) {
        _free(it);
      }
      _free(args);
    }
  }

  return 0;
}