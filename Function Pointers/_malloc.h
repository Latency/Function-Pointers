/* **************************************************************************
 * Project:  Function-Pointers
 * File:     _malloc.h
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
 * Wrapper for malloc()
 */
inline void _malloc(void **ary, size_t size) {
  *ary = malloc(size);
  if (*ary == NULL) {
    perror("Unable to allocate memory!");
    exit(1);
  }
}
