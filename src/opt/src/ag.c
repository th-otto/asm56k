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
/* $Id: ag.c,v 3.7 2003/03/14 20:22:30 jt Exp $ */
/* ag.c */

#ifndef	SINGLEFILE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "opt.h"                /* needed for __PROTO */
#include "ag.h"
#endif

/* primitive routines for manipulation argument vectors */

/*
  make-new-argvector              ag_new()
  reset-argvector                 ag_reset()
  flag-end-of-word                ag_eow()
  what-is-character               ag_c()
  what-is-string                  ag_s()

  read-character                  ag_c()
  read-next-character             ag_c_next()
  read-next-non-null-character    ag_cnn_next()
  read-character-advance          ag_c_advance()
  read-string                     ag_s()
  read-next-string                ag_s_next()
  read-string-advance             ag_s_advance()
  word-number                     ag_w_number()
  word-advance                    ag_w_advance()
  backspace                       ag_backspace()
  clear                           ag_clear()

  get-argc                        ag_argc()
  get-argv                        ag_argv()

  get-next-argument               argnext()
*/

ARGVECTOR *
ag_new(int argc, char **argv)
{
    ARGVECTOR *ag;
    ag = (ARGVECTOR *)malloc(sizeof(ARGVECTOR));
    /* Note...we do not actually make a COPY of argv; but this
     * shouldn't be necessary, since *none* of the following routines
     * should ever write to ag->c and *especially* not ag->v
     * ...um, actually ag_prepend_argv0 does
     */
    ag->v = argv;
    ag->c = argc;
    ag->iw = 0;
    ag->ic = 0;
    return(ag);
}
void
ag_free(ARGVECTOR *ag)
{
    free((char *)ag);
}
void
ag_fprint(FILE *fp, ARGVECTOR *ag)
{
        int i;
        if (ag == NULL) {
            fprintf(fp,"ag=<nul>\n");
            return;
        }
        for(i=0; i<ag->c; ++i)
            fprintf(fp,"[%s]",ag->v[i]);
        fprintf(fp,", <%d> <%d>\n",ag->iw,ag->ic);
}
void
ag_reset(ARGVECTOR *ag)
{
    ag->iw = 0;
    ag->ic = 0;
}
int
ag_w_number(ARGVECTOR *ag)
{
    return ag->iw;
}
void
ag_w_advance(ARGVECTOR *ag)
{
    ++(ag->iw);             /* advance to next word */
    ag->ic=0;               /* beginning of next word */
}
/*      ag_eow: end of word
        flag whether current position is at end of word
*/
int
ag_eow(ARGVECTOR *ag)
{
    if( ag->iw >= ag->c )
        return(TRUE);
    if( ag->ic >= (int)strlen(ag->v[ag->iw]) )
        return(TRUE);
    return(FALSE);
}
/*      ag_end: end of command line
                flag whether current position is at end of command line 
*/
int
ag_end(ARGVECTOR *ag)
{
        if( ag->iw >= ag->c )
                return(TRUE);
        if( ag_eow(ag) && ag->iw == (ag->c)-1 )
                return(TRUE);
        return(FALSE);
}
/*      ag_c:   return current character
                do not advance
*/
char
ag_c(ARGVECTOR *ag)
{
        return(ag->v[ag->iw][ag->ic]);
}
char
ag_c_next(ARGVECTOR *ag)
{
        return(ag->v[ag->iw][ag->ic+1]);
}
char
ag_cnn_next(ARGVECTOR *ag)
{
    if( ag_c_next(ag) == '\0' ) {
        if(ag->iw+1 >= ag->c)
            return('\0');
        else
            return(ag->v[ag->iw+1][0]);
    }
    else
        return( ag_c_next(ag) );
}
/*      ag_c_advance:   read current character, and advance to next
                        return '\0' if end of word
                        do not advance to next word
*/
char
ag_c_advance(ARGVECTOR *ag)
{
        char    c;              /* current character */

        if( ag_eow(ag) )
                return(c='\0'); /* return NULL to signal that current*/
                                /* character is past end of word     */
        c = ag->v[ag->iw][ag->ic];
        ++(ag->ic);             /* advance to next character */
        return(c);
}
void
ag_backword(ARGVECTOR *ag)
{
    ag->iw -= 1;
}

