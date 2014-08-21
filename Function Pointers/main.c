/*
* Topic for discussion found here http://lnkd.in/bV6XKX6
*/
// Where is the library for hashmap in plain old C?
// I am not going to reinvent the wheel.
// FOrget it, I will just do it the old fashioned way without.

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>

typedef void * fptr_t;
#define PARAMS(args)  \
        fptr_t Type##args(fptr_t userdata)
#define FUNC_PTR(func)  \
        fptr_t (*func)(fptr_t userdata)

// Function pointer method signatures
typedef char* (*signature0_t)();
typedef char* (*signature1_t)(char *);
typedef int   (*signature2_t)(int, int);
typedef char* (*signature3_t)(char *, char *);

// Signature Prototypes - Use a unique/hashkey value as argument
#define SIG_TYPE(x) Type##x
PARAMS(0);
PARAMS(1);
PARAMS(2);
PARAMS(3);

//-------------------------------------------------
// Encapsulation Container
//
struct signatures {
  void *  address;
  union {
    char *param1;
  } type1;
  union {
    int   param1,
          param2;
  } type2;
  union {
    char *param1,
         *param2;
  } type3;
};


//-------------------------------------------------
// Method Commands
//
char * hello_command() {
  return "Hello World!";
}

char * hello_again_command(char *a) {
  return "Hello Again World!";
}

int add_command(int a, int b) {
  return a + b;
}

char* append_command(char *a, char *b) {
  return strcat(a, b);
}


//-------------------------------------------------
// Jump Table
//
#define COMMAND(name, type, method)  { #name, type, &method }

struct command {
  char *name;
  FUNC_PTR(action);
  void *address;
} commands[] = {
  COMMAND(hello,  SIG_TYPE(0), hello_command),
  COMMAND(hello,  SIG_TYPE(1), hello_again_command),
  COMMAND(add,    SIG_TYPE(2), add_command),
  COMMAND(append, SIG_TYPE(3), append_command)
};


//-------------------------------------------------
// Parsers for function pointer invocation
// XXX Private Access XXX
PARAMS(Type1) {
  type1_t *container = (type1_t *)userdata;
  signature0_t action = (signature0_t)container->address;
  return action();
}

PARAMS(Type2) {
  type2_t *container = (type2_t *)userdata;
  signature1_t action = (signature1_t)container->address;
  return (void *)action(container->param1, container->param2);
}

PARAMS(Type3) {
  type3_t *container = (type3_t *)userdata;
  signature2_t action = (signature2_t)container->address;
  return (void *)action(container->param1, container->param2);
}


//-------------------------------------------------
// Helper method
// XXX Public Access XXX
char * getline(void) {
  char * line = (char *)malloc(100), *linep = line;
  size_t lenmax = 100, len = lenmax;
  int c;

  if (line == NULL)
    return NULL;

  for (;;) {
    c = fgetc(stdin);
    if (c == EOF)
      break;

    if (--len == 0) {
      len = lenmax;
      char * linen = (char *)realloc(linep, lenmax *= 2);

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


//-------------------------------------------------
// Entry Point
// XXX Public Access XXX
int main() {
  char buf[256];
  for (size_t x = 0; x < sizeof(commands); x++)
    sprintf(buf + strlen(buf), "%s%s", !strlen(buf) ? "" : " | ", commands[x].name);
  printf("Enter in a command:  (%s | exit)\n", buf);

  char *p;
  do {
    printf("> ");
    p = getline();
    // Convert to lowercase for strcmp. OR use strcasecmp for POSIX.1-2001 and 4.4BSD.
    for (; *p; ++p) *p = tolower(*p);

    va_list ref;
    va_start(ref, p);
    char * arg = va_arg(ref, char *);
    // Parse input
    for (size_t x = 0; x < sizeof(commands); x++) {
      char *format;
      struct signatures signature;

      if (!strcmp(commands[x].name, arg)) {
        signature.address = commands[x].address;

        // Determine type for fill.
        if (commands[x].action == Type1) {
          format = "%s";
        } else if (commands[x].action == Type2) {
          signature.type2.param1 = atoi(va_arg(ref, char *));
          signature.type2.param2 = atoi(va_arg(ref, char *));
          format = "%d %d";
        } else if (commands[x].action == Type3) {
          signature.type2.param1 = va_arg(ref, char *);
          signature.type2.param2 = va_arg(ref, char *);
          format = "%s %s";
        } else {
          puts("Signature type not found!");
          break;
        }

        fptr_t data = (commands[x].action)(&signature);

        // Output the data
        puts((char *)data);
        break;
      }
    }

    va_end(ref);
  } while (strcmp(p, "exit"));

  return 0;
}