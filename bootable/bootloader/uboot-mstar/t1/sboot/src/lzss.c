/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
#ifdef __mips__
typedef unsigned char   U8;
typedef unsigned int   U16;
typedef unsigned long  U32;
#define NULL 0

#include "ms_decompress_priv.h"

#define RIU_MAP  0xBF000000
#define RIU      ((unsigned short volatile *) RIU_MAP)

#define BIT0        0x0001
#define BIT1        0x0002
#define BIT2        0x0004
#define BIT3        0x0008
#define BIT4        0x0010
#define BIT5        0x0020
#define BIT6        0x0040
#define BIT7        0x0080
#define BIT8        0x0100
#define BIT9        0x0200
#define BIT10       0x0400
#define BIT11       0x0800
#define BIT12       0x1000
#define BIT13       0x2000
#define BIT14       0x4000
#define BIT15       0x8000

#ifdef CONFIG_MSTAR_KAISER
#define AP_IN_FLASH_ADDR        0x94020000              //This address should be 4' alignment
#else
#define AP_IN_FLASH_ADDR    0xBFC20000        //This address should be 4' alignment
#endif

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

void DoLZSSDecompression(void);
void DoLZSSDecompression(void)
{
    U8 IsCompressMagicNumber = 0;
    U32 CompressedFileLength = 0;
    U32 u32ROM_START = *((U32 *)(AP_IN_FLASH_ADDR + 0));    //This address should be 4' alignment
    U32 i;

    // Check if this bin file is compressed format, find compress format magic number
    //*((U32 *)(0xbf206704)) = *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-4)+3));
    //*((U32 *)(0xbf206708)) = *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-4)+2));
    //*((U32 *)(0xbf20670C)) = *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-4)+1));

    if ( ( *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-4)+3)) == 0xDE )
    && ( *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-4)+2)) == 0xAD )
    && ( *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-4)+1)) == 0xBE ) )
        IsCompressMagicNumber = 1;
    else
        IsCompressMagicNumber = 0;

    if ( IsCompressMagicNumber )
    {
        U32 u32RAM_START, u32RAM_END;

        //Compressed Format
        u32RAM_START = *((U32 *)(AP_IN_FLASH_ADDR + 4));    //This address should be 4' alignment
        u32RAM_END = *((U32 *)(AP_IN_FLASH_ADDR + 8));

        //Copy uncompressed part first
        for(i=0;i<((UNCOMPRESSED_LENGTH + 7) & ~7UL);i+=4)
        {
            *((U32 *)(u32RAM_START+i)) = *((U32 *)((U32)(u32ROM_START+i)));
        }

        //Copy compressed part second
        for(i=0;i<(((u32RAM_END-u32RAM_START-UNCOMPRESSED_LENGTH) + 7) & ~7UL);i+=4)
        {
            *((U32 *)(DECOMPRESS_BUF + i)) = *((U32 *)(u32ROM_START + UNCOMPRESSED_LENGTH + i));
        }

        CompressedFileLength=*((U8 *)(u32RAM_START+COMPRESSED_LENGTH_POS+3));
        CompressedFileLength<<=8;
        CompressedFileLength|=*((U8 *)(u32RAM_START+COMPRESSED_LENGTH_POS+2));
        CompressedFileLength<<=8;
        CompressedFileLength|=*((U8 *)(u32RAM_START+COMPRESSED_LENGTH_POS+1));
        CompressedFileLength<<=8;
        CompressedFileLength|=*((U8 *)(u32RAM_START+COMPRESSED_LENGTH_POS+0));
        CompressedFileLength -= 2;

        // Start to decompress
        //if ( IsCompressMagicNumber )
        {

    #if 0
            static       U32 _u32BitPos = 0;
            static       U32 u32NextChar = 0;
            static       U8 *pOutStream;
            static       U32 _u32Offset = 0;
            static       U8 *pSlidingWindow;

            register      U32 i ;
            register      U32 offset,length;
            register      U32 mask=0xFFF;
            register      U32 u32BitPos = 0;
            register      U8 *pStreamEnd = NULL;
            register      U32 tmp0,tmp1;
    #else
            U32 _u32BitPos = 0;
            U32 u32NextChar = 0;
            U8 *pOutStream;
            U32 _u32Offset = 0;
            U8 *pSlidingWindow;

            //U32 i ;
            U32 offset,length;
            U32 mask=0xFFF;
            U32 u32BitPos = 0;
            U8 *pStreamEnd = NULL;
            U32 tmp0,tmp1;
    #endif

            U8 *pInStream;
            U32 u32DataLen;
            pSlidingWindow = (U8 *)(DECOMPRESS_BUF-WINDOW_SIZE);

            for(i=0;i<WINDOW_SIZE;i++)
            {
                *(pSlidingWindow+i)=0;
            }

            _u32BitPos = 8;
            u32NextChar = 1;
            _u32Offset = 0;

            {
                //pInStream = (U8 *)(DECOMPRESS_BUF+UNCOMPRESSED_LENGTH);
                pInStream = (U8 *)(DECOMPRESS_BUF);
                pOutStream = (U8 *)(u32RAM_START + UNCOMPRESSED_LENGTH);
                u32DataLen = CompressedFileLength - UNCOMPRESSED_LENGTH;

                u32BitPos = _u32BitPos;
                pInStream = pInStream;
                pStreamEnd = pInStream + u32DataLen;

                pInStream += _u32Offset;

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
            //We don't use these two variables any more, so don't care their values at this time
            //        _u32BitPos = u32BitPos;
            //        _u32Offset = pInStream - pStreamEnd;
            //DeCompressedLength = pOutStream - pOutHead - sizeof(U32);
            //return (pOutStream - pOutHead);
        }
    }
    else
    {
        // Uncompressed format
        asm volatile (
        // Load ROM_START, RAM_START, RAM_END, ROM_END from 0xBFC20000
        #if defined( CONFIG_MSTAR_KAISERIN ) || defined( CONFIG_MSTAR_KAISER )
            "li      $15, 0x94020000;"
        #else
            "li      $15, 0xbfc20000;"
        #endif
            "lw      $8, 0($15);"
            "lw      $9, 4($15);"
            "lw      $10, 8($15);"
            "lw      $11, 12($15);"

        "1:;"
            "ld      $12, 0($8);"      //#ld=lw lw to save time
            "ld      $14, 8($8);"
            "sd      $12, 0($9);"
            "sd      $14, 8($9);"
            "addu    $8, $8, 16;"
            "addu    $9, $9, 16;"
            "bltu    $9, $10, 1b;"
            "nop;"
            :::"$15", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "memory"
        );
    }
}
#endif
