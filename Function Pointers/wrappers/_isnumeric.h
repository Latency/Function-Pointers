/* **************************************************************************
 * Project:  Function-Pointers
 * File:     _isnumeric.h
 * Author:   Latency McLaughlin
 * Date:     08/20/2014
 ****************************************************************************/
#pragma once
/*
 * Topic for discussion found here http://lnkd.in/bV6XKX6
 */
#pragma warning( push)
#pragma warning( disable : 4001)
#include <ctype.h>    /* isdigit */
#pragma warning( pop)
#include "config.h"
#include "string.h"


#if defined(WIN32)
/* C4514: Suppress 'unreferenced inline function has been removed' */
#pragma warning(disable: 4514)
#pragma warning(push)
#endif
/* -------------------------------------------------
 * Check if a string is numeric
 */
inline bool _isnumeric(const string str) {
  for (string it = str; *it; ++it) {
    if (!isdigit(*it))
      return false;
  }
  return true;
}
#if defined(WIN32)
#pragma warning(pop)
#endif