char
ag_backspace(ARGVECTOR *ag)
{
    if( --(ag->ic) < 0 ) {          /* if back past beginning of word */
        ag->ic=0;
        if(--(ag->iw) < 0) {        /* goto beginning of previous word */
            ag_reset(ag);           /* if no previous, reset */
        }
        else {
            while( !ag_eow(ag) )    /* goto end of prevous word */
                ag_c_advance(ag);
            ag_backspace(ag);       /* back to just before end */
        }
    }
    return(ag->v[ag->iw][ag->ic]);
}
/*      ag_s:   returns current string
                returns "" if current position is at end of word
                returns NULL if past end of argument vector
*/
char    *
ag_s(ARGVECTOR *ag)
{
        if( ag->iw < ag->c )
                return( ag->v[ag->iw]+ag->ic );
        else    return( NULL );
}
char    *
ag_s_next(ARGVECTOR *ag)
{
        if( ag->v[ag->iw][ag->ic+1] == '\0' )
        {
                if(ag->iw+1 >= ag->c)
                        return(NULL);
                else    return(ag->v[ag->iw+1]);
        }
        else    return( ag->v[ag->iw]+ag->ic+1 );
}
/*      ag_s_advance:   read current string and advance to next
                        returns NULL if current string is at end
                        does not check advanced string
*/
char    *
ag_s_advance(ARGVECTOR *ag)
{
    char    *s;             /* current string */
    
    if( ag_eow(ag) )        /* if end of word, go to next word */
        ag_w_advance(ag);
    if( ag_eow(ag) )        /* if still at end of word,  */
        s = NULL;       /* signify by returning NULL */
    else
        s = ag->v[ag->iw] + ag->ic;
    
    ag_w_advance(ag);       /* advance to next word */
    
    return(s);
}
int
ag_clear(ARGVECTOR *ag)
{
    while( !ag_end(ag) )
        argnext(ag);
    return 1;
}

int
ag_argc(ARGVECTOR *ag)
{
    return ag->c - ag->iw;
}
char **
ag_argv(ARGVECTOR *ag)
{
    return ag->v + ag->iw;
}
ARGVECTOR *
ag_copy(ARGVECTOR *ag)
{
    /* Here is how you would make a copy of an ARGVECTOR */
    /* To free this, do not use ag_free, but ag_freeall */
    int i;
    ARGVECTOR *agnew;
    agnew = (ARGVECTOR *)malloc(sizeof(ARGVECTOR));
    agnew->c = ag->c;
    agnew->v = (char **)malloc(sizeof(char *));
    for (i=0; i< agnew->c; ++i) {
        agnew->v[i] = strdup(ag->v[i]);
    }
    agnew->iw = 0;
    agnew->ic = 0;
    return agnew;
}
void
ag_freeall(ARGVECTOR *ag)
{
    int i;
    for (i=0; i<ag->c; ++i) {
        free(ag->v[i]);
    }
    free((char *)ag->v);
    free((char *)ag);
}
ARGVECTOR *
ag_prepend_argv0(ARGVECTOR *ag, char *argv0)
{
    int i;
    ARGVECTOR *agnew;
    agnew = (ARGVECTOR *)malloc(sizeof(ARGVECTOR));
    agnew->c = ag->c - ag->iw + 1;
    agnew->v = (char **)malloc(agnew->c * sizeof(char *));
    agnew->v[0] = argv0; /* NOTE: we are not strdup'ing */
    for (i=1; i< agnew->c; ++i) {
        agnew->v[i] = ag->v[i + ag->iw - 1];
    }
    agnew->iw = 0;
    agnew->ic = 0;
    return agnew;
}

    


	/* ------------------------ */
	/* return the next argument */
	/* ------------------------ */
char *
argnext(ARGVECTOR *ag)
{
    static char nullchar='\0';
    char *s;

    s = ag_s_advance(ag);
    if( s==NULL )	
    	s = &nullchar;
    return(s);
}




