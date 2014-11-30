/* **************************************************************************
 * Project:  Function-Pointers
 * File:     jmptbl.c
 * Author:   Latency McLaughlin
 * Date:     08/20/2014
 ****************************************************************************/
#pragma once
/*
 * Topic for discussion found here http://lnkd.in/bV6XKX6
 */
#include "structs.h"
#include "signatures.h"
#include "stubs.h"
#include "wrappers/_free.h"
#include "wrappers/_malloc.h"

/* -------------------------------------------------
 * Jump Table
 *
 * A function pointer is converted to or from a data pointer.
 * This conversion is allowed under Microsoft extensions (/Ze) but not under ANSI C.
 *
 * Why?
 *
 * An architecture doesn't have to store code and data in the same memory.
 * With a Harvard architecture, code and data are stored in completely different memory.
 * Most architectures are Von Neumann architectures with code and data in the same memory
 * but C doesn't limit itself to only certain types of architectures if at all possible.
 *
 * To resolve this problem, instead of using a generic type to type-cast to/from, there will
 * need to be a union that can store the proper type associated.
 *
 * This only will work in C99+ for what is considered 'Designated Initializers'
 * This construct (bug) was fixed in VS2013 SP3 to support C99 featureness.
 */
#if defined(C99)
struct Command commands[] = {
    { "hello",  1, &_0, { ._0 = hello_command } },
    { "hello",  0, &_1, { ._1 = hello_again_command } },
    { "append", 0, &_2, { ._2 = append_command } },
    { "add",    1, &_3, { ._3 = add_command } }
};
#else
void Initialize(struct Command_List *list) {
  signature0_t s0 = hello_command;
  signature1_t s1 = hello_again_command;
  signature2_t s2 = append_command;
  signature3_t s3 = add_command;

  /* Change this if adding a new command to the list 'list->cmds[???]' */
  list->size = 4;
  /*
   * Since ANSI C forbids casting function pointers -> data pointers prior to C99,
   * Then there is no safe way to map the signatures unless done explicitly.
   *
   * This should be fairly straight forward, but messy as hell.
   * The reason is because we do not have constant l-values being assigned to a
   * predefined static container.  The language simpily does not allow for it
   * (without warning).
   */
  /* Dispose any objects allocated previously.  -sanity check- */
  if (list->cmds) {
    for (size_t x = 0; x < list->size; ++x) {
      _free(list->cmds[x]->name);
      _free(list->cmds[x]);
    }
    _free(list->cmds);
  }

  _malloc((void *)&(list->cmds), sizeof(struct Command) * list->size);
  for (size_t x = 0; x < list->size; ++x)
    _malloc((void *)&(list->cmds[x]), sizeof(struct Command));

  list->cmds[0]->name = _strdup("hello");
  list->cmds[0]->action = SIG_NAME(0);
  list->cmds[0]->address._0 = s0;
  list->cmds[0]->is_disposed = 1;

  list->cmds[1]->name = _strdup("hello");
  list->cmds[1]->action = SIG_NAME(1);
  list->cmds[1]->address._1 = s1;
  list->cmds[1]->is_disposed = 0;

  list->cmds[2]->name = _strdup("append");
  list->cmds[2]->action = SIG_NAME(2);
  list->cmds[2]->address._2 = s2;
  list->cmds[2]->is_disposed = 0;

  list->cmds[3]->name = _strdup("add");
  list->cmds[3]->action = SIG_NAME(3);
  list->cmds[3]->address._3 = s3;
  list->cmds[3]->is_disposed = 1;
}
#endif
