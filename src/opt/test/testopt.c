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
/* testopt.c */

#include <stdio.h>
#include <stdlib.h>
#include <opt.h>

int month=4;
int day=24;
int year=1993;
int julian=0;
char *who=NULL;
int altversion=99;

int go(int argc, char **argv)
{
    if (argc>1) {
        printf("In program %s, Extra option: %s\n",argv[0],argv[1]);
    }

    if (optinvoked(&month)) {
        printf("User set month...\n");
    }

    if (month == 9 && day == 11 && year == 1989) {
        printf("Happy birthday, Max\n");
    } else {
        printf("Hello, %s: %4d/%02d/%02d %s\n",(who==NULL ? "world" : who),
               year,month,day,(julian ? "(Julian)" : ""));
    }
    return OPT_OK;
}

int checkyear(void *v)
{
    if (year == 2000) {
        printf("Watch out for that year=2000 bug!\n");
        return OPT_ERROR;
    }
    return OPT_OK;
}
int quit()
{
    printf("Bye...\n");
    return OPT_OK;
}
int write_altversion(void *v)
{
  /* not sure what the void *v argument would be used for... */
    printf("Version %d\n",altversion);
    optExitNumber(12);
    return OPT_QUIT;
}
int fix_mon(void *v)
{
    int m;
    /* fix whatever int variable v is pointing to */
    m = *((int *)v);
    if (m < 1 || m > 12) 
        m=1;
    *((int *)v) = m;
    return OPT_OK;
} 

int
main(int argc, char **argv)
{
    optreg(&month,OPT_INT,'m',"Month");
    optlongname(&month,"month");
    opthook(&month,fix_mon);

    optrega(&day,OPT_INT,'d',"day","Day");
    opthelp(&day,"Use day of month, should be less than 32");
    
    optreg_INT(&year,'y',"Year...");
    optreg(&year,OPT_INT,'Y',"Year");
    optdescript(&year,"What Year");
    opthook(&year,checkyear);

    optregs(&julian,OPT_BOOL,"julian");

    optregp(&who,OPT_STRING,"who","Who to say hello to");

    /* This is one way to write a version... */
    optexec("altversion",write_altversion,"Write altversion number and exit");
    /* But opt also has a builtin --version that writes string and exits */
    optVersion("3.14159+");

    optEnvVarName( "OPT" );
    optDefaultFile( "testoptrc" ); /* in practice, you do ~/.testoptrc */
    
    optMain(go);
    optQuit(quit);

    opt(&argc,&argv);
    go(argc,argv);

    opt_free();
    return 0;
}



