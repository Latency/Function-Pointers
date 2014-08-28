/* ***************************************************************************
 * Project:  Function-Pointers
 * File:     config.h
 * Author:   Latency McLaughlin
 * Date:     08/20/2014
 ****************************************************************************/
#pragma once
/*
 * Topic for discussion found here http://lnkd.in/bV6XKX6
 */

#if defined(__STDC__)
# pragma message("__STDC__")
# define C89
# if defined(__STDC_VERSION__)
#  pragma message("__STDC_VERSION__")
#  define C90
#  if (__STDC_VERSION__ >= 199409L)
#   define C94
#  endif
#  if (__STDC_VERSION__ >= 199901L)
#   define C99
#  endif
# endif
#endif

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L
# if defined(_MSC_VER)
#  define inline __forceinline /* use __forceinline (VC++ specific) */
# else
#  define inline               /* use standard inline */
# endif
# define register
#endif
