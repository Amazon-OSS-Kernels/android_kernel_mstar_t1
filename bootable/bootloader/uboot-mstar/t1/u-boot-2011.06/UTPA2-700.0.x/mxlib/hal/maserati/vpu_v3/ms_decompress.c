/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//******************************************************************************
//
//
//
//
//
//
//
//
//
//******************************************************************************
////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdlib.h>
#include <string.h>
#else
#include <linux/string.h>
#endif

#include "MsCommon.h"
//#include "datatype.h"
#include "ms_decompress.h"
#include "ms_decompress_priv.h"

#define _ReadChar(A)\
{\
    A=(*pInStream << (8 - u32BitPos))|(*(pInStream+1)>>u32BitPos);\
    pInStream++;\
}

#define _ReadBit(A)\
{\
    u32BitPos--;\
    A = (*pInStream >> u32BitPos) & 0x01;\
    if(u32BitPos == 0)\
    {\
        u32BitPos = 8;\
        pInStream++;\
    }\
}
static MS_U8 *pSlidingWindow;
static MS_U32 _u32BitPos = 0;
static MS_U32 u32NextChar = 0;
static MS_U8 *pOutStream;
static MS_U32 _u32Offset = 0;

MS_BOOL ms_VDECDecompressInit(MS_U8 *pSliding, MS_U8 *pOut)
{
//    pSlidingWindow = malloc(WINDOW_SIZE);
    pSlidingWindow = pSliding;
    memset(pSlidingWindow, 0, WINDOW_SIZE);
    _u32BitPos = 8;
    u32NextChar = 1;
    pOutStream = pOut;
    _u32Offset = 0;
    return TRUE;
}

void ms_VDECDecompressDeInit(void)
{
//    free(pSlidingWindow);
}

int ms_VDECDecompress(const MS_U8 *pInStream, MS_U32 u32DataLen)
{
    register MS_U32 i ;
    register MS_U32 offset,length;
    register MS_U32 mask=0xFFF;
    register MS_U32 u32BitPos = 0;
    register const MS_U8 *pStreamEnd = NULL;
    register MS_U32 tmp0,tmp1;
    const MS_U8 *pOutHead = pOutStream;

    //pSlidingWindow = malloc(WINDOW_SIZE);
    if (pSlidingWindow)
    {

        u32BitPos = _u32BitPos;
        pStreamEnd = pInStream + u32DataLen;

        pInStream += _u32Offset;
        //pInStream = pInStream;
        //printf("I-pOutStream = %d \n",pOutStream);

        while (pInStream <= pStreamEnd)
        {
            _ReadBit(tmp0);
            if (tmp0 != UNCODED)
            {
                _ReadChar(tmp0);
                _ReadChar(tmp1);
                offset = (tmp0 << 8) | tmp1;
                length = (offset & 0x0F) + 2;
                offset >>= 4;
                if ((u32NextChar+length) < WINDOW_SIZE)
                {
                    for (i = 0; i < length; i++)
                    {
                        *pOutStream = pSlidingWindow[(offset+i) & mask];
                        pSlidingWindow[(u32NextChar+i)] = *pOutStream++;
                    }
                }
                else
                {
                    for (i = 0; i < length; i++)
                    {
                        *pOutStream = pSlidingWindow[(offset+i) & mask];
                        pSlidingWindow[(u32NextChar+i) & mask] = *pOutStream++;
                    }
                }
                u32NextChar+=length;
            }
            else
            {
                _ReadChar(tmp0);
                *pOutStream = tmp0;
                pSlidingWindow[u32NextChar] = *pOutStream++;
                u32NextChar++;
            }
            u32NextChar = u32NextChar & mask;
        }

    }
    _u32BitPos = u32BitPos;
    _u32Offset = pInStream - pStreamEnd;
    return (pOutStream - pOutHead);
}

//*************************************************************************************************

