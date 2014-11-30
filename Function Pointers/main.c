/* **************************************************************************
 * Project:  Function-Pointers
 * File:     main.c
 * Author:   Latency McLaughlin
 * Date:     08/20/2014
 ****************************************************************************/
/*
 * Topic for discussion found here http: *lnkd.in/bV6XKX6
 *
 * Use C99 compilation rules for 'inline' types.
 * > gcc -std=c99 -c main.c
 *
 * MSVC++ or MSVC > v2013
 */


#pragma warning( push)
#pragma warning( disable : 4001)
#include <stdio.h>    /* BUFSIZ */
#include <malloc.h>	  /* _msize */
#include <stdlib.h>   /* atoi */
#include <ctype.h>    /* tolower */
#pragma warning( pop)
#include "signatures.h"
#include "wrappers/_free.h"
#include "wrappers/_malloc.h"
#include "wrappers/_realloc.h"


/* -------------------------------------------------
 * External prototypes
 */
extern string    getline(void);
extern size_t    split(string **, const string a);
extern size_t    lowercase(string *, const string);
extern void      classify_tokens(set_t **, const string *);
extern void      Initialize(struct Command_List *);


/* -------------------------------------------------
 * Entry Point
 */
#if defined(_DEBUG)
int main(int argc, char **argv) {
#else
int main(void) {
#endif
  struct Command_List list = { 0 };
  Initialize(&list);

  size_t cmd_count = list.size,
         ary_size = 0,
         idx = 0;
  string buf = NULL,
        *ary = NULL;

  for (size_t x = 0; x < cmd_count; x++) {
    short duplicates = 0;

    for (size_t y = 0; y < idx; y++) {
      string ptr = ary[y];
      if (!strcmp(list.cmds[x]->name, (!ptr ? "" : ptr))) {
        duplicates++;
        break;
      }
    }

    if (!duplicates) {
      /* Dynamically allocate size of string for optomial memory usage */
      string tmp;
      _malloc(&tmp,
              (buf != NULL ? strlen(buf) : 0) +
              (buf != NULL ? 3 : 0) /* strlen(" | ") or strlen("") */ +
              strlen(list.cmds[x]->name) +
              1 /* '\0' */
            );

#if defined(WIN32) && !defined(_CRT_SECURE_NO_WARNINGS)
      sprintf_s(tmp, _msize(tmp), "%s%s%s", ((buf && *buf) ? buf : ""), ((buf && *buf) ? " | " : ""), list.cmds[x]->name);
#else
      sprintf(tmp, "%s%s%s", ((buf && *buf) ? buf : ""), ((buf && *buf) ? " | " : ""), list.cmds[x]->name);
#endif
      _free(buf);    /* Cleanup old memory location */
      buf = tmp;

      /* Assign command to list */
      /* Reallocate and show new size: */
      ary = _realloc(ary, ary_size + sizeof(string));
      ary[idx++] = list.cmds[x]->name;
#ifdef WIN32
      ary_size = _msize(ary);
#else
      ary_size = malloc_usable_size(ary);
#endif
    }
  }
  printf("Enter in a command:  (%s | exit)\n", buf);

  string p;
  for (;;) {
#if defined(_DEBUG)
    char buf[BUFSIZ] = { "" };
    for (int y = 1; y < argc; ++y)
#if defined(WIN32) && !defined(_CRT_SECURE_NO_WARNINGS)
      sprintf_s(buf + strlen(buf), BUFSIZ - 1, " %s", argv[y]);
#else
      sprintf(buf + strlen(buf), " %s", argv[y]);
#endif
    p = buf;
#else /* Merge the args from the command line into one string */
    printf("> ");
    p = getline();
#endif
    /* Left trim whitespace */
    while (isspace(*p)) p++;

    string tmp = NULL;
    lowercase(&tmp, p);

    /* Tokenize string */
    string *args = NULL;
    size_t token_count = split(&args, tmp);

    /* Cleanup resources since memory copied to 'args' */
    _free(tmp);

    /* Classify token reference pointers mapping to 'args' */
    set_t *tokens = NULL;      /* tokens[token_count] */
    classify_tokens(&tokens, args);

    if (!strcmp(tokens[0].param, "exit"))
      break;

    /* Parse input */
    for (size_t x = 0; x < cmd_count; x++) {
      for (size_t y = 0; y < token_count; y++) {
        if (!strcmp(list.cmds[x]->name, (!tokens[y].param ? "" : tokens[y].param))) {
          /* Detect signature type from arguments */
          struct Signatures signature;
          signature.address = list.cmds[x]->address;
 
          /* Determine type for container fill / BOOST_STATIC_ASSERT(boost::is_same<T, U>::value); */
          if (list.cmds[x]->action == _0 && !tokens[1].param) {
            ;
          } else if (list.cmds[x]->action == SIG_NAME(1) &&
              tokens[1].param && !tokens[1].is_numeric) {
            signature.arg._1.param1 = tokens[1].param;
          } else if (list.cmds[x]->action == SIG_NAME(2) &&
              tokens[1].param && !tokens[1].is_numeric &&
              tokens[2].param && !tokens[2].is_numeric) {
            signature.arg._2.param1 = tokens[1].param;
            signature.arg._2.param2 = tokens[2].param;
          } else if (list.cmds[x]->action == SIG_NAME(3) &&
              tokens[1].param && tokens[1].is_numeric &&
              tokens[2].param && tokens[2].is_numeric) {
            signature.arg._3.param1 = atoi(tokens[1].param);
            signature.arg._3.param2 = atoi(tokens[2].param);
          } else
            break;

          (list.cmds[x]->action)(&signature);

          /*
           * Type-cast the data according to the stub retval type.  For value types,
           * a string cast will be acceptable and serve as duel funcationality for output.
           *
           * Since special consideration was made for marking which signature
           * types need cleanup handling, internal cleanup handling will be done
           * here marked by 'is_disposed'.  This is due to the return value of the
           * methods being called which have dynamically allocated memory internally.

           * Special consideration for retval types that are numeric.
           * Add more here if applicable.
           */
          if (list.cmds[x]->action == SIG_NAME(3)) {
            char buff[33];
#if defined(WIN32)
            _itoa_s((int)signature.data, buff, 32, 10);
#else
            itoa((int)signature.data, buff, 10);
#endif
            signature.data = buff;
          }

          /* Output the data */
          puts((string) signature.data);

          /* Internal cleanup */
          if (!list.cmds[x]->is_disposed)
            _free(signature.data);

          goto Cleanup;
        }
      }
    }
    /* Finalizers */
  Cleanup: {
      /* Cleanup reference pointers from 'tokens' */
      _free(tokens);

      /* Cleanup pointer to array of 'args' */
      ary_size = 0;
      for (string it = args[0]; it; it = args[++ary_size]) {
        _free(it);
      }
      _free(args);
    }
  }

  return 0;
}
