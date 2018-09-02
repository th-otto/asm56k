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
/* $Id: opt_reg.c,v 3.50 2003/03/18 16:29:40 jt Exp $ */
/* opt_reg.c */

/************************ register options ********************************
 *
 *    options are stored in an array,
 *    each element of which is a structure
 *
 *    The members of the structure contain information about
 *        1) the variable which is altered
 *        2) the type of variable
 *        3) a character by which the variable is invoked
 *        4) a string by which the variable can be invoked
 *        5) the positional value of the option if applicable
 *        6) a brief description of the variable's role in the program
 *        7) a longer help message
 *        8) a flag indicating that an option has been invoked
 *        9) a "hook", a function that is called when variable is invoked
 *        10) a pointer to an array structure if the option is for an array
 *            
 *    The structure elements are not assigned directly, but are
 *    "registered" with a call to the function optreg().
 *        
 *    In this file are the routines which access the structure of
 *    registered options.
 *
 *          I) Register options 
 *         II) Get number corresponding to name of option
 *        III) Print options out
 *             a) to a file which can be used as an input command line
 *             b) to the screen as part of a usage message
 *             c) to the screen as part of an interactive menu
 *         IV) Process single delimited and positional options
 */

#ifndef	SINGLEFILE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "opt.h"
#include "ag.h"
#include "opt_p.h"
#endif

/**
 * If you would like to remove these warnings then re-compile opt with
 * gcc -DOPT_DEPRECATED_OK
 */

static void
opt_CSTRING_deprecated(void)
{
#ifndef OPT_DEPRECATED_OK
  opt_warning("OPT_CSTRING is deprecated...you should use OPT_STRING instead");
  opt_warning("If this warning message is a problem, then recompile the source");
  opt_warning("with the symbol OPT_DEPRECATED_OK defined.");
#endif
}

static void
opt_UNDELIM_deprecated(void)
{
#ifndef OPT_DEPRECATED_OK
  opt_warning("OPT_UNDELIM and OPT_UNDELIMC are deprecated...");
  opt_warning("You should use positional options instead.");
  opt_warning("If this warning message is a problem, then recompile the source");
  opt_warning("with the symbol OPT_DEPRECATED_OK defined.");
#endif
}



/* Default is to indicate whether a flag is true or false by using + or -
 * But, by #define'ing FLAG_ZERO_ONE, we will use 1 or 0 instead
 */

#ifdef FLAG_ZERO_ONE
#define FLAG_TRUE '1'
#define FLAG_FALSE '0'        
#else
#define FLAG_TRUE '+'
#define FLAG_FALSE '-'        
#endif

int opt_exit_number=0;          /* if we have to exit(), then exit
                                 * with this number.
                                 */

        /* -------------------------- */
        /* List of Registered Options */
        /* -------------------------- */


typedef struct {
  void **value;      /* pointer to array of data */
  int *size;         /* pointer to size of data (number of elements in array) */
  char delim;        /* char delimits elements of array: eg "," in "1,2,3" */
  opt_TYPE base_type;  /* what the array of data is... */
}   OptArray_t;


typedef struct {
  void      *value;         /* pointer to value of option */
  opt_TYPE   type;          /* will be cast according to type */
  char       name;          /* name by which option is invoked */
  char      *longname;      /* long version of the name */
  opt_MODE   mode;          /* delimited, positional or flexible? */
  char      *descript;      /* a brief description */
  char      *help;          /* longer help message */
  int        invoked;       /* counts number of times option is invoked */
  OPT_HOOK   hook;          /* code to evaluate if option is invoked */
  OptArray_t  *array;       /* set to null if not an array option */
} Option_t;

static	Option_t *optlist=NULL;             /* array of options */
int              opt_nreg=0;                /* number of registered opts */

/* Prototypes for static (local) functions */
static void opt_fileheader(FILE *);
static int opt_action(int, ARGVECTOR *);
static int opt_number(void *);
static int opt_longname_number(char *);
static char *optarray_getstrval(int, void *, opt_TYPE, char);
static char *opt_getstrval( void *, opt_TYPE );
static int optsizeof(opt_TYPE);
static void opt_setvalue(void *, opt_TYPE, char *);
static void optarray_action(OptArray_t *, char *);

static char opt_array_delim=',';    /* the delimiter for arrays. (eads)*/

    /* ---------- */
    /* OPT macros */
    /* ---------- */

#define OPT_isvalidnum(n) ((n)>=0 && (n)<opt_nreg)

#define	OPT_isflagtype(o) \
        ((o)==OPT_TOGGLE || (o)==OPT_BOOL || \
         (o)==OPT_NEGTOGGLE || (o)==OPT_NEGBOOL)


/* OPT_GETVALUE: returns value of what 'void *' points to */
/* OPT_SETVALUE: sets the value that the 'void *' points to */

#define OPT_GETVALUE(typ,v)      ((typ)(*((typ *)(v))))
#define OPT_SETVALUE(typ,v,val)  do { typ *xptr;       \
                                      xptr = (typ *)v; \
                                      *xptr = (typ)val; } while (0)

/* OPT_GET_ITH_VALUE: gets value of i'th option */
/* OPT_SET_ITH_VALUE: sets value of i'th option */

#define	OPT_GET_ITH_VALUE(typ,i)     OPT_GETVALUE(typ,optlist[i].value)
#define OPT_SET_ITH_VALUE(typ,i,val) OPT_SETVALUE(typ,optlist[i].value,val)


void
opt_free(void)
{
  int i;
  for (i=0; i<opt_nreg; ++i) {
    OPT_FREE(optlist[i].longname);
    OPT_FREE(optlist[i].help);
    OPT_FREE(optlist[i].descript);
    if (optlist[i].array && optlist[i].type == OPT_STRING) {
      char *s = *((char **)(optlist[i].value));
      OPT_FREE(s);
      OPT_FREE(optlist[i].value);
    }
    OPT_FREE(optlist[i].array);
  }
  OPT_FREE(optlist);
  opt_nreg=0;
  opt_freestrings();
}

