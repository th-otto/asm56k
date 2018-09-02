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
#include <Value.h>
#include <CodeUtils.h>		//eventually this will be removed
#include <SymbolTable.h>
#include <Parser.h>
#include <TokenStream.h>
#include <MacroProxy.h>
#include <StringBuffer.h>

// -----------------------------------------------------------------------------------------------

hashEntry 		hash_tab[HASH_SIZE];
hs* 			g_pSymbolSlot;
hs* 			free_slot;
int				node_len=0;

// -----------------------------------------------------------------------------------------------
/*
	Initialise hash tables                
*/
// -----------------------------------------------------------------------------------------------
void InitSymbolTable()
{
	g_pSymbolSlot=(hs*)malloc(MAX_DESC*sizeof(hs));
	mtest(g_pSymbolSlot,__LINE__,__FILE__);
	free_slot=g_pSymbolSlot;
	memset( (void*) hash_tab, sizeof(hash_tab),0);
}
// -----------------------------------------------------------------------------------------------
/*  
	hashtable string menagment            
	TODO: use crc32 to calculate hash
*/	
// -----------------------------------------------------------------------------------------------
uint hash(const char* pString)
{
	uint sum = 0;

	while (*pString)
	{
		sum<<=1;
		sum^=*pString;
		pString++;
	}

	return sum & ( HASH_SIZE-1 );
}
// -----------------------------------------------------------------------------------------------
/* 
	Add a new string to the hash table. If not enough memory in the tables expand... 
	If string already exists, warn..    
*/
// -----------------------------------------------------------------------------------------------

hs* AddSymbol(const char* pString,int len,int forceCopy)
{
	uint hash_val=hash(pString);
	hs* pNewEntry;


#ifdef _MSC_BUILD
    _ASSERT ( _CrtCheckMemory( ) );
#endif
												// check if the string is already defined
	if( FindSymbol(pString) != NULL )
	{
		return NULL;							
	}
												// if we have no empty hash slots
												// expand...
	if( node_len >= MAX_DESC )
	{
		g_pSymbolSlot=(hs*)malloc(MAX_DESC*sizeof(hs));
		mtest(g_pSymbolSlot,__LINE__,__FILE__);
		free_slot=g_pSymbolSlot;
		node_len=0;
	}
	else
	{
		node_len++;
	}

	// add the string into the table
	// if a given hash slot level is not initialised yet
	// do it...

#ifdef _MSC_BUILD
    _ASSERT ( _CrtCheckMemory( ) );
#endif

	if( forceCopy )
	{
		pString = StringBufferInsert((char*)pString);
	}

#ifdef _MSC_BUILD
    _ASSERT ( _CrtCheckMemory( ) );
#endif

	if(hash_tab[hash_val].pHead==0)
	{
		pNewEntry=free_slot++;
		hash_tab[hash_val].pHead=pNewEntry;
		hash_tab[hash_val].pTail=pNewEntry;
	}
	else
	{							// if the hash level already contains entries then use them.
		pNewEntry=free_slot++;
		hash_tab[hash_val].pTail->pNext=pNewEntry;
		hash_tab[hash_val].pTail=pNewEntry;
	}

	pNewEntry->pString=pString;
	pNewEntry->len=len;
	pNewEntry->pNext=0;
	pNewEntry->mem_space=-1;

#ifdef _MSC_BUILD
    _ASSERT ( _CrtCheckMemory( ) );
#endif
	return pNewEntry;
}

// -----------------------------------------------------------------------------------------------
/* 
	Find a string in the hash table. If not found, warn. If found, return slot pointer.             
*/
// -----------------------------------------------------------------------------------------------

hs* FindSymbol(const char* pString)
{
	int	hash_val=hash(pString);
	hs*	pNext=hash_tab[hash_val].pHead;

	while ( pNext )
	{
		if( strcmp(pString,pNext->pString)==0)
		{
			return pNext;
		}

		pNext = pNext->pNext;
	}

	return 0;
}

/* set meaning of a given hash slot */

void SymSetValue(hs* slot,int meaning,Value val)
{
    if ( Val_CheckResolved ( val ) )
    {
        yyerror ( "Forward referenced symbols not allowed." );
        return ; 
    }

	slot->type=(char)meaning;
	slot->m_val=val;
}

void SymSetValueMacro(hs* slot,int meaning,void* val1,void* val2,int val3)
{
	slot->type=(char)meaning;
	slot->m_data1=val1;
	slot->m_data2=val2;
	slot->m_data3=val3;
}

void SymbolSetMemSpace(hs* slot,int mem_space)
{
	slot->mem_space=(char)mem_space;
}

/* debugging aid - lists the symbol (hash) table */

void ListSymbolTable()
{
	int	a;

	for( a=0; a<HASH_SIZE; a++ )
	{
		if ( hash_tab[a].pHead != NULL )
		{
			hs* pNext = hash_tab[a].pHead;
			while ( pNext )
			{
				debugprint("%s\r\n",pNext->pString);
				pNext = pNext->pNext;
			}
		}
	}
}

// -----------------------------------------------------------------------------------------------

void SymSet(const char* pSymbol,Value val)
{
    hs* temp;
    dc_flag=FALSE;
    if((temp=FindSymbol(pSymbol))==0)
    {
        char	temp_str[512];
        snprintf(temp_str,512,"Undefined symbol: %s",pSymbol);
        yyerror(temp_str);
    }
    SymSetValue(temp,T_VALUE,val);
}

// -----------------------------------------------------------------------------------------------

hs* AddSym(stext* pSymName,int forceCopy)
{
    PASS1
        int curr_pc = pc;
		hs* temp;
        temp=AddSymbol(pSymName->ptr,pSymName->len,forceCopy);
        if( temp == NULL )
        {
			yyerror("Symbol defined twice: \"%s\"",pSymName->ptr);
            return NULL;
        }

		// correct L: memory address
        if ( GetCurrentMemType() == L_MEM )
        {
            curr_pc = GetCurrentChunkBegin() + ( ( pc - GetCurrentChunkBegin() ) >> 1 ) ;
        }

        SymSetValue( temp, T_PTR, Val_CreateInt(curr_pc) );
		return temp;
    PASS2
		return FindSymbol( pSymName->ptr );
    PASSEND


#ifdef _MSC_BUILD
        _ASSERT ( _CrtCheckMemory( ) );
#endif
}

// -----------------------------------------------------------------------------------------------

Value GetSym(const char* pSymbolName)
{
    hs* pSymbol=FindSymbol(pSymbolName);
    
    if ( pSymbol==0 )
    { 
        if ( g_passNum )
        {
		    yyerror("Undefined symbol: \"%s\"",pSymbolName);
        }    
    }
    else
    {
        return pSymbol->m_val;
    }

    return Val_CreateUnresolved();
}

// -----------------------------------------------------------------------------------------------

hs* AddLabel( stext* pSymName )
{   
    hs* ret = NULL;
    ResetLocalLabel(pSymName->ptr);
    ret = AddSym(pSymName, TopPosStream() > 0 );
    if ( ret )
    {	
        ret->mem_space = GetCurrentMemType();
    }
    return ret;
}