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
/* $Id: opt_util.c,v 3.13 2003/03/15 05:09:11 jt Exp $ */
/**************************
 *  some useful utilities *
 **************************/
/*
 *  *** NO LONGER AVAILABLE!!
 *  *** 1) Variable argument lists: for error messages
 *  ***   opt_errmess(), gives message and then aborts run
 *     
 *  2) Long Jump: for aborting run() without losing the entire job
 *     optBeginRun()
 *     optAbortRun()
 *     2a) Signal: trapping ^C so that it aborts run()
 *  4) Readline: interface to GNU readline
 *     
 */
/*  Can enable longjmp with: #define HAVE_LONGJMP 1
 *  Only if longjmp is enabled can signal be enabled, using
 *                           #define HAVE_SIGNAL 1
 *  Finally, readline can be enabled with
 *                           #define HAVE_READLINE 1
 *  If this version of opt came with a configure script, then those
 *  values should be set automatically by the system at configure time.
 */
#ifndef SINGLEFILE
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ag.h"
#ifdef __cplusplus
}
#endif
#include "opt.h"
#include "opt_p.h"
#endif /* SINGLEFILE */


/* If these are not defined on the compile command line, then assume
 * the worst, that you don't have anything!
 */
#ifndef HAVE_LONGJMP
#define HAVE_LONGJMP 0
#endif
#ifndef HAVE_SIGNAL
#define HAVE_SIGNAL 0
#endif
#ifndef RETSIGTYPE
#define RETSIGTYPE void
#endif
#ifndef HAVE_READLINE
#define HAVE_READLINE 0
#endif

/* Provide message/warning/fatal functions */
/* Ordinary messages go to stdout, but warn/fatal messages to stderr;
 * (This convention adopted at suggestion of Jason V. Morgan) */
void
opt_message(char *s)
{
    if (s) fputs(s,stdout);
}
void
opt_warning(char *s)
{
    fputs("OPT Warning: ",stderr);
    if (s) fputs(s,stderr);
    fputs("\n",stderr);
}
void
opt_fatal(char *s)
{
    fputs("OPT Fatal error: ",stderr);
    if (s) fputs(s,stderr);
    exit(opt_exit_number);
}

void
opt_setstring(char **s, char *t)
{
  if (*s) free(*s);
  *s = strdup(t);
}

/* A function to justify text to a given width, useful for      */
/* printing long description strings. Returns a pointer to      */
/* a malloc'd string buffer which must be free'd by the caller. */
char*
opt_justify(char* s, int width, int indent, int initial, char* prefix) 
{
    int i, j, n, p, len, approxLines, bufSize;
    size_t k;
    char* buf;

    /* If s is null or empty, then return an empty string */
    if (ISEMPTYSTRING(s)) {
      buf = (char*) malloc(1);
      buf[0] = '\0';
      return buf;
    }

    /* Allocate a nice big buffer for the justified string */
    /* (This is very generous estimate...) */
    len = strlen(s);
    approxLines = len / width + 1;
    bufSize = initial + width + 1 +         /* The first line, inc. newline */
	(1 + indent + width) * approxLines * 3; /* Justfied lines, assuming bad line breaks */

    buf = (char*) malloc(bufSize + 1);
    if (!buf)
	opt_fatal("unable to allocate memory for string processing");

    j = 0;
    
    /* Insert initial indent and prefix */
    for (n = 0; n < initial; ++n)
	buf[j++] = ' ';
    if (prefix) {
	strcpy(buf + j, prefix);
	j += strlen(prefix);
    }

    /* Scan through input, breaking up into words */
    p = 0;   /* Position with justified column */
    for (i = 0; i < len; ++i) {
	/* Skip any leading whitespace */
	k = strspn(s + i, " \t\r\n");
	i += k;
	if (i >= len)
	    break;
	/* Search for next whitespace char */
	k = strcspn(s + i, " \t\r\n");
	/* Do we need a line break? */
	if (p + k + 1 >= width && p != 0) {
	    buf[j++] = '\n';
	    for (n = 0; n < indent; ++n)
		buf[j++] = ' ';
	    if (prefix) {
		strcpy(buf + j, prefix);
		j += strlen(prefix);
	    }
	    p = 0;
	}
	/* Add the word to the output buffer */
	/* If not at the beginning of the line, add a space */
	if (p != 0) {
	    buf[j++] = ' ';
	    ++p;
	}
	/* Copy over the word */
	strncpy(buf + j, s + i, k);
	p += k;
	i += k;
	j += k;
    }
    /* Terminate output buffer */
    buf[j] = '\0';

    /* Return the justified buffer - remember to free this! */
    return buf;
}