static char *
optarray_getstrval( int size, void *val, opt_TYPE otype, char delim )
{
  int n;
  int len=0;
  char *str_val = NULL; /* string with all values in array 
                          * will be re-allocated for each item
                          */
  /* Convert the array 'val' into a string 'str_val' */
  for ( n = 0; n < size; n++ ) {
    /* this next line may look a little arcane...
     * 1. Recall val is in real life a pointer to an array, that is type **
     *    but we don't know the type, so we pass it through the argument list
     *    as a void *
     * 2. *(void **)val recasts val to a void ** and then dereferences it
     *    so that it is the pointer that is the array, but its type is
     *    `void *' instead of `type *'
     * 3. (char *)(*(void **)val) recasts this generic `void *' to be
     *    a `char *' because we know what the size of a char is (it's 1)
     *    and we can do arithmetic with it
     * 4. n*optsizeof(otype) is the offset of the n'th element of the array
     * 5. nth_val points to the n'th value in the array and if we were to
     *    recast it as *(type *)nth_val it would be the explicit value.
     *    Of course we don't know type, but we do know otype, and so
     *    we let opt_getstrval() take care of that
     */
    char *nth_val = (char *)(*(void **)val) + n*optsizeof( otype );
    char *str_nth_val = opt_getstrval( nth_val, otype );
    char str_delim[2] = { delim, '\0' }; 

    if ( n == size-1 ) str_delim[0]='\0';  /* no delimiter at the end */
      
    /* Make room in str_val for the next item */
    len += strlen(str_nth_val) + strlen(str_delim);
    if (str_val) {
      str_val = (char *)realloc(str_val,sizeof(char)*(len+1));
    } else {
      str_val = (char *)malloc(sizeof(char)*(len+1));
      str_val[0]='\0';
    }
    strcat(str_val,str_nth_val);
    strcat(str_val,str_delim);
  }
  return str_val;
}

        /* ------------------------------------------- */
        /* Routine to register each option as an array */
        /* Added by: Damian Eads <dre9227@cs.rit.edu>  */
        /* ------------------------------------------- */

int
optrega_array(  int *size, void *val, opt_TYPE otype, char name,
                char *longname, char *descript )
{
#if 0  
  static char **str_val_array=NULL;
  static int    str_val_array_n=0;

    ++str_val_array_n;
    str_val_array = (char **)realloc(str_val_array,
                                     str_val_array_n*sizeof(char *));
    str_val_array[str_val_array_n-1] = optarray_getstrval(*size,val,otype,opt_array_delim);
  
    /* Register the option as an OPT_STRING */
    optrega( &str_val_array[str_val_array_n-1],
             OPT_STRING, name, longname, descript );
#else
    char **s = (char **)malloc(sizeof(char *));
    *s = optarray_getstrval(*size,val,otype,opt_array_delim);
    optrega( s, OPT_STRING, name, longname, descript );
#endif     

    /* Warn if inappropriate base_type is specified */
    switch (otype) {
      /* These are all okay */
    case OPT_INT:
    case OPT_UINT:
    case OPT_SHORT:
    case OPT_USHORT:
    case OPT_LONG:
    case OPT_ULONG:
    case OPT_FLOAT:
    case OPT_DOUBLE:
      break;

      /* These _should_ be okay too */
    case OPT_STRING:
      break;

      /* These might be problematic */
    case OPT_CSTRING:    
    case OPT_UNDELIMC:
      opt_CSTRING_deprecated(); /* no break */
    case OPT_CHAR:
    case OPT_UCHAR:
    case OPT_INTLEVEL:
    case OPT_TOGGLE:
    case OPT_BOOL:
    case OPT_NEGTOGGLE:
    case OPT_NEGBOOL:
    case OPT_UNDELIM:
      opt_UNDELIM_deprecated(); /* no break */
    case OPT_NUL:
      opt_warn_1("Undefined behavior for %s array",optstrtyp(opt_nreg-1));
      break;

    default:
        opt_fatal("opt_getstrval: Undefined o-type");
        break;
    }

    /* Allocate an OptArray_t structure */
    optlist[ opt_nreg-1 ].array = (OptArray_t *)malloc(sizeof(OptArray_t));
                                   
    /* Then add array-specific information to the registered option */
    optlist[ opt_nreg-1 ].array->value     = (void **)val;
    optlist[ opt_nreg-1 ].array->size      = size;
    optlist[ opt_nreg-1 ].array->base_type = otype;
    optlist[ opt_nreg-1 ].array->delim     = opt_array_delim;
    
    return opt_nreg-1;
}

int optreg_array( int *size, void *val, opt_TYPE otype, char name,
		  char *descript ) {
    return optrega_array(size,val,otype,name,NULL,descript);
}
int optregc_array( int *size, void *val, opt_TYPE otype, char name ) {
    return optrega_array(size,val,otype,name,NULL,NULL);
}
int optregcb_array( int *size, void *val, opt_TYPE otype, char name,
		    char *descript ) {
    return optrega_array(size,val,otype,name,NULL,descript);
}
int optregs_array( int *size, void *val, opt_TYPE otype, char *longname ) {
    return optrega_array(size,val,otype,'\0',longname,longname);
}
int optregsb_array( int *size, void *val, opt_TYPE otype, char *longname,
		    char *descript) {
    return optrega_array(size,val,otype,'\0',longname,descript);
}
int optregp_array( int *size, void *val, opt_TYPE otype, char* longname, char* descript) {
    int n = optrega_array(size,val,otype,'\0',longname,descript);
    optmode_n(n, OPT_POSITIONAL);
    return n;
}
int optregf_array( int *size, void *val, opt_TYPE otype, char name, char* longname, char* descript) {
    int n = optrega_array(size,val,otype,name,longname,descript);
    optmode_n(n, OPT_FLEXIBLE);
    return n;
}


        /* ------------------------------- */
        /* Routine to register each option */
        /* ------------------------------- */

int optreg(void *val, opt_TYPE otype, char name, char *descript) {
    return optrega(val,otype,name,NULL,descript);
}
int optregc(void *val, opt_TYPE otype, char name) {
    return optrega(val,otype,name,NULL,NULL);
}
int optregcb(void *val, opt_TYPE otype, char name, char *descript) {
    return optrega(val,otype,name,NULL,descript);
}
int optregs(void *val, opt_TYPE otype, char *longname) {
    return optrega(val,otype,'\0',longname,longname);
}
int optregsb(void *val, opt_TYPE otype, char *longname, char *descript) {
    return optrega(val,otype,'\0',longname,descript);
}
int optregp(void *val, opt_TYPE otype, char* longname, char* descript) {
  int n = optrega(val, otype, '\0', longname, descript);
  optmode_n(n, OPT_POSITIONAL);
  return n;
}
int optregf(void *val, opt_TYPE otype, char name, char* longname, char* descript) {
  int n = optrega(val, otype, name, longname, descript);
  optmode_n(n, OPT_FLEXIBLE);
  return n;
}
int optexec(char *longname, OPT_HOOK fcn, char *descript)
{
    int n;
    n = optrega(NULL,OPT_NUL,'\0',longname,descript);
    opthook_n(n,fcn);
    return n;
}
     
