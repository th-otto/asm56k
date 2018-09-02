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
/* $Id: tarray.c,v 3.6 2003/03/15 05:09:15 jt Exp $ */

#include <stdio.h>
#include <stdlib.h>
#include <opt.h>

char delim=':';

int nx=0;
double *x=NULL;

int ny=0;
char **y=NULL;

int
run()
{
  int i;
  for (i=0; i<nx; ++i) {
    printf("%d %g\n",i,x[i]);
  }
  for (i=0; i<ny; ++i) {
    printf("STRING %d %s\n",i,y[i]);
  }
  return 0;
}
int setxdelim(void *v)
{
  optarraydelim(&x,delim);
  return OPT_OK;
}
int
main(int argc, char **argv)
{
  int i;
  nx = 5;
  x = (double *)malloc(nx*sizeof(double));
  for (i=0; i<nx; ++i) {
    x[i] = i;
  }

  optregp_array(&nx,&x,OPT_DOUBLE,"xarray","Array of x values (delimited by +)");
  optarraydelim(&x,'+');
  optmode(&x,OPT_FLEXIBLE);
  optreg(&delim,OPT_CHAR,'d',"delimiter for x array");
  opthook(&delim,setxdelim);
  optrega_array(&ny,&y,OPT_STRING,'y',"yarray","Array of strings (delimited by ,)");
  optRun(run);
  opt(&argc,&argv);
  opt_free();
  run();
  free(x);
}