#if HAVE_LONGJMP
#include <setjmp.h>
static jmp_buf	opt_jumpstart;
static int	opt_jump_set=FALSE;

/* prototypes */
static RETSIGTYPE set_signal(void);
static RETSIGTYPE unset_signal(void);

/**********
 * SIGNAL *
 **********/
#if HAVE_SIGNAL
#include <signal.h>

static void
catch_signal(int sig
#if 0                           /* someday make this portable! */
             ,int code, struct sigcontext *scp, char *addr
#endif                         
             )
{
    opt_message("\nOPT Interrupted:\n");
    longjmp(opt_jumpstart,1);
    perror("Did not long-jump");
    exit(0);
}

static RETSIGTYPE (*old_catcher)(int) = NULL;
static RETSIGTYPE
set_signal(void)
{
    old_catcher = signal(SIGINT,catch_signal);
}
static RETSIGTYPE
unset_signal(void)
{
    signal(SIGINT,old_catcher);
}
#else

static RETSIGTYPE   set_signal(void) {}
static RETSIGTYPE unset_signal(void) {}

#endif /* end SIGNAL */


/* optBeginRun, optAbortRun: these functions are for running
 * the run hook defined by the programmer in optRun().
 */

int
optBeginRun(OPT_PFI run)
{
    int value;

    opt_jump_set=OPT_TRUE;
    set_signal();

    value = setjmp(opt_jumpstart);
    if(value!=0)
        opt_message("OPT: Run aborted...try again\n");
    else
        value = (*run)();
	
    unset_signal();
    opt_jump_set=OPT_FALSE;

    return value;
}

void
optAbortRun(void)
{
    if(opt_jump_set)
        longjmp(opt_jumpstart,1);
    else
        /** should this be some opt_exit() ?? **/
        opt_quit();
}

int optBeginMain(OPT_PFI_ARG run_args,int argc, char **argv)
{
    int value;

    opt_jump_set=OPT_TRUE;
    set_signal();

    value = setjmp(opt_jumpstart);
    if(value!=0)
        opt_message("OPT: Run aborted...try again\n");
    else
        value = (*run_args)(argc,argv);
	
    unset_signal();
    opt_jump_set=OPT_FALSE;

    return value;
}    

#else  /* If LONGJMP is unavailable, or disabled, then these functions
        * are rather trivial.
        */
int  optBeginRun(OPT_PFI run) { return (*run)(); }
void optAbortRun() { exit(opt_exit_number); }
int  optBeginMain(OPT_PFI_ARG run_args,int argc, char **argv)
{
    return (*run_args)(argc,argv);
}

#endif /* end LONGJMP */

void optAbortMain() { optAbortRun(); }


/************
 * Readline *
 ************/

#if HAVE_READLINE
#ifdef __cplusplus
extern "C" {
#include <readline/readline.h>
#include <readline/history.h>
}
#else
#include <readline/readline.h>
#include <readline/history.h>
#endif
#endif

void
opt_readline_init(char *s)
{
#if HAVE_READLINE
    rl_readline_name = s;
#endif
    return;
}

void 
opt_menu_getline(char *prompt, char *respond, int maxlen)
{
#if HAVE_READLINE
    char *line;
    line = readline(prompt);
    if (line && *line) {
	add_history(line);
	strncpy(respond,line,maxlen);
	free(line);
    } else
	respond[0] = '\0';
	/* double terminate the line */
    respond[strlen(respond)+1] = '\0';
#else
    opt_message(prompt);
    opt_getline(respond,stdin);
#endif
}


#if !HAVE_STRDUP
/* Provided here in case it is not available on system */     
char *strdup(char *s)
{
    char *sdup;
    int i,n;
    n = strlen(s);
    sdup = (char *)malloc(n+1);
    for (i=0; i<n; ++i)
        sdup[i] = s[i];
    sdup[n]='\0';
    return sdup;
}
#endif
    






