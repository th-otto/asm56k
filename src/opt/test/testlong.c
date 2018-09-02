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
/* testlong.c */

/* A test program for long options and description strings */

#include <stdio.h>
#include <opt.h>
#include <string.h>

int main(int argc, char** argv) {
  int u = 0;
  unsigned int v=9;
  int x = 1;
  double y = 3.14159;
  char* z = 0;
  int w=0;

  optrega(&x, OPT_INT, 'x', 
	  "this-is-a-ridiculously-long-option-for-the-x-value",
	  "But at least the description is short");
  optregsb(&v, OPT_UINT, "vvvvvvvv","v is all there is to say");
  optregsb(&u, OPT_INT, "uuuuuuu",
	  "The u option is also notable for having an option description string that is far longer than normal and that is expressly designed to test opt's ability to handle such long descriptions without falling flat on its face; u alsohas a string-name instead of a simple character-name.");
           
  optrega(&y, OPT_DOUBLE, 'y', "y-value",
	  "The y option is most notable for having an option description string that is far longer than normal and that is expressly designed to test opt's ability to handle such long descriptions without falling flat on its face.");
  optrega(&z, OPT_STRING, 'z', "this-is-a-ridiculously-long-option-for-the-z-value", "The z option shares the worst characteristics of both the x and y options, having both a long long name, AND a long description string. Let's see how it handles this one then...");
  optreg(&w,OPT_BOOL,'w',"descriptionIsOneLongWwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwword");

  opt(&argc, &argv);
  opt_free();
  
  printf("x = %d\n", x);

  /* Test some values */
  if (x == 42) {
    printf("Opt has found the answer!\n");
  }

  if (z && !strcmp(z, "An exceptionally long value string to go with all these long options")) {
    printf("Opt can handle the big stuff!\n");
  }

  return OPT_OK;
}



