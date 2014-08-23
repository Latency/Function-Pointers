// ****************************************************************************
// * Project:  Function-Pointers
// * File:     lowercase.c
// * Author:   Latency McLaughlin
// * Date:     08/20/2014
// ****************************************************************************
/*
 * Topic for discussion found here http://lnkd.in/bV6XKX6
 */
#include <ctype.h>        // tolower
#include <stdlib.h>       // exit, free
#include "string.h"
#include "_malloc.h"
#include "_realloc.h"
#include "_free.h"


//-------------------------------------------------
// Convert to lowercase for strcmp. OR use strcasecmp for POSIX.1-2001 and 4.4BSD.
// Returns:  The length of the string found in 'destination'
// XXX Public Access XXX
size_t lowercase(string *destination, const string source) {
  size_t ary_size = 0;
  string c = source;

  _free(*destination);
  _malloc(&(*destination), 0);

  while (*c) {
    // Strip the CRLF
    if (*c == '\n' || *c == '\r')
      break;

    *destination = _realloc(*destination, ary_size + (1 * sizeof(char)));
    *(*destination + ary_size++) = tolower(*c);
    c++;
  }

  *destination = _realloc(*destination, ary_size + (1 * sizeof(char)));
  *(*destination + ary_size) = '\0';

  return ary_size;
}