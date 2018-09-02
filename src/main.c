// -----------------------------------------------------------------------------------------------
/*

Project:    asm56k
Author:     M.Buras (sqward)

*/
// -----------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>

#ifdef _MSC_BUILD
    #include <crtdbg.h>
    #include <direct.h>
#else
    #include <unistd.h>
#endif

#include <math.h>
#include <string.h>
#include <asm_types.h>
#include "export.h"
#include <Value.h>
#include <SymbolTable.h>
#include <CodeUtils.h>
#include <Parser.h>
#include <TokenStream.h>
#include <MacroProxy.h>
#include <OutputLod.h>
#include <OutputP56.h>
#include <OutputEmbededAsm.h>
#include <opt.h>

// -----------------------------------------------------------------------------------------------

int 			g_currentLine = 1;
int 			g_passNum;
int				g_errorCount;
int				g_warnCount;
int             g_LocalSerial = 0;
jmp_buf         critical_error;

int				yyparse();

// -----------------------------------------------------------------------------------------------

void mtest(void* pMem,int Line,char* File_)
{
	if( pMem == NULL ) 
	{
		yyerror("Out of memory. Program aborted.\n");
		asm_abort();
	}
}

// -----------------------------------------------------------------------------------------------

void debugprint(char* pFmt, ... )
{
#ifdef DEBUG
	va_list arglist;
	char buff[512];
	va_start(arglist,pFmt);

	vprintf(pFmt,arglist);
	va_end(arglist);
#endif
};

int yyerror(char* s, ... )
{
	char error_buf[512];
	va_list arglist;
    int i;

	for ( i = 0; i < g_incStackDeepth; i++)
	{
		printf ( "%s:%d: In file included from here.\n",inc_names[i],inc_lines[i]);
	}

	va_start( arglist,s);
	snprintf( error_buf,512,"%s:%d: %s\n",inc_names[g_incStackDeepth],g_currentLine,s);
	vprintf( error_buf,arglist);
	g_errorCount++;
	va_end(arglist);
	return 0;
};

int yywarning(char* s, ...)
{
	char warn_buf[512];
	va_list arglist;
    int i;

    for ( i = 0; i < g_incStackDeepth; i++)
    {
        printf ( "%s:%d: In file included from here.\n", inc_names[i], inc_lines[i] );
    }

	va_start(arglist,s);
	snprintf (warn_buf, 512,"%s:%d: Warning: %s\n", inc_names[g_incStackDeepth], g_currentLine,s);
	vprintf(warn_buf,arglist);
	g_warnCount++;
	va_end(arglist);
	return 0;
};

// -----------------------------------------------------------------------------------------------

void asm_abort()
{
	printf(  "Terminating execution.\n" );
	longjmp( critical_error, 1);
}

// -----------------------------------------------------------------------------------------------

void InitParserPass1()
{
    g_LocalSerial = 0;
	in_section=FALSE;
	g_passNum=0;	// pass 1
	pc=0;
	g_currentLine=1;
	num_chunks=0;
	num_chunks2=0;
	dc_flag=0;
	if_stack_l=0;
	ResetStream();
	PushStream( g_tokens,inc_names[0] ,1, -1, 0);
}

void InitParserPass2()
{
    g_LocalSerial = 0;
    in_section=FALSE;
	g_passNum=1;	// pass 2
	pc=0;
	dc_flag=0;
	if_stack_l=0;
	g_currentLine=1;
	ResetStream();
	PushStream( g_tokens,inc_names[0],1, -1 , 0);
}

// -----------------------------------------------------------------------------------------------

char* lod_output_name = NULL;
char* p56_output_name = NULL;
char* embed_output_name = NULL;

char* input_name = NULL;
char* def_symbol = NULL;
char* include_path = NULL;
int g_dsp_cpu = 56001;
int g_falcon = 0;
int g_output_symbols=0;
int g_write_zero_sections=0;

// -----------------------------------------------------------------------------------------------

