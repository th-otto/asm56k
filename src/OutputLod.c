/*

Project:    asm56k
Author:     M.Buras (sqward)

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <asm_types.h>
#include <export.h>
#include <Value.h>
#include "SymbolTable.h"
#include "CodeUtils.h"
#include "OutputLod.h"

/*
 * Generate output code in LOD format
 */

void LOD_OutputSymbols(FILE* output_file, int memspace)
{
	int i;

	for ( i = 0; i < HASH_SIZE ; i++ )
	{
		hs* pSymbol = hash_tab[i].pHead;

		while ( pSymbol != NULL )
		{
			if ( pSymbol->mem_space == memspace && pSymbol->type == T_PTR )
			{
				fprintf(output_file,"%s  I %.6X\r\n",pSymbol->pString,Val_GetAsInt( pSymbol->m_val ) );
			}
			pSymbol = pSymbol->pNext;
		}
	}
}

/*
 * offset and skip are used for L memory only
 */

void LOD_SaveData(FILE* output_file,int chunkIndex, const char* MemType, int offset, int skip)
{
    int j,code_word;
    int mod_cnt;
    unsigned char* code;

	fprintf(output_file,"_DATA ");

    fprintf(output_file,"%s %.4X\r\n",MemType,chunks[chunkIndex].pc);

    mod_cnt = 0;
    code = chunks[chunkIndex].code_ptr + offset;

    debugprint("codelen %d, %d\r\n",chunks[chunkIndex].code_len/3,chunks[chunkIndex].code_len);

    j = (chunks[chunkIndex].code_len/3) >> ( skip ? 1 : 0 ) ;

    for( ; j != 0; j-- )
    {
        code_word=*code++;
        code_word<<=8;
        code_word|=*code++;
        code_word<<=8;
        code_word|=*code++;

        code += skip;

        if(mod_cnt==7 || j==1)
            fprintf(output_file,"%.6X\r\n",code_word);
        else
            fprintf(output_file,"%.6X ",code_word);

        mod_cnt++;
        mod_cnt&=0x7;
    }
}

void SaveFileLod(char* name,char* iname)
{
	FILE *output_file;
	int i;

	output_file=fopen(name,"wb");
	if(output_file<=0)
	{
		printf("error while opening file: %s for write.\n",name);
		return;
	}
	/* write out LOD header: */

	fprintf(output_file,"_START %s 0000 0000 0000 asm56k v0.1a\r\n\r\n",iname);

	if(num_chunks==0)
	{
		return;
	}

	for( i = 0; i != num_chunks2 ;i++ )
	{	
		int j;
        int num_zeros = 0;

        for ( j = 0; j < chunks[i].code_len; j++ )
        {
            if ( chunks[i].code_ptr[j] == 0 )
            {
                num_zeros ++;
            }
        }

        if ( num_zeros != chunks[i].code_len || g_write_zero_sections ) 
        {
		    switch(chunks[i].mem_type)
		    {
			    break; case P_MEM:  LOD_SaveData(output_file, i, "P", 0, 0 );
			    break; case X_MEM:  LOD_SaveData(output_file, i, "X", 0, 0 );
			    break; case Y_MEM:  LOD_SaveData(output_file, i, "Y", 0, 0 );
                break; case L_MEM:  LOD_SaveData(output_file, i, "X", 3, 3 );
                                    LOD_SaveData(output_file, i, "Y", 0, 3 );
            }
        }
	}

	fprintf(output_file,"_SYMBOL X\r\n");
	LOD_OutputSymbols(output_file,X_MEM);
	fprintf(output_file,"_SYMBOL Y\r\n");
	LOD_OutputSymbols(output_file,Y_MEM);
	fprintf(output_file,"_SYMBOL P\r\n");
	LOD_OutputSymbols(output_file,P_MEM);

	fprintf(output_file,"_END 0000\r\n");

    fclose( output_file );
}
