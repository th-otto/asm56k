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
/* opt_regt.c */
/* Note: this file generated automatically from makeopth.pl */
/* Edits to this file may be lost! */
#include "opt.h"
int
optrega_CHAR(char *v, char c, char *n, char *B) {
      return optrega(v,OPT_CHAR,c,n,B);
}
int
optreg_CHAR(char *v, char c, char *B) {
      return optreg(v,OPT_CHAR,c,B);
}
int
optregc_CHAR(char *v, char c) {
      return optregc(v,OPT_CHAR,c);
}
int
optregs_CHAR(char *v, char *n) {
      return optregs(v,OPT_CHAR,n);
}
int
optregcb_CHAR(char *v, char c, char *B) {
      return optregcb(v,OPT_CHAR,c,B);
}
int
optregsb_CHAR(char *v, char *s, char *B) {
      return optregsb(v,OPT_CHAR,s,B);
}
int
optrega_CSTRING(char *v, char c, char *n, char *B) {
      return optrega(v,OPT_CSTRING,c,n,B);
}
int
optreg_CSTRING(char *v, char c, char *B) {
      return optreg(v,OPT_CSTRING,c,B);
}
int
optregc_CSTRING(char *v, char c) {
      return optregc(v,OPT_CSTRING,c);
}
int
optregs_CSTRING(char *v, char *n) {
      return optregs(v,OPT_CSTRING,n);
}
int
optregcb_CSTRING(char *v, char c, char *B) {
      return optregcb(v,OPT_CSTRING,c,B);
}
int
optregsb_CSTRING(char *v, char *s, char *B) {
      return optregsb(v,OPT_CSTRING,s,B);
}
int
optrega_UNDELIM(char **v, char c, char *n, char *B) {
      return optrega(v,OPT_UNDELIM,c,n,B);
}
int
optreg_UNDELIM(char **v, char c, char *B) {
      return optreg(v,OPT_UNDELIM,c,B);
}
int
optregc_UNDELIM(char **v, char c) {
      return optregc(v,OPT_UNDELIM,c);
}
int
optregs_UNDELIM(char **v, char *n) {
      return optregs(v,OPT_UNDELIM,n);
}
int
optregcb_UNDELIM(char **v, char c, char *B) {
      return optregcb(v,OPT_UNDELIM,c,B);
}
int
optregsb_UNDELIM(char **v, char *s, char *B) {
      return optregsb(v,OPT_UNDELIM,s,B);
}
int
optrega_VSTRING(char **v, char c, char *n, char *B) {
      return optrega(v,OPT_VSTRING,c,n,B);
}
int
optreg_VSTRING(char **v, char c, char *B) {
      return optreg(v,OPT_VSTRING,c,B);
}
int
optregc_VSTRING(char **v, char c) {
      return optregc(v,OPT_VSTRING,c);
}
int
optregs_VSTRING(char **v, char *n) {
      return optregs(v,OPT_VSTRING,n);
}
int
optregcb_VSTRING(char **v, char c, char *B) {
      return optregcb(v,OPT_VSTRING,c,B);
}
int
optregsb_VSTRING(char **v, char *s, char *B) {
      return optregsb(v,OPT_VSTRING,s,B);
}
int
optrega_DOUBLE(double *v, char c, char *n, char *B) {
      return optrega(v,OPT_DOUBLE,c,n,B);
}
int
optreg_DOUBLE(double *v, char c, char *B) {
      return optreg(v,OPT_DOUBLE,c,B);
}
int
optregc_DOUBLE(double *v, char c) {
      return optregc(v,OPT_DOUBLE,c);
}
int
optregs_DOUBLE(double *v, char *n) {
      return optregs(v,OPT_DOUBLE,n);
}
int
optregcb_DOUBLE(double *v, char c, char *B) {
      return optregcb(v,OPT_DOUBLE,c,B);
}
int
optregsb_DOUBLE(double *v, char *s, char *B) {
      return optregsb(v,OPT_DOUBLE,s,B);
}
int
optrega_NUL(void *v, char c, char *n, char *B) {
      return optrega(v,OPT_NUL,c,n,B);
}
int
optreg_NUL(void *v, char c, char *B) {
      return optreg(v,OPT_NUL,c,B);
}
int
optregc_NUL(void *v, char c) {
      return optregc(v,OPT_NUL,c);
}
int
optregs_NUL(void *v, char *n) {
      return optregs(v,OPT_NUL,n);
}
int
optregcb_NUL(void *v, char c, char *B) {
      return optregcb(v,OPT_NUL,c,B);
}
int
optregsb_NUL(void *v, char *s, char *B) {
      return optregsb(v,OPT_NUL,s,B);
}
int
optrega_BOOL(int *v, char c, char *n, char *B) {
      return optrega(v,OPT_BOOL,c,n,B);
}
int
optreg_BOOL(int *v, char c, char *B) {
      return optreg(v,OPT_BOOL,c,B);
}
int
optregc_BOOL(int *v, char c) {
      return optregc(v,OPT_BOOL,c);
}
int
optregs_BOOL(int *v, char *n) {
      return optregs(v,OPT_BOOL,n);
}
int
optregcb_BOOL(int *v, char c, char *B) {
      return optregcb(v,OPT_BOOL,c,B);
}
int
optregsb_BOOL(int *v, char *s, char *B) {
      return optregsb(v,OPT_BOOL,s,B);
}
int
optrega_UNDELIMV(char **v, char c, char *n, char *B) {
      return optrega(v,OPT_UNDELIMV,c,n,B);
}
int
optreg_UNDELIMV(char **v, char c, char *B) {
      return optreg(v,OPT_UNDELIMV,c,B);
}
int
optregc_UNDELIMV(char **v, char c) {
      return optregc(v,OPT_UNDELIMV,c);
}
int
optregs_UNDELIMV(char **v, char *n) {
      return optregs(v,OPT_UNDELIMV,n);
}
int
optregcb_UNDELIMV(char **v, char c, char *B) {
      return optregcb(v,OPT_UNDELIMV,c,B);
}
int
optregsb_UNDELIMV(char **v, char *s, char *B) {
      return optregsb(v,OPT_UNDELIMV,s,B);
}
int
optrega_LONG(long *v, char c, char *n, char *B) {
      return optrega(v,OPT_LONG,c,n,B);
}
int
optreg_LONG(long *v, char c, char *B) {
      return optreg(v,OPT_LONG,c,B);
}
int
optregc_LONG(long *v, char c) {
      return optregc(v,OPT_LONG,c);
}
int
optregs_LONG(long *v, char *n) {
      return optregs(v,OPT_LONG,n);
}
int
optregcb_LONG(long *v, char c, char *B) {
      return optregcb(v,OPT_LONG,c,B);
}
int
optregsb_LONG(long *v, char *s, char *B) {
      return optregsb(v,OPT_LONG,s,B);
}
int
optrega_UINT(unsigned int *v, char c, char *n, char *B) {
      return optrega(v,OPT_UINT,c,n,B);
}
int
optreg_UINT(unsigned int *v, char c, char *B) {
      return optreg(v,OPT_UINT,c,B);
}
int
optregc_UINT(unsigned int *v, char c) {
      return optregc(v,OPT_UINT,c);
}
int
optregs_UINT(unsigned int *v, char *n) {
      return optregs(v,OPT_UINT,n);
}
int
optregcb_UINT(unsigned int *v, char c, char *B) {
      return optregcb(v,OPT_UINT,c,B);
}
int
optregsb_UINT(unsigned int *v, char *s, char *B) {
      return optregsb(v,OPT_UINT,s,B);
}
int
optrega_UNSINT(unsigned int *v, char c, char *n, char *B) {
      return optrega(v,OPT_UNSINT,c,n,B);
}
int
optreg_UNSINT(unsigned int *v, char c, char *B) {
      return optreg(v,OPT_UNSINT,c,B);
}
int
optregc_UNSINT(unsigned int *v, char c) {
      return optregc(v,OPT_UNSINT,c);
}
int
optregs_UNSINT(unsigned int *v, char *n) {
      return optregs(v,OPT_UNSINT,n);
}
int
optregcb_UNSINT(unsigned int *v, char c, char *B) {
      return optregcb(v,OPT_UNSINT,c,B);
}
int
optregsb_UNSINT(unsigned int *v, char *s, char *B) {
      return optregsb(v,OPT_UNSINT,s,B);
}
int
optrega_STRING(char **v, char c, char *n, char *B) {
      return optrega(v,OPT_STRING,c,n,B);
}
int
optreg_STRING(char **v, char c, char *B) {
      return optreg(v,OPT_STRING,c,B);
}
int
optregc_STRING(char **v, char c) {
      return optregc(v,OPT_STRING,c);
}
int
optregs_STRING(char **v, char *n) {
      return optregs(v,OPT_STRING,n);
}
int
optregcb_STRING(char **v, char c, char *B) {
      return optregcb(v,OPT_STRING,c,B);
}
int
optregsb_STRING(char **v, char *s, char *B) {
      return optregsb(v,OPT_STRING,s,B);
}
int
optrega_UCHAR(unsigned char *v, char c, char *n, char *B) {
      return optrega(v,OPT_UCHAR,c,n,B);
}
int
optreg_UCHAR(unsigned char *v, char c, char *B) {
      return optreg(v,OPT_UCHAR,c,B);
}
int
optregc_UCHAR(unsigned char *v, char c) {
      return optregc(v,OPT_UCHAR,c);
}
int
optregs_UCHAR(unsigned char *v, char *n) {
      return optregs(v,OPT_UCHAR,n);
}
int
optregcb_UCHAR(unsigned char *v, char c, char *B) {
      return optregcb(v,OPT_UCHAR,c,B);
}
int
optregsb_UCHAR(unsigned char *v, char *s, char *B) {
      return optregsb(v,OPT_UCHAR,s,B);
}
int
optrega_SHORT(short *v, char c, char *n, char *B) {
      return optrega(v,OPT_SHORT,c,n,B);
}
int
optreg_SHORT(short *v, char c, char *B) {
      return optreg(v,OPT_SHORT,c,B);
}
int
optregc_SHORT(short *v, char c) {
      return optregc(v,OPT_SHORT,c);
}
int
optregs_SHORT(short *v, char *n) {
      return optregs(v,OPT_SHORT,n);
}
int
optregcb_SHORT(short *v, char c, char *B) {
      return optregcb(v,OPT_SHORT,c,B);
}
int
optregsb_SHORT(short *v, char *s, char *B) {
      return optregsb(v,OPT_SHORT,s,B);
}
int
optrega_USHORT(unsigned short *v, char c, char *n, char *B) {
      return optrega(v,OPT_USHORT,c,n,B);
}
int
optreg_USHORT(unsigned short *v, char c, char *B) {
      return optreg(v,OPT_USHORT,c,B);
}
int
optregc_USHORT(unsigned short *v, char c) {
      return optregc(v,OPT_USHORT,c);
}
int
optregs_USHORT(unsigned short *v, char *n) {
      return optregs(v,OPT_USHORT,n);
}
int
optregcb_USHORT(unsigned short *v, char c, char *B) {
      return optregcb(v,OPT_USHORT,c,B);
}
int
optregsb_USHORT(unsigned short *v, char *s, char *B) {
      return optregsb(v,OPT_USHORT,s,B);
}
int
optrega_INT(int *v, char c, char *n, char *B) {
      return optrega(v,OPT_INT,c,n,B);
}
int
optreg_INT(int *v, char c, char *B) {
      return optreg(v,OPT_INT,c,B);
}
int
optregc_INT(int *v, char c) {
      return optregc(v,OPT_INT,c);
}
int
optregs_INT(int *v, char *n) {
      return optregs(v,OPT_INT,n);
}
int
optregcb_INT(int *v, char c, char *B) {
      return optregcb(v,OPT_INT,c,B);
}
int
optregsb_INT(int *v, char *s, char *B) {
      return optregsb(v,OPT_INT,s,B);
}
int
optrega_NEGBOOL(int *v, char c, char *n, char *B) {
      return optrega(v,OPT_NEGBOOL,c,n,B);
}
int
optreg_NEGBOOL(int *v, char c, char *B) {
      return optreg(v,OPT_NEGBOOL,c,B);
}
int
optregc_NEGBOOL(int *v, char c) {
      return optregc(v,OPT_NEGBOOL,c);
}
int
optregs_NEGBOOL(int *v, char *n) {
      return optregs(v,OPT_NEGBOOL,n);
}
int
optregcb_NEGBOOL(int *v, char c, char *B) {
      return optregcb(v,OPT_NEGBOOL,c,B);
}
int
optregsb_NEGBOOL(int *v, char *s, char *B) {
      return optregsb(v,OPT_NEGBOOL,s,B);
}
int
optrega_UNDELIMC(char *v, char c, char *n, char *B) {
      return optrega(v,OPT_UNDELIMC,c,n,B);
}
int
optreg_UNDELIMC(char *v, char c, char *B) {
      return optreg(v,OPT_UNDELIMC,c,B);
}
int
optregc_UNDELIMC(char *v, char c) {
      return optregc(v,OPT_UNDELIMC,c);
}
int
optregs_UNDELIMC(char *v, char *n) {
      return optregs(v,OPT_UNDELIMC,n);
}
int
optregcb_UNDELIMC(char *v, char c, char *B) {
      return optregcb(v,OPT_UNDELIMC,c,B);
}
int
optregsb_UNDELIMC(char *v, char *s, char *B) {
      return optregsb(v,OPT_UNDELIMC,s,B);
}
int
optrega_ULONG(unsigned long *v, char c, char *n, char *B) {
      return optrega(v,OPT_ULONG,c,n,B);
}
int
optreg_ULONG(unsigned long *v, char c, char *B) {
      return optreg(v,OPT_ULONG,c,B);
}
int
optregc_ULONG(unsigned long *v, char c) {
      return optregc(v,OPT_ULONG,c);
}
int
optregs_ULONG(unsigned long *v, char *n) {
      return optregs(v,OPT_ULONG,n);
}
int
optregcb_ULONG(unsigned long *v, char c, char *B) {
      return optregcb(v,OPT_ULONG,c,B);
}
int
optregsb_ULONG(unsigned long *v, char *s, char *B) {
      return optregsb(v,OPT_ULONG,s,B);
}
int
optrega_FLOAT(float *v, char c, char *n, char *B) {
      return optrega(v,OPT_FLOAT,c,n,B);
}
int
optreg_FLOAT(float *v, char c, char *B) {
      return optreg(v,OPT_FLOAT,c,B);
}
int
optregc_FLOAT(float *v, char c) {
      return optregc(v,OPT_FLOAT,c);
}
int
optregs_FLOAT(float *v, char *n) {
      return optregs(v,OPT_FLOAT,n);
}
int
optregcb_FLOAT(float *v, char c, char *B) {
      return optregcb(v,OPT_FLOAT,c,B);
}
int
optregsb_FLOAT(float *v, char *s, char *B) {
      return optregsb(v,OPT_FLOAT,s,B);
}
int
optrega_NEGTOGGLE(int *v, char c, char *n, char *B) {
      return optrega(v,OPT_NEGTOGGLE,c,n,B);
}
int
optreg_NEGTOGGLE(int *v, char c, char *B) {
      return optreg(v,OPT_NEGTOGGLE,c,B);
}
int
optregc_NEGTOGGLE(int *v, char c) {
      return optregc(v,OPT_NEGTOGGLE,c);
}
int
optregs_NEGTOGGLE(int *v, char *n) {
      return optregs(v,OPT_NEGTOGGLE,n);
}
int
optregcb_NEGTOGGLE(int *v, char c, char *B) {
      return optregcb(v,OPT_NEGTOGGLE,c,B);
}
int
optregsb_NEGTOGGLE(int *v, char *s, char *B) {
      return optregsb(v,OPT_NEGTOGGLE,s,B);
}
int
optrega_INTLEVEL(int *v, char c, char *n, char *B) {
      return optrega(v,OPT_INTLEVEL,c,n,B);
}
int
optreg_INTLEVEL(int *v, char c, char *B) {
      return optreg(v,OPT_INTLEVEL,c,B);
}
int
optregc_INTLEVEL(int *v, char c) {
      return optregc(v,OPT_INTLEVEL,c);
}
int
optregs_INTLEVEL(int *v, char *n) {
      return optregs(v,OPT_INTLEVEL,n);
}
int
optregcb_INTLEVEL(int *v, char c, char *B) {
      return optregcb(v,OPT_INTLEVEL,c,B);
}
int
optregsb_INTLEVEL(int *v, char *s, char *B) {
      return optregsb(v,OPT_INTLEVEL,s,B);
}
int
optrega_TOGGLE(int *v, char c, char *n, char *B) {
      return optrega(v,OPT_TOGGLE,c,n,B);
}
int
optreg_TOGGLE(int *v, char c, char *B) {
      return optreg(v,OPT_TOGGLE,c,B);
}
int
optregc_TOGGLE(int *v, char c) {
      return optregc(v,OPT_TOGGLE,c);
}
int
optregs_TOGGLE(int *v, char *n) {
      return optregs(v,OPT_TOGGLE,n);
}
int
optregcb_TOGGLE(int *v, char c, char *B) {
      return optregcb(v,OPT_TOGGLE,c,B);
}
int
optregsb_TOGGLE(int *v, char *s, char *B) {
      return optregsb(v,OPT_TOGGLE,s,B);
}
