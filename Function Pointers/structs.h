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


#ifdef _MSC_VER
  #define INLINE __forceinline // use __forceinline (VC++ specific)
#else
  #define INLINE inline        // use standard inline
#endif


//-------------------------------------------------
// Macros
//
typedef void * fptr_t;

#define SIG_TYPE(x) Type##x   // Use a unique/hashkey value as argument

#define PARAMS(key)  \
        INLINE fptr_t SIG_TYPE(key)(struct Signatures *container)

#define FUNC_PTR(func)  \
        fptr_t (*func)(struct Signatures *)

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


//-------------------------------------------------
// Function pointer method signatures &
// Inlined connectors for function pointer invocation
// XXX Private Access XXX
PARAMS(0) {
  typedef const string(*signature0_t)(void);
  signature0_t signature = (signature0_t)container->address;
  return (void *) signature();
}

PARAMS(1) {
  typedef const string(*signature1_t)(const string);
  signature1_t signature = (signature1_t)container->address;
  return (void *) signature(container->type._1.param1);
}

PARAMS(2) {
  typedef int(*signature2_t)(int, int);
  signature2_t signature = (signature2_t)container->address;
  return (void *) signature(container->type._2.param1, container->type._2.param2);
}

PARAMS(3) {
  typedef const string(*signature3_t)(const string, const string);
  signature3_t signature = (signature3_t)container->address;
  return (void *) signature(container->type._3.param1, container->type._3.param2);
}