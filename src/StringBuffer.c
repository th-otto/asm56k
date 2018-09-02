/*

Project:    asm56k
Author:     M.Buras (sqward)


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "StringBuffer.h"
#include <export.h>

char* pStrBufPtr        = 0;
char* pStrBufCurrent    = 0;
int  StrBufBlockSize   = 0;

int StringBufferInit(int bufferSize)
{
    pStrBufPtr = (char*)malloc(bufferSize);
    if (pStrBufPtr)
    {
        pStrBufCurrent = pStrBufPtr;
        StrBufBlockSize = bufferSize;
        return 0;
    }
    return 1; /* error */
}

/* TODO: deduplication */

const char* StringBufferInsert(char* pStr)
{
    int len = strlen( pStr );
    const char* pRetString;

    if ( ( pStrBufPtr + StrBufBlockSize ) < ( pStrBufCurrent + len + 1 ) )
    {
        pStrBufPtr = (char*)malloc(StrBufBlockSize);
		if (!pStrBufPtr)
		{
			return 0;
		}
		pStrBufCurrent = pStrBufPtr;
	}
	pRetString = pStrBufCurrent;
	memcpy(pStrBufCurrent,pStr,len + 1 );
	pStrBufCurrent += len + 1;

#ifdef _MSC_BUILD
    _ASSERT ( _CrtCheckMemory( ) );
#endif

	return pRetString;
}
