// ****************************************************************************
// * Project:  Function-Pointers
// * File:     structs.h
// * Author:   Latency McLaughlin
// * Date:     08/20/2014
// ****************************************************************************
#pragma once
/*
 * Topic for discussion found here http://lnkd.in/bV6XKX6
 */
#include "stubs.h"


//-------------------------------------------------
// Macros
//
typedef void * fptr_t;

#define FUNC(func)  \
        fptr_t (_ ## func)(struct Signatures *container)

#define FUNC_PTR(func)  \
        fptr_t (*_ ## func)(struct Signatures *)

#define COMMAND(name, type, method)  { #name, type, &method }

#define ARYSIZE(ary) (sizeof(ary) / sizeof(ary[0]))


//-------------------------------------------------
// Parameter Containers For Signature Types
// XXX Private Access XXX
typedef struct {
  string  param1;
} type1_t;

typedef struct {
  int     param1,
          param2;
} type2_t;

typedef struct {
  string  param1,
          param2;
} type3_t;


//-------------------------------------------------
// Encapsulation Containers
// Signature Container
// XXX Sealed Public Access XXX
struct Signatures {
  fptr_t address;

  union {
    type1_t _1;
    type2_t _2;
    type3_t _3;
  } type;
};


//-------------------------------------------------
// Token Classification
//
typedef struct {
  string  param;
  int     is_numeric;
} set_t;


//-------------------------------------------------
// Jump Table Layout
//
struct Command {
  string name;
  FUNC_PTR(action);
  fptr_t address;
};
