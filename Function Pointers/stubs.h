// ****************************************************************************
// * Project:  Function-Pointers
// * File:     stubs.h
// * Author:   Latency McLaughlin
// * Date:     08/20/2014
// ****************************************************************************
#pragma once
/*
 * Topic for discussion found here http://lnkd.in/bV6XKX6
 */
#include "string.h"


//-------------------------------------------------
// (Method) Stub Prototypes
//
extern string hello_command();
extern string hello_again_command(const string);
extern int    add_command(int, int);
extern string append_command(string, const string);