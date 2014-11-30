/* **************************************************************************
 * Project:  Function-Pointers
 * File:     split.c
 * Author:   Latency McLaughlin
 * Date:     08/20/2014
 ****************************************************************************/
/*
 * Topic for discussion found here http://lnkd.in/bV6XKX6
 */
#pragma warning( push)
#pragma warning( disable : 4001)
#include <ctype.h>    /* isspace */
#include <stdlib.h>   /* exit, perror */
#include <malloc.h>   /* _msize */
#pragma warning( pop)
#include "string.h"
#include "wrappers/_free.h"
#include "wrappers/_realloc.h"
#include "wrappers/_malloc.h"


/* -------------------------------------------------
 * Spit a string into tokens
 */
size_t split(string *ary[], const string str) {
  string tok = NULL;
  size_t pos = 0,
         idx = 0,
         ary_size = 0,
         tok_size = 0,
         found = 0;

  if (*ary) {
    perror("Array construct must be NULL!");
    exit(1);
  }

  idx = 0;
  for (string it = str; *it; ++it) {
    if (!found) {
      *ary = _realloc(*ary, ary_size + sizeof(string));

      /* Create new token */
      _malloc(&tok, 0);
      *(*ary + idx) = tok;

      ary_size = _msize(*ary);
    }

    found = 1;
    if (isspace(*it)) {
      Assign: {
        tok = _realloc(tok, tok_size + sizeof(char));
        tok[pos] = '\0';

        /* Skip whitespace */
        while (isspace(*it)) it++;
        it--;

        tok_size = 0;
        idx++;
        pos = 0;
        found = 0;
      }
    } else {
      tok = _realloc(tok, tok_size + sizeof(char));
      tok[pos++] = *it;
      tok_size = _msize(tok);
    }
  }

  if (found)
    goto Assign;

  /* Add terminator element */
  *ary = _realloc(*ary, ary_size + sizeof(string));
  *(*ary + idx) = NULL;

  return idx;
}