int
optrega(void *val, opt_TYPE otype, char name, char *longname, char *descript)
{
    opt_MODE mode = OPT_DELIMITED;

    /* write a warning if that option name has been used before */
    if( name != '\0' && opt_char_number(name) != -1 ) {
        opt_warn_1("optrega: Duplicate option name \'%c\'",name);
    }
    if (!ISEMPTYSTRING(longname) && opt_longname_number(longname) != -1) {
        opt_warn_1("optrega: Duplicate option longname \"%s\"",longname);
    }

    /* val points to the memory where the value of the option
     * is maintained.  This cannot be NULL.
     */
    if( val == NULL && otype != OPT_NUL ) {
        if (name)     { opt_warn_1("Name: -%c", name); }
        if (longname) { opt_warn_1("Name: --%s",longname); }
        opt_fatal("optrega: invalid first argument=NULL\n");
    }
    
    /* This is probably a mistake to print out a warning message,
       since it will be printed every time the program is run.
       Perhaps it should only be run if the user types 'run --testopt'
       */
#if 0    
    if( val != NULL  && opt_val_number(val) != -1) {
        opt_warn_1("optrega: This variable [%s] has already been registered",
                   thename);
    }
#endif    
    
    /* Programmer may optionally signal that an option is to be a
     * positional option by setting the name to '\0' and the longname
     * to NULL or an empty string.
     */
    if(name=='\0' && ISEMPTYSTRING(longname)) {
      mode = OPT_POSITIONAL;
    }

    /* The old opt concept of undelimited strings has been */
    /* replaced with that of positional options. The old   */
    /* OPT_UNDELIM type for instance, corresponds to a new */
    /* option of type OPT_STRING and mode OPT_POSITIONAL.  */
    if (otype == OPT_UNDELIM) {
      opt_UNDELIM_deprecated();
      otype = OPT_STRING;
      mode = OPT_POSITIONAL;
    }
    else if (otype == OPT_UNDELIMC) {
      opt_UNDELIM_deprecated();
      otype = OPT_CSTRING;
      mode = OPT_POSITIONAL;
    }

    /* Make sure that the longname doesn't have any whitespace */
    if (!ISEMPTYSTRING(longname)) {
     	char *t = longname;
        while (*t != '\0') {
          if (isspace(*t)) {
            opt_fatal_1("invalid longname [%s] contains whitespace\n",longname);
          }
          ++t;
        }
    }

    /* reallocate another struct for the new opt */
    ++opt_nreg;
    optlist = (Option_t *)realloc(optlist,opt_nreg*sizeof(Option_t));

    /* Having checked for various warnings, now register the options */
    
    optlist[opt_nreg-1].value         = val;
    optlist[opt_nreg-1].type          = otype;
    optlist[opt_nreg-1].name          = name;
    optlist[opt_nreg-1].longname      = longname ? strdup(longname) : NULL;
    optlist[opt_nreg-1].mode          = mode;
    optlist[opt_nreg-1].descript      = descript ? strdup(descript) : NULL;

    optlist[opt_nreg-1].invoked       = 0;
    optlist[opt_nreg-1].help          = NULL;
    optlist[opt_nreg-1].hook          = NULL;
    optlist[opt_nreg-1].array         = NULL;

    return opt_nreg-1;

}

void optchar_n(int n, char name)
{
    if (OPT_isvalidnum(n)) optlist[n].name = name;
}    
void optmode_n(int n, opt_MODE mode)
{
  if (OPT_isvalidnum(n)) {
    if (!(mode == OPT_POSITIONAL || mode == OPT_DELIMITED || mode == OPT_FLEXIBLE))
      opt_warning("optmode() was passed an invalid mode");
    optlist[n].mode = mode;
  }
}
void optdescript_n(int n, char *descript)
{
    if (OPT_isvalidnum(n)) opt_setstring(&optlist[n].descript,descript);
}
void opthelp_n(int n, char *help)
{
    if (OPT_isvalidnum(n)) opt_setstring(&optlist[n].help,help);
}
void optlongname_n(int n, char *longname)
{
    if (OPT_isvalidnum(n)) opt_setstring(&optlist[n].longname,longname);
}
void opthook_n(int n, OPT_HOOK hook)
{
    if (OPT_isvalidnum(n)) optlist[n].hook = hook;
}
static int optinvoked_n(int n)
{
    if (OPT_isvalidnum(n))
        return optlist[n].invoked;
    return 0;
}

int optinvoked(void *v) { return optinvoked_n( opt_number(v) ); }

void optchar(void *v,  char name) { optchar_n( opt_number(v),   name ); }
void opthelp(void *v,  char *help) { opthelp_n( opt_number(v),  help ); }
void opthook(void *v,  OPT_HOOK hook) { opthook_n( opt_number(v), hook ); }      
void optlongname(void *v, char *longname)
{
    optlongname_n( opt_number(v), longname );
}
void optmode(void* v, opt_MODE mode) 
{
  optmode_n( opt_number(v), mode );
}
void optdescript(void *v, char *descript)
{
    optdescript_n( opt_number(v), descript );
}
void optarraydelim_n(int n, char delim)
{
    if (OPT_isvalidnum(n) && optlist[n].array) {
      if (optlist[n].array->delim != delim) {
        optlist[n].array->delim = delim;
        if (*(optlist[n].array->size) > 0) {
          OPT_SET_ITH_VALUE(char *,n,\
                            optarray_getstrval(*(optlist[n].array->size),
                                               optlist[n].array->value,
                                               optlist[n].array->base_type,
                                               optlist[n].array->delim));
        }
        
      }
    }
}  
void optarraydelim(void *v, char delim)
{
    int n;
    if (v == NULL) {
      /* then reset default array delim and all opt delims*/
      opt_array_delim = delim;
      for (n=0; n<opt_nreg; ++n) {
        optarraydelim_n(n,delim);
      }
    } else {
      /* just reset for the specific option */
      n = opt_number(v);
      optarraydelim_n(n,delim);
    }
}
    
