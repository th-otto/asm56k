/* opt_regcc.cc */
/* Note: this file generated automatically from makeopth.pl */
/* Edits to this file may be lost! */
/* C++ bindings for opt */
#ifndef OPT_NOCPLUSINTERFACE
#ifdef __cplusplus
#include <cstring>
#include "opt.h"
using namespace std;
int OptRegister(char *v, char c, char *n, char *B)
{
    return optrega(v,OPT_CHAR,c,n,B);
}
int OptRegister(char *v, char *n, char *B)
{
    return optrega(v,OPT_CHAR,'\0',n,B);
}
int OptRegister(char *v, opt_TYPE o, char c, char *n, char *B)
{
    return optrega(v,o,c,n,B);
}
int OptRegister(char *v, opt_TYPE o, char *n, char *B)
{
    return optrega(v,o,'\0',n,B);
}
int OptRegister(char *v, opt_MODE m, char c, char *n, char *B)
{
    int i = optrega(v,OPT_CHAR,c,n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(char *v, opt_MODE m, char *n, char *B)
{
    int i = optrega(v,OPT_CHAR,'\0',n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(char *v, opt_TYPE o, opt_MODE m, char c, char *n, char *B)
{
    int i = optrega(v,o,c,n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(char *v, opt_TYPE o, opt_MODE m, char *n, char *B)
{
    int i = optrega(v,o,'\0',n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(short *v, char c, char *n, char *B)
{
    return optrega(v,OPT_SHORT,c,n,B);
}
int OptRegister(short *v, char *n, char *B)
{
    return optrega(v,OPT_SHORT,'\0',n,B);
}
int OptRegister(short *v, opt_TYPE o, char c, char *n, char *B)
{
    return optrega(v,o,c,n,B);
}
int OptRegister(short *v, opt_TYPE o, char *n, char *B)
{
    return optrega(v,o,'\0',n,B);
}
int OptRegister(short *v, opt_MODE m, char c, char *n, char *B)
{
    int i = optrega(v,OPT_SHORT,c,n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(short *v, opt_MODE m, char *n, char *B)
{
    int i = optrega(v,OPT_SHORT,'\0',n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(short *v, opt_TYPE o, opt_MODE m, char c, char *n, char *B)
{
    int i = optrega(v,o,c,n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(short *v, opt_TYPE o, opt_MODE m, char *n, char *B)
{
    int i = optrega(v,o,'\0',n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(unsigned short *v, char c, char *n, char *B)
{
    return optrega(v,OPT_USHORT,c,n,B);
}
int OptRegister(unsigned short *v, char *n, char *B)
{
    return optrega(v,OPT_USHORT,'\0',n,B);
}
int OptRegister(unsigned short *v, opt_TYPE o, char c, char *n, char *B)
{
    return optrega(v,o,c,n,B);
}
int OptRegister(unsigned short *v, opt_TYPE o, char *n, char *B)
{
    return optrega(v,o,'\0',n,B);
}
int OptRegister(unsigned short *v, opt_MODE m, char c, char *n, char *B)
{
    int i = optrega(v,OPT_USHORT,c,n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(unsigned short *v, opt_MODE m, char *n, char *B)
{
    int i = optrega(v,OPT_USHORT,'\0',n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(unsigned short *v, opt_TYPE o, opt_MODE m, char c, char *n, char *B)
{
    int i = optrega(v,o,c,n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(unsigned short *v, opt_TYPE o, opt_MODE m, char *n, char *B)
{
    int i = optrega(v,o,'\0',n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(int *v, char c, char *n, char *B)
{
    return optrega(v,OPT_INT,c,n,B);
}
int OptRegister(int *v, char *n, char *B)
{
    return optrega(v,OPT_INT,'\0',n,B);
}
int OptRegister(int *v, opt_TYPE o, char c, char *n, char *B)
{
    return optrega(v,o,c,n,B);
}
int OptRegister(int *v, opt_TYPE o, char *n, char *B)
{
    return optrega(v,o,'\0',n,B);
}
int OptRegister(int *v, opt_MODE m, char c, char *n, char *B)
{
    int i = optrega(v,OPT_INT,c,n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(int *v, opt_MODE m, char *n, char *B)
{
    int i = optrega(v,OPT_INT,'\0',n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(int *v, opt_TYPE o, opt_MODE m, char c, char *n, char *B)
{
    int i = optrega(v,o,c,n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(int *v, opt_TYPE o, opt_MODE m, char *n, char *B)
{
    int i = optrega(v,o,'\0',n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(unsigned long *v, char c, char *n, char *B)
{
    return optrega(v,OPT_ULONG,c,n,B);
}
int OptRegister(unsigned long *v, char *n, char *B)
{
    return optrega(v,OPT_ULONG,'\0',n,B);
}
int OptRegister(unsigned long *v, opt_TYPE o, char c, char *n, char *B)
{
    return optrega(v,o,c,n,B);
}
int OptRegister(unsigned long *v, opt_TYPE o, char *n, char *B)
{
    return optrega(v,o,'\0',n,B);
}
int OptRegister(unsigned long *v, opt_MODE m, char c, char *n, char *B)
{
    int i = optrega(v,OPT_ULONG,c,n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(unsigned long *v, opt_MODE m, char *n, char *B)
{
    int i = optrega(v,OPT_ULONG,'\0',n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(unsigned long *v, opt_TYPE o, opt_MODE m, char c, char *n, char *B)
{
    int i = optrega(v,o,c,n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(unsigned long *v, opt_TYPE o, opt_MODE m, char *n, char *B)
{
    int i = optrega(v,o,'\0',n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(double *v, char c, char *n, char *B)
{
    return optrega(v,OPT_DOUBLE,c,n,B);
}
int OptRegister(double *v, char *n, char *B)
{
    return optrega(v,OPT_DOUBLE,'\0',n,B);
}
int OptRegister(double *v, opt_TYPE o, char c, char *n, char *B)
{
    return optrega(v,o,c,n,B);
}
int OptRegister(double *v, opt_TYPE o, char *n, char *B)
{
    return optrega(v,o,'\0',n,B);
}
int OptRegister(double *v, opt_MODE m, char c, char *n, char *B)
{
    int i = optrega(v,OPT_DOUBLE,c,n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(double *v, opt_MODE m, char *n, char *B)
{
    int i = optrega(v,OPT_DOUBLE,'\0',n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(double *v, opt_TYPE o, opt_MODE m, char c, char *n, char *B)
{
    int i = optrega(v,o,c,n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(double *v, opt_TYPE o, opt_MODE m, char *n, char *B)
{
    int i = optrega(v,o,'\0',n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(long *v, char c, char *n, char *B)
{
    return optrega(v,OPT_LONG,c,n,B);
}
int OptRegister(long *v, char *n, char *B)
{
    return optrega(v,OPT_LONG,'\0',n,B);
}
int OptRegister(long *v, opt_TYPE o, char c, char *n, char *B)
{
    return optrega(v,o,c,n,B);
}
int OptRegister(long *v, opt_TYPE o, char *n, char *B)
{
    return optrega(v,o,'\0',n,B);
}
int OptRegister(long *v, opt_MODE m, char c, char *n, char *B)
{
    int i = optrega(v,OPT_LONG,c,n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(long *v, opt_MODE m, char *n, char *B)
{
    int i = optrega(v,OPT_LONG,'\0',n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(long *v, opt_TYPE o, opt_MODE m, char c, char *n, char *B)
{
    int i = optrega(v,o,c,n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(long *v, opt_TYPE o, opt_MODE m, char *n, char *B)
{
    int i = optrega(v,o,'\0',n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(unsigned int *v, char c, char *n, char *B)
{
    return optrega(v,OPT_UINT,c,n,B);
}
int OptRegister(unsigned int *v, char *n, char *B)
{
    return optrega(v,OPT_UINT,'\0',n,B);
}
int OptRegister(unsigned int *v, opt_TYPE o, char c, char *n, char *B)
{
    return optrega(v,o,c,n,B);
}
int OptRegister(unsigned int *v, opt_TYPE o, char *n, char *B)
{
    return optrega(v,o,'\0',n,B);
}
int OptRegister(unsigned int *v, opt_MODE m, char c, char *n, char *B)
{
    int i = optrega(v,OPT_UINT,c,n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(unsigned int *v, opt_MODE m, char *n, char *B)
{
    int i = optrega(v,OPT_UINT,'\0',n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(unsigned int *v, opt_TYPE o, opt_MODE m, char c, char *n, char *B)
{
    int i = optrega(v,o,c,n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(unsigned int *v, opt_TYPE o, opt_MODE m, char *n, char *B)
{
    int i = optrega(v,o,'\0',n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(char **v, char c, char *n, char *B)
{
    return optrega(v,OPT_STRING,c,n,B);
}
int OptRegister(char **v, char *n, char *B)
{
    return optrega(v,OPT_STRING,'\0',n,B);
}
int OptRegister(char **v, opt_TYPE o, char c, char *n, char *B)
{
    return optrega(v,o,c,n,B);
}
int OptRegister(char **v, opt_TYPE o, char *n, char *B)
{
    return optrega(v,o,'\0',n,B);
}
int OptRegister(char **v, opt_MODE m, char c, char *n, char *B)
{
    int i = optrega(v,OPT_STRING,c,n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(char **v, opt_MODE m, char *n, char *B)
{
    int i = optrega(v,OPT_STRING,'\0',n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(char **v, opt_TYPE o, opt_MODE m, char c, char *n, char *B)
{
    int i = optrega(v,o,c,n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(char **v, opt_TYPE o, opt_MODE m, char *n, char *B)
{
    int i = optrega(v,o,'\0',n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(float *v, char c, char *n, char *B)
{
    return optrega(v,OPT_FLOAT,c,n,B);
}
int OptRegister(float *v, char *n, char *B)
{
    return optrega(v,OPT_FLOAT,'\0',n,B);
}
int OptRegister(float *v, opt_TYPE o, char c, char *n, char *B)
{
    return optrega(v,o,c,n,B);
}
int OptRegister(float *v, opt_TYPE o, char *n, char *B)
{
    return optrega(v,o,'\0',n,B);
}
int OptRegister(float *v, opt_MODE m, char c, char *n, char *B)
{
    int i = optrega(v,OPT_FLOAT,c,n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(float *v, opt_MODE m, char *n, char *B)
{
    int i = optrega(v,OPT_FLOAT,'\0',n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(float *v, opt_TYPE o, opt_MODE m, char c, char *n, char *B)
{
    int i = optrega(v,o,c,n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(float *v, opt_TYPE o, opt_MODE m, char *n, char *B)
{
    int i = optrega(v,o,'\0',n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(unsigned char *v, char c, char *n, char *B)
{
    return optrega(v,OPT_UCHAR,c,n,B);
}
int OptRegister(unsigned char *v, char *n, char *B)
{
    return optrega(v,OPT_UCHAR,'\0',n,B);
}
int OptRegister(unsigned char *v, opt_TYPE o, char c, char *n, char *B)
{
    return optrega(v,o,c,n,B);
}
int OptRegister(unsigned char *v, opt_TYPE o, char *n, char *B)
{
    return optrega(v,o,'\0',n,B);
}
int OptRegister(unsigned char *v, opt_MODE m, char c, char *n, char *B)
{
    int i = optrega(v,OPT_UCHAR,c,n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(unsigned char *v, opt_MODE m, char *n, char *B)
{
    int i = optrega(v,OPT_UCHAR,'\0',n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(unsigned char *v, opt_TYPE o, opt_MODE m, char c, char *n, char *B)
{
    int i = optrega(v,o,c,n,B);
    optmode_n(i, m);
    return i;
}
int OptRegister(unsigned char *v, opt_TYPE o, opt_MODE m, char *n, char *B)
{
    int i = optrega(v,o,'\0',n,B);
    optmode_n(i, m);
    return i;
}
#endif /* __cpluscplus */
#endif /* OPT_NOCPLUSINTERFACE */
