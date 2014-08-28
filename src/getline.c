// ****************************************************************************
// * Project:  Function-Pointers
// * File:     getline.c
// * Author:   Latency McLaughlin
// * Date:     08/20/2014
// ****************************************************************************
#if defined(WIN32)
/*
 * Topic for discussion found here http://lnkd.in/bV6XKX6
 */
#include <stdlib.h>
#include <stdio.h>
#include "string.h"


//-------------------------------------------------
// Helper method
// XXX Public Access XXX
string getline() {
  string line  = malloc(100),
         linep = line;
  size_t lenmax = 100,
         len = lenmax;
  int c;

  if (line == NULL)
    return NULL;

  for (;;) {
    c = fgetc(stdin);
    if (c == EOF)
      break;

    if (--len == 0) {
      len = lenmax;
      string linen = realloc(linep, lenmax *= 2);

      if (linen == NULL) {
        free(linep);
        return NULL;
      }
      line = linen + (line - linep);
      linep = linen;
    }

    if ((*line++ = c) == '\n')
      break;
  }
  *line = '\0';
  return linep;
}
#endif
