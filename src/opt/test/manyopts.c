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
/* $Id: manyopts.c,v 3.4 2003/03/15 05:09:15 jt Exp $ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <opt.h>

#define HOWMANYOPTS 1000

int x[HOWMANYOPTS];

int
main(int argc, char **argv)
{
  int i;
  char s[10];
  for (i=0; i<HOWMANYOPTS; ++i) {
    sprintf(s,"x%04d",i);
    optrega(&x[i], OPT_INT, '\0',s,"description");
  }
  opt(&argc,&argv);
  opt_free();
  return 0;
}
