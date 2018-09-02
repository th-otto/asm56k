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
/* testmain.c */
/* $Id: testmain.c,v 3.5 2003/03/15 05:09:15 jt Exp $ */
/* Test and/or Show-off how the new optMain() works.
   Also test optDisableMenu()
 */

#include <stdio.h>
#include <opt.h>

#define MYVERSION "3.14159+"

long N=1;

int oldmain(int argc, char **argv)
{
    int i;

    if (N < argc) {
        printf("Truncated to %ld words:\n",N);
    }
    printf("<%s> ",argv[0]);
    for (i=1; i<argc && i<=N; ++i) {
            printf(" [%s]",argv[i]);
    }
    printf("\n");
    return OPT_OK;
}

int
main(int argc, char **argv)
{
    optreg(&N,OPT_LONG,'N',"Max number of arguments echoed");
    optVersion(MYVERSION);
    optMain(oldmain);
    optDisableMenu();
    opt(&argc,&argv);
    opt_free();
    return oldmain(argc,argv);
}



