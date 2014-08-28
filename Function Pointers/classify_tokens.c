/* **************************************************************************
 * Project:  Function-Pointers
 * File:     classify_tokens.c
 * Author:   Latency McLaughlin
 * Date:     08/20/2014
 ****************************************************************************/
/*
 * Topic for discussion found here http://lnkd.in/bV6XKX6
 */
#pragma warning( push)
#pragma warning( disable : 4001)
#include <stdlib.h>       /* exit, perror */
#include <malloc.h>       /* _msize */
#pragma warning( pop)
#include "structs.h"      /* set_t */
#include "_realloc.h"
#include "_isnumeric.h"


/* -------------------------------------------------
 * Classify a token by determinnig its datatype
 */
void classify_tokens(set_t *ary[], const string args[]) {
  size_t size = 0,
         idx = 0;

  if (*ary) {
    perror("Array construct must be NULL!");
    exit(1);
  }

  for (string it = args[0]; it; it = args[idx]) {
    *ary = _realloc(*ary, size + (1 * sizeof(set_t)));
    (*ary + idx)->param = it;
    (*ary + idx)->is_numeric = _isnumeric(it);

    idx++;
#ifdef WIN32
    size = _msize(*ary);
#else
    size = malloc_usable_size(*ary);
#endif
  }

  /* Add terminator element */
  *ary = _realloc(*ary, size + (1 * sizeof(set_t)));
  (*ary + idx)->param = 0;
  (*ary + idx)->is_numeric = -1;
}
