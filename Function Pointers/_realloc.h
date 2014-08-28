/* **************************************************************************
 * Project:  Function-Pointers
 * File:     _realloc.h
 * Author:   Latency McLaughlin
 * Date:     08/20/2014
 ****************************************************************************/
#pragma once
/*
 * Topic for discussion found here http://lnkd.in/bV6XKX6
 */
#include "config.h"
#pragma warning( push)
#pragma warning( disable : 4001)
#include <stdlib.h>
#pragma warning( pop)


/* -------------------------------------------------
 *  Wrapper for realloc()
 */
inline void *_realloc(void *ary, size_t size) {
  void *orig_ary = ary;
  ary = realloc(ary, size);
  if (ary == NULL) {
    free(orig_ary);          /* free original block */
    perror("Unable to reallocate memory!");
    exit(1);
  }
  return ary;
}
