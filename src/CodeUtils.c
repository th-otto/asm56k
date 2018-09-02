// -----------------------------------------------------------------------------------------------
/*

Project:    asm56k
Author:     M.Buras (sqward)


*/
// -----------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <asm_types.h>
#include <export.h>
#include "CodeUtils.h"
// -----------------------------------------------------------------------------------------------

chunk chunks[1024];
int	num_chunks;		/* pass 1 */
int	num_chunks2;	/* pass 2 */

int	pc;
unsigned char*	c_ptr;
unsigned char*	c_max_ptr;
int mem_space;
int in_section;

 // -----------------------------------------------------------------------------------------------

 void allocate_chunk(int type)
 {
	 unsigned char* pNewChunkMem;

	 if(in_section)
	 {
		 chunks[num_chunks2].code_len2=(c_ptr-chunks[num_chunks2].code_ptr);
		 debugprint("d2 0x%x, 0x%x, 0x%x\n",c_ptr,chunks[num_chunks2].code_len2,chunks[num_chunks2].code_len);

		 if(chunks[num_chunks2].code_len2==0)
		 {
            if ( g_passNum )
			     yywarning("Empty section defined.");
		 }
		 num_chunks2++;
	 }

	 pNewChunkMem=(unsigned char*)malloc(chunks[num_chunks2].code_len);
	 mtest(pNewChunkMem,__LINE__,__FILE__);
	 c_ptr=chunks[num_chunks2].code_ptr=pNewChunkMem;
	 chunks[num_chunks2].mem_type=type;
	 chunks[num_chunks2].pc=pc;
	 in_section=TRUE;
 }
 
// -----------------------------------------------------------------------------------------------

int GetCurrentMemType()
{
    if ( g_passNum )
	    return chunks[num_chunks2].mem_type;
    else
        return chunks[num_chunks].mem_type;
}

// -----------------------------------------------------------------------------------------------

void    CheckCodeInLMem()
{
    if ( GetCurrentMemType() == L_MEM )
        yyerror ( "No code in L: memory allowed." );
}

// -----------------------------------------------------------------------------------------------

int GetCurrentPC()
{
    return pc;
}

// -----------------------------------------------------------------------------------------------

int GetCurrentChunkBegin()
{
	if ( g_passNum )
		return chunks[num_chunks2].pc;
	else
		return chunks[num_chunks].pc;
}
// -----------------------------------------------------------------------------------------------

void close_chunk()
{
	if(in_section)
	{
		int	temp_len;
		temp_len=chunks[num_chunks2].code_len2=(c_ptr-chunks[num_chunks2].code_ptr);
		debugprint("d2 0x%x, 0x%x, 0x%x\n",c_ptr,chunks[num_chunks2].code_len2,chunks[num_chunks2].code_len);
		if(chunks[num_chunks2].code_len2==0)
		{
			yywarning("Empty section defined.");
		}
		num_chunks2++;
	}
}

 // -----------------------------------------------------------------------------------------------

 void  allocate_vchunk(int type)
 {
	 mem_space=type;

	 if(in_section)
	 {
		 chunks[num_chunks].code_len=(long)c_ptr;
		 num_chunks++;
	 }
	 c_ptr=0;

     chunks[num_chunks].mem_type=type;
     chunks[num_chunks].pc=pc;
	 
     in_section=TRUE;
 }

 // -----------------------------------------------------------------------------------------------

 void close_vchunk()
 {
	 if(in_section)
	 {
		 chunks[num_chunks].code_len=(long)c_ptr;
		 num_chunks++;
	 }
 }

 // -----------------------------------------------------------------------------------------------
void GenOrg(uint memSpace, uint address)
{
	PASS1
		pc = address;
		allocate_vchunk(memSpace);
	PASS2
		pc = address;
		allocate_chunk(memSpace);
	PASSEND
}

// -----------------------------------------------------------------------------------------------

void GenDc(Value data)
{ 
	IN_PASS1 
		if ( GetCurrentMemType() == L_MEM )
			inst_code.sflag = true;
	IN_PASS2 
		if ( GetCurrentMemType() == L_MEM )
		{
			u64 raw_val = Val_GetAsFract48(data);
			inst_code.sflag = true;
			inst_code.w0 = raw_val & 0xffffff;
			inst_code.w1 = raw_val >> 24;
		}
		else
		{
			u32 raw_val = Val_GetAsFract24(data);
			inst_code.w0 = raw_val;
		}
	AFTER_PASSES
}