void optExitNumber(int n) {
    opt_exit_number = n;
}


void
opt_get_help(char c)
{
    int n;
    n = opt_char_number(c);
    if (OPT_isvalidnum(n) && optlist[n].help != NULL)
        opt_mess_2("%c: %s\n",c,optlist[n].help);
    else
        opt_mess_1("Help unavailable for \'%c\'\n",c);
}
	
	

/* opt_XXX_number(): Get number corresponding to option name; return
 * a value of -1 if the option does not exist.
 *  opt_char_number(c) takes as input the single-character name;
 *  opt_longname_number(longname) takes as input the long name
 *  opt_number(void *) takes as input pointer to variable
 */

int
opt_char_number(char c)
{
    int i;                         /* see which registered option */
    for(i=0; i<opt_nreg; ++i) {
        if( c == optlist[i].name )
            return i;
    }
    return -1;
}
    
static int
opt_number(void *v)
{
    int i;                         /* see which registered option */
    for(i=0; i<opt_nreg; ++i) {
      if (optlist[i].array && v == optlist[i].array->value) return i;
      if (!optlist[i].array && v == optlist[i].value ) return i;
    }
    return -1;                     /* to signify not an option */
}
static int
opt_longname_number(char *s)
{
    int i;                         /* see which registered option */
    if ( ISEMPTYSTRING(s) ) return -1;
    for(i=0; i<opt_nreg; ++i) {
        if ( !ISEMPTYSTRING(optlist[i].longname) ) {
            if (0==strcmp(s,optlist[i].longname)) {
                return i;
            }
        }
    }
    return -1;                     /* to signify not an option */
}

	/* -------------------------------------------- */
	/* Print value of registered option to a string */
	/* -------------------------------------------- */

/* optstrval:
 * returns a string corresponding to the ith option
 * Thus if x = 12.6, this returns the string "12.6"
 * Calls opt_getstrval for all types except INTLEVEL.
 * The return value is a pointer to a static string which is
 * overwritten with each call
 * FLAG values are returned as "+" or "-"
 * Previously: INTLEVEL values are returned as "- -xxxx..." depending on value
 * Now, more straightforward, treat INTLEVEL just like INT
 */

char *
optstrval(int i)
{
  return opt_getstrval(optlist[i].value,optlist[i].type);
}/*optstrval*/

	/* -------------------------------------------- */
	/* Print value of registered option to a string */
        /* without specifying an index. (eads)          */
	/* -------------------------------------------- */

static char *
opt_getstrval( void *val, opt_TYPE typ ) {
    int maybe;
    static char stval_buf[80];

    switch( typ ) {
    case OPT_INT:
        opt_snprintf_1(stval_buf,80,"%d", OPT_GETVALUE(int,val) );
        break;
    case OPT_UINT:
        opt_snprintf_1(stval_buf,80,"%u", OPT_GETVALUE(unsigned int,val) );
        break;
    case OPT_SHORT:
        opt_snprintf_1(stval_buf,80,"%d", OPT_GETVALUE(short,val) );
        break;
    case OPT_USHORT:
        opt_snprintf_1(stval_buf,80,"%u", OPT_GETVALUE(unsigned short,val) );
        break;
    case OPT_LONG:
        opt_snprintf_1(stval_buf,80,"%ld", OPT_GETVALUE(long,val) );
        break;
    case OPT_ULONG:
        opt_snprintf_1(stval_buf,80,"%lu", OPT_GETVALUE(unsigned long,val) );
        break;
    case OPT_CHAR:
        opt_snprintf_1(stval_buf,80,"%c", OPT_GETVALUE(char,val));
        break;
    case OPT_UCHAR:
        opt_snprintf_1(stval_buf,80,"%c", OPT_GETVALUE(unsigned char,val));
        break;
    case OPT_FLOAT:
        opt_snprintf_1(stval_buf,80,"%g", OPT_GETVALUE(float,val) );
        break;
    case OPT_DOUBLE:
        opt_snprintf_1(stval_buf,80,"%g", OPT_GETVALUE(double,val) );
        break;
    case OPT_INTLEVEL:
        if (1) {
          int v = OPT_GETVALUE(int,val);
          if (v < 0) {
            opt_fatal_1("Invalid (negative) value [v=%d] for OPT_INTLEVEL",v);
          }
          if (v < 10) {
            opt_snprintf_1(stval_buf,80,"%1d",v);
          }
          if (v >= 10) {
            /* Could set it to "- -xxxxx" but need to know x */
            /* and it only works if 'x' is a one-character name */
            opt_warn_1("Cannot set string for OPT_INTLEVEL v=%d > 9",v);
          }
        }
        break;
    case OPT_TOGGLE:
    case OPT_BOOL:
        maybe = OPT_GETVALUE(int,val);
        opt_snprintf_1(stval_buf,80,"%c", (maybe==OPT_TRUE ? FLAG_TRUE : FLAG_FALSE) );
        break;
    case OPT_NEGTOGGLE:
    case OPT_NEGBOOL:
        maybe = OPT_GETVALUE(int,val);
        opt_snprintf_1(stval_buf,80,"%c", (maybe==OPT_TRUE ? FLAG_FALSE : FLAG_TRUE) );
        break;
    case OPT_CSTRING:    
        opt_CSTRING_deprecated();
        opt_snprintf_1(stval_buf,80,"\"%s\"",(char *)val);
        break;
    case OPT_STRING:
        if( val == NULL )	/* this should never happen */
            sprintf(stval_buf,"\"\"");
        else 
            if( OPT_GETVALUE(char *,val) == NULL )
                sprintf(stval_buf,"\"\"");
            else
                opt_snprintf_1(stval_buf,80,"\"%s\"",OPT_GETVALUE(char *,val));
        break;
    case OPT_NUL:
        stval_buf[0]='\0';
        break;
    default:
        opt_fatal("opt_getstrval: Undefined o-type");
        break;
    }
    /* ---- Return value is static buffer ---- */
    return stval_buf;
}/* opt_getstrval */

