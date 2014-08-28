/* **************************************************************************
 * Project:  Function-Pointers
 * File:     stubs.h
 * Author:   Latency McLaughlin
 * Date:     08/20/2014
 ****************************************************************************/
#pragma once
/*
 * Topic for discussion found here http://lnkd.in/bV6XKX6
 */
#include "string.h"


/* -------------------------------------------------
 * (Method) Stub Prototypes
 */
extern const string hello_command(void);
extern const string hello_again_command(const string);
extern const string append_command(const string, const string);
extern int          add_command(int, int);
