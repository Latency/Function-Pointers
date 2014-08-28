// ****************************************************************************
// * Project:  Function-Pointers
// * File:     _free.c
// * Author:   Latency McLaughlin
// * Date:     08/20/2014
// ****************************************************************************
/*
 * Topic for discussion found here http://lnkd.in/bV6XKX6
 */
#include <stdlib.h>


//-------------------------------------------------
// Wrapper for free()
// XXX Public Access XXX
void _free(void *ptr) {
  if (ptr) {
    free(ptr);
    ptr = NULL;
  }
}