void GenDS( Value val1 )
{
    int lmem = 1;
    int size;

    int val = 0;

    if ( Val_CheckResolved ( val1 ) )
    {
        yyerror ( "Forward referenced symbols not allowed for DS statement." );
        return ; 
    }

    val = Val_GetAsInt( val1 );

    if ( GetCurrentMemType() == L_MEM )
    {
        lmem = 2;        
    }

    size = val * lmem * 3;

    pc += val * lmem ;

    if ( g_passNum )
    {
        int i;

        for ( i = 0; i < size; i++ )
        {
            *c_ptr++ = 0;
        }
    }
    else
    {
        c_ptr += size;
    } 
}

int GenAlign( Value val1 )
{
    int old_pc = pc;

    int val = 0;

    if ( Val_CheckResolved ( val1 ) )
    {
        yyerror ( "Forward referenced symbols not allowed for ALIGN statement." );
        return ; 
    }

    val = Val_GetAsInt( val1 );

    if ( GetCurrentMemType() == L_MEM )
        val <<= 1;        

    while ( 1 )
    {
        if ( pc % val == 0 )
        {
            break;
        }

        if ( g_passNum )
        {
            *c_ptr++ = 0;
            *c_ptr++ = 0;
            *c_ptr++ = 0;
        }
        else
        {
            c_ptr += 3;
        }

        pc++;
    }

    return pc - old_pc;
}

void GenDSM( hs* pLabel, Value val1 )
{
    int wasted = 0;
    u32 align_mask = 0x80000000;

    int val = 0;

    if ( Val_CheckResolved ( val1 ) )
    {
        yyerror ( "Forward referenced symbols not allowed for DSM statement." );
        return ; 
    }

    val = Val_GetAsInt( val1 );

    if ( val == 0  && g_passNum )  
    {
        yywarning("Zero size DSM buffer not allowed.");
        return;
    }

    while ( 1 )
    {
        if ( val & align_mask )
        {
            break;
        }
        align_mask >>= 1;
    }

    wasted = GenAlign( Val_CreateInt( align_mask ) );

    if ( pLabel )
    {
        SymSetValue( pLabel, T_PTR, Val_CreateInt( pc ) );              // reset label address value
    }   

    if ( wasted != 0 && g_passNum )
    {

        yywarning("DSM statement wasted %d words of memory.", wasted );
    }

    GenDS( Val_CreateInt( val ) );
}

// -----------------------------------------------------------------------------------------------

 void verify_code()
 {
	 int	a;

	 if(num_chunks!=num_chunks2)
	 {
		 yyerror("Internal error #1.");
	 }
	 else
	 {
		 for ( a = 0; a < num_chunks; a++ )
		 {
			 if(chunks[a].code_len2!=chunks[a].code_len)
			 {
                 yyerror("Internal error #2: mem: %d, org: 0x%x, skew: %d",chunks[a].mem_type,chunks[a].pc, (chunks[a].code_len2 - chunks[a].code_len) / 3 );
			 }
		 }
	 }

 }

// -----------------------------------------------------------------------------------------------

void   insert_code_w(bcode *inst_code)
{

//     if ( (inst_code->w1& 0xffffff) == 0x7FFF80 )
//     {
//         yywarning( "found!");
// 
//     }

    *c_ptr++=(unsigned char)(inst_code->w0>>16);
    *c_ptr++=(unsigned char)(inst_code->w0>>8);
    *c_ptr++=(unsigned char)(inst_code->w0);
    pc++;

    if(inst_code->sflag)
    {

        *c_ptr++=(unsigned char)(inst_code->w1>>16);
        *c_ptr++=(unsigned char)(inst_code->w1>>8);
        *c_ptr++=(unsigned char)(inst_code->w1);
        pc++;
    }
}

// -----------------------------------------------------------------------------------------------

void   InsertString(const char *pString,int str_len)
{
    int wordLength;

    str_len++;      // take the termination character into account

    wordLength = ( str_len / 3 ) +( (str_len % 3) > 0 ? 1 : 0 );

    pc+=wordLength;

	if ( g_passNum )
	{
        int i = ( wordLength * 3 ) % str_len;

		strcpy( c_ptr, pString ) ;

        c_ptr+=str_len;

		for(  ; i!=0 ; i--)
		{		
			*c_ptr++ = 0;  //pad out the remaining bytes
		}
    }
    else
    {
        c_ptr+=wordLength*3;
    }
}

// -----------------------------------------------------------------------------------------------

void   insert_vcode_w(bcode *inst_code)
{
    if(in_section)
    {
        if(inst_code->sflag==0)
        {
            c_ptr+=3;
            pc++;
        }
        else
        {
            c_ptr+=6;
            pc+=2;
        }
    }
    else
    {
        yyerror("No output memory section defined.");
        asm_abort();
    }
}

// -----------------------------------------------------------------------------------------------

void	retInit( raddr *ret )
{
	ret->sflag=0;
	ret->abs_value=0;
	ret->type=0;
	ret->value=0;
}

// -----------------------------------------------------------------------------------------------
