/* **************************************************************************
 * Project:  Function-Pointers
 * File:     structs.h
 * Author:   Latency McLaughlin
 * Date:     08/20/2014
 ****************************************************************************/
#pragma once
/*
 * Topic for discussion found here http://lnkd.in/bV6XKX6
 */
#include "string.h"
#pragma warning( push)
#pragma warning( disable : 4001)
#include <string.h>   /* size_t */
#pragma once


/* -------------------------------------------------
 * Parameter Containers For Signature Argument Types
 */
typedef struct {
  string  param1;
} arg_type1_t;

typedef struct {
  string  param1,
          param2;
} arg_type2_t;

typedef struct {
  int     param1,
          param2;
} arg_type3_t;

#pragma pack(push, 1)
typedef struct {
  string  param;
  bool    is_numeric;
} set_t;
#pragma pack(pop)

/* Function Pointer Signature Types */
typedef const string(*signature0_t)(void);
typedef const string(*signature1_t)(const string);
typedef const string(*signature2_t)(const string, const string);
typedef          int(*signature3_t)(int, int);


/* -------------------------------------------------
 * Encapsulation Containers
 */

/* Function Signature Types */
typedef union {
  signature0_t _0;
  signature1_t _1;
  signature2_t _2;
  signature3_t _3;
} fstype_t;


/* Argument Parameter Types */
struct Signatures {
#if defined(COMDAT_FOLDING)
  size_t primary_key;
#endif
  fstype_t address;
  void *data;
  union {
    arg_type1_t _1;
    arg_type2_t _2;
    arg_type3_t _3;
  } arg;
};


/* -------------------------------------------------
 * Jump Table Layout
 */
#pragma pack(push, 1)
struct Command {
  string name;
  bool is_disposed;
  void (*action)(struct Signatures *);
  fstype_t address;
};
#pragma pack(pop)


struct Command_List {
  size_t size;
  struct Command **cmds;
};