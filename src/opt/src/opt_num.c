/* OPT v3.19: options parsing tool */
/*
 * This SOFTWARE has been authored by an employee of the University of
 * California, operator of the Los Alamos National Laboratory under
 * Contract No. W-7405-ENG-36 with the U.S. Department of Energy. The
 * U.S. Government has rights to use, reproduce, and distribute this
 * SOFTWARE. Neither the Government nor the University makes any
 * warranty, express or implied, or assumes any liability or
 * responsibility for the use of this SOFTWARE. If SOFTWARE is modified
 * to produce derivative works, such modified SOFTWARE should be clearly
 * marked, so as not to confuse it with the version available from LANL.
 * 
 * Additionally, this program is free software; you can distribute it
 * and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or any later version.  Accordingly, this program is
 * distributed in the hope that it will be useful, but WITHOUT A
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details (http://www.gnu.org/copyleft/gpl.txt).
 * 
 * 
 * The OPT package is available from
 * http://nis-www.lanl.gov/~jt/Software
 */
/* $Id: opt_num.c,v 3.5 2001/08/22 19:50:08 jt Exp $ */
/* isreal: returns whether a string corresponds to
 * a real number
 */

#ifndef SINGLEFILE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include "opt.h"
#include "ag.h"
#include "opt_p.h"
#endif

#define IsSpace(t) isspace((int)(t))
#define IsDigit(t) isdigit((int)(t))
#define IsSign(t)  ((t)=='+' || (t)=='-')

int
opt_isvalidnumber(char *t)
{
  int status = OPT_NUM_VALID; /* innocent until proven guilty */
  
  if (!t) return FALSE;       /* null string is not a number */

  while( *t != '\0' && IsSpace(*t) ) ++t;   /* leading white space */
  
  if   ( *t != '\0' && IsSign(*t)  ) {      /* leading sign */
    if ( *t == '-' ) status |= OPT_NUM_NEG;
    ++t;  
  }

  if   (*t=='e' || *t=='E' ) return FALSE;  /* 'e5' not allowed! */
  
  while( *t != '\0' && IsDigit(*t) ) ++t;   /* many digits */
  if   ( *t != '\0' && ( *t=='.' ) ) {      /* at most one decimal point */
    status |= OPT_NUM_FLOAT;
    ++t;
  }
  while( *t != '\0' && IsDigit(*t) ) ++t;   /* more digits */

  /* check for exponent */
  if   (*t=='e' || *t=='E' ) {
    ++t;
    if ( *t == '\0' ) return FALSE;  /* '5e' isn't a number! */
    if ( *t != '\0' && IsSign(*t) ) {  
      if ( *t == '-' ) status |= OPT_NUM_FLOAT;  /* '1e-3' = 0.001 is FLOAT */
      ++t;
    }
    while( *t !=  '\0' && IsDigit(*t) ) ++t;
  }

  /* trailing space is okay */
  while( *t != '\0' && IsSpace(*t) ) ++t; 

  /* if made it to the end then its a real */
  return (*t=='\0') ? status : FALSE;

}

/* return numeric value of string: basically a wrapper on atof() */

double
opt_atof(char *s)
{
  int valid;
  double x;
  x = atof(s); /* Call atof() whether or not string is valid,
                * because some strings, eg '15x' can still be
                * interpreted as numeric.  But give a warning
                * unless the string really is valid
                */
  valid = opt_isvalidnumber(s);
  if (!valid) {
    opt_warn_2("String [%s] is not a valid number, will use [%g]",s,x);
  }
  return x;
}
long
opt_atoi(char *s)
{
  int valid;
  long x;
  x = (long)atof(s); /* Call atof() whether or not string is valid,
                      * because some strings, eg '15x' can still be
                      * interpreted as numeric.  But give a warning
                      * unless the string really is valid
                      */
  
  valid = opt_isvalidnumber(s);
  if (!valid || (valid & OPT_NUM_FLOAT)) {
    opt_warn_2("String [%s] is not a valid integer, will use [%ld]",s,x);
  }
  return x;
}

unsigned long
opt_atou(char *s)
{
  int valid;
  unsigned long x;
  x = (unsigned long)atof(s);
  valid = opt_isvalidnumber(s);
  if (!valid ||
      (valid & OPT_NUM_FLOAT) ||
      (valid & OPT_NUM_NEG)) {
    opt_warn_2("String [%s] is not a valid positive integer, will use [%lu]",s,x);
  }
  return x;
}

