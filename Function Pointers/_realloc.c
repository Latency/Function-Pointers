// ****************************************************************************
// * Project:  Function-Pointers
// * File:     _realloc.c
// * Author:   Latency McLaughlin
// * Date:     08/20/2014
// ****************************************************************************
/*
 * Topic for discussion found here http://lnkd.in/bV6XKX6
 */
#include <stdlib.h>


//-------------------------------------------------
// Wrapper for realloc()
// XXX Public Access XXX
void *_realloc(void *ary, size_t size) {
  void *orig_ary = ary;
  ary = realloc(ary, size);
  if (ary == NULL) {
    free(orig_ary);          // free original block
    perror("Unable to reallocate memory!");
    exit(1);
  }
  return ary;
}