static int
optsizeof( opt_TYPE typ )
{
    int retval=0;
    
    switch( typ ) {
    case OPT_INT:
        retval = sizeof(int);
        break;
    case OPT_UINT:
        retval = sizeof(unsigned int);
        break;
    case OPT_SHORT:
        retval = sizeof(short);
        break;
    case OPT_USHORT:
        retval = sizeof(unsigned short);
        break;
    case OPT_LONG:
        retval = sizeof(long);
        break;
    case OPT_ULONG:
        retval = sizeof(unsigned long);
        break;
    case OPT_CHAR:
        retval = sizeof(char);
        break;
    case OPT_UCHAR:
        retval = sizeof(unsigned char);
        break;
    case OPT_FLOAT:
        retval = sizeof(float);
        break;
    case OPT_DOUBLE:
        retval = sizeof(double);
        break;
    case OPT_INTLEVEL:
        retval = sizeof(int);
        break;
    case OPT_TOGGLE:
    case OPT_BOOL:
        retval = sizeof(int);
        break;
    case OPT_NEGTOGGLE:
    case OPT_NEGBOOL:
        retval = sizeof(int);
        break;
    case OPT_CSTRING:
        opt_CSTRING_deprecated();
        retval = sizeof(char*);
        break;
    case OPT_STRING:
        retval = sizeof(char*);
        break;
    case OPT_NUL:
      /* FIXME */
        retval = sizeof(int(*)(void));
        break;
    default:
        opt_fatal("optsizeof: Undefined o-type");
        break;
    }
    return retval;
}/*optsizeof*/

/* optstrtyp:
 * returns the type of the ith registered option as a string
 * Thus if x = 12.6 is a float , this returns the string "<FLOAT>"
 * The return value is a pointer to a static string which is
 * overwritten with each call
 */
char *
optstrtyp(int i)
{
    static char	sttyp_buf[80];
    opt_TYPE o;

    o = (optlist[i].array) ? optlist[i].array->base_type : optlist[i].type;
    
    switch( o ) {
        /* Integers */
    case OPT_INT:         strcpy(sttyp_buf,"<INT"); break;
    case OPT_UINT:        strcpy(sttyp_buf,"<UINT"); break;
    case OPT_SHORT:       strcpy(sttyp_buf,"<SHORT"); break;
    case OPT_LONG:        strcpy(sttyp_buf,"<LONG"); break;
    case OPT_USHORT:      strcpy(sttyp_buf,"<USHORT"); break;
    case OPT_ULONG:       strcpy(sttyp_buf,"<ULONG"); break;
        /* Floating point */
    case OPT_FLOAT:       strcpy(sttyp_buf,"<FLOAT"); break;
    case OPT_DOUBLE:      strcpy(sttyp_buf,"<DOUBLE"); break;
        /* Char */
    case OPT_CHAR:        strcpy(sttyp_buf,"<CHAR"); break;
    case OPT_UCHAR:       strcpy(sttyp_buf,"<UCHAR"); break;
        /* Flag */
    case OPT_INTLEVEL:    strcpy(sttyp_buf,"<INTLEVEL"); break;
    case OPT_TOGGLE:      strcpy(sttyp_buf,"<TOGGLE"); break;
    case OPT_BOOL:        strcpy(sttyp_buf,"<BOOL"); break;
    case OPT_NEGTOGGLE:   strcpy(sttyp_buf,"<NEGTOGGLE"); break;
    case OPT_NEGBOOL:     strcpy(sttyp_buf,"<NEGBOOL"); break;
        /* String */
    case OPT_STRING:      strcpy(sttyp_buf,"<STRING"); break;
    case OPT_CSTRING:     strcpy(sttyp_buf,"<STRING"); break;
        /* Nul */
    case OPT_NUL:         strcpy(sttyp_buf,"<NUL"); break;
        
    default:
        opt_fatal("usage: undefined o-type");
    }
    /* If it's an array, then add " ARRAY" to the name of the type */
    if ( optlist[i].array ) {
      strcat(sttyp_buf," ARRAY");
    }
    /* finish with closing bracket */
    strcat(sttyp_buf, ">");
    return sttyp_buf;
}

		/* ------------------------------ */
		/* Put registered options to file */
		/* ------------------------------ */

void
opt_tofile(FILE *fp)
{
    int	i;
    char *format;
    /* Short Option Names */
    char *fnumstr="-%c %-33s ";	/* numbers and strings  */
    char *fflgchr="-%c%-33s  ";	/* flags and characters */

    opt_fileheader(fp);
    /* Short Option Names First */
    for(i=0; i<opt_nreg; ++i) {
        char *s;
        if (optlist[i].name == '\0')
            continue;
        if (optlist[i].type == OPT_NUL)
            continue;
	if (optlist[i].mode == OPT_POSITIONAL)
	  continue;

        if( OPT_isflagtype( optlist[i].type )
            || optlist[i].type == OPT_INTLEVEL
            || optlist[i].type == OPT_CHAR )
            format = fflgchr;
        else
            format = fnumstr;

        fprintf(fp,format,optlist[i].name,optstrval(i));
        s = optlist[i].descript;
        if (strlen(s) > 37) {
          s = opt_justify(optlist[i].descript,55,20,20,"; ");
          fprintf(fp,"\n%s\n",s);
          free(s);
        } else {
          fprintf(fp,"; %s\n",s);
        }
          
        /* If an option has both a short and a long option name
         * then write it as a comment '    ; -b == --option-b
         */
        if (!ISEMPTYSTRING(optlist[i].longname)) {
          if (strlen(optlist[i].descript) > 37) {
            fprintf(fp,"%-20s%c -%c == --%s\n","",IGNOREEOL,
                    optlist[i].name,optlist[i].longname);
          } else {
            fprintf(fp,"%-37s%c -%c == --%s\n","",IGNOREEOL,
                    optlist[i].name,optlist[i].longname);
          }
        }
    }
    /* Now Long Option Names (thanks to Jason V. Morgan) */
    for(i=0; i<opt_nreg; ++i) {
        int len;
        char *s;
        /* do not output the long name if a short name exists */
        if (optlist[i].name != '\0')
           continue;
        if (!optlist[i].longname) 
           continue;
	if (optlist[i].mode == OPT_POSITIONAL)
	  continue;
        /* The following arguably applies to short options too ?? */
        /* don't want anything with a hook (do we?) */
        if (optlist[i].type == OPT_NUL)
            continue;
        if (optlist[i].hook != NULL)
            continue;

        len = 34 - strlen(optlist[i].longname);
        if (len<1) len=1;
        fprintf(fp, "--%s=%-*s", optlist[i].longname, len, optstrval(i));

        s = optlist[i].descript;
        if (strlen(s) > 35) {
          s = opt_justify(optlist[i].descript,55,20,20,"; ");
          fprintf(fp,"\n%s\n",s);
          free(s);
        } else {
          fprintf(fp,"; %s\n",s);
        }
    }
}
static void
opt_fileheader(FILE *fp)
{
    extern char *opt_program_name;
    int	i,n;
    char buf[80];

    opt_snprintf_1(buf,80,"Options file created by: [%.30s]",opt_program_name);
    n = strlen(buf);

    fputc(IGNOREEOL,fp);
    fprintf(fp,"%s",buf);
    fputc(IGNOREEOL,fp); fputc('\n',fp);

    fputc(IGNOREEOL,fp);
    for(i=0;i<n;++i) fprintf(fp,"-");
	fputc(IGNOREEOL,fp); fputc('\n',fp);
}
		/* ----------------------------------------- */
		/* write options out to the interactive menu */
		/* ----------------------------------------- */

