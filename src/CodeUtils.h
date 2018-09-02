// -----------------------------------------------------------------------------------------------
/*

Project:    asm56k
Author:     M.Buras (sqward)


*/
// -----------------------------------------------------------------------------------------------
#ifndef _CODEUTILS_H_
#define _CODEUTILS_H_

#include <Value.h>
#include <SymbolTable.h>

#define T_LONG 0
#define T_SHORT 1
#define T_REGISTER 2

#define P_MEM 0
#define X_MEM 1
#define Y_MEM 2
#define L_MEM 3

// -----------------------------------------------------------------------------------------------

typedef struct
{
    uint pc;
    uchar *code_ptr;
    int	code_len;		// code length for pass 1
    int code_len2;		// code length for pass 2, for verification
    int	mem_type;
}chunk;

typedef struct
{
    int sflag;
	uint w0;
    uint w1;
} bcode;

typedef struct {
    int sflag;
    int type;
    int	value;
    int abs_value;
}raddr;

extern chunk	chunks[1024];
extern int	num_chunks;		/* pass 1 */
extern int	num_chunks2;	/* pass 2 */

extern int		pc;
extern unsigned char*	c_ptr;
extern unsigned char*	c_max_ptr;
extern int 	mem_space;
extern int 	in_section;

// -----------------------------------------------------------------------------------------------

void	CreateOutputName(char* input_name,char** output_name);
void	allocate_chunk(int type);
void	close_chunk();
void	allocate_vchunk(int type);
void	close_vchunk();
void	verify_code();
int		GetCurrentMemType();
int     GetCurrentPC();
void	insert_code();
void	insert_code_w(bcode *inst_code);
void	InsertString(const char *string,int str_len);
void	insert_vcode();
void	insert_vcode_w(bcode *inst_code);
void	GenDc( Value data );
void	GenDS( Value val1 );
int		GenAlign( Value val1 );
void	GenDSM( hs* pLabel, Value val1  );
void	GenOrg(uint memSpace, uint address);
void    CheckCodeInLMem();
int		GetCurrentChunkBegin();
void	retInit( raddr *ret );

// -----------------------------------------------------------------------------------------------

// this stuff looks really dodgy, but it helps a lot in code generators.

#define PASS1 if(!g_passNum){
#define PASS2 }else{ 
#define PASSEND };

#define PAR1 bcode move; move.sflag=0; move.w0 = 0; move.w1 = 0; if(!g_passNum){
#define PAR2 }else{ 
#define PAREND }; return move;

#define IN_PASS1 if(!g_passNum){ bcode inst_code; inst_code.sflag = 0;  inst_code.w0 = 0; inst_code.w1 = 0;
#define IN_PASS2 insert_vcode_w(&inst_code); }else{ bcode inst_code;  inst_code.sflag = 0; inst_code.w0 = 0; inst_code.w1 = 0;
#define AFTER_PASSES insert_code_w(&inst_code); };

#define DSP56301 if ( g_dsp_cpu != 56301 ) yyerror ("Illegal 56301 instruction. CPU is set to %d",g_dsp_cpu );
// this is a bit pointless...
#define DSP56001 if ( g_dsp_cpu != 56001 ) yyerror ("Illegal 56001 instruction. CPU is set to %d",g_dsp_cpu );

#endif //_CODEUTILS_H_