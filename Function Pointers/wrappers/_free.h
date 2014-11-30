/* **************************************************************************
 * Project:  Function-Pointers
 * File:     _free.h
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


/*-------------------------------------------------
 * Wrapper for free()
 */
inline void _free(void *ptr) {
  if (ptr) {
    free(ptr);
    ptr = NULL;
  }
}
