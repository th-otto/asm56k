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
/* $Id: opt_menu.c,v 3.8 2003/03/18 16:29:40 jt Exp $ */
/* opt_menu.c */


#ifndef	SINGLEFILE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "opt.h"
#include "ag.h"
#include "opt_p.h"
#endif /* SINGLEFILE */

/* --------------------- menu flag ------------------- */

int opt_menuflag=OPT_FALSE;

static char mgstring[160];	/* global menu string */

#define menu_wr_string(str)       opt_message(str)

static void write_the_menu(int iopfrom, int iopto);
static int auto_prefix_delim(char *r);


/* ----------------------------------------------------	*/
/*  opt_menu:	Get options from an interactive menu	*/
/* ----------------------------------------------------	*/

#define	MAXRESPONDLINE	280
#ifndef	MAXOPTSINMENU
#define	MAXOPTSINMENU	20
#endif

char *MENUPROMPT="-> ";

void
opt_menu(void)
{
    char respond[MAXRESPONDLINE+2];
    static int maxoptsinmenu=MAXOPTSINMENU;
    int	iopfrom,iopto;

    opt_menuflag=OPT_TRUE;	/* turn on MENUFLAG in case it is not set already */
    
    iopfrom = 0;
    iopto = ( opt_nreg < maxoptsinmenu ? opt_nreg : maxoptsinmenu );
    
    respond[0]='\0';

    opt_mess_1("%s\n",optgetTitle());
    write_the_menu(iopfrom,iopto);

    while( opt_menuflag ) {
        opt_menu_getline(MENUPROMPT,respond,MAXRESPONDLINE);
		
        switch(*respond) {
	case REASONCH:
	    opt_message( "Option Information: To be implemented.\n" );
	    opt_message( "\n   Name:        " );
	    opt_message( "\n   Type:        " );
	    opt_message( "\n   Reason:      " );
	    break;
        case ADDITIONAL_OPTS:
            if( respond[1] != '\0' && respond[1] != ' ' ) {
                maxoptsinmenu = atoi(respond+1);
                if(maxoptsinmenu < 1)
                    maxoptsinmenu = opt_nreg;
                opt_snprintf_1(mgstring,160,"Scroll %d options\n",maxoptsinmenu);
                menu_wr_string(mgstring);
                iopfrom = 0;
                iopto = 
                    ( opt_nreg < maxoptsinmenu ? opt_nreg : maxoptsinmenu );
            } else {
                iopfrom += maxoptsinmenu;
                if( iopfrom > opt_nreg)
                    iopfrom = 0;
                iopto = iopfrom + maxoptsinmenu;
                if( iopto > opt_nreg )
                    iopto = opt_nreg;
            }
            write_the_menu(iopfrom,iopto);
            break;
        case INTERACT:
            opt_menuflag=OPT_FALSE;
            break;
        case BANG:
            system( respond+1 );
            break;
        case '\0':
            write_the_menu(iopfrom,iopto);
            break;
        case QUITCH:
            /* Only quit if the QUITCH is followed by whitespace.  In
             * other words, if respond = '.m=5', don't quit.  However,
             * note that respond = '. m=5' will cause a quit.
             */
            if ( respond[1]=='\0' || respond[1]==' ' )
                opt_quit();
            else
                opt_mess_1("Invalid line: [%s]\n",respond);
            break;
        case DELIM:
#ifdef PERMIT_ALTDELIM            
        case ALTDELIM:
#endif            
            opt_lineprocess(respond);
            break;
        default:
            auto_prefix_delim(respond);
            opt_lineprocess(respond);
            break;
        }
    }
}/* opt_menu */


/**********
 * write_the_menu:
 *	write the menu including options from iopfrom to iopto.
 */
static void
write_the_menu(int iopfrom, int iopto)
{
    int iop;
    int fullmenu;

    fullmenu = ((iopfrom==0 && iopto==opt_nreg) ? OPT_TRUE : OPT_FALSE );

    if( !fullmenu ) {
        opt_snprintf_3(mgstring,160,"menu: %d->%d [%d]\n",iopfrom,iopto,opt_nreg);
        menu_wr_string(mgstring);
    }

    for(iop=iopfrom; iop<iopto; ++iop) {
        char *s;
        s = opt_mstring(iop);
        if (s!=NULL) {
            menu_wr_string(s);
            menu_wr_string("\n");
	    free(s);
        }
    }
    if (!fullmenu) {
        opt_snprintf_1(mgstring,160,"%c Additional options\n",ADDITIONAL_OPTS);
        menu_wr_string(mgstring);
    }
    opt_snprintf_1(mgstring,160,"(Type %c for Help)\n",HELPCH);
    menu_wr_string(mgstring);
}

/*	auto_prefix_delim:	
 *		this is a fru-fru piece of code that automatically
 *		sticks a DELIM character onto the front of a string
 *		in cases where it imagines that that is what the user
 *		really meant.  Thus
 *		-> m4
 *		gives the same effect as
 *		-> -m4
 *		But be warned that this only applies in limited cases.
 *		Eg.,
 *		-> m4 b3
 *		is not the same as
 *		-> -m4 -b3
 *
 *		a '-' will be prepended in the case that 
 *		the first character is a registered name
 */
static int
auto_prefix_delim(char *r)
{
    if( opt_char_number( *r ) != -1 ) {
        int len;
        len = strlen(r)+1;	/* +1 since double terminated */
        while(len>=0) {
            r[len+1]=r[len];
            --len;
        }
        r[0]=DELIM;
        return(1);
    }
    else
        return(0);
}/* auto_prefix_delim */