/* Produces a malloc'd string that can be used to display    */
/* the essential info about an option in a line or two. Note */
/* the string must be free'd by the caller!                  */

char *
opt_mstring(int i)
{
    char* mstring;
    char* justified_descript;
    char	flgstr[10];
    char	*s;
    char	*format = "-%c %s%*s%s";
    char        *lformat = "--%s=%s%*s%s";

    /* If we have an unnamed option then don't bother returning it */
    if (optlist[i].name == '\0' && ISEMPTYSTRING(optlist[i].longname))
      return NULL;

    /* If the option is purely positional then don't return it either */
    if (optlist[i].mode == OPT_POSITIONAL)
      return NULL;

    s = optstrval(i);
    if( optlist[i].type == OPT_INTLEVEL ) {
        int f;
        f = OPT_GET_ITH_VALUE(int,i);
        if(f==0)
            strcpy(flgstr,"OFF");
        else if (f==1)
            strcpy(flgstr,"ON");
        else
            opt_snprintf_1(flgstr,10,"ON:%d",f);
        s=flgstr;
    } else if( optlist[i].type == OPT_CHAR ) {
        switch (s[0]) {
        case '\0':	sprintf(flgstr,"\'\\0\'"); break;
        case '\n':	sprintf(flgstr,"\'\\n\'"); break;
        case '\t':	sprintf(flgstr,"\'\\t\'"); break;
        case ' ':	sprintf(flgstr,"\'  \'");  break;
        default:	sprintf(flgstr,"%c",s[0]); break;
        }
        s = flgstr;
    } else if( OPT_isflagtype( optlist[i].type ) ) {
        if( *s == '+' || *s == '1')
            strcpy(flgstr,"TRUE");
        if( *s == '-' || *s == '0')
            strcpy(flgstr,"FALSE");
        s=flgstr;
    }

    /* Note that opt_justify() always returns a valid string, even */
    /* if descript is NULL or empty.                               */
    justified_descript = opt_justify(optlist[i].descript, 40, 35, 0, 0);

    mstring = (char*) malloc(strlen(justified_descript) + 200);
    if (!mstring)
	opt_fatal("unable to allocate memory for menu text strings");

    if (optlist[i].name)
        sprintf(mstring,format,optlist[i].name,s,
		35-strlen(s)-3,"",justified_descript);
    else if (optlist[i].longname)
        sprintf(mstring,lformat,optlist[i].longname,s,
		35-strlen(optlist[i].longname)-strlen(s)-3,"",justified_descript);

    free (justified_descript);
    return mstring;
}

/* opt_usage: Write a usage statement describing registered options.
 * This is what the program writes in response to '--help' on the
 * command line.
 */

#define OPTUSAGEDEBUG 0
#define OPTDESCRIPTWIDTH  43
#define OPTDESCRIPTOFFSET 38
#define OPTMAXLONGNAMELEN 14
void
opt_usage(void)
{
    static char *dformat =        " -%c                   %-14s  %s\n";
    static char *lformat =                 " -%c, --%-14s %-14s  %s\n";
    static char *pformat =                        " %-20s %-14s  %s\n";
    static char *uformat =  " %-10s                              %s\n";
    static char *aformat =                  "     --%-14s %-14s  %s\n";
    int  i;
    int initial;
    char* justified_descript;

    /* First we print the positional options */
    initial = 1;
    for (i = 0; i < opt_nreg; ++i) {
      if (optlist[i].mode != OPT_DELIMITED) {
	if (initial) {
	  opt_message("The arguments are:\n");
	  initial = 0;
	}
	/* Get a suitably formatted version of the description  */
	/* descript may be NULL, but opt_justify always returns */
	/* a valid (perhaps empty) string.                      */
	justified_descript = opt_justify(optlist[i].descript,
                                         OPTDESCRIPTWIDTH,
                                         OPTDESCRIPTOFFSET,0,0);
        if(OPTUSAGEDEBUG) fprintf(stderr,"%2d u ",i);
	if (!ISEMPTYSTRING(optlist[i].descript))
	  opt_mess_3(pformat,optlist[i].longname,optstrtyp(i),justified_descript);
	else
	  opt_mess_2(uformat,optstrtyp(i),justified_descript);
	free(justified_descript);
      }
    }

    /* Second we print the delimited options */
    initial = 1;
    for(i=0; i<opt_nreg; ++i) {
      if (optlist[i].mode != OPT_POSITIONAL && 
	  (optlist[i].name != '\0' || !ISEMPTYSTRING(optlist[i].longname))) {
	if (initial) {
	  opt_message("The options are:\n");
	  initial = 0;
	}
	/* Get a suitably formatted version of the description */
	/* If the option is positional, then refer to the argument description */
	if (optlist[i].mode == OPT_FLEXIBLE) {
	  justified_descript = opt_justify("(See argument description above)",
                                           OPTDESCRIPTWIDTH,
                                           OPTDESCRIPTOFFSET,0,0);
        } else if (optlist[i].longname &&
                   strlen(optlist[i].longname) > OPTMAXLONGNAMELEN) {
	  justified_descript = opt_justify(optlist[i].descript,
                                           OPTDESCRIPTWIDTH,
                                           OPTDESCRIPTOFFSET,
                                           OPTDESCRIPTOFFSET,0);
        } else {
	  justified_descript = opt_justify(optlist[i].descript,
                                           OPTDESCRIPTWIDTH,
                                           OPTDESCRIPTOFFSET,0,0);
        }
        
	if (optlist[i].name == '\0') {
	  if(OPTUSAGEDEBUG) fprintf(stderr,"a ");
          if (strlen(optlist[i].longname) > OPTMAXLONGNAMELEN) {
            opt_mess_3(aformat,optlist[i].longname,
                       optstrtyp(i),"");
            opt_mess_1("%s\n",justified_descript);
          } else {
            opt_mess_3(aformat,optlist[i].longname,
                       optstrtyp(i),justified_descript);
          }            
	} else {
	  if (!ISEMPTYSTRING(optlist[i].longname)) {
	    if(OPTUSAGEDEBUG) fprintf(stderr,"l ");
            if (strlen(optlist[i].longname) > OPTMAXLONGNAMELEN) {
              opt_mess_4(lformat,optlist[i].name,optlist[i].longname,
                         optstrtyp(i),"");
              opt_mess_1("%s\n",justified_descript);
            } else {
              opt_mess_4(lformat,optlist[i].name,optlist[i].longname,
                         optstrtyp(i),justified_descript);
            }            
	  } else {
	    if(OPTUSAGEDEBUG) fprintf(stderr,"d ");
	    opt_mess_3(dformat,optlist[i].name,
		       optstrtyp(i),justified_descript);
	  }
	}
	free(justified_descript);
      }
    }

    if (opt_additional_usage_fcn != NULL) {
        /* precede additional usage by -- to indicate that we
         * are finished with the options
         */
        opt_message(" --\n");
        /* return value is ignored */
        (*opt_additional_usage_fcn)();
    }
}

	/* ----------------------- */
	/* Get and process options */
	/* ----------------------- */

	/*
	 * The routines below make use of the ARGVECTOR structure
	 * defined in "ag.h" to process the command line options
	 */
		
	/* -------------------------------------- */
	/* get and process a positional option */
	/* -------------------------------------- */

