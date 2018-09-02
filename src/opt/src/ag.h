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
/* $Id: ag.h,v 3.3 2001/08/15 22:36:27 jt Exp $ */
/* ag.h */


#ifndef _AG_H
#define _AG_H

#ifndef	TRUE
#define	TRUE	1
#define	FALSE	0
#endif

/*************************************
 * ARGVECTOR structure
 * Basically, an (argc,argv) construct
 * with indices to which character of 
 * which word is the current position
 */

typedef struct {
    char    **v;            /* argument vector */
    int     c;              /* argument count  */
    int     iw;             /* current word    */
    int     ic;             /* current character */
}   ARGVECTOR;

/* 
 * Function prototypes 
 */


extern  ARGVECTOR *ag_new(int,char **);
extern  void       ag_free(ARGVECTOR *);
extern  void       ag_fprint(FILE *, ARGVECTOR *);
extern  int        ag_enstring(char *,ARGVECTOR *,int);
extern  void       ag_reset(ARGVECTOR *);
extern  int        ag_w_number(ARGVECTOR *);
extern  void       ag_w_advance(ARGVECTOR *);
extern  int        ag_eow(ARGVECTOR *);
extern  int        ag_end(ARGVECTOR *);
extern  char       ag_c(ARGVECTOR *);
extern  char       ag_c_next(ARGVECTOR *);
extern  char       ag_cnn_next(ARGVECTOR *);
extern  char       ag_c_advance(ARGVECTOR *);
extern  char       ag_backspace(ARGVECTOR *);
extern  void       ag_backword(ARGVECTOR *);
extern  char      *ag_s(ARGVECTOR *);
extern  char      *ag_s_next(ARGVECTOR *);
extern  char      *ag_s_advance(ARGVECTOR *);
extern  int        ag_clear(ARGVECTOR *);
extern  int        ag_argc(ARGVECTOR *);
extern  char     **ag_argv(ARGVECTOR *);
extern  ARGVECTOR *ag_copy(ARGVECTOR *);
extern  void       ag_freeall(ARGVECTOR *);
extern  ARGVECTOR *ag_prepend_argv0(ARGVECTOR *, char *);
extern  char      *argnext(ARGVECTOR *);


#endif /* _AG_H */








