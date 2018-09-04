/*

Project:    asm56k
Author:     M.Buras (sqward)

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>

#ifdef _MSC_VER
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
#include <StringBuffer.h>
#include <getopt.h>



int g_currentLine = 1;
int g_passNum;
int g_errorCount;
int g_warnCount;
int g_LocalSerial = 0;
jmp_buf critical_error;

static char const program[] = "asm56k";
static char const version[] = "0.92";


void mtest(void *pMem, int Line, char *File_)
{
	if (pMem == NULL)
	{
		yyerror("Out of memory. Program aborted.\n");
		asm_abort();
	}
}


void debugprint(const char *pFmt, ...)
{
#ifdef DEBUG
	va_list arglist;
	char buff[512];

	va_start(arglist, pFmt);

	vprintf(pFmt, arglist);
	va_end(arglist);
#endif
};

int yyerror(const char *s, ...)
{
	char error_buf[512];
	va_list arglist;
	int i;

	for (i = 0; i < g_incStackDeepth; i++)
	{
		printf("%s:%d: In file included from here.\n", inc_names[i], inc_lines[i]);
	}

	va_start(arglist, s);
	snprintf(error_buf, 512, "%s:%d: %s\n", inc_names[g_incStackDeepth], g_currentLine, s);
	vprintf(error_buf, arglist);
	g_errorCount++;
	va_end(arglist);
	return 0;
};

int yywarning(const char *s, ...)
{
	char warn_buf[512];
	va_list arglist;
	int i;

	for (i = 0; i < g_incStackDeepth; i++)
	{
		printf("%s:%d: In file included from here.\n", inc_names[i], inc_lines[i]);
	}

	va_start(arglist, s);
	snprintf(warn_buf, 512, "%s:%d: Warning: %s\n", inc_names[g_incStackDeepth], g_currentLine, s);
	vprintf(warn_buf, arglist);
	g_warnCount++;
	va_end(arglist);
	return 0;
}


void asm_abort(void)
{
	printf("Terminating execution.\n");
	longjmp(critical_error, 1);
}


static void InitParserPass1(void)
{
	g_LocalSerial = 0;
	in_section = FALSE;
	g_passNum = 0;						/* pass 1 */
	pc = 0;
	g_currentLine = 1;
	num_chunks = 0;
	num_chunks2 = 0;
	dc_flag = 0;
	if_stack_l = 0;
	ResetStream();
	PushStream(g_tokens, inc_names[0], 1, -1, 0);
}

static void InitParserPass2(void)
{
	g_LocalSerial = 0;
	in_section = FALSE;
	g_passNum = 1;						/* pass 2 */
	pc = 0;
	dc_flag = 0;
	if_stack_l = 0;
	g_currentLine = 1;
	ResetStream();
	PushStream(g_tokens, inc_names[0], 1, -1, 0);
}


char *lod_output_name = NULL;
char *p56_output_name = NULL;
char *embed_output_name = NULL;

char *input_name = NULL;
int g_dsp_cpu = 56001;
int g_falcon = 0;
int g_output_symbols = 0;
int g_write_zero_sections = 0;