int
opt_parse_positional(ARGVECTOR *ag)
{
    int     i;
 
    /* Find the first positional option that has not been invoked and
     * set that to the argument string.  Note that once a positional
     * option has been invoked, it cannot be un-invoked.
     */

    for (i=0; i<opt_nreg; ++i) {
        if (optlist[i].invoked || optlist[i].mode == OPT_DELIMITED) 
	  continue;
	return opt_action(i,ag);
    }
    /* If we get here, we didn't find an uninvoked positional option */
    return 0;
}

		/* ---------------------------------- */
		/* get and process a delimited option */
		/* ---------------------------------- */

int
opt_parse_delim(ARGVECTOR *ag)
{
    int      i;
    char     c;
 
    c = ag_c_advance(ag);   /* first character gives name of option */
    i = opt_char_number(c);      /* this is number of option w/ name 'c' */
 
    if(! OPT_isvalidnum(i)) { /* if invalid option name */
        opt_warn_1("%c not a registered option name\n",c);
        return 0;
    }
    return opt_action(i,ag);
}


int
opt_parse_longdelim(ARGVECTOR *ag)
{
    int      i;
    char     c;
    char     *sc,*scptr;

    /* this code parses input options of the form --var=value */
    
    sc = strdup(ag_s(ag));      /* long option name is a string */
    scptr = sc;
    while( !ag_eow(ag) ) {
        c=ag_c_advance(ag);
        if (c == '=') {
            *scptr = '\0';
            break;
        }
        ++scptr;
    }
    
    i  = opt_longname_number(sc);   /* this is number of option w/ name `sc' */
    if(! OPT_isvalidnum(i)) {
        opt_warn_1("%s not a registered option name\n",sc);
        return 0;
    }
    free((char *)sc); 
    return opt_action(i,ag);
}

static void
opt_setvalue(void *v, opt_TYPE o, char *s)
{
  switch(o) {
  case OPT_INT:
    OPT_SETVALUE(int,v, opt_atoi(s));
    break;
  case OPT_UINT:
    OPT_SETVALUE(unsigned int, v, opt_atou(s));
    break;
  case OPT_SHORT:
    OPT_SETVALUE(short, v, opt_atoi(s));
    break;
  case OPT_LONG:
    OPT_SETVALUE(long, v, opt_atoi(s));
    break;
  case OPT_USHORT:
    OPT_SETVALUE(unsigned short, v, opt_atou(s));
    break;
  case OPT_ULONG:
    OPT_SETVALUE(unsigned long, v, opt_atou(s));
    break;
  case OPT_FLOAT:
    OPT_SETVALUE(float, v, opt_atof(s));
    break;
  case OPT_DOUBLE:
    OPT_SETVALUE(double, v, opt_atof(s));
    break;
  case OPT_CHAR:
    OPT_SETVALUE(char, v, s[0]);
    break;
  case OPT_UCHAR:
    OPT_SETVALUE(unsigned char, v, s[0]);
    break;
        
  case OPT_STRING: /* warning: memory leak! */
    OPT_SETVALUE(char *, v,strdup(s));
    break;
  case OPT_CSTRING:
    opt_CSTRING_deprecated();
    strncpy( (char *)v , s, OPT_MAXSTRLEN );
    break;
        
  default:
    opt_fatal("Invalid type");
  }
}
    
static void
optarray_action(OptArray_t *arr, char *stmp)
{
  char *s = strdup(stmp);
  char *t = s;
  char *u = s;
  char cu = '\0';
  char *v;  /* why is this a char * and not a void *, even though it
             * is meant as a generic pointer?  see the long comment
             * above in the optarray_getstrval() function
             */

  /* Reset the array values */
  if (*(arr->value)) {
#if 0 /* Trying to free some memory, but I get "FREE DANGLING" */   
    if (arr->base_type == OPT_STRING) {
      int i;
      for (i=0; i < *(arr->size); ++i) {
        char *ith_val = (char *)(*(arr->value)) + i;
        free( ith_val );
      }
    }
#endif    
    free(*(arr->value));
  }
  *(arr->value)=NULL;
  *(arr->size)=0;

  while (*t) {
    for (u=t; !(*u == '\0' || *u == arr->delim); ++u) {
      /* read until the first delimiter */
      /* t is first character of word */
      /* u is delimiter, so u-1 is last character of t's word */
    }
    cu = *u;
    *u = '\0';  /* now t is null-terminated string */

    /* add new element to array */
    *(arr->size) += 1;
    *(arr->value) = realloc(*(arr->value),
                            *(arr->size)*optsizeof(arr->base_type));
    /* v points to the next element in the array */
    v = (char *)(*(arr->value)) + (*(arr->size)-1)*optsizeof(arr->base_type);
    /* set the value of v according to the string t */
    opt_setvalue(v,arr->base_type,t);
    /* now go to the next word */
    if (cu) t = u+1;
    else    t = u; /* unless that was the last word */
  }
  free(s);
}
                
