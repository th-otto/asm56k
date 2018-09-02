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
/* iloop.c */
/* This routine shows how the interrupt works, when, for example, you
 * are in an infinite loop.  To try this out run it in the menu mode,
 * ie
 *
 *    iloop --menu
 *    ->
 *
 * If you enter 'N -1 =', meaning run with N=-1 (no, this isn't
 * postfix!)  then after 100 times through the loop, it will interrupt
 * itself with optAbortRun() which will bring you back to the menu.
 * If you enter any other negative N (or just a VERY large positive
 * N), then you will get into an infinite loop.  Hit ^C to get out of
 * the loop.  If the signal trapping works, then you should be
 * returned to the prompt instead of exiting the whole program.
 * 
 */
#include <stdio.h>
#include <opt.h>

long N=1;

int run()
{
    long n;
    /** if N is negative, this gives an infinite loop **/
    for (n=0; n!=N; ++n) {
        printf("Type ^C to exit\n");
        if (n==10 && N == -1) optAbortRun();
    }
    return OPT_OK;
}

int
main(int argc, char **argv)
{
    optreg(&N,OPT_LONG,'N',"Number of times through the loop");
    optRun(run);
    opt(&argc,&argv);
    return 0;    
}



