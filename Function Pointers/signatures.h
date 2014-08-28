/* **************************************************************************
 * Project:  Function-Pointers
 * File:     signatures.h
 * Author:   Latency McLaughlin
 * Date:     08/20/2014
 ****************************************************************************/
#pragma once
/*
 * Topic for discussion found here http://lnkd.in/bV6XKX6
 */
#include "structs.h"


/* -------------------------------------------------
 * Macros
 */
#define SIG_NAME(name)  _ ## name
#define SIGNATURE(name) void _##name(struct Signatures *container)


/* -------------------------------------------------
 * (Method) Stub Prototypes
 */

/* C4115: named type definition in parentheses */
#pragma warning(disable: 4115)
#pragma warning(push)
#if !defined(SIGNATURE_C)
 extern SIGNATURE(0);
 extern SIGNATURE(1);
 extern SIGNATURE(2);
 extern SIGNATURE(3);
#endif
#pragma warning(pop)
