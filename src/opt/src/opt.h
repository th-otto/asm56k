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
/* $Id: opt.h,v 3.28 2003/03/15 05:09:11 jt Exp $ */
/* opt.h */
/*
	User Include File for options package
*/

#ifndef _OPT_H
#define	_OPT_H 	/* Signal that this header file has been included */

/***
 * Enable linking with C++
 ***/
#ifdef __cplusplus
extern "C"
{
#endif



typedef enum    {
    OPT_NUL,
    OPT_INT,        OPT_SHORT,      OPT_LONG,       OPT_CHAR,
    OPT_UINT,       OPT_USHORT,     OPT_ULONG,      OPT_UCHAR,
    OPT_FLOAT,      OPT_DOUBLE,
    OPT_TOGGLE,     OPT_BOOL,
    OPT_NEGTOGGLE,  OPT_NEGBOOL,    OPT_INTLEVEL,       
    OPT_STRING,     
    /* now deprecated */
    OPT_UNDELIM, OPT_CSTRING,    OPT_UNDELIMC
} opt_TYPE;

  /* define the deprecated typenames */
#define OPT_FLAG OPT_TOGGLE
#define OPT_ABSFLAG OPT_BOOL
#define OPT_NEGFLAG OPT_NEGTOGGLE
#define OPT_ABSNEGFLAG OPT_NEGBOOL
#define OPT_VSTRING OPT_STRING
#define OPT_UNSINT OPT_UINT
#define OPT_UNDELIMV OPT_UNDELIM

/* OPT_PFI is the ever popular: Pointer to Function returning an Integer */
typedef	int (*OPT_PFI)(void);
typedef int (*OPT_HOOK)(void *);
typedef	int (*OPT_PFI_ARG)(int,char**);

#define	OPT_EXT	".opt"	  /* standard options file extension */
#define OPT_MAXSTRLEN 80  /* CSTRING's should be no longer than this */

#define OPT_TRUE 1
#define OPT_FALSE 0

/* Return values from hook functions */
#define OPT_OK    0
#define OPT_ERROR 1
#define OPT_ABORT 2
#define OPT_QUIT  3
#define OPT_EXIT  4

/* Possible values for the option mode attribute */
typedef enum {
  OPT_POSITIONAL,
  OPT_DELIMITED,
  OPT_FLEXIBLE
} opt_MODE;

/* opt(&argc,&argv) is the main function call that does all the work.
 * it processes the options on the command line, setting variables,
 * calling hooks, etc. 
 */

extern void opt(int *,char ***);


/* But before opt() can be called, variables/hooks/etc must be
 * registered.  Various ways are provided to register variables; one
 * identifies the variable by its (void *) pointer.  To register a
 * variable, an opt_TYPE must be specified, and either a character or
 * a string name (or both), and optionally a brief description.
 */
/*  optrega: "all"
 *  optreg:  standard
 *  optregc: just sets character name
 *  optregs: just sets string name
 *  optregcb: sets character name and brief description
 *  optregsb: sets string name and brief description
 *  optregp: sets string name, brief description and OPT_POSITIONAL mode
 *  optregp: sets character name, string name, brief description and OPT_FLEXIBLE mode
 *  optexec: registers an 'exec function' (somthing that is run, then exits)
 */

/*    ARGUMENT LIST: &var    OPT_INT   'c'   "name"   "Brief Description" */
extern int optrega(  void *, opt_TYPE, char,  char *, char * );
extern int optreg(   void *, opt_TYPE, char,          char * );
extern int optregc(  void *, opt_TYPE, char                  );
extern int optregcb( void *, opt_TYPE, char,          char * );
extern int optregs(  void *, opt_TYPE,        char *         );
extern int optregsb( void *, opt_TYPE,        char *, char * );
extern int optregp(  void *, opt_TYPE,        char *, char * );
extern int optregf(  void *, opt_TYPE, char,  char *, char * );
/*    ARGUMENT LIST: &var    OPT_INT   'c'   "name"   "Brief Description" */

/*    ARGUMENTS:    "name"  hook      "Descript" */
extern int optexec( char *, OPT_HOOK, char * );


  /* ARRAY OPTIONS, same as scalar except first argument points to
   * the size of the array. Not currently available for positionals */
  /*  ARGUMENT LIST:      &nelts, &array, OPT_INT, 'c'   "name"  "Descript" */
extern int optrega_array(  int *, void *, opt_TYPE, char, char *, char *);
extern int optreg_array(   int *, void *, opt_TYPE, char,         char *);
extern int optregc_array(  int *, void *, opt_TYPE, char                );
extern int optregcb_array( int *, void *, opt_TYPE, char,         char *);
extern int optregs_array(  int *, void *, opt_TYPE,       char *        );
extern int optregsb_array( int *, void *, opt_TYPE,       char *, char *);
extern int optregp_array(  int *, void *, opt_TYPE,       char *, char *);
extern int optregf_array(  int *, void *, opt_TYPE, char, char *, char *);
  
/* Using the following routines, one can specify features (such as
 * char, longname, descript) of the variable that were left out in the
 * registration process, or one can add features (such as help, hook,
 * exec) that the registration routines do not permit.
 */
/* These functions use &var to identify which option */
extern void optchar(     void *,char);
extern void optlongname( void *,char *);
extern void optmode(     void *,opt_MODE);
extern void optdescript( void *,char *);
extern void opthelp(     void *,char *);
extern void opthook(     void *,OPT_HOOK);
extern void optarraydelim(void *, char);
/* These functions use option number to identify which option */
extern void optchar_n(       int, char);
extern void optlongname_n(   int, char *);
extern void optmode_n(       int, opt_MODE);
extern void optdescript_n(   int, char *);
extern void opthelp_n(       int, char *);
extern void opthook_n(       int, OPT_HOOK);
extern void optarraydelim_n( int, char);  

/* The following routines are used to set features (such as usage),
 * which are not specific to single variables, but apply to the whole
 * program.  In general, I try to use the convention optUpperCase()
 * for the registration of these more global features, while
 * optlowercase() refers to functions that apply to a specific
 * registered variable.
 */
extern void optUsage(char *);
extern void optTitle(char *);
extern void optProgName(char *);
extern void optVersion(char *);
extern void optEnvVarName(char *);
extern void optDefaultString(char *);
extern void optDefaultFile(char *);
extern void optDisableMenu();

extern void optAdditionalUsage(OPT_PFI);
extern void optQuit(OPT_PFI);
extern void optRun(OPT_PFI);
extern void optMain(OPT_PFI_ARG);
extern void optExitNumber(int);

/* The following routines provide opt-related services that might
 * be useful after opt() has been set up and run.
 */
/* optPrintusage() writes the opt usage message to stdout
 */
extern void optPrintUsage();
/* optinvoked(&var) returns the number of times the option was invoked
 * on the command line.  This function is not used in the registering
 * stage, but is used during execution of the code to see whether or
 * not a value was actually set (or if it's just using the default).
 */
extern int  optinvoked(void *);
/* opt_free() is called when you are sure you won't be using any more
 * opt functions; usually, you could call it right after opt(), but
 * after opt_free(), you won't be able to use any other opt functions,
 * such as optinvoked
 */
extern void opt_free();

  
/* The following prototypes arguably belong in opt_p.h because it is
 * not clear that the functions would ever be used in a program that
 * uses opt.  Certainly, opt itself makes great use of these, behind
 * the scenes.
 */
extern void optAbortRun(void);
extern  int optBeginRun(OPT_PFI);
extern void optAbortMain(void);
extern  int optBeginMain(OPT_PFI_ARG, int, char **);

extern void opt_message(char *);
extern void opt_warning(char *);
extern void opt_fatal(char *);




/* The following prototypes are deprecated; they are defined for
 * compatibility with versions of opt prior to v3.0, but they are not
 * recommended.
 */
extern  int getopts(int,char **);
#define	optregister(val,typ,c,str) optreg(&val,typ,c,str)
#define optrunset(r) do {int r(); opt_run_set(r);} while(0)

#ifdef __cplusplus
}
#endif

