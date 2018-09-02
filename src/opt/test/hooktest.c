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
/****************************************
*	Show up a weirdness in opt hooks.
*       Written by Mark Muldoon 21 Oct 97
*       The weirdness was in fact a bug in opt,
*       which was fixed in going from v3.0 to v3.1
*/
# include <stdio.h>
# include <opt.h>

/****************************************
*	Prototypes for hooks.
*/
int check_hook(void *a_ptr);

/****************************************
*	Global variables.
*/
int		a_global ;
/* _______________________________________ */

int
main( int argc, char *argv[] ) 
/****************************************
*	Register a single command line 
* option and give it a hook.
*/
{
	optrega( &a_global, OPT_INT, 'a', 
			"set_a_global", "Set a global variable" ) ;

	opthook( &a_global, check_hook ) ;

	opt( &argc, &argv ) ;

        opt_free();

        return 0;
}
/* ____________________________________ */

int check_hook( void *a_ptr )
/****************************************
*	Check whether the hook recieves the
* correct value.
*/
{
	int		a_local, verdict ;

	a_local = *((int *) a_ptr) ;
	if( a_local == a_global ) { 
		printf( "All is well.\n" ) ;

		verdict = OPT_OK ;
	}
	else {
		printf( "Confusion reigns: *a_ptr = %d, ", a_local ) ;
		printf( "but a_global = %d.\n", a_global ) ;

		verdict = OPT_ERROR ;
	}

	return( verdict ) ;
}
