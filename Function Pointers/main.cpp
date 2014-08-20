// Where is the library for hashmap in plain old C?
// I am not going to reinvent the wheel.
// FOrget it, I will just do it the old fashioned way without.

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>

typedef void * fptr_t; 
#define FUNC(func)  \
        fptr_t func(fptr_t userdata)
#define FUNC_PTR(func)  \
        fptr_t (*func)(fptr_t userdata)

// Function pointer method signatures
typedef char* (*signature0_t)();
typedef int   (*signature1_t)(int, int);
typedef char* (*signature2_t)(char *, char *);
FUNC(Type1);
FUNC(Type2);
FUNC(Type3);

//-------------------------------------------------
// Payload Encapsulation Containers
//
typedef struct {
  void * address;
} type1_t;

typedef struct {
  void * address;
  int    param1,
         param2;
} type2_t;

typedef struct {
  void * address;
  char * param1,
       * param2;
} type3_t;


//-------------------------------------------------
// Method Commands
//
char * hello_command()
{
  return "Hello World!";
}

int add_command(int a, int b)
{
  return a + b;
}

char* append_command(char *a, char *b)
{
  return strcat(a, b);
}


//-------------------------------------------------
// Jump Table
//
struct command
{
  char *name;
  FUNC_PTR(action);
  void *address;
};

#define COMMAND(name, type)  { #name, type, & name ## _command }

struct command commands[] = {
  COMMAND(hello,  Type1),
  COMMAND(add,    Type2),
  COMMAND(append, Type3)
};


//-------------------------------------------------
// Parsers for function pointer invocation
// XXX Private Access XXX
FUNC(Type1)
{
  type1_t *container = (type1_t *)userdata;
  signature0_t action = (signature0_t)container->address;
  return action();
}

FUNC(Type2)
{
  type2_t *container = (type2_t *)userdata;
  signature1_t action = (signature1_t)container->address;
  return (void *)action(container->param1, container->param2);
}

FUNC(Type3)
{
  type3_t *container = (type3_t *)userdata;
  signature2_t action = (signature2_t)container->address;
  return (void *)action(container->param1, container->param2);
}


//-------------------------------------------------
// Helper method
// XXX Public Access XXX
char * getline(void)
{
  char * line = (char *) malloc(100), *linep = line;
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
      char * linen = (char *) realloc(linep, lenmax *= 2);

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
int main()
{
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
      void *userdata = NULL;
      char *format;

      if (!strcmp(commands[x].name, arg)) {
        // Determine type for fill.
        if (commands[x].action == Type1) {
          type1_t z = {
            commands[x].address
          };
          userdata = &z;
          format = "%s";
        } else if (commands[x].action == Type2) {
          type2_t z = {
            commands[x].address,
            atoi(va_arg(ref, char *)),
            atoi(va_arg(ref, char *))
          };
          userdata = &z;
          format = "%d %d";
        } else if (commands[x].action == Type3) {
          type3_t z = {
            va_arg(ref, char *),
            va_arg(ref, char *)
          };
          userdata = &z;
          format = "%s %s";
        } else {
          puts("Signature type not found!");
          break;
        }

        fptr_t data = (commands[x].action)(&userdata);

        // Output the data
        puts((char *)data);
        break;
      }
    }

    va_end(ref);
  } while (strcmp(p, "exit"));

  return 0;
}


