// -----------------------------------------------------------------------------------------------
/*

Project:    asm56k
Author:     M.Buras (sqward)

*/
// -----------------------------------------------------------------------------------------------

#ifndef _EXPORT_H_
#define _EXPORT_H_

#define TRUE 1
#define FALSE 0

extern int		g_dsp_cpu;
extern int	    g_passNum;
extern int      g_write_zero_sections;
extern void		asm_abort();

extern int		dc_flag;        // this is form grammar.y
extern int 		g_currentLine;
extern int      g_LocalSerial;
extern int		yyerror(char* s, ... );
extern int      yywarning(char* s, ... );
extern int		yylex();
extern int		asmlex();
extern void		mtest(void* vector,int Line,char* File_);
extern void		debug_print(char* s, ... );

#ifdef _MSC_BUILD
	#define snprintf sprintf_s
	#include <malloc.h>
    #include <crtdbg.h>
#endif

//#define DEBUG

#define TRUE 1
#define FALSE 0

#endif //_EXPORT_H_