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
/* tstring.c */

#include <stdio.h>
#include <stdlib.h>
#include <opt.h>

char *s = "hello";
char *t; /* this really is an error in opt! */
char *u = "undelim";
char *v = "vee-initial";
char *w = NULL;

int main(int argc, char **argv)
{
  optreg(&s,OPT_STRING,'s',"string (init=hello)");
  optreg(&t,OPT_STRING,'t',"uninitiated string");
  optregp(&u,OPT_STRING,"undelim","undelimited string");
  optrega(&v,OPT_STRING,'\0',"vee","use --vee=string");
  optrega(&w,OPT_STRING,'\0',"dubya","use --dubya=string");

  opt(&argc,&argv);
  if (s) printf("s-string = %s\n",s);
  if (optinvoked(&t)) /* this is how you get around that error */
    printf("t-string = %s\n",t);
  if (u) printf("u-string = %s\n",u);
  if (v) printf("v-string = %s\n",v);
  if (w) printf("w-string = %s\n",w);
  opt_free();
  return 0;
}
