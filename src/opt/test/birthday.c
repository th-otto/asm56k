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
/* birthday.c */

#include <stdio.h>
#include <opt.h>                /* part of opt package */

/* Parameters that user has access to via opt package;
 * They are typically (but not necessarily) global variables.
 * Their default values are provided in the assignement statements.
 */
int month=9;
int day=11;
int year=1989;
int verb=0;
int pade=0;
int greg=0;

/* All of what the program itself does is in the birthday() function;
 * This function does what a non-options parsing main() might do.
 */
int birthday(int argc, char **argv)
{
    if (month == 9 && day == 11 && year == 1989)
        printf("Happy birthday, Max\n");
    else if (month == 4 && day == 24 && year == 1993)
        printf("Happy birthday, Sky\n");

    if (verb)
        printf("Hello, world: %4d/%02d/%02d\n",year,month,day);

    return OPT_OK;
}

/* all of the options parsing is in the new main() function */

int main(int argc, char **argv)
{
    /* optrega() registers short name '-m' and long name '--month' to
     * variable 'month', and provides brief description "Month"
     */
    optrega(&month,OPT_INT,'m',"month","Month");
    optrega(&day,  OPT_INT,'d',"day",  "Day of month");
    /* optreg() only provides short name '-y' */
    optreg(&year,OPT_INT,'y',"Year");
    /* register some flag variables... */
    optreg(&verb,OPT_INTLEVEL,'v',"Verbose");
    optreg(&pade,OPT_BOOL,'p',"Use Pade Approximants");
    optreg(&greg,OPT_BOOL,'g',"Gregorian");

    /* the function birthday() is registered with opt */
    optMain(birthday);

    /* opt() is the routine that actually parses the argc/argv
     * variables
     */
    opt(&argc,&argv);
    opt_free();
    /* and when it's done, argc/argv will contain the leftover
     * argc/argv variables, including the same argv[0] as in
     * the original argc/argv
     */

    /* Now that variables are parsed, run birthday() */
    return birthday(argc,argv);
}