static int
opt_action(int i, ARGVECTOR *ag)
{
    /* ------------- */
    /* act on option */
    /* ------------- */
    opt_TYPE  o;
    char      c;
    char      *s;
    int       yes,no,maybe,toggle;    /* flag values */

    if (!OPT_isvalidnum(i)) {
        opt_warn_1("opt_action: invalid number %d\n",i);
        return 0; /* failure */
    }

    switch( o=optlist[i].type ) {
    case OPT_INT:
    case OPT_UINT:
    case OPT_SHORT:
    case OPT_LONG:
    case OPT_USHORT:
    case OPT_ULONG:
    case OPT_FLOAT:
    case OPT_DOUBLE:
      opt_setvalue(optlist[i].value,o,argnext(ag));
      break;
    case OPT_CHAR:
      OPT_SET_ITH_VALUE(char,i,ag_c_advance(ag));
      break;
    case OPT_UCHAR:
      OPT_SET_ITH_VALUE(unsigned char,i,ag_c_advance(ag));
      break;

    case OPT_TOGGLE:
    case OPT_BOOL:
    case OPT_NEGTOGGLE:
    case OPT_NEGBOOL:
        /* define terms:
         * yes: value of flag when  ON
         * no:  value of flag when OFF
         * maybe:  present value of flag
         * toggle: negative of maybe
         */
      maybe  = OPT_GET_ITH_VALUE(int,i);
      toggle = (maybe==OPT_TRUE ? OPT_FALSE : OPT_TRUE);
      if( o == OPT_TOGGLE || o==OPT_BOOL )
        yes=OPT_TRUE;
      else
        yes=OPT_FALSE;
      no = (yes==OPT_TRUE ? OPT_FALSE : OPT_TRUE);
      c = ag_c(ag);           /* character following 'c' */
      switch(c) {
        case '+':
        case '1':
            OPT_SET_ITH_VALUE(int,i,yes);
            ag_c_advance(ag);       /* eat the '+' */
            break;
        case '-':
        case '0':
            OPT_SET_ITH_VALUE(int,i,no);
            ag_c_advance(ag);       /* eat the '-' */
            break;
        default:
            if( o==OPT_BOOL || o==OPT_NEGBOOL ) {
                OPT_SET_ITH_VALUE(int,i,yes);
            } else {    /* otherwise toggle value */
                OPT_SET_ITH_VALUE(int,i,toggle);
            }
            break;
      }
      break;

    case OPT_INTLEVEL:
      /* default is to increment */
        OPT_SET_ITH_VALUE(int,i,OPT_GET_ITH_VALUE(int,i) + 1);  
        c = ag_c(ag);
        switch(c) {
        case '+':
            /* we've already incremented */
            ag_c_advance(ag);
            break;
        case '-':
            OPT_SET_ITH_VALUE(int,i,0);
            ag_c_advance(ag);
            break;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            OPT_SET_ITH_VALUE(int,i,c-'0');
            ag_c_advance(ag);
        default:
            break;
        }
        break;
        
    case OPT_STRING:
        s = argnext(ag);
        if (optinvoked_n(i) && optlist[i].value != NULL) {
          /* if this option has been invoked before, then we can be
           * confident that it can be safely freed, before reassigning
           * it to a new value
           */
#if 0
          fprintf(stderr,"invoked opt[%d] for %d times:s=%s\n",i,optinvoked_n(i),s);
#endif          
          free(*((char **)(optlist[i].value)));
        }
        OPT_SET_ITH_VALUE(char *,i,strdup(s));
        /* If this type is really an array, then act accordingly */
        if (optlist[i].array) {
          optarray_action(optlist[i].array,s);
        }
        break;
    case OPT_CSTRING:
        opt_CSTRING_deprecated();
        strncpy( (char *)optlist[i].value , argnext(ag), OPT_MAXSTRLEN );
        break;
        
    case OPT_NUL:
        break;
        
    default:
        opt_fatal("opt_delim: don't know this type");
        break;
    }

    /* Indicate that the option was invoked */
    optlist[i].invoked += 1;

    /* Run the hook if its defined */
    if (optlist[i].hook) {
        int retval;
        retval = optlist[i].hook(optlist[i].value);

        if (retval == OPT_ERROR) {
            /* do nothing for now */;
        }
        if (retval == OPT_ABORT && !opt_menuflag)
            exit(opt_exit_number);
        if (retval == OPT_QUIT)
            opt_quit();
        if (retval == OPT_EXIT)
            exit(opt_exit_number);
    }
    
    return 1;                  /* success */
}

static char *optquotedstrtyp(int i)
{
  char *s,*ss;
  ss = s = optstrtyp(i);
  while (*s) {
    if (*s == '<') { *s = '\"'; ++s; }
    else if (*s == '>') { *s = '\"'; ++s; *s='\0'; }
    else ++s;
  }
  return ss;
}
  
static void opt_xml(int i, FILE *fp)
{
  fprintf(fp,"  <option type=%s",optquotedstrtyp(i));
  if (optlist[i].mode == OPT_POSITIONAL) {
    fprintf(fp," mode=\"POSITIONAL\"");
  } else if (optlist[i].mode == OPT_FLEXIBLE) {
    fprintf(fp," mode=\"FLEXIBLE\"");
  }    
  fprintf(fp,">\n");
  if (optlist[i].name)     fprintf(fp,"    <short>%c</short>\n",optlist[i].name);
  if (optlist[i].longname) fprintf(fp,"    <long>%s</long>\n",optlist[i].longname);
  if (optlist[i].descript) fprintf(fp,"    <description>%s</description>\n",optlist[i].descript);
  if (optlist[i].help)     fprintf(fp,"    <help>%s</help>\n",optlist[i].help);
  if (optlist[i].array) {
    fprintf(fp,"    <array-delimiter>%c</array-delimiter>\n",
            optlist[i].array->delim);
  }
  fprintf(fp,"  </option>\n");
}
void opt_write_xml_file(FILE *fp)
{
  int i;
  fprintf(fp,"<opt-interface program=\"%s\">\n",opt_program_name);
  for (i=0; i<opt_nreg; ++i) {
    opt_xml(i,fp);
  }
  fprintf(fp,"</opt-interface>\n");
}  














