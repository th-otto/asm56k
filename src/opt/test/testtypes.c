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
/* testtypes.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <opt.h>

int aint = 1;
unsigned int buint = 2;
short cshort = 3;
long dlong = 4;
char echar = '5';
int flevel = 6;
float ffloat = 7.0;
double gdouble = 8.0;
int htoggle = OPT_TRUE;
int jbool = OPT_TRUE;
int knegbool = OPT_FALSE;
char *lvstring = "hello";
char mcstring[OPT_MAXSTRLEN] = "hello";
/* Note that 'char mcstring[] = "hello";' would lead to a coredump!
 * That's why CSTRING's are deprecated!!
 */

unsigned short nushort=32769;      /* == 2^15+1 */
unsigned long oulong=2150000000UL; /* just larger than 2^31 */

int oldmain(int argc, char **argv)
{
    printf("a=%d, b=%u, c=%hd, d=%ld, e=[%c]\n",
           aint,buint,cshort,dlong,echar);
    printf("n=%hu, o=%lu\n",nushort,oulong);
    printf("m[%d/%d]=%s\n",strlen(mcstring),sizeof(mcstring)/sizeof(mcstring[0]),mcstring);
    return OPT_OK;
}

int
main(int argc, char **argv)
{
    optreg_INT(&aint,'a',"int");
    optreg_UINT(&buint,'b',"unsigned int");
    optreg_SHORT(&cshort,'c',"short");
    optreg_LONG(&dlong,'d',"long");
    optreg_CHAR(&echar,'e',"char");
    optreg_INTLEVEL(&flevel,'F',"int level");
    optreg_FLOAT(&ffloat,'f',"float");
    optreg_DOUBLE(&gdouble,'g',"double");
    optreg_TOGGLE(&htoggle,'h',"toggle");
    optreg_BOOL(&jbool,'j',"boolean");
    optreg_NEGBOOL(&knegbool,'k',"negated boolean");
    /* Note: VSTRING is now deprecated in favor of STRING */
    optreg_STRING(&lvstring,'l',"string");

    /* NOTE: the CSTRING is the only type in which the
     * actual variable name is used instead of a pointer
     * to the variable name!  However, on many systems,
     * &mcstring and mcstring are identical, so it doesn't
     * really matter.!
     */
    optreg_CSTRING(mcstring,'m',"c-string");

    optreg_USHORT(&nushort,'n',"unsigned short");
    optreg_ULONG(&oulong,'o',"unsigned long");
    
    optMain(oldmain);

    opt(&argc,&argv);
    opt_free();

    return oldmain(argc,argv);
}
