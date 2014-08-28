// ****************************************************************************
// * Project:  Function-Pointers
// * File:     stubs.c
// * Author:   Latency McLaughlin
// * Date:     08/20/2014
// ****************************************************************************
/*
 * Topic for discussion found here http://lnkd.in/bV6XKX6
 */
#include <string.h>     // memcpy, memmove, strcat, strlen
#include <malloc.h>     // _msize
#include "string.h"
#include "_malloc.h"
#include "_realloc.h"


//-------------------------------------------------
// (Method) Stubs
//
const string hello_command() {
  return "Hello World!";
}


const string hello_again_command(const string a) {
  string buf = hello_command();
  string tmp;
  _malloc(&tmp, strlen(buf) + strlen(a) + 1 /* '\0' */);
  // "Hello World!" - '\0'
  memcpy(tmp, buf, 6);
  // "Hello " + "xxxxxxxxx" + "World!\0"
  memcpy(tmp + 6 + strlen(a), buf + 5, strlen(buf) + 1 /* ' ' */ + 1 /* '\0' */ - 6 /* DONE */);
  // "Hello " + "?????????" + " World!"
  memcpy(tmp + 6, a, strlen(a));
  return tmp;
}


int add_command(int a, int b) {
  return a + b;
}


string append_command(const string a, const string b) {
  string tmp;
  _malloc(&tmp, strlen(a) + 1 /* ' ' */ + strlen(b) + 1 /* '\0' */);
  size_t len = strlen(a);
  // Copy 'a'
  memcpy(tmp + 0, a, len);
  // Add a space
  memcpy(tmp + len++, " ", 1);
  // Append 'b'
  memcpy(tmp + len, b, strlen(b) + 1);
  return tmp;
}