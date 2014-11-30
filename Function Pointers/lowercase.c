/* **************************************************************************
 * Project:  Function-Pointers
 * File:     lowercase.c
 * Author:   Latency McLaughlin
 * Date:     08/20/2014
 ****************************************************************************/
/*
 * Topic for discussion found here http://lnkd.in/bV6XKX6
 */
#pragma warning( push)
#pragma warning( disable : 4001)
#include <ctype.h>        /* tolower */
#include <stdlib.h>       /* exit, free */
#pragma warning( pop)
#include "string.h"
#include "wrappers/_malloc.h"
#include "wrappers/_realloc.h"
#include "wrappers/_free.h"


/* -------------------------------------------------
 * Convert to lowercase for strcmp. OR use strcasecmp for POSIX.1-2001 and 4.4BSD.
 * Returns:  The length of the string found in 'destination'
 */
size_t lowercase(string *destination, const string source) {
  size_t ary_size = 0;
  string c = source;

  _free(*destination);
  _malloc(&(*destination), 0);

  while (*c) {
    /* Strip the CRLF */
    if (*c == '\n' || *c == '\r')
      break;

    *destination = _realloc(*destination, ary_size + (1 * sizeof(char)));
    *(*destination + ary_size++) = (char) tolower(*c);    /* WChar vs Char? */
    c++;
  }

  *destination = _realloc(*destination, ary_size + (1 * sizeof(char)));
  *(*destination + ary_size) = '\0';

  return ary_size;
}