int asm56k(int argc,char* argv[])
{
	FILE* input;
	int	num_tokens = 0;

    if ( input_name == NULL )
    {
        printf("No input file given.\n");
        return -1;
    }

    if (    lod_output_name == NULL && 
            p56_output_name == NULL && 
            embed_output_name == NULL   )
    {
        printf("No output file given.\n");
        return -1;
    }
	
    AddIncDir("./");

	if( PushNewFile( input_name ) != 0 )
	{
        printf("File not found: '%s'\n",argv[1]);
		return -1;
	}

	debugprint("PASS1\n");

    if( setjmp( critical_error ) == 0 )
    {

	    InitTokenStream(input,input_name);
	    InitMacroProxy();

        num_tokens = PrefetchTokens();

        InitParserPass1();
	    debugprint("%d tokens fetched\n", num_tokens );
        
	    if( yyparse() == 0 && g_errorCount == 0 )
	    {	
		    //	pass 2
		    debugprint("PASS2\n");
		    close_vchunk();
		    InitMacroProxy();
		    InitParserPass2();
		    yyparse();
		    close_chunk();
		    verify_code();
	    }
    			
	    #ifdef DEBUG
	        debugprint("Listing symbols:");
	        ListSymbolTable();
	        debugprint("end of symbols...");
	    #endif
    	
        if( g_errorCount )
        {
            printf( "Finished with %d error(s).\n",g_errorCount);
        }
        else
        {
            if( NULL != lod_output_name )
	        {
		        SaveFileLod( lod_output_name, input_name );
	        }

            if( NULL != p56_output_name )
            {
                SaveFileP56( p56_output_name );
            }

            if( NULL != embed_output_name )
            {
                SaveFileEmbeded( embed_output_name );
            }
            
        }
    }

	return g_errorCount;
}

// -----------------------------------------------------------------------------------------------

int DefineSymbol(void *v) 
{
	char* pSymbol;
	char* pValue;

	if ( def_symbol )
	{
		stext symstr;
		pSymbol = def_symbol;
		pValue = strchr(pSymbol,'=');
		if (pValue)
		{
			*pValue++=0;
		}
		symstr.len=strlen(pSymbol);
		symstr.ptr=pSymbol;
		AddSym(&symstr,1);
		if(pValue)
		{
			Value val = Val_CreateInt( atoi(pValue) );
			SymSet(pSymbol,val);
		}
	}
	return OPT_OK; 
}

int AddIncludePath(void *v) 
{
	AddIncDir(include_path);
	debugprint("adding incdir: %s\n",include_path);
	return OPT_OK; 
}

// -----------------------------------------------------------------------------------------------

int main(int argc,char* argv[])
{
    int ret = 0;

#ifdef _MSC_BUILD
    int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
    tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
    tmpFlag &= ~_CRTDBG_CHECK_CRT_DF;
    _CrtSetDbgFlag( tmpFlag );
#endif

    optreg(&g_output_symbols,OPT_BOOL,'s',"Output symbols (in LOD).");

	optrega(&lod_output_name,OPT_STRING,'o',"lod-file","LOD output file.");
	optrega(&p56_output_name,OPT_STRING,'p',"p56-file","P56 output file.");
	optrega(&embed_output_name,OPT_STRING,'e',"embed-file","Output devpac/vasm file.");	

    optrega(&g_write_zero_sections,OPT_BOOL,'z',"write-zero","Output section even if it contains only zeros.");

	optreg(&def_symbol,OPT_STRING,'D',"Define a symbol: [-Dsymbolname[=val]]");
	opthook(&def_symbol, DefineSymbol);

	optreg(&include_path,OPT_STRING,'I',"Add include path: [-Ipath]");
	opthook(&include_path, AddIncludePath);

    optrega(&g_dsp_cpu,OPT_INT,'c',"cpu","Sets CPU type. [--cpu=56301]");

    optregp(&input_name,OPT_STRING,"input-file","File to process");

    optProgName( "asm56k" );
    optVersion("0.92");
    optUsage( "[options] input-file" );

	StringBufferInit(0x8000);
	InitSymbolTable();

    opt(&argc,&argv);

    ret = asm56k(argc,argv);
    opt_free();

    return ret;
}

// -----------------------------------------------------------------------------------------------
