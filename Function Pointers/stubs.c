/****************************************************************************
 * Project:  Function-Pointers
 * File:     stubs.c
 * Author:   Latency McLaughlin
 * Date:     08/20/2014
 ****************************************************************************/
/*
 * Topic for discussion found here http://lnkd.in/bV6XKX6
 */
#pragma warning( push)
#pragma warning( disable : 4001)
#include <string.h>     /* memcpy, memmove, strcat, strlen */
#include <malloc.h>     /* _msize */
#pragma warning( pop)
#include "string.h"     /* string, bool */
#include "wrappers/_malloc.h"
#include "wrappers/_realloc.h"


/* -------------------------------------------------
 * (Method) Stubs
 */
#if defined(WIN32)
/* warning C4711: function 'hello_command' selected for automatic inline expansion */
# pragma warning(disable: 4711)
# pragma warning(push)
#endif
const string hello_command(void) {
  return "Hello World!";
}
#if defined(WIN32)
# pragma warning(pop)
#endif


const string hello_again_command(const string a) {
  string buf = hello_command();
  string tmp;
  _malloc(&tmp, strlen(buf) + 1 /* ' ' */ + strlen(a) + 1 /* '\0' */);
  /* "Hello World!" - '\0' */
  memcpy(tmp, buf, 6);
  /* "Hello " + "xxxxxxxxx" + "World!\0" */
  memcpy(tmp + 6 + strlen(a), buf + 5, strlen(buf) + 1 /* ' ' */ + 1 /* '\0' */ - 6 /* DONE */);
  /* "Hello " + "?????????" + " World!" */
  memcpy(tmp + 6, a, strlen(a));
  return tmp;
}


const string append_command(const string a, const string b) {
  string tmp;
  _malloc(&tmp, strlen(a) + strlen(b) + 1 /* '\0' */);
  size_t len = strlen(a);
  /* Copy 'a' */
  memcpy(tmp + 0, a, len);
  /* Append 'b' */
  memcpy(tmp + len, b, strlen(b) + 1);
  return tmp;
}


int add_command(int a, int b) {
  return a + b;
}