/* BEGIN_AUTOGENERATED */

/* Define the symbol OPT_NOCPLUSINTERFACE to disable the opt C++ interface */
/* when compiling C++ programs. You may need this if your C++ */
/* is broken! */
#ifndef OPT_NOCPLUSINTERFACE

/* OptRegister() only available on C++ systems */
#ifdef __cplusplus
extern int OptRegister(char *v, char c, char *n=0, char *B=0);
extern int OptRegister(char *v, char *n, char *B=0);
extern int OptRegister(char *v, opt_TYPE o, char c, char *n=0, char *B=0);
extern int OptRegister(char *v, opt_TYPE o, char *n, char *B=0);
extern int OptRegister(char *v, opt_MODE m, char c, char *n=0, char *B=0);
extern int OptRegister(char *v, opt_MODE m, char *n, char *B=0);
extern int OptRegister(char *v, opt_TYPE o, opt_MODE m, char c, char *n=0, char *B=0);
extern int OptRegister(char *v, opt_TYPE o, opt_MODE m, char *n, char *B=0);
extern int OptRegister(short *v, char c, char *n=0, char *B=0);
extern int OptRegister(short *v, char *n, char *B=0);
extern int OptRegister(short *v, opt_TYPE o, char c, char *n=0, char *B=0);
extern int OptRegister(short *v, opt_TYPE o, char *n, char *B=0);
extern int OptRegister(short *v, opt_MODE m, char c, char *n=0, char *B=0);
extern int OptRegister(short *v, opt_MODE m, char *n, char *B=0);
extern int OptRegister(short *v, opt_TYPE o, opt_MODE m, char c, char *n=0, char *B=0);
extern int OptRegister(short *v, opt_TYPE o, opt_MODE m, char *n, char *B=0);
extern int OptRegister(unsigned short *v, char c, char *n=0, char *B=0);
extern int OptRegister(unsigned short *v, char *n, char *B=0);
extern int OptRegister(unsigned short *v, opt_TYPE o, char c, char *n=0, char *B=0);
extern int OptRegister(unsigned short *v, opt_TYPE o, char *n, char *B=0);
extern int OptRegister(unsigned short *v, opt_MODE m, char c, char *n=0, char *B=0);
extern int OptRegister(unsigned short *v, opt_MODE m, char *n, char *B=0);
extern int OptRegister(unsigned short *v, opt_TYPE o, opt_MODE m, char c, char *n=0, char *B=0);
extern int OptRegister(unsigned short *v, opt_TYPE o, opt_MODE m, char *n, char *B=0);
extern int OptRegister(int *v, char c, char *n=0, char *B=0);
extern int OptRegister(int *v, char *n, char *B=0);
extern int OptRegister(int *v, opt_TYPE o, char c, char *n=0, char *B=0);
extern int OptRegister(int *v, opt_TYPE o, char *n, char *B=0);
extern int OptRegister(int *v, opt_MODE m, char c, char *n=0, char *B=0);
extern int OptRegister(int *v, opt_MODE m, char *n, char *B=0);
extern int OptRegister(int *v, opt_TYPE o, opt_MODE m, char c, char *n=0, char *B=0);
extern int OptRegister(int *v, opt_TYPE o, opt_MODE m, char *n, char *B=0);
extern int OptRegister(unsigned long *v, char c, char *n=0, char *B=0);
extern int OptRegister(unsigned long *v, char *n, char *B=0);
extern int OptRegister(unsigned long *v, opt_TYPE o, char c, char *n=0, char *B=0);
extern int OptRegister(unsigned long *v, opt_TYPE o, char *n, char *B=0);
extern int OptRegister(unsigned long *v, opt_MODE m, char c, char *n=0, char *B=0);
extern int OptRegister(unsigned long *v, opt_MODE m, char *n, char *B=0);
extern int OptRegister(unsigned long *v, opt_TYPE o, opt_MODE m, char c, char *n=0, char *B=0);
extern int OptRegister(unsigned long *v, opt_TYPE o, opt_MODE m, char *n, char *B=0);
extern int OptRegister(double *v, char c, char *n=0, char *B=0);
extern int OptRegister(double *v, char *n, char *B=0);
extern int OptRegister(double *v, opt_TYPE o, char c, char *n=0, char *B=0);
extern int OptRegister(double *v, opt_TYPE o, char *n, char *B=0);
extern int OptRegister(double *v, opt_MODE m, char c, char *n=0, char *B=0);
extern int OptRegister(double *v, opt_MODE m, char *n, char *B=0);
extern int OptRegister(double *v, opt_TYPE o, opt_MODE m, char c, char *n=0, char *B=0);
extern int OptRegister(double *v, opt_TYPE o, opt_MODE m, char *n, char *B=0);
extern int OptRegister(long *v, char c, char *n=0, char *B=0);
extern int OptRegister(long *v, char *n, char *B=0);
extern int OptRegister(long *v, opt_TYPE o, char c, char *n=0, char *B=0);
extern int OptRegister(long *v, opt_TYPE o, char *n, char *B=0);
extern int OptRegister(long *v, opt_MODE m, char c, char *n=0, char *B=0);
extern int OptRegister(long *v, opt_MODE m, char *n, char *B=0);
extern int OptRegister(long *v, opt_TYPE o, opt_MODE m, char c, char *n=0, char *B=0);
extern int OptRegister(long *v, opt_TYPE o, opt_MODE m, char *n, char *B=0);
extern int OptRegister(unsigned int *v, char c, char *n=0, char *B=0);
extern int OptRegister(unsigned int *v, char *n, char *B=0);
extern int OptRegister(unsigned int *v, opt_TYPE o, char c, char *n=0, char *B=0);
extern int OptRegister(unsigned int *v, opt_TYPE o, char *n, char *B=0);
extern int OptRegister(unsigned int *v, opt_MODE m, char c, char *n=0, char *B=0);
extern int OptRegister(unsigned int *v, opt_MODE m, char *n, char *B=0);
extern int OptRegister(unsigned int *v, opt_TYPE o, opt_MODE m, char c, char *n=0, char *B=0);
extern int OptRegister(unsigned int *v, opt_TYPE o, opt_MODE m, char *n, char *B=0);
extern int OptRegister(char **v, char c, char *n=0, char *B=0);
extern int OptRegister(char **v, char *n, char *B=0);
extern int OptRegister(char **v, opt_TYPE o, char c, char *n=0, char *B=0);
extern int OptRegister(char **v, opt_TYPE o, char *n, char *B=0);
extern int OptRegister(char **v, opt_MODE m, char c, char *n=0, char *B=0);
extern int OptRegister(char **v, opt_MODE m, char *n, char *B=0);
extern int OptRegister(char **v, opt_TYPE o, opt_MODE m, char c, char *n=0, char *B=0);
extern int OptRegister(char **v, opt_TYPE o, opt_MODE m, char *n, char *B=0);
extern int OptRegister(float *v, char c, char *n=0, char *B=0);
extern int OptRegister(float *v, char *n, char *B=0);
extern int OptRegister(float *v, opt_TYPE o, char c, char *n=0, char *B=0);
extern int OptRegister(float *v, opt_TYPE o, char *n, char *B=0);
extern int OptRegister(float *v, opt_MODE m, char c, char *n=0, char *B=0);
extern int OptRegister(float *v, opt_MODE m, char *n, char *B=0);
extern int OptRegister(float *v, opt_TYPE o, opt_MODE m, char c, char *n=0, char *B=0);
extern int OptRegister(float *v, opt_TYPE o, opt_MODE m, char *n, char *B=0);
extern int OptRegister(unsigned char *v, char c, char *n=0, char *B=0);
extern int OptRegister(unsigned char *v, char *n, char *B=0);
extern int OptRegister(unsigned char *v, opt_TYPE o, char c, char *n=0, char *B=0);
extern int OptRegister(unsigned char *v, opt_TYPE o, char *n, char *B=0);
extern int OptRegister(unsigned char *v, opt_MODE m, char c, char *n=0, char *B=0);
extern int OptRegister(unsigned char *v, opt_MODE m, char *n, char *B=0);
extern int OptRegister(unsigned char *v, opt_TYPE o, opt_MODE m, char c, char *n=0, char *B=0);
extern int OptRegister(unsigned char *v, opt_TYPE o, opt_MODE m, char *n, char *B=0);
#endif /*__cplusplus */
#endif /* OPT_NOCPLUSINTERFACE */

