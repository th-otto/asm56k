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
/* $Id: opt_p.h,v 3.23 2003/03/18 16:29:40 jt Exp $ */
/* opt_p.h */
/*
 *	Private header file for OPT package.
 */

#ifndef _OPT_P_H
#define _OPT_P_H

/***
 * Enable linking with C++
 ***/
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef	OPT_TRUE
#define	OPT_TRUE	1
#define	OPT_FALSE	0
#endif

#ifndef DEBUG
#define DEBUG 0
#endif

/* 
 * These are delimiter characters 
 */
#define DELIM		'-'		/* option delimiter character */
#define	ALTDELIM	'/'		/* alternate delimiter character */
#define	OPTFROMFILE	'@'		/* denotes options are in a file */
#define	OPTTOFILE	'%'		/* says to put options in a file */
#define	DOCUMENT	'-'		/* write document to file */
#define	INTERACT	'$'		/* Flags interactive menu */
#define IGNOREEOL       ';'		/* Ignore until the end of line */

/*
 * These are not available on command line 
 * But may be invoked from a file or the menu
 */
#define	RUN             '='		/* Says to just run to completion */
#define	QUITCH		'.'		/* Quit character */
/*
 * These are not available on command line or from a file
 * But may be invoked from the menu
 */
#define	BANG		'!'		/* Shell escape character */
#define	ADDITIONAL_OPTS	'+'		/* Additional options */

/*
 * These are not available from a file, but may be used on
 * the command line or from the menu.
 */
#define	HELPCH		'?'		/* Help character */
#define REASONCH        '~'             /* Displays deprecation reason */

#define ISEMPTYSTRING(s) (!((s) && *(s)))

/******************
 * Global variables
 */
extern	int	opt_nreg;
extern	int	opt_menuflag;
extern  int     opt_exit_number;

/*********************
 * Function Prototypes
 */

extern	void	opt_get_help(char);
extern  char   *optgetTitle(void);
extern  int     opt_char_number(char);

extern  void	opt_tofile(FILE *);
extern	char	*optstrval(int);
extern	char	*optstrtyp(int);
extern  int     opt_parse_positional(ARGVECTOR *);
extern  int     opt_parse_delim(ARGVECTOR *);
extern  int     opt_parse_longdelim(ARGVECTOR *);

extern void short_usage(void);
extern void long_usage(void);

  
#define OPT_NUM_VALID   0x1
#define OPT_NUM_FLOAT   0x2
#define OPT_NUM_NEG     0x4

extern double        opt_atof(char *);
extern long          opt_atoi(char *);
extern unsigned long opt_atou(char *);
extern int           opt_isvalidnumber(char *);
extern void          opt_setstring(char **, char *);
extern char         *opt_justify(char* s, int width, int indent, 
                                 int initial, char* prefix); 

		/* --------------- */
		/* Process Options */
		/* --------------- */

extern	char	  *opt_program_name;
extern  OPT_PFI    opt_additional_usage_fcn; 

extern	void       opt_wr_title(void);
extern  int        opt_fromfname(char *);
extern	int	   opt_tofname(char *);
extern  int        opt_getline(char *,FILE *);
extern	char	  *opt_mstring(int);
extern  ARGVECTOR *opt_process(int,char **);
extern  char      *short_progname(char *);
extern  int        opt_lineprocess(char *);
extern	void	   opt_menu(void);
extern  void       opt_menu_getline(char *, char *, int);
extern  void       opt_freestrings(void);

extern	void	opt_help(char *);
extern	void	opt_quit(void);
extern	void	opt_usage(void);
extern  void    opt_readline_init(char *);

extern  void    opt_write_xml_file(FILE *fp);

#define opt_mess_1(fmt,var1)                printf(fmt,var1)
#define opt_mess_2(fmt,var1,var2)           printf(fmt,var1,var2)
#define opt_mess_3(fmt,var1,var2,var3)      printf(fmt,var1,var2,var3)
#define opt_mess_4(fmt,var1,var2,var3,var4) printf(fmt,var1,var2,var3,var4)

#if HAVE_SNPRINTF /* if snprintf exists, then use it */
#define opt_snprintf_1(buf,n,fmt,v1)          snprintf(buf,n,fmt,v1)
#define opt_snprintf_2(buf,n,fmt,v1,v2)       snprintf(buf,n,fmt,v1,v2)
#define opt_snprintf_3(buf,n,fmt,v1,v2,v3)    snprintf(buf,n,fmt,v1,v2,v3)
#define opt_snprintf_4(buf,n,fmt,v1,v2,v3,v4) snprintf(buf,n,fmt,v1,v2,v3,v4)
#else /* if no snprintf, then use sprintf and take your chances! */
#define opt_snprintf_1(buf,n,fmt,v1)           sprintf(buf,fmt,v1)
#define opt_snprintf_2(buf,n,fmt,v1,v2)        sprintf(buf,fmt,v1,v2)
#define opt_snprintf_3(buf,n,fmt,v1,v2,v3)     sprintf(buf,fmt,v1,v2,v3)
#define opt_snprintf_4(buf,n,fmt,v1,v2,v3,v4)  sprintf(buf,fmt,v1,v2,v3,v4)
#endif

#define OPT_ERRMAXSTRLEN 1024  /* shouldn't be fixed length, but it is! */
  
#define opt_warn_1(fmt,var1) do { \
    char gstr[OPT_ERRMAXSTRLEN]; \
        opt_snprintf_1(gstr,OPT_ERRMAXSTRLEN,fmt,var1); \
        opt_warning(gstr); } while(0)
#define opt_warn_2(fmt,var1,var2) do { \
    char gstr[OPT_ERRMAXSTRLEN]; \
        opt_snprintf_2(gstr,OPT_ERRMAXSTRLEN,fmt,var1,var2); \
	opt_warning(gstr); } while(0)
#define opt_warn_3(fmt,var1,var2,var3) do { \
    char gstr[OPT_ERRMAXSTRLEN]; \
        opt_snprintf_3(gstr,OPT_ERRMAXSTRLEN,fmt,var1,var2,var3); \
	opt_warning(gstr); } while(0)

#define opt_fatal_1(fmt,var1) do { \
    char gstr[OPT_ERRMAXSTRLEN]; \
        opt_snprintf_1(gstr,OPT_ERRMAXSTRLEN,fmt,var1); \
	opt_fatal(gstr); } while(0)
#define opt_fatal_2(fmt,var1,var2) do { \
    char gstr[OPT_ERRMAXSTRLEN]; \
        opt_snprintf_2(gstr,OPT_ERRMAXSTRLEN,fmt,var1,var2); \
	opt_fatal(gstr); } while(0)
#define opt_fatal_3(fmt,var1,var2,var3) do { \
    char gstr[OPT_ERRMAXSTRLEN]; \
        opt_snprintf_3(gstr,OPT_ERRMAXSTRLEN,fmt,var1,var2,var3); \
	opt_fatal(gstr); } while(0)

#define OPT_FREE(s) do { if (s) { free(s); s=NULL; } } while (0)


/* ...well, assume that you do have strdup, since if you do and you
 * say you don't you can get into trouble.  if you really don't, you
 * should '#define HAVE_STRDUP 0' (or -DHAVE_STRDUP=0 on command line)
 */
#ifndef HAVE_STRDUP
#define HAVE_STRDUP 1
#endif

#if !HAVE_STRDUP
char *strdup(char *s);
#endif

/***
 * Enable linking with C++
 ***/
#ifdef __cplusplus
}
#endif
#endif /* _OPT_P_H */






