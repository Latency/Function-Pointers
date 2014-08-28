/* **************************************************************************
// * Project:  Function-Pointers
// * File:     signatures.c
// * Author:   Latency McLaughlin
// * Date:     08/20/2014
// **************************************************************************/
#define SIGNATURE_C
/*
 * Topic for discussion found here http://lnkd.in/bV6XKX6
 */
#include "signatures.h"


/* -------------------------------------------------
 * Integer is 4 bytes and a pointer is 4 bytes in x86_32 (8 bytes in x86_64)
 * There is not guarantee that a pointer and an integer have the same size.
 * Hence, using a generic data pointer type is only supported in Von Numan
 * architechtures.
 *
 * The C standard does not specify this.
 *
 * To work around this (and address any warning that may result), explitict
 * encapsulation of the function pointer types will be required.
 * Hence as a result, we can NOP any warning disable featureness.
 *
 * Also note, that the compiler will optomize these signatures by default when linked
 * and throw out any duplicates that shall be similar.  The ordering will take
 * precidence (or may vary but predictible).
 *
 * Problematic:
 * ======================
 * When COMDAT Folding is enabled, the linker will notice that each function has
 * exactly the same code, which in fact really doesn't, since the signature types are
 * being forced to match and the body contents are different but in similar size.
 * The compiler gets confused and chooses to consolidate these into one which may be
 * okay under ordinary circumstances, but not when there is a dependancy on a specific
 * block of memory that is being compared against later in the code.
 *
 * Must enforce  (/OPT:NOICF)
 *
 * Setting a 'primary key' will hint for the compiler to NOT fold COMDATS with similar signatures.
 */
/* -------------------------------------------------
 * Function pointer method signatures & connectors for function pointer invocation.
 */
SIGNATURE(0) {
#if defined(COMDAT_FOLDING)
  container->primary_key = 0;
#endif
  container->data = (void *)container->address._0();
}

SIGNATURE(1) {
#if defined(COMDAT_FOLDING)
  container->primary_key = 1;
#endif
  container->data = (void *)container->address._1(container->arg._1.param1);
}

/* COMDAT folding WILL occur for Signatures (2 & 3) since signatures are generic and equiv. */
SIGNATURE(2) {
#if defined(COMDAT_FOLDING)
  container->primary_key = 2;
#endif
  container->data = (void *)container->address._2(container->arg._2.param1, container->arg._2.param2);
}

SIGNATURE(3) {
#if defined(COMDAT_FOLDING)
  container->primary_key = 3;
#endif
  container->data = (void *)container->address._3(container->arg._3.param1, container->arg._3.param2);
}
