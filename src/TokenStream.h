// -----------------------------------------------------------------------------------------------
/*

Project:    asm56k
Author:     M.Buras (sqward)

*/
// -----------------------------------------------------------------------------------------------
#ifndef _TOKENSTREAM_H_
#define _TOKENSTREAM_H_

#define MAX_CONDITION_NESTS 1024

#define	MACRO_NEST_DEPTH 64
#define	MACRO_PARAMS_TOKEN_BUFFER 2048
#define MACRO_PARAMS_POINTER_BUFFER 32*64

#define LEX_BUFFER 0x8000			// this is size of the flex buffer (for each included file)

// -----------------------------------------------------------------------------------------------

struct TokenVal;

typedef struct  
{
	int token;
	union
	{
		void* pNextBlock;
		YYSTYPE val;
	}data;
} TokenVal;

typedef struct{
	TokenVal* 	macro_ptr;
	int			instancesNumber;
	int			line_num;
	int 	    menager_mode;
	int 	    params_count;
	TokenVal**	params_array;
	TokenVal*	pParamsPool;
}StreamStackEntry;

// -----------------------------------------------------------------------------------------------

extern void*	buffer;

extern	int		if_stack_l;
extern	char	if_stack[];

// includes stack:
extern const char*	inc_names[];
extern const char*	g_CurrentFile;
extern FILE*		inc_handles[];
extern void*		inc_buffers[];
extern int			inc_lines[];
extern int			g_incStackDeepth;

extern TokenVal* 	g_tokens;
extern TokenVal* 	wglobal_tokens;

extern int 			g_streamsStrackIndex;
extern StreamStackEntry	streamsStack[];

extern TokenVal* 	g_pParamsPool;
extern TokenVal** 	g_pParamsArrayPool;
extern int			g_MacroNumInstances;
extern TokenVal*	params_pointers[];
extern TokenVal		macros_params[];

// -----------------------------------------------------------------------------------------------

void		InitTokenStream(FILE* input,const char *file_name);
void		PushStream( TokenVal* pMacro,const char* pFileName, int curline, int params_count, int instancesNumber );
int			PopStream();
int			TopPosStream();
void		ResetStream();
TokenVal*	GetCurrentStreamPos();
int			GetToken(TokenVal** pTokenValue);
int			SkipToken();
TokenVal*	CopyToken(int token,TokenVal* pToken);
int			PrefetchTokens();
void    	Skip_line();
int     	SkipConditional();
int     	PopFile();
int         PushNewFile(const char* pFileName);
void    	IncludeFile();

void		AddIncDir(char* pDir);
bool		GetIncDir(const char **pDir, uint* pDirNum);

// -----------------------------------------------------------------------------------------------

#endif // _TOKENSTREAM_H_