static int asm56k(void)
{
	FILE *input = NULL;
	int num_tokens = 0;

	if (input_name == NULL)
	{
		fprintf(stderr, "No input file given.\n");
		return EXIT_FAILURE;
	}

	if (lod_output_name == NULL && p56_output_name == NULL && embed_output_name == NULL)
	{
		fprintf(stderr, "No output file given.\n");
		return EXIT_FAILURE;
	}

	AddIncDir("./");

	if (PushNewFile(input_name) != 0)
	{
		fprintf(stderr, "File not found: '%s'\n", input_name);
		return EXIT_FAILURE;
	}

	debugprint("PASS1\n");

	if (setjmp(critical_error) == 0)
	{

		InitTokenStream(input, input_name);
		InitMacroProxy();

		num_tokens = PrefetchTokens();

		InitParserPass1();
		debugprint("%d tokens fetched\n", num_tokens);

		if (yyparse() == 0 && g_errorCount == 0)
		{
			/*  pass 2 */
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

		if (g_errorCount)
		{
			printf("Finished with %d error(s).\n", g_errorCount);
		} else
		{
			if (NULL != lod_output_name)
			{
				SaveFileLod(lod_output_name, input_name);
			}

			if (NULL != p56_output_name)
			{
				SaveFileP56(p56_output_name);
			}

			if (NULL != embed_output_name)
			{
				SaveFileEmbeded(embed_output_name);
			}

		}
	}

	return g_errorCount != 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}


static void DefineSymbol(char *v)
{
	char *pSymbol;
	char *pValue;
	Value val;
	stext symstr;

	pSymbol = v;
	pValue = strchr(pSymbol, '=');
	if (pValue)
	{
		*pValue++ = 0;
	}
	symstr.len = strlen(pSymbol);
	symstr.ptr = pSymbol;
	AddSym(&symstr, 1);
	if (pValue)
		val = Val_CreateInt(atoi(pValue));
	else
		val = Val_CreateInt(1);
	SymSet(pSymbol, val);
}

static void AddIncludePath(const char *v)
{
	AddIncDir(v);
	debugprint("adding incdir: %s\n", v);
}


enum opt {
	OPT_FLAG_SET = 0,
	OPT_HELP = 'h',
	OPT_VERSION = 'V',
	
	OPT_EMBED = 'e',
	OPT_OUTPUT = 'o',
	OPT_P56 = 'p',
	OPT_SYMBOLS = 's',
	OPT_ZEROES = 'z',
	OPT_DEFINE = 'D',
	OPT_INCLUDE = 'I',
	OPT_CPU = 'c'
};

static struct option const long_options[] = {
	{ "lod-file", required_argument, NULL, OPT_OUTPUT },
	{ "output", required_argument, NULL, OPT_OUTPUT },
	{ "p56-file", required_argument, NULL, OPT_P56 },
	{ "embed-file", required_argument, NULL, OPT_EMBED },
	{ "symbols", no_argument, NULL, OPT_SYMBOLS },
	{ "write-zero", no_argument, NULL, OPT_ZEROES },
	{ "zero", no_argument, NULL, OPT_ZEROES },
	{ "define", required_argument, NULL, OPT_DEFINE },
	{ "include", required_argument, NULL, OPT_INCLUDE },
	{ "cpu", required_argument, NULL, OPT_CPU },
	{ "help", no_argument, NULL, OPT_HELP },
	{ "version", no_argument, NULL, OPT_VERSION },
	{ NULL, no_argument, NULL, 0 },
};


static void usage(FILE *fp, int status)
{
	fprintf(fp, "usage: %s [options] <input-file>\n", program);
	fprintf(fp, "options:\n");
	fprintf(fp, "  -o, --output <file>          LOD output file.\n");
	fprintf(fp, "  -p, --p56-file <file>        P56 output file.\n");
	fprintf(fp, "  -e, --embed-file <file>      Output devpac/vasm file.\n");
	fprintf(fp, "  -s, --symbols                Output symbols (in LOD).\n");
	fprintf(fp, "  -z, --write-zero             Output section even if it contains only zeros.\n");
	fprintf(fp, "  -D, --define <name[=value]>  Define a symbol.\n");
	fprintf(fp, "  -I, --include <dir>          Add include path.\n");
	
	exit(status);
}


int main(int argc, char *argv[])
{
	int ret = EXIT_SUCCESS;
	int c;
	
	StringBufferInit(0x8000);
	InitSymbolTable();

	while ((c = getopt_long(argc, argv, "c:e:o:p:szD:I:", long_options, NULL)) != -1)
	{
		switch ((enum opt) c)
		{
		case OPT_OUTPUT:
			free(lod_output_name);
			lod_output_name = strdup(optarg);
			break;

		case OPT_P56:
			free(p56_output_name);
			p56_output_name = strdup(optarg);
			break;

		case OPT_EMBED:
			free(embed_output_name);
			embed_output_name = strdup(optarg);
			break;

		case OPT_SYMBOLS:
			g_output_symbols = 1;
			break;
		
		case OPT_ZEROES:
			g_write_zero_sections = 1;
			break;
		
		case OPT_DEFINE:
			DefineSymbol(optarg);
			break;
		
		case OPT_INCLUDE:
			AddIncludePath(optarg);
			break;
		
		case OPT_CPU:
			g_dsp_cpu = (int)strtol(optarg, NULL, 10);
			break;
		
		case OPT_VERSION:
			fprintf(stdout, "%s version %s\n", program, version);
			return EXIT_SUCCESS;
		
		case OPT_HELP:
			usage(stdout, EXIT_SUCCESS);
			break;
		
		case OPT_FLAG_SET:
			break;
		
		default:
			return EXIT_FAILURE;
		}
	}

	if (optind < argc)
		input_name = argv[optind];
	
	ret = asm56k();

	return ret;
}