/* The following prototypes are deprecated and may be removed some day */

#ifdef __cplusplus
extern "C" {
#endif
#define CHAR OPT_CHAR /* ...but use the OPT_CHAR */
extern int optrega_CHAR(char *v, char c, char *n, char *B);
extern int optreg_CHAR(char *v, char c, char *B);
extern int optregc_CHAR(char *v, char c);
extern int optregs_CHAR(char *v, char *n);
extern int optregcb_CHAR(char *v, char c, char *B);
extern int optregsb_CHAR(char *v, char *s, char *B);
#define CSTRING OPT_CSTRING /* ...but use the OPT_CSTRING */
extern int optrega_CSTRING(char *v, char c, char *n, char *B);
extern int optreg_CSTRING(char *v, char c, char *B);
extern int optregc_CSTRING(char *v, char c);
extern int optregs_CSTRING(char *v, char *n);
extern int optregcb_CSTRING(char *v, char c, char *B);
extern int optregsb_CSTRING(char *v, char *s, char *B);
#define UNDELIM OPT_UNDELIM /* ...but use the OPT_UNDELIM */
extern int optrega_UNDELIM(char **v, char c, char *n, char *B);
extern int optreg_UNDELIM(char **v, char c, char *B);
extern int optregc_UNDELIM(char **v, char c);
extern int optregs_UNDELIM(char **v, char *n);
extern int optregcb_UNDELIM(char **v, char c, char *B);
extern int optregsb_UNDELIM(char **v, char *s, char *B);
#define VSTRING OPT_VSTRING /* ...but use the OPT_VSTRING */
extern int optrega_VSTRING(char **v, char c, char *n, char *B);
extern int optreg_VSTRING(char **v, char c, char *B);
extern int optregc_VSTRING(char **v, char c);
extern int optregs_VSTRING(char **v, char *n);
extern int optregcb_VSTRING(char **v, char c, char *B);
extern int optregsb_VSTRING(char **v, char *s, char *B);
#define DOUBLE OPT_DOUBLE /* ...but use the OPT_DOUBLE */
extern int optrega_DOUBLE(double *v, char c, char *n, char *B);
extern int optreg_DOUBLE(double *v, char c, char *B);
extern int optregc_DOUBLE(double *v, char c);
extern int optregs_DOUBLE(double *v, char *n);
extern int optregcb_DOUBLE(double *v, char c, char *B);
extern int optregsb_DOUBLE(double *v, char *s, char *B);
#define NUL OPT_NUL /* ...but use the OPT_NUL */
extern int optrega_NUL(void *v, char c, char *n, char *B);
extern int optreg_NUL(void *v, char c, char *B);
extern int optregc_NUL(void *v, char c);
extern int optregs_NUL(void *v, char *n);
extern int optregcb_NUL(void *v, char c, char *B);
extern int optregsb_NUL(void *v, char *s, char *B);
#define BOOL OPT_BOOL /* ...but use the OPT_BOOL */
extern int optrega_BOOL(int *v, char c, char *n, char *B);
extern int optreg_BOOL(int *v, char c, char *B);
extern int optregc_BOOL(int *v, char c);
extern int optregs_BOOL(int *v, char *n);
extern int optregcb_BOOL(int *v, char c, char *B);
extern int optregsb_BOOL(int *v, char *s, char *B);
#define UNDELIMV OPT_UNDELIMV /* ...but use the OPT_UNDELIMV */
extern int optrega_UNDELIMV(char **v, char c, char *n, char *B);
extern int optreg_UNDELIMV(char **v, char c, char *B);
extern int optregc_UNDELIMV(char **v, char c);
extern int optregs_UNDELIMV(char **v, char *n);
extern int optregcb_UNDELIMV(char **v, char c, char *B);
extern int optregsb_UNDELIMV(char **v, char *s, char *B);
#define LONG OPT_LONG /* ...but use the OPT_LONG */
extern int optrega_LONG(long *v, char c, char *n, char *B);
extern int optreg_LONG(long *v, char c, char *B);
extern int optregc_LONG(long *v, char c);
extern int optregs_LONG(long *v, char *n);
extern int optregcb_LONG(long *v, char c, char *B);
extern int optregsb_LONG(long *v, char *s, char *B);
#define UINT OPT_UINT /* ...but use the OPT_UINT */
extern int optrega_UINT(unsigned int *v, char c, char *n, char *B);
extern int optreg_UINT(unsigned int *v, char c, char *B);
extern int optregc_UINT(unsigned int *v, char c);
extern int optregs_UINT(unsigned int *v, char *n);
extern int optregcb_UINT(unsigned int *v, char c, char *B);
extern int optregsb_UINT(unsigned int *v, char *s, char *B);
#define UNSINT OPT_UNSINT /* ...but use the OPT_UNSINT */
extern int optrega_UNSINT(unsigned int *v, char c, char *n, char *B);
extern int optreg_UNSINT(unsigned int *v, char c, char *B);
extern int optregc_UNSINT(unsigned int *v, char c);
extern int optregs_UNSINT(unsigned int *v, char *n);
extern int optregcb_UNSINT(unsigned int *v, char c, char *B);
extern int optregsb_UNSINT(unsigned int *v, char *s, char *B);
#define STRING OPT_STRING /* ...but use the OPT_STRING */
extern int optrega_STRING(char **v, char c, char *n, char *B);
extern int optreg_STRING(char **v, char c, char *B);
extern int optregc_STRING(char **v, char c);
extern int optregs_STRING(char **v, char *n);
extern int optregcb_STRING(char **v, char c, char *B);
extern int optregsb_STRING(char **v, char *s, char *B);
#define UCHAR OPT_UCHAR /* ...but use the OPT_UCHAR */
extern int optrega_UCHAR(unsigned char *v, char c, char *n, char *B);
extern int optreg_UCHAR(unsigned char *v, char c, char *B);
extern int optregc_UCHAR(unsigned char *v, char c);
extern int optregs_UCHAR(unsigned char *v, char *n);
extern int optregcb_UCHAR(unsigned char *v, char c, char *B);
extern int optregsb_UCHAR(unsigned char *v, char *s, char *B);
#define SHORT OPT_SHORT /* ...but use the OPT_SHORT */
extern int optrega_SHORT(short *v, char c, char *n, char *B);
extern int optreg_SHORT(short *v, char c, char *B);
extern int optregc_SHORT(short *v, char c);
extern int optregs_SHORT(short *v, char *n);
extern int optregcb_SHORT(short *v, char c, char *B);
extern int optregsb_SHORT(short *v, char *s, char *B);
#define USHORT OPT_USHORT /* ...but use the OPT_USHORT */
extern int optrega_USHORT(unsigned short *v, char c, char *n, char *B);
extern int optreg_USHORT(unsigned short *v, char c, char *B);
extern int optregc_USHORT(unsigned short *v, char c);
extern int optregs_USHORT(unsigned short *v, char *n);
extern int optregcb_USHORT(unsigned short *v, char c, char *B);
extern int optregsb_USHORT(unsigned short *v, char *s, char *B);
#define INT OPT_INT /* ...but use the OPT_INT */
extern int optrega_INT(int *v, char c, char *n, char *B);
extern int optreg_INT(int *v, char c, char *B);
extern int optregc_INT(int *v, char c);
extern int optregs_INT(int *v, char *n);
extern int optregcb_INT(int *v, char c, char *B);
extern int optregsb_INT(int *v, char *s, char *B);
#define NEGBOOL OPT_NEGBOOL /* ...but use the OPT_NEGBOOL */
extern int optrega_NEGBOOL(int *v, char c, char *n, char *B);
extern int optreg_NEGBOOL(int *v, char c, char *B);
extern int optregc_NEGBOOL(int *v, char c);
extern int optregs_NEGBOOL(int *v, char *n);
extern int optregcb_NEGBOOL(int *v, char c, char *B);
extern int optregsb_NEGBOOL(int *v, char *s, char *B);
#define UNDELIMC OPT_UNDELIMC /* ...but use the OPT_UNDELIMC */
extern int optrega_UNDELIMC(char *v, char c, char *n, char *B);
extern int optreg_UNDELIMC(char *v, char c, char *B);
extern int optregc_UNDELIMC(char *v, char c);
extern int optregs_UNDELIMC(char *v, char *n);
extern int optregcb_UNDELIMC(char *v, char c, char *B);
extern int optregsb_UNDELIMC(char *v, char *s, char *B);
#define ULONG OPT_ULONG /* ...but use the OPT_ULONG */
extern int optrega_ULONG(unsigned long *v, char c, char *n, char *B);
extern int optreg_ULONG(unsigned long *v, char c, char *B);
extern int optregc_ULONG(unsigned long *v, char c);
extern int optregs_ULONG(unsigned long *v, char *n);
extern int optregcb_ULONG(unsigned long *v, char c, char *B);
extern int optregsb_ULONG(unsigned long *v, char *s, char *B);
#define FLOAT OPT_FLOAT /* ...but use the OPT_FLOAT */
extern int optrega_FLOAT(float *v, char c, char *n, char *B);
extern int optreg_FLOAT(float *v, char c, char *B);
extern int optregc_FLOAT(float *v, char c);
extern int optregs_FLOAT(float *v, char *n);
extern int optregcb_FLOAT(float *v, char c, char *B);
extern int optregsb_FLOAT(float *v, char *s, char *B);
#define NEGTOGGLE OPT_NEGTOGGLE /* ...but use the OPT_NEGTOGGLE */
extern int optrega_NEGTOGGLE(int *v, char c, char *n, char *B);
extern int optreg_NEGTOGGLE(int *v, char c, char *B);
extern int optregc_NEGTOGGLE(int *v, char c);
extern int optregs_NEGTOGGLE(int *v, char *n);
extern int optregcb_NEGTOGGLE(int *v, char c, char *B);
extern int optregsb_NEGTOGGLE(int *v, char *s, char *B);
#define INTLEVEL OPT_INTLEVEL /* ...but use the OPT_INTLEVEL */
extern int optrega_INTLEVEL(int *v, char c, char *n, char *B);
extern int optreg_INTLEVEL(int *v, char c, char *B);
extern int optregc_INTLEVEL(int *v, char c);
extern int optregs_INTLEVEL(int *v, char *n);
extern int optregcb_INTLEVEL(int *v, char c, char *B);
extern int optregsb_INTLEVEL(int *v, char *s, char *B);
#define TOGGLE OPT_TOGGLE /* ...but use the OPT_TOGGLE */
extern int optrega_TOGGLE(int *v, char c, char *n, char *B);
extern int optreg_TOGGLE(int *v, char c, char *B);
extern int optregc_TOGGLE(int *v, char c);
extern int optregs_TOGGLE(int *v, char *n);
extern int optregcb_TOGGLE(int *v, char c, char *B);
extern int optregsb_TOGGLE(int *v, char *s, char *B);
#ifdef __cplusplus
}
#endif
/* END_AUTOGENERATED */

#endif /* _OPT_H */


