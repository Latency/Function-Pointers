// ****************************************************************************
// * Project:  Function-Pointers
// * File:     is_numeric.c
// * Author:   Latency McLaughlin
// * Date:     08/20/2014
// ****************************************************************************
/*
 * Topic for discussion found here http://lnkd.in/bV6XKX6
 */
#include <ctype.h>    // isdigit
#include "string.h"


//-------------------------------------------------
// Check if a string is numeric
// XXX Public Access XXX
int is_numeric(const string str) {
  for (string it = str; *it; ++it) {
    if (!isdigit(*it))
      return 0;
  }
  return 1;
}