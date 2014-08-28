// ****************************************************************************
// * Project:  Function-Pointers
// * File:     _malloc.c
// * Author:   Latency McLaughlin
// * Date:     08/20/2014
// ****************************************************************************
/*
 * Topic for discussion found here http://lnkd.in/bV6XKX6
 */
#include <stdlib.h>
#include <stdio.h>


//-------------------------------------------------
// Wrapper for malloc()
// XXX Public Access XXX
void _malloc(void **ary, size_t size) {
  *ary = malloc(size);
  if (*ary == NULL) {
    perror("Unable to allocate memory!");
    exit(1);
  }
}
