/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#if (ENABLE_NON_OS)

//---------------------------------------------------------------------------
// Fix undefined warning
#ifndef ENABLE_MSTAR_MARIA10
    #define ENABLE_MSTAR_MARIA10    0
#endif
#ifndef ENABLE_MSTAR_MACAW12
    #define ENABLE_MSTAR_MACAW12    0
#endif
#ifndef ENABLE_MSTAR_EDEN
    #define ENABLE_MSTAR_EDEN       0
#endif
#ifndef ENABLE_MSTAR_EULER
    #define ENABLE_MSTAR_EULER      0
#endif
#ifndef ENABLE_MSTAR_NASA
    #define ENABLE_MSTAR_NASA       0
#endif

#ifndef ENABLE_MSTAR_EMERALD
    #define ENABLE_MSTAR_EMERALD    0
#endif

//---------------------------------------------------------------------------
#include "zlib.h"
#include "MSDecompress_NonOS_priv.h"
//#include "version.h"

//---------------------------------------------------------------------------
#if (ENABLE_MSTAR_MARIA10)
    #include "maria10/hwreg.h"
#elif (ENABLE_MSTAR_MACAW12)
    #include "macaw12/hwreg.h"
#elif (ENABLE_MSTAR_EDEN)
    #include "eden/hwreg.h"
#elif (ENABLE_MSTAR_MAYA)
    #include "maya/hwreg.h"
#elif (ENABLE_MSTAR_EULER)
    #include "euler/hwreg.h"
#elif (ENABLE_MSTAR_WHISKY)
    #include "whisky/hwreg.h"
#elif (ENABLE_MSTAR_MELODY)
    #include "melody/hwreg.h"
#elif (ENABLE_MSTAR_NASA)
    #include "nasa/hwreg.h"

#elif (ENABLE_MSTAR_MILAN)
    #include "milan/include/hwreg_MILAN.h"

#else

#endif

//---------------------------------------------------------------------------
// Type define
typedef unsigned char   U8;
typedef unsigned short  U16;
typedef unsigned int    U32;

/// data type unsigned char, data length 1 byte
typedef unsigned char               MS_U8;                              // 1 byte
/// data type unsigned short, data length 2 byte
typedef unsigned short              MS_U16;                             // 2 bytes
/// data type unsigned int, data length 4 byte
typedef unsigned long               MS_U32;                             // 4 bytes

#ifndef BOOL
typedef unsigned int BOOL;
#endif

/// defination for FALSE
#define FALSE           0
/// defination for TRUE
#define TRUE            1

#define NULL    0

#define BIT0    (0x0001)
#define BIT1    (0x0002)
#define BIT2    (0x0004)
#define BIT3    (0x0008)
#define BIT4    (0x0010)
#define BIT5    (0x0020)
#define BIT6    (0x0040)
#define BIT7    (0x0080)

//---------------------------------------------------------------------------

#define ENABLE_PRINTF   1

#if( ENABLE_PRINTF )

    #if defined(__aeon__)// AEON_R2

        #if 0
            #define ENABLE_AEON_UART_PRINTF     1

        #elif( ENABLE_MSTAR_WHISKY )
            #define ENABLE_LOADER2_AEON_UART    1
            #define ENABLE_SIMPLE_PRINTF        1
        #endif

    #else // MIPS

      #if ( ENABLE_MSTAR_MILAN ) // Milan use PIU Uart
        #define ENABLE_SIMPLE_PRINTF  1
      #endif

    #endif

#endif


#ifndef ENABLE_AEON_UART_PRINTF // Use system uart driver & printf()
    #define ENABLE_AEON_UART_PRINTF    0
#endif

#ifndef ENABLE_LOADER2_AEON_UART // Use ld_uart driver
    #define ENABLE_LOADER2_AEON_UART    0
#endif

#ifndef ENABLE_SIMPLE_PRINTF // Enable uart_printf
    #define ENABLE_SIMPLE_PRINTF  0
#endif

//---------------------------------------------------------------------------

#if 1//(ENABLE_SBOOT_USE_BDMA_TO_CHECK_CRC)
#define USE_BDMA_FOR_CRC_CHECK 1
#endif

#define UBOOT_IDENTIFIER    0x0000B007

//---------------------------------------------------------------------------

#if ENABLE_MSTAR_WHISKY
    #define ENABLE_JOBA_BIN 1
#else
    #define ENABLE_JOBA_BIN 0
#endif

#if(ENABLE_JOBA_BIN)
    #define JOBA_MMAP_CODE_ADDR     0x010000
    //#define JOBA_MMAP_DEBUG_ADDR    0x0F0000
    #define JOBA_PARA_MAGIC_ID      0x985598AA
#endif

//---------------------------------------------------------------------------

#if defined(__mips__)
    #define RIU_MAP 0xBF200000
    #define LOCAL_BDMA_REG_BASE 0x900
#elif defined(__aeon__)
    #define RIU_MAP 0xA0000000
#elif defined(__arm__) || defined(__arm64__)
    #define RIU_MAP 0x1F000000
    #define LOCAL_BDMA_REG_BASE    0x900
    #define USE_BDMA_MOVE_CODE 1
#else
    #error not define
#endif

#if( defined(__aeon__) || defined(__mips__) )
    #define RIU     ((unsigned short volatile *) RIU_MAP)
    #define RIU8    ((unsigned char  volatile *) RIU_MAP)
#endif

//---------------------------------------------------------------------------

#ifdef __mips__
    #if (ENABLE_BOOTING_FROM_OTP_WITH_PM51==1)
        #define AP_IN_FLASH_ADDR    0xB4030000      //This address should be 4' alignment
    #else
            #ifdef CONFIG_MSTAR_KAISERIN
                #define AP_IN_FLASH_ADDR    0x94020000      //This address should be 4' alignment
            #else
              #if defined(CONFIG_PM_SIZE_KB_FORCED) && (CONFIG_PM_SIZE_KB_FORCED!=0)
                #define AP_IN_FLASH_ADDR    (0xBFC10000+(CONFIG_PM_SIZE_KB_FORCED*0x400))      //This address should be 4' alignment
              #else
			#if defined(CONFIG_MSTAR_RT_PM_IN_SPI)
				#define AP_IN_FLASH_ADDR    0xBFC30000      //This address should be 4' alignment
			#else
				#if defined(CONFIG_ENABLE_BOOTING_FROM_ROM_WITH_COMPRESS_UBOOT)
					#define AP_IN_FLASH_ADDR    0xA51F0180//CONFIG_UBOOT_LOADADDR//0xBFC20000      //This address should be 4' alignment
					#define AP_IN_FLASH_ADDR_CACHE    0x851F0180//CONFIG_UBOOT_LOADADDR//0xBFC20000      //This address should be 4' alignment
				#else
                        #if defined(CONFIG_DISABLE_PM_PARTITION)
                            #define AP_IN_FLASH_ADDR    0xBFC10000      //This address should be 4' alignment
                        #else
                            #define AP_IN_FLASH_ADDR    0xBFC20000
                        #endif
				#endif
                #endif
              #endif
            #endif
    #endif

#elif( defined(__aeon__) ) // Aeon

    #define AP_IN_FLASH_ADDR    0x00020000      //This address should be 4' alignment
#endif

//====================================================================================

#if defined(__mips__) || defined(__arm__) || defined(__arm64__)
    #define __loader_2nd
    #define __loader_2nd_bss
#else
    #define __loader_2nd     __attribute__ ((__section__ (".loader_2nd.text")))
    #define __loader_2nd_bss __attribute__ ((__section__ (".loader_2nd.bss")))
#endif

//====================================================================================

unsigned char __loader_2nd_bss *pDecompressionBuf = NULL;
unsigned char __loader_2nd_bss *pEndofDecompressionBuf = NULL;
unsigned char __loader_2nd_bss *pCurrentBufPtr = NULL;

void MemoryAllocatorInit(unsigned char *ptr, unsigned long nSize);
extern void __BDMA_FlashCopy2MIU0(U32 FlashPA, U32 DRAMPA, U32 Len);
extern void mhal_dcache_flush(U32 u32Base, U32 u32Size);
void *zalloc(void *ptr, unsigned long nNum, unsigned long nSize);
void zfree(void *x, void *address, unsigned nbytes);
U8 MsDecompress(U8 *pSrc, U8 *pDst, U32 srclen, U32 dstlen, U32 *pDecompressionLength);
void DoMSDecompression(void);

//--------------------------------------------------------------------------------------------

//================================================================================

/*void init_version(void);

void __loader_2nd init_version(void)
{
    //Init version
    static U8 tmpVar;
    tmpVar = _sbt_version.APP.tag[0];
}
*/

void __loader_2nd MemoryAllocatorInit(unsigned char *ptr, unsigned long nSize)
{
   pDecompressionBuf = ptr;
   pEndofDecompressionBuf = pDecompressionBuf + nSize*sizeof(unsigned char);
   pCurrentBufPtr = pDecompressionBuf;
}

void __loader_2nd *zalloc(void *ptr, unsigned long nNum, unsigned long nSize)
{
// Because C language use call by value
// so if we modify ptr value, it will not afftect the caller value
// Considering decompression performance, we don't do memory pool
// region check here
#if 0
    nSize = ( nSize*nNum + 3 ) & ~3;

    ptr = (void *)pCurrentBufPtr;
    pCurrentBufPtr += nSize;

    if (pCurrentBufPtr > pEndofDecompressionBuf)
    {
        printf("Memory Allocate Fail\n");
        ptr = NULL;
    }
#else
    ptr = (void *)pCurrentBufPtr;
    // 4' alignment
    pCurrentBufPtr += ( nSize*nNum + 3 ) & ~3;
#endif
    return ptr;
}

void __loader_2nd zfree(void *x, void *address, unsigned nbytes)
{
    // Considering decompression performance, we don't
    // do memory free operation here
    // This will cause a lot of memory usage, but I think we can
    // afford this, because our target files are not so big
    x = x;
    address = address;
    nbytes = nbytes;
}

/*inline void _HaltCPU(void)
{
    //halt cpu
    __asm__ __volatile__(
        "\tSDBBP");
}*/

U8 __loader_2nd MsDecompress(U8 *pSrc, U8 *pDst, U32 srclen, U32 dstlen, U32 *pDecompressionLength)
{
    z_stream z;
    z.opaque=NULL;
    z.zalloc = (alloc_func)zalloc;
    z.zfree = zfree;
    z.next_in = pSrc;
    z.avail_in = srclen;
    z.next_out = pDst;
    z.avail_out = dstlen;

    if ( inflateInit2(&z, -MAX_WBITS) != Z_OK )
    {
        //printf("Z Error:%s\n", z.msg);
        inflateEnd(&z);
        return 1;
    }

    if ( inflate(&z, Z_FINISH) < Z_OK )
    {
        //printf("Z Error:%s\n", z.msg);
        inflateEnd(&z);
        return 1;
    }

    *pDecompressionLength = z.total_out;

    inflateEnd(&z);

    return 0;
}

/////////////////////////////////////////////////////////////////////////////
/* 2009-08-14 : Igor Pavlov : Public domain */

#include "7types.h"
#include "7alloc.h"
#include "lzmadec.h"

__loader_2nd void *SzAlloc(void *p, size_t size);
__loader_2nd void SzFree(void *p, void *address);
U8 __loader_2nd MsDecompress7(U8 *pSrc, U8 *pDst, U32 srclen);

__loader_2nd void *SzAlloc(void *p, size_t size) { p = p; return MyAlloc(size); }
__loader_2nd void SzFree(void *p, void *address) { p = p; MyFree(address); }
__loader_2nd_bss ISzAlloc g_Alloc = { SzAlloc, SzFree };
/////////////////////////////////////////////////////////////////////////////

#if (ENABLE_BOOTING_FROM_EXT_EMMC_WITH_CPU == 0)
U8 __loader_2nd MsDecompress7(U8 *pSrc, U8 *pDst, U32 srclen)
{

    U32 OriginalFileLength = 0;
    int result = SZ_OK;
    U8 i;
    ELzmaStatus estatus;
    SizeT destLen, srcLen;

    #define LZMA_PROPS_SIZE 5
    estatus=LZMA_STATUS_NOT_SPECIFIED;

    /* header: 5 bytes of LZMA properties and 8 bytes of uncompressed size */
    srclen -= (LZMA_PROPS_SIZE + 8);

    // It seems that in our system, we never have a chance to have a compressed data which needs to use U64 integer to store its file size
    // So we use just U32 here to instead of U64
    /* Read and parse header */
    for (i = 0; i < 8; i++)
        OriginalFileLength += ( (U32)pSrc[LZMA_PROPS_SIZE + i] << (i << 3) );

    destLen = OriginalFileLength;
    srcLen = srclen;

    result = LzmaDecode(pDst, &destLen, pSrc + (LZMA_PROPS_SIZE + 8), &srcLen, pSrc, LZMA_PROPS_SIZE, LZMA_FINISH_ANY, &estatus, &g_Alloc);
    switch( estatus )
    {
    case LZMA_STATUS_FINISHED_WITH_MARK:
    case LZMA_STATUS_NOT_FINISHED:
    case LZMA_STATUS_MAYBE_FINISHED_WITHOUT_MARK:
        break;
    default:
    //LZMA_STATUS_NOT_SPECIFIED,               /* use main error code instead */
    //LZMA_STATUS_NEEDS_MORE_INPUT,            /* you must provide more input bytes */
        //printf("ELzmaStatus = %d\n", estatus);
        //Decompression fail!
        return FALSE;
    }

    if (result != SZ_OK)
    {
    //  SZ_ERROR_DATA - Data error
    //  SZ_ERROR_MEM  - Memory allocation error
    //  SZ_ERROR_UNSUPPORTED - Unsupported properties
    //  SZ_ERROR_INPUT_EOF - It needs more bytes in input buffer (src).
    //#define SZ_ERROR_DATA 1
    //#define SZ_ERROR_MEM 2
    //#define SZ_ERROR_CRC 3
    //#define SZ_ERROR_UNSUPPORTED 4
    //#define SZ_ERROR_PARAM 5
    //#define SZ_ERROR_INPUT_EOF 6
    //#define SZ_ERROR_OUTPUT_EOF 7
    //#define SZ_ERROR_READ 8
    //#define SZ_ERROR_WRITE 9
    //#define SZ_ERROR_PROGRESS 10
    //#define SZ_ERROR_FAIL 11
    //#define SZ_ERROR_THREAD 12
    //#define SZ_ERROR_ARCHIVE 16
    //#define SZ_ERROR_NO_ARCHIVE 17
        //printf("error code:%d\n", result);
        //Decompression fail!
        return FALSE;
    }

    if ( destLen != OriginalFileLength )
    {
        //Decompression fail!
        return FALSE;
    }

    if ( srcLen != srclen )
    {
        //Decompression fail!
        return FALSE;
    }
    //Decompression OK!


    return TRUE;
}
#endif //#if (ENABLE_BOOTING_FROM_EXT_EMMC_WITH_CPU == 0)

#if( defined(__mips__) || defined(__aeon__) ) // Non-OS - Milan

U8 __loader_2nd msReadByte( U32 u32Reg );
U8 __loader_2nd msReadByte( U32 u32Reg )
{
#if( defined(__aeon__) )
    return RIU8[(u32Reg << 1) - (u32Reg & 1)];
#endif

#if( defined(__mips__) )
    return ((unsigned char volatile *)0xBF000000)[(u32Reg << 1) - (u32Reg & 1)];
#endif
}

U16 __loader_2nd msRead2Bytes( U32 u32Reg );
U16 __loader_2nd msRead2Bytes( U32 u32Reg )
{
    if( (u32Reg & 0x01) )
    {
        u32Reg <<= 1;
        return RIU8[u32Reg - 1] + (RIU8[u32Reg + 2] << 8);
    }
    else
    {
        return RIU[u32Reg];
    }
}

void __loader_2nd msWriteByte( U32 u32Reg, U8 u8Value );
void __loader_2nd msWriteByte( U32 u32Reg, U8 u8Value )
{
#if( defined(__aeon__) )
    RIU8[(u32Reg << 1) - (u32Reg & 1)] = u8Value;
#endif

#if( defined(__mips__) )
    ((unsigned char volatile *)0xBF000000)[(u32Reg << 1) - (u32Reg & 1)] = u8Value;
#endif
}

void __loader_2nd msWrite2Bytes( U32 u32Reg, U16 u16Value );
void __loader_2nd msWrite2Bytes( U32 u32Reg, U16 u16Value )
{
    if( ((u32Reg) & 0x01) )
    {
        RIU8[((u32Reg) * 2) - 1] = (U8)((u16Value));                                  \
        RIU8[((u32Reg) + 1) * 2] = (U8)((u16Value) >> 8);                             \
    }
    else
    {
        RIU[u32Reg] = u16Value;                                                       \
    }
}

#endif

//=================================================================================

#if defined(__aeon__)// AEON_R2
    #include "MSDecompress_NonOS_AeonUart.c"
#endif // Aeon

#include "MSDecompress_NonOS_driver.c"

//=================================================================================

#ifdef __AEONR2__
#if 0//(ENABLE_MSTAR_MARIA10 || ENABLE_MSTAR_MACAW12 || ENABLE_MSTAR_EDEN || ENABLE_MSTAR_EULER || ENABLE_MSTAR_MAYA || ENABLE_MSTAR_NASA || ENABLE_MSTAR_WHISKY  || ENABLE_MSTAR_MELODY)
#if(ENABLE_MSTAR_EULER || ENABLE_MSTAR_MAYA)
#if defined(CONFIG_MSTAR_EULER_MMAP_128MB)
#define _DRAM_RESET_VECTOR_BASE_ADDR  0x400000
#else
#define _DRAM_RESET_VECTOR_BASE_ADDR  0x200000
#endif
#else
#define _DRAM_RESET_VECTOR_BASE_ADDR  0x200000
#endif
void __loader_2nd Boot2DRAM(void);
void __loader_2nd Boot2DRAM(void)
{
    U32 u32Addr = _DRAM_RESET_VECTOR_BASE_ADDR;

    //set up reset vector base

    RIU[0x1002B4] = (U16)(u32Addr >> 16);
    RIU[0x002E54] = (U16)(0x829f);  // set wtd_mcu_rst passwd
    RIU[0x002E52] &= (~0x0100); // non-fire wtd_mcu_rst
    //wdt reset to dram: bellow 2 lines must in the same cache line 256bytes
    RIU[0x100280] |= (0x002F); // set sdr_boot=1, release rstz_miu
    RIU[0x002E52] |= (0x0100); // fire wtd_mcu_rst

    //########################################
    //#
    //# Due to the fact that __loader_2nd is a specified section
    //# Have to avoid to use some api functions out of this section
    //#
    //########################################
    //MDrv_Write2Byte(0x1002B4, (U16)(u32Addr >> 16));
    //MDrv_Write2Byte(0x002E54, 0x829f);  // set wtd_mcu_rst passwd
    //MDrv_Write2Byte(0x002E52, MDrv_Read2Byte(0x002E52)&(~BIT8));  // non-fire wtd_mcu_rst
    //wdt reset to dram: bellow 2 lines must in the same cache line 256bytes
    //MDrv_Write2Byte(0x100280, MDrv_Read2Byte(0x100280)|(0x002F));  // set sdr_boot=1, release rstz_miu
    //MDrv_Write2Byte(0x002E52, MDrv_Read2Byte(0x002E52)|(BIT8));  // fire wtd_mcu_rst
}
#endif

void __loader_2nd ReBootAtDRAM(U32 u32MemAddr);
void __loader_2nd ReBootAtDRAM(U32 u32MemAddr)
{
    //U32 u32Addr = _DRAM_RESET_VECTOR_BASE_ADDR;

    //set up reset vector base

    RIU[0x1002B4] = (U16)(u32MemAddr >> 16);
    RIU[0x002E54] = (U16)(0x829f);  // set wtd_mcu_rst passwd
    RIU[0x002E52] &= (~0x0100); // non-fire wtd_mcu_rst
    //wdt reset to dram: bellow 2 lines must in the same cache line 256bytes
    RIU[0x100280] |= (0x002F); // set sdr_boot=1, release rstz_miu
    RIU[0x002E52] |= (0x0100); // fire wtd_mcu_rst

    //########################################
    //#
    //# Due to the fact that __loader_2nd is a specified section
    //# Have to avoid to use some api functions out of this section
    //#
    //########################################
    //MDrv_Write2Byte(0x1002B4, (U16)(u32Addr >> 16));
    //MDrv_Write2Byte(0x002E54, 0x829f);  // set wtd_mcu_rst passwd
    //MDrv_Write2Byte(0x002E52, MDrv_Read2Byte(0x002E52)&(~BIT8));  // non-fire wtd_mcu_rst
    //wdt reset to dram: bellow 2 lines must in the same cache line 256bytes
    //MDrv_Write2Byte(0x100280, MDrv_Read2Byte(0x100280)|(0x002F));  // set sdr_boot=1, release rstz_miu
    //MDrv_Write2Byte(0x002E52, MDrv_Read2Byte(0x002E52)|(BIT8));  // fire wtd_mcu_rst
}

#endif // __AEONR2__

//===============================================================================

#if defined(__mips__) || defined(__arm__) || defined(__arm64__)

void __loader_2nd uart_putc(char ch);

#if ( ENABLE_MSTAR_MILAN ) // Milan use PIU Uart
#if 1//(PIU_UART_VER == 2)
#define UART_RX        (0 * 2)  // In:  Receive buffer (DLAB=0)
#define UART_TX        (0 * 2)  // Out: Transmit buffer (DLAB=0)
#define UART_DLL       (0 * 2)  // Out: Divisor Latch Low (DLAB=1)
#define UART_DLM       (1 * 2)  // Out: Divisor Latch High (DLAB=1)
#define UART_IER       (1 * 2)  // Out: Interrupt Enable Register
#define UART_IIR       (2 * 2)  // In:  Interrupt ID Register
#define UART_FCR       (2 * 2)  // Out: FIFO Control Register
#define UART_LCR       (3 * 2)  // Out: Line Control Register
#define UART_MCR       (4 * 2)	// Out: Modem Control Register
#define UART_LSR       (5 * 2)  // In:  Line Status Register
#define UART_MSR       (6 * 2)  // In:  Modem Status Register
#define UART_USR       (7 * 2)  // Out: USER Status Register
#endif
#define UART_LSR_THRE               0x20          // Transmit-hold-register empty
void __loader_2nd uart_putc(char ch)
{
    U32 timeout_count = 0;

    //while ( (!( UART_REG8(UART_LSR) & UART_LSR_THRE)) && (timeout_count++ < 2000) );
    while ( (!( (*(volatile U8*)(0xbf201300+UART_LSR)) & UART_LSR_THRE)) && (timeout_count++ < 2000) );

    //UART_REG8(UART_TX) = *buf++;
    *(volatile U8*)(0xbf201300+UART_TX) = ch;
}

#else
void __loader_2nd uart_putc(char ch)
{
  #if defined(__arm__) || defined(__arm64__)
    *(volatile U32*)(0x1f201300) = ch;
  #else
    *(volatile U32*)(0xbf201300) = ch;
  #endif
}
#endif // #if ( ENABLE_MSTAR_MILAN)
#endif // defined(__mips__) || defined(__arm__) || defined(__arm64__)


//========================================================

#if ( ENABLE_SIMPLE_PRINTF )

#if defined(__aeon__)
  #if ENABLE_LOADER2_AEON_UART
    #define LOADER2_UART_PUTC   loader2_Aeon_uart_putc
  #else
    #define LOADER2_UART_PUTC   uart_putc
  #endif
#else
    #define LOADER2_UART_PUTC   uart_putc
#endif

#define PRINT_FLAG_HEX  0x01//_BIT0

void __loader_2nd DWordToStr(U32 dwVal, U8* acStrBuf, U8 ucFlag );
void __loader_2nd DWordToStr(U32 dwVal, U8* acStrBuf, U8 ucFlag )
{
    U8 i;
    char c;
    BOOL bHex = 0;
    U32 dwDivider;
    BOOL bNotZero = 0;


    if( dwVal == 0 )
    {
        acStrBuf[0] = '0';
        acStrBuf[1] = 0;
        return;
    }

    if( ucFlag&PRINT_FLAG_HEX ) // 16¶i¨î
        bHex = TRUE;

    i = 0;
    if( bHex )
        dwDivider = 0x10000000;
    else
        dwDivider = 1000000000;

    for( ; dwDivider; )
    {
        c = dwVal/dwDivider;
        if( c )
            bNotZero = 1;

        if( bNotZero )
        {
            if( c > 9 )
                acStrBuf[i] = c - 10 + 'A';
            else
                acStrBuf[i] = c + '0';
            ++ i;
        }
        dwVal = dwVal%dwDivider;

        if( bHex )
            dwDivider /= 0x10;
        else
            dwDivider /= 10;
    }
    acStrBuf[i] = 0;

}

void __loader_2nd uart_printU32(U32 dwVal, U8 ucFlag);
void __loader_2nd uart_printU32(U32 dwVal, U8 ucFlag)
{
    U8 acStrBuf[11];
    U8 i;

    DWordToStr( dwVal, acStrBuf, ucFlag );
    for( i = 0; acStrBuf[i]!=0; ++ i )
    {
        LOADER2_UART_PUTC(acStrBuf[i]);
    }
}

void __loader_2nd uart_printf(const char *pFmt, U32 dwVal);
void __loader_2nd uart_printf(const char *pFmt, U32 dwVal)
{
    U8 ucBff;

    //if( 0 == g_UART_bEnablePrint )
    //    return;

#if 0//defined(__aeon__)
    LOADER2_UART_PUTC('f');
    uart_printU32((U32)pFmt, PRINT_FLAG_HEX);
    LOADER2_UART_PUTC(' ');
#endif


    while( 1 )
    {
    // decompress is running in dram, but string is not in dram!!
    // String is in flash!
    #if defined(__aeon__)
        ucBff = Ld_MDrv_Flash_ReadU8((U32)pFmt);
        pFmt += 1;
    #else
        ucBff = *(pFmt++);
    #endif

        if( ucBff == 0 )
            break;

        if( ucBff == '%' ) // check special case
        {
        #if defined(__aeon__)
            ucBff = Ld_MDrv_Flash_ReadU8((U32)pFmt);
            pFmt += 1;
        #else
            ucBff = *(pFmt++);
        #endif

            if( ucBff == 0 )
                break;

            switch( ucBff ) // check next character
            {
                case 'x': // hexadecimal number
                case 'X':
                    //LOADER2_UART_PUTC('0');
                    //LOADER2_UART_PUTC('x');
                    uart_printU32(dwVal, PRINT_FLAG_HEX);
                    break;
                case 'd': // decimal number
                case 'i':
                case 'u':
                //case 'U':
                    uart_printU32(dwVal,0);
                    break;

                case 'U':
                    uart_printU32(dwVal,0);
                    //putstr("(0x");
                    LOADER2_UART_PUTC('(');
                    LOADER2_UART_PUTC('0');
                    LOADER2_UART_PUTC('x');
                    uart_printU32(dwVal, PRINT_FLAG_HEX);
                    LOADER2_UART_PUTC(')');
                    break;
            } // switch
        }
        else if( ucBff == '\r' )// general
        {}
        else if( ucBff == '\n' )
        {
            LOADER2_UART_PUTC('\r'); // put a character
            LOADER2_UART_PUTC('\n'); // put a character
        }
        else // general
        {
            LOADER2_UART_PUTC(ucBff); // put a character
        }
    } // while}
}
#else

void __loader_2nd uart_printf(const char *pFmt, U32 dwVal);
void __loader_2nd uart_printf(const char *pFmt, U32 dwVal)
{
    pFmt=pFmt;
    dwVal=dwVal;
}

#endif // #if( ENABLE_SIMPLE_PRINTF )


#if( defined(__aeon__) && ENABLE_AEON_UART_PRINTF )
extern void MDrv_InitConsole( void );
extern void uart_putc(char c);
extern int printf(const char *fmt, ...);
/*
void __loader_2nd Ld_puts(const char *s);
void __loader_2nd Ld_puts(const char *s)
{
    char cTmp;

    for (; (cTmp = *s); s++)
    {
        if( cTmp == '\r' )
        {
        }
        else if( cTmp == '\n' )
        {
            uart_putc('\r');
            uart_putc('\n');
        }
        else
        {
            uart_putc(cTmp);
        }
    }
}*/

    #define Ld_printf(...)    printf(__VA_ARGS__)
    #define Ld_puts(str)      printf(str)

#elif ( ENABLE_SIMPLE_PRINTF )//( ENABLE_LOADER2_AEON_UART )
    #define Ld_printf(...)    uart_printf(__VA_ARGS__)
    #define Ld_puts(str)      uart_printf(str, 0)

#else
    #define Ld_printf(...)
    #define Ld_puts(str)
#endif

//========================================================
typedef struct
{
    U32 u32RomStart;
    U32 u32RomEnd;
    //U32 u32RomSize;

    U32 u32RamStart;
    U32 u32RamEnd;
    //U32 u32RamSize;

    U32 u32CRC;

    U32 u32BootLoaderSize;
    //U32 u32BinaryId;

} StuChunkHeaderInfo;

#if defined(__aeon__)
void __loader_2nd Loader_Get_ChunkHeader(U32 u32FlashAddr, StuChunkHeaderInfo* pstChunkHeaderInfo);
void __loader_2nd Loader_Get_ChunkHeader(U32 u32FlashAddr, StuChunkHeaderInfo* pstChunkHeaderInfo)
{
    pstChunkHeaderInfo->u32RomStart = Ld_MDrv_Flash_ReadU32(u32FlashAddr + 0 ); //This address should be 4' alignment
    pstChunkHeaderInfo->u32RomEnd = Ld_MDrv_Flash_ReadU32( u32FlashAddr + 12 ); //This address should be 4' alignment
    //pstChunkHeaderInfo->u32RomSize = pstChunkHeaderInfo->u32RomEnd - pstChunkHeaderInfo->u32RomStart + 1;

    pstChunkHeaderInfo->u32RamStart = Ld_MDrv_Flash_ReadU32(u32FlashAddr + 4); //This address should be 4' alignment
    pstChunkHeaderInfo->u32RamEnd = Ld_MDrv_Flash_ReadU32(u32FlashAddr + 8);
    //pstChunkHeaderInfo->u32RamSize = pstChunkHeaderInfo->u32RamEnd - pstChunkHeaderInfo->u32RamStart + 1;

    pstChunkHeaderInfo->u32CRC = Ld_MDrv_Flash_ReadU32(u32FlashAddr + CHUNK_HEADER_CRC32_POS);

    pstChunkHeaderInfo->u32BootLoaderSize = Ld_MDrv_Flash_ReadU32(u32FlashAddr + 0x18);

    //pstChunkHeaderInfo->u32BinaryId = Loader2_ReadFlashU32(u32FlashAddr + 0x1C);
}
#endif

//==============================================================================
#if ( defined(__mips__) && ENABLE_MSTAR_MILAN && ENABLE_NON_OS) // Non-OS - Milan
#define REG_BDMA_BASE   0x900

#define DEBUG_DECOMP(x)     //x
#define DEBUG_DECOMP_CRC(x) x

void __loader_2nd DoMSDecompression(void)
{
    U8 IsCompressMagicNumber = 0;
    U32 CompressedFileLength = 0;
    U32 OriginalFileLength = 0;
    U32 nOutputLength=0;

    U32 u32BOOT_LOADER_LENGTH = 0;
    U32 u32ROM_START = 0;//*((U32 *)(AP_IN_FLASH_ADDR + u32BOOT_LOADER_LENGTH + 0));    //This address should be 4' alignment
    U32 u32ROM_END = 0;//*((U32 *)(AP_IN_FLASH_ADDR + u32BOOT_LOADER_LENGTH + 12));    //This address should be 4' alignment
    U32 i = 0;
    U32 u32CRC32_HW = 0, u32CRC32_SW = 0;
    U32 u32RAM_START, u32RAM_END;
    BOOL bIsBootToBL = FALSE; // Not AP
    U32 u32Header_BootLoaderSize;



    uart_printf("\n", 0 );

    DEBUG_DECOMP( uart_printf("AP_IN_FLASH_ADDR=%X\n", AP_IN_FLASH_ADDR); );


    // Get boot loader bin size from flash chunker-header
    u32Header_BootLoaderSize = *((U32 *)(AP_IN_FLASH_ADDR + 0x18));
    DEBUG_DECOMP( uart_printf("u32Header_BootLoaderSize=%X\n", u32Header_BootLoaderSize););

    if( u32Header_BootLoaderSize )
    {
        // Get u32BOOT_LOADER_LENGTH
        u32BOOT_LOADER_LENGTH = u32Header_BootLoaderSize - 0x20000; //minus 64k for sboot and 64k for pm code
    }
    else
    {
        u32BOOT_LOADER_LENGTH = 0;
    }
    u32ROM_START = *((U32 *)(AP_IN_FLASH_ADDR + u32BOOT_LOADER_LENGTH + 0));    //This address should be 4' alignment
    u32ROM_END = *((U32 *)(AP_IN_FLASH_ADDR + u32BOOT_LOADER_LENGTH + 12));    //This address should be 4' alignment

    DEBUG_DECOMP( uart_printf("u32BOOT_LOADER_LENGTH=%X\n", u32BOOT_LOADER_LENGTH););
    DEBUG_DECOMP( uart_printf("Chunker_Header_addr=%X\n", u32BOOT_LOADER_LENGTH+0x20000););
    DEBUG_DECOMP( uart_printf("u32ROM_START=%X\n", u32ROM_START););
    DEBUG_DECOMP( uart_printf("u32ROM_END=%X\n", u32ROM_END););


    //Compressed Format
    if( (u32ROM_START>=AP_IN_FLASH_ADDR)
      && (u32ROM_START<(AP_IN_FLASH_ADDR+0x800000))
      && (u32ROM_END>=AP_IN_FLASH_ADDR)
      && (u32ROM_END<(AP_IN_FLASH_ADDR+0x800000)) ) //8MB flash
    {
        // Main AP
        u32RAM_START = *((U32 *)(AP_IN_FLASH_ADDR + u32BOOT_LOADER_LENGTH + 4));    //This address should be 4' alignment
        u32RAM_END = *((U32 *)(AP_IN_FLASH_ADDR + u32BOOT_LOADER_LENGTH + 8));

        DEBUG_DECOMP(uart_printf("u32RAM_START=%X\n", u32RAM_START););
        DEBUG_DECOMP(uart_printf("u32RAM_END=%X\n", u32RAM_END););

        // Check magic end/start
        {
            U32 u32MAGIC_END_ADDR, u32MAGIC_END, u32MAGIC_START;

            DEBUG_DECOMP(uart_printf("u32MAGIC_END_ADDR FlashAddr=%X\n", (u32ROM_START+COMPRESSED_LENGTH_POS+4)););

            u32MAGIC_END_ADDR = *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS+4)+3));
            u32MAGIC_END_ADDR <<= 8;
            u32MAGIC_END_ADDR |= *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS+4)+2));
            u32MAGIC_END_ADDR <<= 8;
            u32MAGIC_END_ADDR |= *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS+4)+1));
            u32MAGIC_END_ADDR <<= 8;
            u32MAGIC_END_ADDR |= *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS+4)+0));
            DEBUG_DECOMP(uart_printf("u32MAGIC_END_ADDR=%X\n", u32MAGIC_END_ADDR););

            u32MAGIC_END = *((U8 *)(u32MAGIC_END_ADDR+3));
            u32MAGIC_END <<= 8;
            u32MAGIC_END |= *((U8 *)(u32MAGIC_END_ADDR+2));
            u32MAGIC_END <<= 8;
            u32MAGIC_END |= *((U8 *)(u32MAGIC_END_ADDR+1));
            u32MAGIC_END <<= 8;
            u32MAGIC_END |= *((U8 *)(u32MAGIC_END_ADDR+0));
            DEBUG_DECOMP(uart_printf("u32MAGIC_END=%X\n", u32MAGIC_END););
            if(u32MAGIC_END != 0x55AAABCD)
            {
                uart_printf("u32MAGIC_END(%X) != 0x55AAABCD\n", u32MAGIC_END);
                goto LOAD_LOADER_AP;
            }

            // Magic start is at  .prog_img first 3 bytes
            u32MAGIC_START = *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-0x4A)+3));
            u32MAGIC_START <<= 8;
            u32MAGIC_START |= *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-0x4A)+2));
            u32MAGIC_START <<= 8;
            u32MAGIC_START |= *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-0x4A)+1));
            u32MAGIC_START <<= 8;
            u32MAGIC_START |= *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-0x4A)+0));
            DEBUG_DECOMP(uart_printf("u32MAGIC_START=%X\n", u32MAGIC_START););
            if(u32MAGIC_START != 0x55AA5678)
            {
                uart_printf("u32MAGIC_START(%X) != 0x55AA5678\n", u32MAGIC_START);
                goto LOAD_LOADER_AP;
            }
        }
    }
    else
    {
        // Load Boot-Loader
LOAD_LOADER_AP:

        uart_printf("LOAD Frist AP:\n", 0);

        bIsBootToBL = TRUE;
        u32ROM_START = *((U32 *)(AP_IN_FLASH_ADDR + 0));    //This address should be 4' alignment
        u32RAM_START = *((U32 *)(AP_IN_FLASH_ADDR + 4));    //This address should be 4' alignment
        u32RAM_END = *((U32 *)(AP_IN_FLASH_ADDR + 8));
        u32ROM_END = *((U32 *)(AP_IN_FLASH_ADDR + 12));    //This address should be 4' alignment
    }

    u32CRC32_SW = *((U8 *)(AP_IN_FLASH_ADDR + u32BOOT_LOADER_LENGTH +CRC32_POS+3));
    u32CRC32_SW <<= 8;
    u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR + u32BOOT_LOADER_LENGTH +CRC32_POS+2));
    u32CRC32_SW <<= 8;
    u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR + u32BOOT_LOADER_LENGTH +CRC32_POS+1));
    u32CRC32_SW <<= 8;
    u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR + u32BOOT_LOADER_LENGTH +CRC32_POS+0));

    DEBUG_DECOMP(uart_printf("u32CRC32_SW=%X\n", u32CRC32_SW););

LOAD_CODE_POINT:
    DEBUG_DECOMP(uart_printf("LOAD_CODE_POINT:\n", 0););

    // Check if this bin file is compressed format, find compress format magic number
    DEBUG_DECOMP(uart_printf("compress format magic1=%X\n", *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-4)+3))););
    DEBUG_DECOMP(uart_printf("compress format magic1=%X\n", *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-4)+2))););
    if( ( *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-4)+3)) == 0xDE )
      && ( *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-4)+2)) == 0xAD ) )
    {
        U8 nCompressFormat = *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-4)+1));

        DEBUG_DECOMP(uart_printf("nCompressFormat=%X\n", nCompressFormat););

        if ( nCompressFormat == 0xBE )
            IsCompressMagicNumber = 1;
        else if ( nCompressFormat == 0xEF )
            IsCompressMagicNumber = 2;
        else
            IsCompressMagicNumber = 0;
    }
    else
    {
        IsCompressMagicNumber = 0;
    }

    DEBUG_DECOMP(uart_printf("IsCompressMagicNumber=%X\n", IsCompressMagicNumber););

    if ( IsCompressMagicNumber )
    {
        CompressedFileLength = *((U8 *)(u32ROM_START+COMPRESSED_LENGTH_POS+3));
        CompressedFileLength <<= 8;
        CompressedFileLength |= *((U8 *)(u32ROM_START+COMPRESSED_LENGTH_POS+2));
        CompressedFileLength <<= 8;
        CompressedFileLength |= *((U8 *)(u32ROM_START+COMPRESSED_LENGTH_POS+1));
        CompressedFileLength <<= 8;
        CompressedFileLength |= *((U8 *)(u32ROM_START+COMPRESSED_LENGTH_POS+0));
        CompressedFileLength -= 6; /*6: 2 for crc16 and 4 for length*/

        DEBUG_DECOMP(uart_printf("CompressedFileLength=%U\n", CompressedFileLength););

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Calculate CRC32 from flash

    #if 0
        //clk_bdma to clk_miu
        //swch 4
        //wriu    0x00100b4e 0x04
        uart_printf("0b4e=%X\n", msReadByte(0x00100b4e));
        msWriteByte(0x00100b4e, 0x04);
        uart_printf("0b4e=%X\n", msReadByte(0x00100b4e));

        //clk_spi: internal clock
        //swch 4
        uart_printf("0b2c=%X\n", msReadByte(0x00100b2c));
        //wriu    0x00100b2c 0x08     // clk_spi_p1      to  216
        msWriteByte(0x00100b2c, 0x08);
        //wriu    0x00100b2c 0x28     // clk_spi_p       to  216
        msWriteByte(0x00100b2c, 0x28);
        uart_printf("0b2c=%X\n", msReadByte(0x00100b2c));

        //clk_spi_pm
        //swch 3
        uart_printf("0f41=%X\n", msReadByte(0x00000f41));
        //wriu    0x00000f41 0x04     // clk_spi_p1 (PM) to  27
        //msWriteByte(0x00000f41, 0x04);
        //wriu    0x00000f41 0x44     // clk_spi_p  (PM) to  27
        //msWriteByte(0x00000f41, 0x44);
        uart_printf("0f41=%X\n", msReadByte(0x00000f41));
    #endif

    for(int iCrcRetry = 0; iCrcRetry < 5; iCrcRetry ++)
    {
    #if 0 // Reset BDMA engine
        //MDrv_WriteByte(REG_BDMA_TRIG__STOP, BDMA_CH_STOP); // [4]:Reg_stop
        RIU8[(LOCAL_BDMA_REG_BASE + 0x00)*2] = 0x10;

        //MDrv_PIU_DelayUs(1);
        uart_putc('.');

        // Un-Reset BDMA engine
        //MDrv_WriteByte(REG_BDMA_TRIG__STOP, 0);
        RIU8[(LOCAL_BDMA_REG_BASE + 0x00)*2] = 0x00;
    #endif

        // Wait hw not busy
        if( (RIU8[(LOCAL_BDMA_REG_BASE + 0x02)*2] & 0x03) )
        {
            uart_printf(" Wait BDMA free\n",0);
            while (RIU8[(LOCAL_BDMA_REG_BASE + 0x02)*2] & 0x03);
        }

        RIU[(LOCAL_BDMA_REG_BASE + 0x08 )] = (U16)((U32)(u32ROM_START&0x3fffff));
        RIU[(LOCAL_BDMA_REG_BASE + 0x0A )] = (U16)(((U32)(u32ROM_START&0x3fffff)) >> 16);
        RIU[(LOCAL_BDMA_REG_BASE + 0x0C )] = 0;
        RIU[(LOCAL_BDMA_REG_BASE + 0x0E )] = 0;
        RIU[(LOCAL_BDMA_REG_BASE + 0x10 )] = (U16)((U32)(u32ROM_END-u32ROM_START));
        RIU[(LOCAL_BDMA_REG_BASE + 0x12 )] = (U16)((U32)((u32ROM_END-u32ROM_START) >> 16));
        //polynomial
        RIU[(LOCAL_BDMA_REG_BASE + 0x14 )] = (U16)(0x1DB7);
        RIU[(LOCAL_BDMA_REG_BASE + 0x16 )] = (U16)(0x04C1);
        //seed
        RIU[(LOCAL_BDMA_REG_BASE + 0x18 )] = 0;
        RIU[(LOCAL_BDMA_REG_BASE + 0x1a )] = 0;

        RIU8[(LOCAL_BDMA_REG_BASE + 0x06)*2] = 0x0;   //increasing
        RIU[(LOCAL_BDMA_REG_BASE+0x04)] = 0x0335;  //((DstDEv|DstDataWidth)  <<8) |(SrcDataWidth|SrcDEv) ;

        RIU8[ (LOCAL_BDMA_REG_BASE*2)] = 0x01; // Trigger

        // Wait BDMA done
        if ( (RIU8[(LOCAL_BDMA_REG_BASE+0x02)*2] & 0x08) != 0x08 )
        {
            uart_printf(" Wait CRC done\n",0);
            while( 1 )
            {
                if ( (RIU8[(LOCAL_BDMA_REG_BASE+0x02)*2] & 0x08) == 0x08 )
                    break;
            }
        }

        // Wait not busy
        if( (RIU8[(LOCAL_BDMA_REG_BASE + 0x02)*2] & 0x03) )
        {
            uart_printf(" Wait BDMA free\n",0);
            while (RIU8[(LOCAL_BDMA_REG_BASE + 0x02)*2] & 0x03);
        }

        //Get CRC32 result
        u32CRC32_HW = RIU[(LOCAL_BDMA_REG_BASE + 0x18 )] ;      //low word
        u32CRC32_HW |= (RIU[(LOCAL_BDMA_REG_BASE + 0x1a )]  <<  16);        //high word
        DEBUG_DECOMP_CRC(uart_printf("u32CRC32_HW=%X\n", u32CRC32_HW););

        if ( u32CRC32_HW != u32CRC32_SW )
        {
            uart_printf("u32CRC32_HW(%X) != u32CRC32_SW\n", u32CRC32_HW);
        }
        else
        {
            DEBUG_DECOMP_CRC(uart_printf("CRC OK\n", 0););
            break;
        }
    }

        //while(1){}

        if ( u32CRC32_HW != u32CRC32_SW )
        {
            uart_printf("u32CRC32_HW(%X) != u32CRC32_SW\n", u32CRC32_HW);

            //AP in flash is bad, load boot loader instead
            u32ROM_START = *((U32 *)(AP_IN_FLASH_ADDR + 0));    //This address should be 4' alignment
            u32RAM_START = *((U32 *)(AP_IN_FLASH_ADDR + 4));    //This address should be 4' alignment
            u32RAM_END = *((U32 *)(AP_IN_FLASH_ADDR + 8));
            u32ROM_END = *((U32 *)(AP_IN_FLASH_ADDR + 12));    //This address should be 4' alignment

            u32CRC32_SW = *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+3));
            u32CRC32_SW <<= 8;
            u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+2));
            u32CRC32_SW <<= 8;
            u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+1));
            u32CRC32_SW <<= 8;
            u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+0));
            goto LOAD_CODE_POINT;
        }

        //DEBUG_DECOMP_CRC(uart_printf("CRC OK\n", 0););

         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 0//(ENABLE_SECURE_BOOT) // secure boot test
        if ( (bIsBootToBL == FALSE) && (IsCompressMagicNumber == 1) )
        {
            uart_putc('\r');
            uart_putc('\n');
            uart_putc('1');
            uart_putc('\r');
            uart_putc('\n');
            //Copy uncompressed part first
            for(i=0;i<(((u32ROM_END-u32ROM_START) + 7) & ~7UL);i+=4)
            {
                *((U32 *)(u32RAM_START+i)) = *((U32 *)((U32)(u32ROM_START+i)));
            }
            uart_putc('\r');
            uart_putc('\n');
            uart_putc('2');
            uart_putc('\r');
            uart_putc('\n');
         {
                extern unsigned char AuthenticationTest(unsigned char *Signature, unsigned char *Public_N, unsigned char *Public_E, unsigned char *Data, unsigned int Len);
                U32 u32AP_LEN;
                U32 secure_header = AP_IN_FLASH_ADDR+0x40000;
                u32AP_LEN = u32ROM_END - u32ROM_START;
                //while(1){}
                if(0 == AuthenticationTest((U8 *)(secure_header+128+260), (U8 *)(secure_header+128), (U8 *)(secure_header+128+256), (U8 *)u32RAM_START, u32AP_LEN))
                {
                    // AuthenticationTest failed
                    //AP in flash is bad, load boot loader instead
                    u32ROM_START = *((U32 *)(AP_IN_FLASH_ADDR + 0));    //This address should be 4' alignment
                    u32RAM_START = *((U32 *)(AP_IN_FLASH_ADDR + 4));    //This address should be 4' alignment
                    u32RAM_END = *((U32 *)(AP_IN_FLASH_ADDR + 8));
                    u32ROM_END = *((U32 *)(AP_IN_FLASH_ADDR + 12));    //This address should be 4' alignment

                    u32CRC32_SW = *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+3));
                    u32CRC32_SW <<= 8;
                    u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+2));
                    u32CRC32_SW <<= 8;
                    u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+1));
                    u32CRC32_SW <<= 8;
                    u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+0));
                    bIsBootToBL = TRUE;
                    goto LOAD_CODE_POINT;
                }
            }
            uart_putc('\r');
            uart_putc('\n');
            uart_putc('3');
            uart_putc('\r');
            uart_putc('\n');
        }
#endif
        //Copy uncompressed part first
        for(i=0;i<((UNCOMPRESSED_LENGTH + 7) & ~7UL);i+=4)
        {
            *((U32 *)(u32RAM_START+i)) = *((U32 *)((U32)(u32ROM_START+i)));
        }

        //Copy compressed part second
        for(i=0;i<(((u32ROM_END-u32ROM_START-UNCOMPRESSED_LENGTH) + 7) & ~7UL);i+=4)
        {
            *((U32 *)(DECOMPRESS_BUF + i)) = *((U32 *)(u32ROM_START + UNCOMPRESSED_LENGTH + i));
        }

//        CompressedFileLength = *((U8 *)(u32RAM_START+COMPRESSED_LENGTH_POS+3));
//        CompressedFileLength <<= 8;
//        CompressedFileLength |= *((U8 *)(u32RAM_START+COMPRESSED_LENGTH_POS+2));
//        CompressedFileLength <<= 8;
//        CompressedFileLength |= *((U8 *)(u32RAM_START+COMPRESSED_LENGTH_POS+1));
//        CompressedFileLength <<= 8;
//        CompressedFileLength |= *((U8 *)(u32RAM_START+COMPRESSED_LENGTH_POS+0));
//        CompressedFileLength -= 6;

        OriginalFileLength = *((U8 *)(DECOMPRESS_BUF+CompressedFileLength-UNCOMPRESSED_LENGTH+3));
        OriginalFileLength <<= 8;
        OriginalFileLength |= *((U8 *)(DECOMPRESS_BUF+CompressedFileLength-UNCOMPRESSED_LENGTH+2));
        OriginalFileLength <<= 8;
        OriginalFileLength |= *((U8 *)(DECOMPRESS_BUF+CompressedFileLength-UNCOMPRESSED_LENGTH+1));
        OriginalFileLength <<= 8;
        OriginalFileLength |= *((U8 *)(DECOMPRESS_BUF+CompressedFileLength-UNCOMPRESSED_LENGTH+0));

        // Start to decompress
        if ( IsCompressMagicNumber == 1 )
        {
            MemoryAllocatorInit((U8 *)MEMORY_POOL_CACHEABLE, MEMORY_POOL_SIZE);
            MsDecompress((U8 *)DECOMPRESS_BUF_CACHEABLE, (U8 *)((u32RAM_START + UNCOMPRESSED_LENGTH) & NON_CACHEABLE_TO_CACHEABLE_MASK), CompressedFileLength, OriginalFileLength, &nOutputLength);

            if ( nOutputLength != OriginalFileLength )
            {
                uart_printf(" => DeComp failed!\n", 0);

                //Decompression fail! Jump to boot loader to recover FW
                u32ROM_START = *((U32 *)(AP_IN_FLASH_ADDR + 0));    //This address should be 4' alignment
                u32RAM_START = *((U32 *)(AP_IN_FLASH_ADDR + 4));    //This address should be 4' alignment
                u32RAM_END = *((U32 *)(AP_IN_FLASH_ADDR + 8));
                u32ROM_END = *((U32 *)(AP_IN_FLASH_ADDR + 12));    //This address should be 4' alignment

                u32CRC32_SW = *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+3));
                u32CRC32_SW <<= 8;
                u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+2));
                u32CRC32_SW <<= 8;
                u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+1));
                u32CRC32_SW <<= 8;
                u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+0));
                goto LOAD_CODE_POINT;
            }
        }
        else    // IsCompressMagicNumber == 2
        {
            MemoryAllocatorInit((U8 *)MEMORY_POOL_CACHEABLE, MEMORY_POOL_SIZE);

            if ( MsDecompress7((U8 *)DECOMPRESS_BUF_CACHEABLE, (U8 *)((u32RAM_START + UNCOMPRESSED_LENGTH) & NON_CACHEABLE_TO_CACHEABLE_MASK), CompressedFileLength+4-UNCOMPRESSED_LENGTH) != TRUE )
            {
                uart_printf(" => DeComp7 failed!\n", 0);

                //Decompression fail! Jump to boot loader to recover FW
                u32ROM_START = *((U32 *)(AP_IN_FLASH_ADDR + 0));    //This address should be 4' alignment
                u32RAM_START = *((U32 *)(AP_IN_FLASH_ADDR + 4));    //This address should be 4' alignment
                u32RAM_END = *((U32 *)(AP_IN_FLASH_ADDR + 8));
                u32ROM_END = *((U32 *)(AP_IN_FLASH_ADDR + 12));    //This address should be 4' alignment

                u32CRC32_SW = *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+3));
                u32CRC32_SW <<= 8;
                u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+2));
                u32CRC32_SW <<= 8;
                u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+1));
                u32CRC32_SW <<= 8;
                u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+0));
                goto LOAD_CODE_POINT;
            }
        }
    }
    else
    {
        // Uncompressed format
#if 1 /*enable HW crc32 check*/
         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         //Calculate CRC32 from flash
        while (RIU8[(LOCAL_BDMA_REG_BASE + 0x02)*2] & 0x01);
        RIU[(LOCAL_BDMA_REG_BASE + 0x08 )] = (U16)((U32)(u32ROM_START&0x3fffff));
        RIU[(LOCAL_BDMA_REG_BASE + 0x0A )] = (U16)(((U32)(u32ROM_START&0x3fffff)) >> 16);
        RIU[(LOCAL_BDMA_REG_BASE + 0x0C )] = 0;
        RIU[(LOCAL_BDMA_REG_BASE + 0x0E )] = 0;
        RIU[(LOCAL_BDMA_REG_BASE + 0x10 )] = (U16)((U32)(u32ROM_END-u32ROM_START)) ;
        RIU[(LOCAL_BDMA_REG_BASE + 0x12 )] = (U16)(((U32)(u32ROM_END-u32ROM_START))>>16);
        //polynomial
        RIU[(LOCAL_BDMA_REG_BASE + 0x14 )] = (U16)(0x1DB7);
        RIU[(LOCAL_BDMA_REG_BASE + 0x16 )] = (U16)(0x04C1);
        //seed
        RIU[(LOCAL_BDMA_REG_BASE + 0x18 )] = 0;
        RIU[(LOCAL_BDMA_REG_BASE + 0x1a )] = 0;

        RIU8[(LOCAL_BDMA_REG_BASE + 0x06)*2] = 0x00;   //increasing
        RIU[(LOCAL_BDMA_REG_BASE+0x04)] = 0x0335;  //((DstDEv|DstDataWidth)  <<8) |(SrcDataWidth|SrcDEv) ;

        RIU8[ (LOCAL_BDMA_REG_BASE*2)] = 0x01 ;
        while( 1 )
        {
            if ( (RIU8[(LOCAL_BDMA_REG_BASE+0x02)*2] & 0x08) == 0x08 )
                break;
        }

        //Get CRC32 result
        u32CRC32_HW = RIU[(LOCAL_BDMA_REG_BASE + 0x18 )] ;      //low word
        u32CRC32_HW |= (RIU[(LOCAL_BDMA_REG_BASE + 0x1a )]  <<  16);        //high word

        //u32CRC32_SW = *((U8 *)(u32ROM_START+CRC32_POS+3));
        //u32CRC32_SW <<= 8;
        //u32CRC32_SW |= *((U8 *)(u32ROM_START+CRC32_POS+2));
        //u32CRC32_SW <<= 8;
        //u32CRC32_SW |= *((U8 *)(u32ROM_START+CRC32_POS+1));
        //u32CRC32_SW <<= 8;
        //u32CRC32_SW |= *((U8 *)(u32ROM_START+CRC32_POS+0));

        if ( u32CRC32_HW != u32CRC32_SW )
        {
            //AP in flash is bad, load boot loader instead
            u32ROM_START = *((U32 *)(AP_IN_FLASH_ADDR + 0));    //This address should be 4' alignment
            u32RAM_START = *((U32 *)(AP_IN_FLASH_ADDR + 4));    //This address should be 4' alignment
            u32RAM_END = *((U32 *)(AP_IN_FLASH_ADDR + 8));
            u32ROM_END = *((U32 *)(AP_IN_FLASH_ADDR + 12));    //This address should be 4' alignment

            u32CRC32_SW = *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+3));
            u32CRC32_SW <<= 8;
            u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+2));
            u32CRC32_SW <<= 8;
            u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+1));
            u32CRC32_SW <<= 8;
            u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+0));
            goto LOAD_CODE_POINT;
        }
         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        asm volatile (
        // Load ROM_START, RAM_START, RAM_END, ROM_END from 0xBFC20000
        #ifdef CONFIG_MSTAR_KAISERIN
            "li      $15, 0x94050000;" /*0xBFC20000 + u32BOOT_LOADER_LENGTH*/
        #else
            "li      $15, 0xbfc50000;" /*0xBFC20000 + u32BOOT_LOADER_LENGTH*/
        #endif
#else
        asm volatile (
        // Load ROM_START, RAM_START, RAM_END, ROM_END from 0xBFC20000
        #ifdef CONFIG_MSTAR_KAISERIN
            "li      $15, 0x94020000;"
        #else
            "li      $15, 0xBFC20000;"
        #endif
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

   // init_version();
}

#elif defined(__mips__) || defined(__arm__) || defined(__arm64__)
void __loader_2nd DoMSDecompression(void)
{
#if (ENABLE_BOOTING_FROM_EXT_EMMC_WITH_CPU == 0 && ENABLE_MSTAR_ROM_BOOT_WITH_EMMC_FLASH == 0)
    U8 IsCompressMagicNumber = 0;
    U32 CompressedFileLength = 0;
    U32 OriginalFileLength = 0;
    U32 nOutputLength=0;

#if (USE_BDMA_FOR_CRC_CHECK!=1)

    U32 u32ROM_START = *((U32 *)(AP_IN_FLASH_ADDR + 0));    //This address should be 4' alignment
    U32 u32ROM_END = *((U32 *)(AP_IN_FLASH_ADDR + 0xC));    //This address should be 4' alignment
    U32 i = 0;
    U32 u32MagicNoOfBin = *((U32 *)(AP_IN_FLASH_ADDR + 0x1C));    //This address should be 4' alignment
    BOOL bIsUBoot = FALSE;

    //Check whether the following binary is U-Boot or non-OS AP.
    if (u32MagicNoOfBin == UBOOT_IDENTIFIER)
    {
        bIsUBoot = TRUE;
    }
    else
    {
        bIsUBoot = FALSE;
    }

    if (bIsUBoot == TRUE) //The following binary is U-Boot.
    {
        U32 u32RAM_START, u32RAM_END;


        //Compressed Format
        u32RAM_START = *((U32 *)(AP_IN_FLASH_ADDR + 4));    //This address should be 4' alignment
        u32RAM_END = *((U32 *)(AP_IN_FLASH_ADDR + 8));

        CompressedFileLength = (u32ROM_END - u32ROM_START);
        OriginalFileLength = (u32RAM_END - u32RAM_START);


        if (CompressedFileLength != OriginalFileLength) //It implys that U-Boot is compressed.
        {
#if (defined(ENABLE_BOOTING_FROM_EXT_SPI_WITH_PM51) && (ENABLE_BOOTING_FROM_EXT_SPI_WITH_PM51==1) && defined(__mips__) )


                    u32ROM_START-=0x10000;
                    u32ROM_END-=0x10000;
#endif
            //Copy compressed UBoot from SPI flash to RAM
#if defined(CONFIG_ENABLE_BOOTING_FROM_ROM_WITH_COMPRESS_UBOOT)
#else

            for(i=0;i<((CompressedFileLength + 7) & ~7UL);i+=4)
            {
                #if (ENABLE_BOOTING_FROM_OTP_WITH_PM51==1)
                *((U32 *)(DECOMPRESS_BUF + i)) = *((U32 *)(u32RAM_START + i));
                #else
                *((U32 *)(DECOMPRESS_BUF + i)) = *((U32 *)(u32ROM_START + i));
                #endif
            }

#endif
            // Start to decompress
            MemoryAllocatorInit((U8 *)MEMORY_POOL_CACHEABLE, MEMORY_POOL_SIZE);
#if defined(CONFIG_ENABLE_BOOTING_FROM_ROM_WITH_COMPRESS_UBOOT)
			MsDecompress((U8 *)AP_IN_FLASH_ADDR_CACHE+0x400, (U8 *)(u32RAM_START & NON_CACHEABLE_TO_CACHEABLE_MASK), CompressedFileLength, OriginalFileLength, &nOutputLength);
#else


           #if defined(CONFIG_MSCOMPRESS7)
           MsDecompress7((U8 *)DECOMPRESS_BUF_CACHEABLE, (U8 *)(u32RAM_START & NON_CACHEABLE_TO_CACHEABLE_MASK), CompressedFileLength);
           #else
           MsDecompress((U8 *)DECOMPRESS_BUF_CACHEABLE, (U8 *)(u32RAM_START & NON_CACHEABLE_TO_CACHEABLE_MASK), CompressedFileLength, OriginalFileLength, &nOutputLength);
           #endif
#endif
            if ( nOutputLength != OriginalFileLength )
            {
                //Decompression fail! Jump to boot loader to recover FW
            }
        }
        else //It implys that U-Boot is not compressed.
        {
#if defined(__arm__) && (USE_BDMA_MOVE_CODE == 0)
            // Uncompressed format
            asm volatile (
            // Load ROM_START, RAM_START, RAM_END, ROM_END from 0x00020000, jc_db: need to remove magic number
            #if (ENABLE_BOOTING_FROM_EXT_SPI_WITH_PM51)
                "ldr     r0, =0x14030000\n"
            #else
                "ldr     r0, =0x14020000\n"
            #endif
                "ldr     r1, [r0,#0]\n"
                "ldr     r2, [r0,#4]\n"
                "ldr     r3, [r0,#8]\n"
                "ldr     r4, [r0,#12]\n"
          "1:\n"
                "LDR     r5, [r1], #4\n"
                "STR     r5, [r2], #4\n"
                "CMP     r1, r4\n"
                "BNE     1b\n"
                :::"r0", "r1", "r2", "r3", "r4", "r5","memory"
            );
            // Uncompressed format
#elif defined(__arm64__) && (USE_BDMA_MOVE_CODE == 0)
            // Uncompressed format
            asm volatile (
            // Load ROM_START, RAM_START, RAM_END, ROM_END from 0x00020000, jc_db: need to remove magic number
            #if (ENABLE_BOOTING_FROM_EXT_SPI_WITH_PM51)
                "ldr     x0, =0x14030000\n"
            #else
                "ldr     x0, =0x14020000\n"
            #endif
                "ldr     x1, [r0,#0]\n"
                "ldr     x2, [r0,#4]\n"
                "ldr     x3, [r0,#8]\n"
                "ldr     x4, [r0,#12]\n"
          "1:\n"
                "ldr     w5, [x1], #4\n"
                "str     w5, [x2], #4\n"
                "cmp     x1, x4\n"
                "b.ne     1b\n"
                :::"r0", "r1", "r2", "r3", "r4", "r5","memory"
            );
            // Uncompressed format
#elif ( (ENABLE_MSTAR_KRONUS==1) || (ENABLE_MSTAR_KAISERIN==1) || (ENABLE_MSTAR_KENYA == 1)|| (ENABLE_MSTAR_KRITI == 1) || (ENABLE_MSTAR_KRATOS == 1))
            asm volatile (
            // Load ROM_START, RAM_START, RAM_END, ROM_END from 0xBFC20000
            #ifdef CONFIG_MSTAR_KAISERIN
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
#else
            // Use BDMA to move code from SPI to DRAM
        #if( (ENABLE_BOOTING_FROM_EXT_SPI_WITH_CPU) || (ENABLE_BOOTING_FROM_EXT_SPI_WITH_PM51))
            __BDMA_FlashCopy2MIU0(u32ROM_START&0x3fffff, u32RAM_START&(CONFIG_MIU0_BUSADDR-1), (u32ROM_END-u32ROM_START));
        #endif
#endif

#if 0
            {
                extern unsigned char AuthenticationTest(unsigned char *Signature, unsigned char *Public_N, unsigned char *Public_E, unsigned char *Data, unsigned int Len);
                U32 u32AP_LEN;

                u32RAM_START = *((U32 *)(AP_IN_FLASH_ADDR + 4));    //This address should be 4' alignment
                u32RAM_END = *((U32 *)(AP_IN_FLASH_ADDR + 8));
                u32AP_LEN = u32RAM_END - u32RAM_START;
                AuthenticationTest((U8 *)(AP_IN_FLASH_ADDR+128+260), (U8 *)(AP_IN_FLASH_ADDR+128), (U8 *)(AP_IN_FLASH_ADDR+128+256), (U8 *)u32RAM_START, u32AP_LEN);
            }
#endif
        }
    }
    else //The following binary is non-OS AP.
    {
        // Check if this bin file is compressed format, find compress format magic number
        if ( ( *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-4)+3)) == 0xDE )
        && ( *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-4)+2)) == 0xAD ) )
        {
            U8 nCompressFormat = *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-4)+1));

            if ( nCompressFormat == 0xBE )
                IsCompressMagicNumber = 1;
            else if ( nCompressFormat == 0xEF )
                IsCompressMagicNumber = 2;
            else
                IsCompressMagicNumber = 0;
        }
        else
        {
            IsCompressMagicNumber = 0;
        }
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
            CompressedFileLength = *((U8 *)(u32RAM_START+COMPRESSED_LENGTH_POS+3));
            CompressedFileLength <<= 8;
            CompressedFileLength |= *((U8 *)(u32RAM_START+COMPRESSED_LENGTH_POS+2));
            CompressedFileLength <<= 8;
            CompressedFileLength |= *((U8 *)(u32RAM_START+COMPRESSED_LENGTH_POS+1));
            CompressedFileLength <<= 8;
            CompressedFileLength |= *((U8 *)(u32RAM_START+COMPRESSED_LENGTH_POS+0));
            CompressedFileLength -= 6;

            OriginalFileLength = *((U8 *)(DECOMPRESS_BUF+CompressedFileLength-UNCOMPRESSED_LENGTH+3));
            OriginalFileLength <<= 8;
            OriginalFileLength |= *((U8 *)(DECOMPRESS_BUF+CompressedFileLength-UNCOMPRESSED_LENGTH+2));
            OriginalFileLength <<= 8;
            OriginalFileLength |= *((U8 *)(DECOMPRESS_BUF+CompressedFileLength-UNCOMPRESSED_LENGTH+1));
            OriginalFileLength <<= 8;
            OriginalFileLength |= *((U8 *)(DECOMPRESS_BUF+CompressedFileLength-UNCOMPRESSED_LENGTH+0));

            // Start to decompress
            if ( IsCompressMagicNumber == 1 )
            {
                MemoryAllocatorInit((U8 *)MEMORY_POOL_CACHEABLE, MEMORY_POOL_SIZE);
                MsDecompress((U8 *)DECOMPRESS_BUF_CACHEABLE, (U8 *)((u32RAM_START + UNCOMPRESSED_LENGTH) & NON_CACHEABLE_TO_CACHEABLE_MASK), CompressedFileLength, OriginalFileLength, &nOutputLength);

                if ( nOutputLength != OriginalFileLength )
                {
                    //Decompression fail! Jump to boot loader to recover FW
                }
            }
            else    // IsCompressMagicNumber == 2
            {
                MemoryAllocatorInit((U8 *)MEMORY_POOL_CACHEABLE, MEMORY_POOL_SIZE);
                if ( MsDecompress7((U8 *)DECOMPRESS_BUF_CACHEABLE, (U8 *)((u32RAM_START + UNCOMPRESSED_LENGTH) & NON_CACHEABLE_TO_CACHEABLE_MASK), CompressedFileLength+4-UNCOMPRESSED_LENGTH) != TRUE )
                {
                    //Decompression fail! Jump to boot loader to recover FW
                }
            }
        }
        else
        {
#if defined(__arm__)
            // Uncompressed format
            asm volatile (
            // Load ROM_START, RAM_START, RAM_END, ROM_END from 0x00020000, jc_db: need to remove magic number
            #if (ENABLE_BOOTING_FROM_EXT_SPI_WITH_PM51)
                "ldr     r0, =0x14020000+0x10000\n"
            #else
                "ldr     r0, =0x14020000\n"
            #endif
                "ldr     r1, [r0,#0]\n"
                "ldr     r2, [r0,#4]\n"
                "ldr     r3, [r0,#8]\n"
                "ldr     r4, [r0,#12]\n"
          "1:\n"
                "LDR     r5, [r1], #4\n"
                "STR     r5, [r2], #4\n"
                "CMP     r1, r4\n"
                "BNE     1b\n"
                :::"r0", "r1", "r2", "r3", "r4", "r5","memory"
            );
#elif defined(__arm64__)
            // Uncompressed format
            asm volatile (
            // Load ROM_START, RAM_START, RAM_END, ROM_END from 0x00020000, jc_db: need to remove magic number
            #if (ENABLE_BOOTING_FROM_EXT_SPI_WITH_PM51)
                "ldr     x0, =0x14020000+0x10000\n"
            #else
                "ldr     x0, =0x14020000\n"
            #endif
                "ldr     x1, [x0,#0]\n"
                "ldr     x2, [x0,#4]\n"
                "ldr     x3, [x0,#8]\n"
                "ldr     x4, [x0,#12]\n"
          "1:\n"
                "ldr     w5, [x1], #4\n"
                "str     w5, [x2], #4\n"
                "cmp     x1, x4\n"
                "b.ne    1b\n"
                :::"r0", "r1", "r2", "r3", "r4", "r5","memory"
            );
#else
            // Uncompressed format
            asm volatile (
            // Load ROM_START, RAM_START, RAM_END, ROM_END from 0xBFC20000
            #ifdef CONFIG_MSTAR_KAISERIN
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
#endif
        }
    }
#else //(USE_BDMA_FOR_CRC_CHECK!=1)
    U32 u32BOOT_LOADER_LENGTH = BOOT_LOADER_LENGTH;
    U32 u32ROM_START = *((U32 *)(AP_IN_FLASH_ADDR + u32BOOT_LOADER_LENGTH + 0));    //This address should be 4' alignment
    U32 u32ROM_END = *((U32 *)(AP_IN_FLASH_ADDR + u32BOOT_LOADER_LENGTH + 12));    //This address should be 4' alignment
    U32 i = 0;
    U32 u32CRC32_HW = 0, u32CRC32_SW = 0;
    U32 u32RAM_START, u32RAM_END;
    BOOL bIsBootToBL = FALSE; // Not AP

    {
        //set u32BOOT_LOADER_LENGTH from flash
        u32BOOT_LOADER_LENGTH = *((U32 *)(AP_IN_FLASH_ADDR + 0x18)) - 0x20000; //minus 64k for sboot and 64k for pm code
        u32ROM_START = *((U32 *)(AP_IN_FLASH_ADDR + u32BOOT_LOADER_LENGTH + 0));    //This address should be 4' alignment
        u32ROM_END = *((U32 *)(AP_IN_FLASH_ADDR + u32BOOT_LOADER_LENGTH + 12));    //This address should be 4' alignment
    }
    //Compressed Format
    if((u32ROM_START>=AP_IN_FLASH_ADDR)
        && (u32ROM_START<(AP_IN_FLASH_ADDR+0x800000))
        && (u32ROM_END>=AP_IN_FLASH_ADDR)
        && (u32ROM_END<(AP_IN_FLASH_ADDR+0x800000))) //8MB flash
    {
        // Main AP
        u32RAM_START = *((U32 *)(AP_IN_FLASH_ADDR + u32BOOT_LOADER_LENGTH + 4));    //This address should be 4' alignment
        u32RAM_END = *((U32 *)(AP_IN_FLASH_ADDR + u32BOOT_LOADER_LENGTH + 8));

        // Check magic end/start
        {
            U32 u32MAGIC_END_ADDR, u32MAGIC_END, u32MAGIC_START;
            u32MAGIC_END_ADDR = *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS+4)+3));
            u32MAGIC_END_ADDR <<= 8;
            u32MAGIC_END_ADDR |= *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS+4)+2));
            u32MAGIC_END_ADDR <<= 8;
            u32MAGIC_END_ADDR |= *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS+4)+1));
            u32MAGIC_END_ADDR <<= 8;
            u32MAGIC_END_ADDR |= *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS+4)+0));

            u32MAGIC_END = *((U8 *)(u32MAGIC_END_ADDR+3));
            u32MAGIC_END <<= 8;
            u32MAGIC_END |= *((U8 *)(u32MAGIC_END_ADDR+2));
            u32MAGIC_END <<= 8;
            u32MAGIC_END |= *((U8 *)(u32MAGIC_END_ADDR+1));
            u32MAGIC_END <<= 8;
            u32MAGIC_END |= *((U8 *)(u32MAGIC_END_ADDR+0));
            if(u32MAGIC_END != 0x55AAABCD)
            {
                goto LOAD_LOADER_AP;
            }
            u32MAGIC_START = *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-0x4A)+3));
            u32MAGIC_START <<= 8;
            u32MAGIC_START |= *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-0x4A)+2));
            u32MAGIC_START <<= 8;
            u32MAGIC_START |= *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-0x4A)+1));
            u32MAGIC_START <<= 8;
            u32MAGIC_START |= *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-0x4A)+0));
            if(u32MAGIC_START != 0x55AA5678)
            {
                goto LOAD_LOADER_AP;
            }
        }
    }
    else
    {
        // Loader AP
LOAD_LOADER_AP:
        bIsBootToBL = TRUE;
        u32ROM_START = *((U32 *)(AP_IN_FLASH_ADDR + 0));    //This address should be 4' alignment
        u32RAM_START = *((U32 *)(AP_IN_FLASH_ADDR + 4));    //This address should be 4' alignment
        u32RAM_END = *((U32 *)(AP_IN_FLASH_ADDR + 8));
        u32ROM_END = *((U32 *)(AP_IN_FLASH_ADDR + 12));    //This address should be 4' alignment
    }
    u32CRC32_SW = *((U8 *)(AP_IN_FLASH_ADDR + u32BOOT_LOADER_LENGTH +CRC32_POS+3));
    u32CRC32_SW <<= 8;
    u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR + u32BOOT_LOADER_LENGTH +CRC32_POS+2));
    u32CRC32_SW <<= 8;
    u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR + u32BOOT_LOADER_LENGTH +CRC32_POS+1));
    u32CRC32_SW <<= 8;
    u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR + u32BOOT_LOADER_LENGTH +CRC32_POS+0));

LOAD_CODE_POINT:

    // Check if this bin file is compressed format, find compress format magic number
    if ( ( *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-4)+3)) == 0xDE )
    && ( *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-4)+2)) == 0xAD ) )
    {
        U8 nCompressFormat = *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS-4)+1));

        if ( nCompressFormat == 0xBE )
            IsCompressMagicNumber = 1;
        else if ( nCompressFormat == 0xEF )
            IsCompressMagicNumber = 2;
        else
            IsCompressMagicNumber = 0;
    }
    else
    {
        IsCompressMagicNumber = 0;
    }

    if ( IsCompressMagicNumber )
    {
        CompressedFileLength = *((U8 *)(u32ROM_START+COMPRESSED_LENGTH_POS+3));
        CompressedFileLength <<= 8;
        CompressedFileLength |= *((U8 *)(u32ROM_START+COMPRESSED_LENGTH_POS+2));
        CompressedFileLength <<= 8;
        CompressedFileLength |= *((U8 *)(u32ROM_START+COMPRESSED_LENGTH_POS+1));
        CompressedFileLength <<= 8;
        CompressedFileLength |= *((U8 *)(u32ROM_START+COMPRESSED_LENGTH_POS+0));
        CompressedFileLength -= 6; /*6: 2 for crc16 and 4 for length*/

         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         //Calculate CRC32 from flash
        while (RIU8[(LOCAL_BDMA_REG_BASE + 0x02)*2] & 0x01);
        RIU[(LOCAL_BDMA_REG_BASE + 0x08 )] = (U16)((U32)(u32ROM_START&0x3fffff));
        RIU[(LOCAL_BDMA_REG_BASE + 0x0A )] = (U16)(((U32)(u32ROM_START&0x3fffff)) >> 16);
        RIU[(LOCAL_BDMA_REG_BASE + 0x0C )] = 0;
        RIU[(LOCAL_BDMA_REG_BASE + 0x0E )] = 0;
        RIU[(LOCAL_BDMA_REG_BASE + 0x10 )] = (U16)((U32)(u32ROM_END-u32ROM_START));
        RIU[(LOCAL_BDMA_REG_BASE + 0x12 )] = (U16)((U32)((u32ROM_END-u32ROM_START) >> 16));
        //polynomial
        RIU[(LOCAL_BDMA_REG_BASE + 0x14 )] = (U16)(0x1DB7);
        RIU[(LOCAL_BDMA_REG_BASE + 0x16 )] = (U16)(0x04C1);
        //seed
        RIU[(LOCAL_BDMA_REG_BASE + 0x18 )] = 0;
        RIU[(LOCAL_BDMA_REG_BASE + 0x1a )] = 0;

        RIU8[(LOCAL_BDMA_REG_BASE + 0x06)*2] = 0x0;   //increasing
        RIU[(LOCAL_BDMA_REG_BASE+0x04)] = 0x0335;  //((DstDEv|DstDataWidth)  <<8) |(SrcDataWidth|SrcDEv) ;

        RIU8[ (LOCAL_BDMA_REG_BASE*2)] = 0x01 ;
        while( 1 )
        {
            if ( (RIU8[(LOCAL_BDMA_REG_BASE+0x02)*2] & 0x08) == 0x08 )
                break;
        }

        //Get CRC32 result
        u32CRC32_HW = RIU[(LOCAL_BDMA_REG_BASE + 0x18 )] ;      //low word
        u32CRC32_HW |= (RIU[(LOCAL_BDMA_REG_BASE + 0x1a )]  <<  16);        //high word

        if ( u32CRC32_HW != u32CRC32_SW )
        {
            //AP in flash is bad, load boot loader instead
            u32ROM_START = *((U32 *)(AP_IN_FLASH_ADDR + 0));    //This address should be 4' alignment
            u32RAM_START = *((U32 *)(AP_IN_FLASH_ADDR + 4));    //This address should be 4' alignment
            u32RAM_END = *((U32 *)(AP_IN_FLASH_ADDR + 8));
            u32ROM_END = *((U32 *)(AP_IN_FLASH_ADDR + 12));    //This address should be 4' alignment

            u32CRC32_SW = *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+3));
            u32CRC32_SW <<= 8;
            u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+2));
            u32CRC32_SW <<= 8;
            u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+1));
            u32CRC32_SW <<= 8;
            u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+0));
            goto LOAD_CODE_POINT;
        }
         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if (ENABLE_SECURE_BOOT) // secure boot test
        if ( (bIsBootToBL == FALSE) && (IsCompressMagicNumber == 1) )
        {
            uart_putc('\r');
            uart_putc('\n');
            uart_putc('1');
            uart_putc('\r');
            uart_putc('\n');
            //Copy uncompressed part first
            for(i=0;i<(((u32ROM_END-u32ROM_START) + 7) & ~7UL);i+=4)
            {
                *((U32 *)(u32RAM_START+i)) = *((U32 *)((U32)(u32ROM_START+i)));
            }
            uart_putc('\r');
            uart_putc('\n');
            uart_putc('2');
            uart_putc('\r');
            uart_putc('\n');
         {
                extern unsigned char AuthenticationTest(unsigned char *Signature, unsigned char *Public_N, unsigned char *Public_E, unsigned char *Data, unsigned int Len);
                U32 u32AP_LEN;
                U32 secure_header = AP_IN_FLASH_ADDR+0x40000;
                u32AP_LEN = u32ROM_END - u32ROM_START;
                //while(1){}
                if(0 == AuthenticationTest((U8 *)(secure_header+128+260), (U8 *)(secure_header+128), (U8 *)(secure_header+128+256), (U8 *)u32RAM_START, u32AP_LEN))
                {
                    // AuthenticationTest failed
                    //AP in flash is bad, load boot loader instead
                    u32ROM_START = *((U32 *)(AP_IN_FLASH_ADDR + 0));    //This address should be 4' alignment
                    u32RAM_START = *((U32 *)(AP_IN_FLASH_ADDR + 4));    //This address should be 4' alignment
                    u32RAM_END = *((U32 *)(AP_IN_FLASH_ADDR + 8));
                    u32ROM_END = *((U32 *)(AP_IN_FLASH_ADDR + 12));    //This address should be 4' alignment

                    u32CRC32_SW = *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+3));
                    u32CRC32_SW <<= 8;
                    u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+2));
                    u32CRC32_SW <<= 8;
                    u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+1));
                    u32CRC32_SW <<= 8;
                    u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+0));
                    bIsBootToBL = TRUE;
                    goto LOAD_CODE_POINT;
                }
            }
            uart_putc('\r');
            uart_putc('\n');
            uart_putc('3');
            uart_putc('\r');
            uart_putc('\n');
        }
#endif
        //Copy uncompressed part first
        for(i=0;i<((UNCOMPRESSED_LENGTH + 7) & ~7UL);i+=4)
        {
            *((U32 *)(u32RAM_START+i)) = *((U32 *)((U32)(u32ROM_START+i)));
        }

        //Copy compressed part second
        for(i=0;i<(((u32ROM_END-u32ROM_START-UNCOMPRESSED_LENGTH) + 7) & ~7UL);i+=4)
        {
            *((U32 *)(DECOMPRESS_BUF + i)) = *((U32 *)(u32ROM_START + UNCOMPRESSED_LENGTH + i));
        }

//        CompressedFileLength = *((U8 *)(u32RAM_START+COMPRESSED_LENGTH_POS+3));
//        CompressedFileLength <<= 8;
//        CompressedFileLength |= *((U8 *)(u32RAM_START+COMPRESSED_LENGTH_POS+2));
//        CompressedFileLength <<= 8;
//        CompressedFileLength |= *((U8 *)(u32RAM_START+COMPRESSED_LENGTH_POS+1));
//        CompressedFileLength <<= 8;
//        CompressedFileLength |= *((U8 *)(u32RAM_START+COMPRESSED_LENGTH_POS+0));
//        CompressedFileLength -= 6;

        OriginalFileLength = *((U8 *)(DECOMPRESS_BUF+CompressedFileLength-UNCOMPRESSED_LENGTH+3));
        OriginalFileLength <<= 8;
        OriginalFileLength |= *((U8 *)(DECOMPRESS_BUF+CompressedFileLength-UNCOMPRESSED_LENGTH+2));
        OriginalFileLength <<= 8;
        OriginalFileLength |= *((U8 *)(DECOMPRESS_BUF+CompressedFileLength-UNCOMPRESSED_LENGTH+1));
        OriginalFileLength <<= 8;
        OriginalFileLength |= *((U8 *)(DECOMPRESS_BUF+CompressedFileLength-UNCOMPRESSED_LENGTH+0));

        // Start to decompress
        if ( IsCompressMagicNumber == 1 )
        {
            MemoryAllocatorInit((U8 *)MEMORY_POOL_CACHEABLE, MEMORY_POOL_SIZE);
            MsDecompress((U8 *)DECOMPRESS_BUF_CACHEABLE, (U8 *)((u32RAM_START + UNCOMPRESSED_LENGTH) & NON_CACHEABLE_TO_CACHEABLE_MASK), CompressedFileLength, OriginalFileLength, &nOutputLength);

            if ( nOutputLength != OriginalFileLength )
            {
                //Decompression fail! Jump to boot loader to recover FW
                u32ROM_START = *((U32 *)(AP_IN_FLASH_ADDR + 0));    //This address should be 4' alignment
                u32RAM_START = *((U32 *)(AP_IN_FLASH_ADDR + 4));    //This address should be 4' alignment
                u32RAM_END = *((U32 *)(AP_IN_FLASH_ADDR + 8));
                u32ROM_END = *((U32 *)(AP_IN_FLASH_ADDR + 12));    //This address should be 4' alignment

                u32CRC32_SW = *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+3));
                u32CRC32_SW <<= 8;
                u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+2));
                u32CRC32_SW <<= 8;
                u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+1));
                u32CRC32_SW <<= 8;
                u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+0));
                goto LOAD_CODE_POINT;
            }
        }
        else    // IsCompressMagicNumber == 2
        {
            MemoryAllocatorInit((U8 *)MEMORY_POOL_CACHEABLE, MEMORY_POOL_SIZE);

            if ( MsDecompress7((U8 *)DECOMPRESS_BUF_CACHEABLE, (U8 *)((u32RAM_START + UNCOMPRESSED_LENGTH) & NON_CACHEABLE_TO_CACHEABLE_MASK), CompressedFileLength+4-UNCOMPRESSED_LENGTH) != TRUE )
            {
                //Decompression fail! Jump to boot loader to recover FW
                u32ROM_START = *((U32 *)(AP_IN_FLASH_ADDR + 0));    //This address should be 4' alignment
                u32RAM_START = *((U32 *)(AP_IN_FLASH_ADDR + 4));    //This address should be 4' alignment
                u32RAM_END = *((U32 *)(AP_IN_FLASH_ADDR + 8));
                u32ROM_END = *((U32 *)(AP_IN_FLASH_ADDR + 12));    //This address should be 4' alignment

                u32CRC32_SW = *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+3));
                u32CRC32_SW <<= 8;
                u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+2));
                u32CRC32_SW <<= 8;
                u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+1));
                u32CRC32_SW <<= 8;
                u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+0));
                goto LOAD_CODE_POINT;
            }
        }
    }
    else
    {
        // Uncompressed format
#if 1 /*enable HW crc32 check*/
         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         //Calculate CRC32 from flash
        while (RIU8[(LOCAL_BDMA_REG_BASE + 0x02)*2] & 0x01);
        RIU[(LOCAL_BDMA_REG_BASE + 0x08 )] = (U16)((U32)(u32ROM_START&0x3fffff));
        RIU[(LOCAL_BDMA_REG_BASE + 0x0A )] = (U16)(((U32)(u32ROM_START&0x3fffff)) >> 16);
        RIU[(LOCAL_BDMA_REG_BASE + 0x0C )] = 0;
        RIU[(LOCAL_BDMA_REG_BASE + 0x0E )] = 0;
        RIU[(LOCAL_BDMA_REG_BASE + 0x10 )] = (U16)((U32)(u32ROM_END-u32ROM_START)) ;
        RIU[(LOCAL_BDMA_REG_BASE + 0x12 )] = (U16)(((U32)(u32ROM_END-u32ROM_START))>>16);
        //polynomial
        RIU[(LOCAL_BDMA_REG_BASE + 0x14 )] = (U16)(0x1DB7);
        RIU[(LOCAL_BDMA_REG_BASE + 0x16 )] = (U16)(0x04C1);
        //seed
        RIU[(LOCAL_BDMA_REG_BASE + 0x18 )] = 0;
        RIU[(LOCAL_BDMA_REG_BASE + 0x1a )] = 0;

        RIU8[(LOCAL_BDMA_REG_BASE + 0x06)*2] = 0x00;   //increasing
        RIU[(LOCAL_BDMA_REG_BASE+0x04)] = 0x0335;  //((DstDEv|DstDataWidth)  <<8) |(SrcDataWidth|SrcDEv) ;

        RIU8[ (LOCAL_BDMA_REG_BASE*2)] = 0x01 ;
        while( 1 )
        {
            if ( (RIU8[(LOCAL_BDMA_REG_BASE+0x02)*2] & 0x08) == 0x08 )
                break;
        }

        //Get CRC32 result
        u32CRC32_HW = RIU[(LOCAL_BDMA_REG_BASE + 0x18 )] ;      //low word
        u32CRC32_HW |= (RIU[(LOCAL_BDMA_REG_BASE + 0x1a )]  <<  16);        //high word

        //u32CRC32_SW = *((U8 *)(u32ROM_START+CRC32_POS+3));
        //u32CRC32_SW <<= 8;
        //u32CRC32_SW |= *((U8 *)(u32ROM_START+CRC32_POS+2));
        //u32CRC32_SW <<= 8;
        //u32CRC32_SW |= *((U8 *)(u32ROM_START+CRC32_POS+1));
        //u32CRC32_SW <<= 8;
        //u32CRC32_SW |= *((U8 *)(u32ROM_START+CRC32_POS+0));

        if ( u32CRC32_HW != u32CRC32_SW )
        {
            //AP in flash is bad, load boot loader instead
            u32ROM_START = *((U32 *)(AP_IN_FLASH_ADDR + 0));    //This address should be 4' alignment
            u32RAM_START = *((U32 *)(AP_IN_FLASH_ADDR + 4));    //This address should be 4' alignment
            u32RAM_END = *((U32 *)(AP_IN_FLASH_ADDR + 8));
            u32ROM_END = *((U32 *)(AP_IN_FLASH_ADDR + 12));    //This address should be 4' alignment

            u32CRC32_SW = *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+3));
            u32CRC32_SW <<= 8;
            u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+2));
            u32CRC32_SW <<= 8;
            u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+1));
            u32CRC32_SW <<= 8;
            u32CRC32_SW |= *((U8 *)(AP_IN_FLASH_ADDR+CRC32_POS+0));
            goto LOAD_CODE_POINT;
        }
         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        asm volatile (
        // Load ROM_START, RAM_START, RAM_END, ROM_END from 0xBFC20000
        #ifdef CONFIG_MSTAR_KAISERIN
            "li      $15, 0x94050000;" /*0xBFC20000 + u32BOOT_LOADER_LENGTH*/
        #else
            "li      $15, 0xbfc50000;" /*0xBFC20000 + u32BOOT_LOADER_LENGTH*/
        #endif
#else
        asm volatile (
        // Load ROM_START, RAM_START, RAM_END, ROM_END from 0xBFC20000
        #ifdef CONFIG_MSTAR_KAISERIN
            "li      $15, 0x94020000;"
        #else
            "li      $15, 0xBFC20000;"
        #endif
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
#endif //(USE_BDMA_FOR_CRC_CHECK!=1)

#else   // (ENABLE_BOOTING_FROM_EXT_EMMC_WITH_CPU == 0 && ENABLE_MSTAR_ROM_BOOT_WITH_EMMC_FLASH == 0)
    // Load Code from EMMC
 #include "mmc/inc/common/eMMC.h"
 #include "aesdma/src/drvAESDMA.h"
 #include "part_emmc.h"
 #define CHUNK_HEADER_SIZE 1024
 #define MMC_SECTOR_SIZE 512
 #define SIG_SIZE 256
 #define SECURITY_INFO_SIZE 16
 emmc_partition_t    mpart;

    U32 u32TempLoadAddr =  0x40000000 + ((CONFIG_DRAM_SIZE_MB_RELOCATE + 1) * 1024 * 1024);

    U32 u32_i;
    U32 u32TmpBufferAddr = u32TempLoadAddr;
    U32 u32_Err = 0;
    U32 u32_AutResult =0;
    U32 u32_RomAuthen = 0;
    U32 u32UbootLen = 0;
    U32 u32UbootRomStart = 0;
    U32 u32UbootRamStart = 0;
    U32 u32UbootRamEnd = 0;
    U32 u32PlaintextStart = 0;
    U32 u32PLaintextLen = 0;
    U32 u32SignatureStart = 0;

    uart_putc('\n');
    uart_putc('\r');
    uart_putc('E');
    uart_putc('M');
    uart_putc('M');
    uart_putc('C');

    //read rom_authen info from bank:0x0038E0[0]
    u32_RomAuthen = (*(volatile U32*)(ROM_AUTHEN_REG))&BIT0;

#if 0
    u32_Err = eMMC_Init();
    if(u32_Err)
        goto HANDLE_FAIL;
#endif

    for(u32_i=0;u32_i<= EMMC_RESERVED_FOR_MAP_V2;u32_i++)
    {
        u32_Err = eMMC_ReadData_MIU((U8*)&mpart,MMC_SECTOR_SIZE, u32_i);
         if(u32_Err)
            goto HANDLE_FAIL;

         if((mpart.name[0]=='M')&&(mpart.name[1]=='B')&&(mpart.name[2]=='O')&&(mpart.name[3]=='O')&&(mpart.name[4]=='T'))
            break;
    }
    //Read chunk Header
    u32_Err = eMMC_ReadData_MIU((U8*) u32TmpBufferAddr,CHUNK_HEADER_SIZE, mpart.start_block);

    if(u32_Err)
        goto HANDLE_FAIL;

    //authenticate chunk header
    if(u32_RomAuthen)
    {
        uart_putc('\n');
        uart_putc('\r');
        uart_putc('C');
        uart_putc('H');
        uart_putc('U');
        uart_putc('N');
        uart_putc('K');
        uart_putc('_');
        uart_putc('A');
        uart_putc('U');
        uart_putc('T');
        u32PlaintextStart = u32TmpBufferAddr;//chunk header ram start
        u32PLaintextLen = CHUNK_HEADER_SIZE - SIG_SIZE -SECURITY_INFO_SIZE;//chunk header size
        u32SignatureStart = u32PlaintextStart + u32PLaintextLen + SECURITY_INFO_SIZE;//chunk header sig start
        u32_AutResult = MDrv_AESDMA_SecureMain(u32PlaintextStart,u32PLaintextLen,u32SignatureStart,0);
        if(!u32_AutResult)
            goto HANDLE_FAIL;
        uart_putc('_');
        uart_putc('O');
        uart_putc('K');
    }

    u32UbootRomStart = *((U32 *)(u32TmpBufferAddr + 0));
    u32UbootRamStart = *((U32 *)(u32TmpBufferAddr + 4));
    u32UbootRamEnd   = *((U32 *)(u32TmpBufferAddr + 8));
    u32UbootLen = u32UbootRamEnd - u32UbootRamStart;
    u32UbootRomStart = u32UbootRomStart / 512;

    //Load uboot to DRAM
    u32UbootRomStart = u32UbootRomStart + mpart.start_block;
    u32UbootLen = ((u32UbootLen >> 9) + 2) << 9;//truncation + signature

    //for debug
    #if 0
    *(volatile U32*)(0x1f206700) = u32UbootRomStart>>16;
    *(volatile U32*)(0x1f206704) = u32UbootRomStart;
    *(volatile U32*)(0x1f206708) = u32UbootLen>>16;
    *(volatile U32*)(0x1f20670C) = u32UbootLen;
    *(volatile U32*)(0x1f206710) = u32UbootRamStart>>16;
    *(volatile U32*)(0x1f206714) = u32UbootRamStart;
    *(volatile U32*)(0x1f206718) = u32UbootRamEnd>>16;
    *(volatile U32*)(0x1f20671C) = u32UbootRamEnd;
    #endif

    u32_Err = eMMC_ReadData_MIU((U8 *)CONFIG_UBOOT_LOADADDR, u32UbootLen, u32UbootRomStart);
    if(u32_Err)
        goto HANDLE_FAIL;

    //authenticate u-boot
    if(u32_RomAuthen)
    {
        uart_putc('\n');
        uart_putc('\r');
        uart_putc('U');
        uart_putc('B');
        uart_putc('O');
        uart_putc('O');
        uart_putc('T');
        uart_putc('_');
        uart_putc('A');
        uart_putc('U');
        uart_putc('T');
        u32PlaintextStart = CONFIG_UBOOT_LOADADDR;//uboot ram start
        u32PLaintextLen = u32UbootRamEnd - u32UbootRamStart;//uboot size
        u32SignatureStart = u32PlaintextStart + u32PLaintextLen + SECURITY_INFO_SIZE;//uboot sig start
        u32_AutResult = MDrv_AESDMA_SecureMain(u32PlaintextStart, u32PLaintextLen, u32SignatureStart, 0);
        if(!u32_AutResult)
            goto HANDLE_FAIL;
        uart_putc('_');
        uart_putc('O');
        uart_putc('K');
    }
    uart_putc('\n');
    uart_putc('\r');
    uart_putc('d');
    uart_putc('o');
    uart_putc('n');
    uart_putc('e');
    uart_putc('\n');
    uart_putc('\r');

    return;

HANDLE_FAIL:
    uart_putc('_');
    uart_putc('f');
    uart_putc('a');
    uart_putc('i');
    uart_putc('l');
    uart_putc('\n');
    uart_putc('\r');
    while(1);

#endif // (ENABLE_BOOTING_FROM_EXT_EMMC_WITH_CPU == 0)
   // init_version();
}

#else

//================================================================
#if 0
void __loader_2nd Loader_PrintChunkHeader(StuChunkHeaderInfo* pstChunkHeaderInfo);
void __loader_2nd Loader_PrintChunkHeader(StuChunkHeaderInfo* pstChunkHeaderInfo)
{
}
#endif

#if( ENABLE_JOBA_BIN )

#define DEBUG_JOBA(x)   //x

void __loader_2nd Loader_Joba_Init_Aeon(U32 u32MemAddr);
void __loader_2nd Loader_Joba_Init_Aeon(U32 u32MemAddr)
{
    // Pass info to joba...
    // Set magic id
    Ld_Mem_WriteU32(u32MemAddr, JOBA_PARA_MAGIC_ID);

    // Set Aeon dram Base
    Ld_Mem_WriteU32(u32MemAddr+4, u32MemAddr);


    Ld_HAL_COPRO_Enable(u32MemAddr);
}

void __loader_2nd Loader_Joba_Init(U32 u32ApFlashAddr);
void __loader_2nd Loader_Joba_Init(U32 u32ApFlashAddr)
{
    U32 u32JobaPtr;
    U16 u16BinCount;
    U32 u32BinTableAddr = u32ApFlashAddr + AP_BIN_TABLE_START_ADDR;

    DEBUG_JOBA( Ld_printf("Loader_Joba_Init() at %u \n", Loader_MDrv_PIU_Get_Time()); );

    u32JobaPtr = Ld_MDrv_Flash_ReadU32(u32ApFlashAddr+AP_IMG_INFO_JOBA_DATA_PTR_ADDR);
    DEBUG_JOBA( Ld_printf(" u32JobaPtr=0x%X\n", u32JobaPtr); );
    if( u32JobaPtr == 0 )
    {
        DEBUG_JOBA( Ld_printf(" No Joba data\n", 0); );
        return;
    }

    // Find joba.bin from bin table...

    // Get bin number
    u16BinCount = Ld_MDrv_Flash_ReadU16_BE(u32BinTableAddr + BIN_TBL_OFFSET_BIN_COUNT);
    DEBUG_JOBA( Ld_printf(" u16BinCount=0x%X\n", u16BinCount); );
    // Check bin count valid?
    if( u16BinCount > 100 )
    {
        return ;
    }

    // Check if joba bin exist
    {
        U8 u8BinIndex;
        U32 u32BinItemAddr;
        U16 u16BinId;
        U32 u32JobaBinFlashAddr;
        U32 u32JobaBinSize;

        for( u8BinIndex = 0 ; u8BinIndex < u16BinCount; u8BinIndex += 1 )
        {
            u32BinItemAddr = u32BinTableAddr + BIN_TBL_OFFSET_BIN_ITEM_START
                             + (u8BinIndex * BIN_TBL_BIN_ITEM_SIZE);

            u16BinId = Ld_MDrv_Flash_ReadU16_BE(u32BinItemAddr);

            DEBUG_JOBA( Ld_printf(" u16BinId=0x%X\n", u16BinId); );

            if( u16BinId == 0x98 ) // Joba.bin id is 0x98
            {
                // Get joba info
                u32JobaBinFlashAddr = Ld_MDrv_Flash_ReadU32_BE(u32BinItemAddr+2);
                u32JobaBinSize = Ld_MDrv_Flash_ReadU32_BE(u32BinItemAddr+6);
                DEBUG_JOBA( Ld_printf(" u32JobaBinFlashAddr=0x%X\n", u32JobaBinFlashAddr); );
                DEBUG_JOBA( Ld_printf(" u32JobaBinSize=0x%X\n", u32JobaBinSize); );

                // BDMA joba to dram (addr=JOBA_MMAP_CODE_ADDR)
                Loader2_MDrv_BDMA_CopyFlash2Dram(u32JobaBinFlashAddr, JOBA_MMAP_CODE_ADDR, u32JobaBinSize);

                Loader_Joba_Init_Aeon(JOBA_MMAP_CODE_ADDR);
                break;
            }
        }
    }
}
#endif

//================================================================

#if( ENABLE_MSTAR_WHISKY )
    #define ENABLE_UART_DEBUG   1
#else
    #define ENABLE_UART_DEBUG   0
#endif

#if( ENABLE_UART_DEBUG )
    #define DEBUG_DECOMP(x)     x
    #define DEBUG_DECOMP_CRC(x) x
#else
    #define DEBUG_DECOMP(x)
    #define DEBUG_DECOMP_CRC(x)
#endif
void __loader_2nd DoMSDecompression(void)
{
    U8 IsCompressMagicNumber = 0;
    U32 CompressedFileLength = 0;
    U32 OriginalFileLength = 0;
    U32 u32OutputLength;
    U32 u32FlashIOAddr;
    U32 u32ROM_START = 0;
    U32 u32RAM_START = 0, u32RAM_END = 0;
    U32 u32BOOT_LOADER_LENGTH = 0;
    U32 u32ROM_END = 0;
    U32 u32CRC32_HW = 0, u32CRC32_SW = 0;
    U32 u32Header_BootLoaderSize;

    StuChunkHeaderInfo stChunkHeaderInfo;
    BOOL bLoad2ndAP = FALSE;
    U8 au8CompressInfo[3];


#if( ENABLE_AEON_UART_PRINTF /*|| ENABLE_LOADER2_AEON_UART*/)
    // Copy sboot.bin  to dram for call function
    Loader2_MDrv_BDMA_CopyFlash2Dram(0x0000, 0x0000, 0x10000 );
#endif

#if( ENABLE_AEON_UART_PRINTF )
    MDrv_InitConsole();
#elif( ENABLE_LOADER2_AEON_UART )
    Loader2_MDrv_Uart_Init();
#endif

    DEBUG_DECOMP( Ld_printf("\n========== DoMSDecompression() at %u ==========\n", Loader_MDrv_PIU_Get_Time()); );


    u32FlashIOAddr = Loader_MDrv_Flash_Get_IOAddr();
    DEBUG_DECOMP( Ld_printf("u32FlashIOAddr=0x%X\n", u32FlashIOAddr); );

    DEBUG_DECOMP( Ld_printf("AP_IN_FLASH_ADDR=0x%X\n", AP_IN_FLASH_ADDR); );


    // Get first ap chunk header info
    DEBUG_DECOMP( Ld_printf(" Get 1st chunk header at flash %X\n", AP_IN_FLASH_ADDR););
    Loader_Get_ChunkHeader(AP_IN_FLASH_ADDR, &stChunkHeaderInfo);


    // Get boot loader bin size from first ap chunker-header
    u32Header_BootLoaderSize = stChunkHeaderInfo.u32BootLoaderSize;
    DEBUG_DECOMP( Ld_printf("  u32Header_BootLoaderSize=%X\n", u32Header_BootLoaderSize););

    if( u32Header_BootLoaderSize ) // boot loader is exist
    {
        // Get u32BOOT_LOADER_LENGTH
        u32BOOT_LOADER_LENGTH = u32Header_BootLoaderSize - 0x20000; //minus 64k for sboot and 64k for pm code
    }
    else
    {
        u32BOOT_LOADER_LENGTH = 0;
    }
    DEBUG_DECOMP( Ld_printf("  u32BOOT_LOADER_LENGTH=%X\n", u32BOOT_LOADER_LENGTH););

    // If boot loader exist, get 2nd ap chunk-header
    if( u32BOOT_LOADER_LENGTH )
    {
        bLoad2ndAP = TRUE;
    }


    // Which AP? BootLoader or Main AP?
    //u32BINARY_ID = *((U32 *)(u32FlashIOAddr + CHUNK_HEADER_ADDR + 0x1C));
    //DEBUG_DECOMP( Ld_printf("u32BINARY_ID=0x%X\n", u32BINARY_ID); );

#if 1 // Aeon in NOS, flash size should 4 or 8M
    //u32REV_FLASH_SIZE = 0x800000;
#else
    if(u32BINARY_ID != 0) // should be u-boot
    {
        u32REV_FLASH_SIZE = 0x400000;
        DEBUG_DECOMP( Ld_printf(" 1. u32REV_FLASH_SIZE=0x%X\n", u32REV_FLASH_SIZE); );
    }
    else
    {
        u32REV_FLASH_SIZE = *((U8 *)((u32ROM_START+0x2000+8)+0)); //.img_info
        u32REV_FLASH_SIZE <<= 8;
        u32REV_FLASH_SIZE |= *((U8 *)((u32ROM_START+0x2000+8)+1));
        u32REV_FLASH_SIZE <<= 8;
        u32REV_FLASH_SIZE |= *((U8 *)((u32ROM_START+0x2000+8)+2));
        u32REV_FLASH_SIZE <<= 8;
        u32REV_FLASH_SIZE |= *((U8 *)((u32ROM_START+0x2000+8)+3));
        DEBUG_DECOMP( Ld_printf(" 2. u32REV_FLASH_SIZE=0x%X\n", u32REV_FLASH_SIZE); );
    }
#endif

    //DEBUG_DECOMP( Ld_printf("AP_IN_FLASH_ADDR+u32FlashAddr=0x%X\n", AP_IN_FLASH_ADDR+u32FlashAddr); );
    //DEBUG_DECOMP( Ld_printf("(AP_IN_FLASH_ADDR+u32REV_FLASH_SIZE+u32FlashAddr)=0x%X\n", (AP_IN_FLASH_ADDR+u32REV_FLASH_SIZE+u32FlashAddr)); );


    // Check magic end/start
    /*if( (u32ROM_START >= AP_IN_FLASH_ADDR)
      && (u32ROM_START < (AP_IN_FLASH_ADDR+u32REV_FLASH_SIZE))
      && (u32ROM_END >= AP_IN_FLASH_ADDR)
      && (u32ROM_END < (AP_IN_FLASH_ADDR+u32REV_FLASH_SIZE))) //u32REV_FLASH_SIZE: flash reserved size
      */
    //bLoad2ndAP = TRUE;
    if( bLoad2ndAP )
    {
        // Main AP
        DEBUG_DECOMP( Ld_puts("\n----- Load 2nd AP chunk header: -----\n"); );

        // Get 2nd ap chunk header info
        DEBUG_DECOMP( Ld_printf(" Get 2nd chunk header at flash %X\n", AP_IN_FLASH_ADDR + u32BOOT_LOADER_LENGTH););
        Loader_Get_ChunkHeader(AP_IN_FLASH_ADDR + u32BOOT_LOADER_LENGTH, &stChunkHeaderInfo);


        //u32ROM_START += u32FlashIOAddr;
        //u32ROM_END += u32FlashIOAddr;
        u32ROM_START = stChunkHeaderInfo.u32RomStart;
        u32ROM_END = stChunkHeaderInfo.u32RomEnd;
        u32RAM_START = stChunkHeaderInfo.u32RamStart;
        u32RAM_END = stChunkHeaderInfo.u32RamEnd;
        u32CRC32_SW = stChunkHeaderInfo.u32CRC;

        DEBUG_DECOMP( Ld_printf("  u32ROM_START=0x%X\n", u32ROM_START); );
        DEBUG_DECOMP( Ld_printf("  u32ROM_END=0x%X\n", u32ROM_END); );
        DEBUG_DECOMP( Ld_printf("  u32RAM_START=0x%X\n", u32RAM_START); );
        DEBUG_DECOMP( Ld_printf("  u32RAM_END=0x%X\n", u32RAM_END); );
        DEBUG_DECOMP_CRC( Ld_printf("  u32CRC32_SW=0x%X\n", u32CRC32_SW); );

        //DEBUG_DECOMP( Ld_printf("COMPRESSED_LENGTH_POS=0x%X\n", (U32)COMPRESSED_LENGTH_POS); );

        // Check magic end/start
        {
            U32 u32MAGIC_END_ADDR, u32MAGIC_END, u32MAGIC_START;

            // Check AP magic ID ...
            DEBUG_DECOMP( Ld_printf("MagicId_1 addr=0x%X\n", u32ROM_START+AP_IMG_INFO_MAGIC_ID_ADDR); );
            //u32MAGIC_START = Ld_MDrv_Flash_ReadU32(u32ROM_START+COMPRESSED_LENGTH_POS-0x4A);
            u32MAGIC_START = Ld_MDrv_Flash_ReadU32(u32ROM_START+AP_IMG_INFO_MAGIC_ID_ADDR);
            DEBUG_DECOMP( Ld_printf("u32MAGIC_START=0x%X\n", u32MAGIC_START); );
            if(u32MAGIC_START != 0x55AA5678)
            {
                //DEBUG_DECOMP( Ld_puts("u32MAGIC_START != 0x55AA5678\n"); );
                Ld_puts("u32MAGIC_START != 0x55AA5678\n");
                goto LOAD_LOADER_AP;
            }


        #if 1
            DEBUG_DECOMP( Ld_printf("u32MAGIC_END_ADDR addr=0x%X\n", u32ROM_START+AP_IMG_INFO_MAGIC_ID_2_ADDR); );
            u32MAGIC_END_ADDR = Ld_MDrv_Flash_ReadU32(u32ROM_START+AP_IMG_INFO_MAGIC_ID_2_ADDR);
        #else
            u32MAGIC_END_ADDR = *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS+4)+3));
            u32MAGIC_END_ADDR <<= 8;
            u32MAGIC_END_ADDR |= *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS+4)+2));
            u32MAGIC_END_ADDR <<= 8;
            u32MAGIC_END_ADDR |= *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS+4)+1));
            u32MAGIC_END_ADDR <<= 8;
            u32MAGIC_END_ADDR |= *((U8 *)((u32ROM_START+COMPRESSED_LENGTH_POS+4)+0));
        #endif

            DEBUG_DECOMP( Ld_printf("u32MAGIC_END_ADDR=0x%X\n", u32MAGIC_END_ADDR); );

        #if 1
            u32MAGIC_END = Ld_MDrv_Flash_ReadU32(u32MAGIC_END_ADDR);
        #else
            u32MAGIC_END_ADDR += u32FlashIOAddr;
            DEBUG_DECOMP( Ld_printf("u32MAGIC_END_ADDR=0x%X\n", u32MAGIC_END_ADDR); );

            u32MAGIC_END = *((U8 *)(u32MAGIC_END_ADDR+3));
            u32MAGIC_END <<= 8;
            u32MAGIC_END |= *((U8 *)(u32MAGIC_END_ADDR+2));
            u32MAGIC_END <<= 8;
            u32MAGIC_END |= *((U8 *)(u32MAGIC_END_ADDR+1));
            u32MAGIC_END <<= 8;
            u32MAGIC_END |= *((U8 *)(u32MAGIC_END_ADDR+0));
        #endif
            DEBUG_DECOMP( Ld_printf("u32MAGIC_END=0x%X\n", u32MAGIC_END); );
            if( u32MAGIC_END != 0x55AAABCD )
            {
                //DEBUG_DECOMP( Ld_puts("u32MAGIC_END != 0x55AAABCD\n"); );
                Ld_puts("u32MAGIC_END != 0x55AAABCD\n");
                goto LOAD_LOADER_AP;
            }

        }
    }
    else
    {
        // Loader 1st AP
LOAD_LOADER_AP:

        DEBUG_DECOMP( Ld_puts("\n----- Get 1st AP chunk header: -----\n"); );

        // Use first AP(No matter what it is)
        Loader_Get_ChunkHeader(AP_IN_FLASH_ADDR, &stChunkHeaderInfo);

        u32ROM_START = stChunkHeaderInfo.u32RomStart;
        u32ROM_END = stChunkHeaderInfo.u32RomEnd;
        u32RAM_START = stChunkHeaderInfo.u32RamStart;
        u32RAM_END = stChunkHeaderInfo.u32RamEnd;
        u32CRC32_SW = stChunkHeaderInfo.u32CRC;

        DEBUG_DECOMP( Ld_printf(" u32ROM_START=0x%X\n", u32ROM_START); );
        DEBUG_DECOMP( Ld_printf(" u32ROM_END=0x%X\n", u32ROM_END); );
        DEBUG_DECOMP( Ld_printf(" u32RAM_START=0x%X\n", u32RAM_START); );
        DEBUG_DECOMP( Ld_printf(" u32RAM_END=0x%X\n", u32RAM_END); );
        DEBUG_DECOMP_CRC( Ld_printf(" u32CRC32_SW=0x%X\n", u32CRC32_SW); );
    }



LOAD_CODE_POINT:
    DEBUG_DECOMP( Ld_printf("\n----- LOAD_CODE_POINT: at %u -----\n", Loader_MDrv_PIU_Get_Time()); );

    au8CompressInfo[0] = Ld_MDrv_Flash_ReadU8( u32ROM_START+AP_IMG_INFO_COMPRESS_INFO_ADDR+0 );
    au8CompressInfo[1] = Ld_MDrv_Flash_ReadU8( u32ROM_START+AP_IMG_INFO_COMPRESS_INFO_ADDR+1 );
    au8CompressInfo[2] = Ld_MDrv_Flash_ReadU8( u32ROM_START+AP_IMG_INFO_COMPRESS_INFO_ADDR+2 );
    DEBUG_DECOMP( Ld_printf(" au8CompressInfo[0]=0x%x\n", au8CompressInfo[0]); );
    DEBUG_DECOMP( Ld_printf(" au8CompressInfo[1]=0x%x\n", au8CompressInfo[1]); );
    DEBUG_DECOMP( Ld_printf(" au8CompressInfo[2]=0x%x\n", au8CompressInfo[2]); );

    // Check if this bin file is compressed format, find compress format magic number
    if( (au8CompressInfo[2] == 0xDE )
     && (au8CompressInfo[1] == 0xAD )
      )
    {
        U8 u8CompressFormat = au8CompressInfo[0];

        DEBUG_DECOMP( Ld_printf(" u8CompressFormat=0x%x\n", u8CompressFormat); );

        if ( u8CompressFormat == 0xBE )
        {
            IsCompressMagicNumber = 1; // comp
        }
        else if ( u8CompressFormat == 0xEF )
        {
            IsCompressMagicNumber = 2; // comp7
        }
        else
        {
            IsCompressMagicNumber = 0;
        }
    }
    else
    {
        IsCompressMagicNumber = 0;
    }

    DEBUG_DECOMP( Ld_printf(" IsCompressMagicNumber=%u\n", IsCompressMagicNumber); );


    //Compressed Format
    if ( IsCompressMagicNumber )
    {
        U32 u32DecompDstMemAddr;
        U32 u32DecompDstMemSize;
        U32 u32DecompSrcMemAddr;
        U32 u32DecompSrcMemSize;
        U32 u32DecompTmpMemAddr;


        DEBUG_DECOMP( Ld_puts("\n----- AP is compressed format -----\n"); );

        //Calculate CRC32 from flash
        DEBUG_DECOMP( Ld_printf(" Start CRC check at %u \n", Loader_MDrv_PIU_Get_Time()); );
        u32CRC32_HW = Loader2_MDrv_BDMA_CalCRC(u32ROM_START, u32ROM_END-u32ROM_START, BDMA_CRC_TYPE_FLASH);
        DEBUG_DECOMP_CRC( Ld_printf(" u32CRC32_HW=0x%X\n", u32CRC32_HW); );
        DEBUG_DECOMP( Ld_printf(" CRC check done at %u \n", Loader_MDrv_PIU_Get_Time()); );
        if ( u32CRC32_HW != u32CRC32_SW )
        {
            Ld_printf("CRC error: u32CRC32_HW=0x%X,", u32CRC32_HW);
            Ld_printf(" u32CRC32_SW=0x%X\n", u32CRC32_SW);

            //AP in flash is bad, load boot loader instead
            goto LOAD_CODE_POINT;
        }


        // Get compressed file size
        CompressedFileLength = Ld_MDrv_Flash_ReadU32( u32ROM_START+AP_IMG_INFO_COMPRESS_LEN_ADDR );
        CompressedFileLength -= 6;
        DEBUG_DECOMP( Ld_printf("  CompressedFileLength=0x%X\n", CompressedFileLength); );

        // Get Original file size
        if( IsCompressMagicNumber == 1 ) // comp
        {
            OriginalFileLength = Ld_MDrv_Flash_ReadU32( u32ROM_START+CompressedFileLength);
        }
        else if( IsCompressMagicNumber == 2 )// comp7
        {
            //OriginalFileLength = 0x800000; // 8M
            OriginalFileLength = Ld_MDrv_Flash_ReadU32( u32ROM_START+UNCOMPRESSED_LENGTH+5);
        }
        else
        {
            OriginalFileLength = 0x800000; // 8M
        }
        DEBUG_DECOMP( Ld_printf("  OriginalFileLength=0x%X\n", OriginalFileLength); );

        // Setup memory for decomp
        u32DecompDstMemAddr = u32RAM_START + UNCOMPRESSED_LENGTH;
        u32DecompDstMemSize = OriginalFileLength;

    #if defined(__aeon__)
         u32DecompSrcMemAddr = MMAP_DECOMP_SRC_BUF_ADDR;
    #else
        u32DecompSrcMemAddr = u32DecompDstMemAddr + u32DecompDstMemSize;
        u32DecompSrcMemAddr = ALIGNED_VALUE(u32DecompSrcMemAddr, 0x10000); // Size = CompressedFileLength
    #endif

        u32DecompSrcMemSize = ALIGNED_VALUE(CompressedFileLength, 0x10000);

        u32DecompTmpMemAddr = u32DecompSrcMemAddr + u32DecompSrcMemSize;

        DEBUG_DECOMP_CRC( Ld_printf(" u32DecompDstMemAddr=0x%X\n", u32DecompDstMemAddr); );
        DEBUG_DECOMP_CRC( Ld_printf(" u32DecompDstMemSize=0x%X\n", u32DecompDstMemSize); );
        DEBUG_DECOMP_CRC( Ld_printf(" u32DecompSrcMemAddr=0x%X\n", u32DecompSrcMemAddr); );
        DEBUG_DECOMP_CRC( Ld_printf(" u32DecompSrcMemSize=0x%X\n", u32DecompSrcMemSize); );
        DEBUG_DECOMP_CRC( Ld_printf(" u32DecompTmpMemAddr=0x%X\n", u32DecompTmpMemAddr); );


        DEBUG_DECOMP( Ld_printf(" BDMA flash to dram at %u \n", Loader_MDrv_PIU_Get_Time()); );

        //Copy uncompressed part first
        Loader2_MDrv_BDMA_CopyFlash2Dram(u32ROM_START, u32RAM_START, UNCOMPRESSED_LENGTH );

        //Copy compressed part second
        //Loader2_MDrv_BDMA_CopyFlash2Dram(u32ROM_START + UNCOMPRESSED_LENGTH, MMAP_DECOMP_SRC_BUF_ADDR, u32RAM_END-u32RAM_START-UNCOMPRESSED_LENGTH );
        Loader2_MDrv_BDMA_CopyFlash2Dram(u32ROM_START + UNCOMPRESSED_LENGTH, u32DecompSrcMemAddr, u32RAM_END-u32RAM_START-UNCOMPRESSED_LENGTH );


    #if( ENABLE_JOBA_BIN )
        Loader_Joba_Init(u32ROM_START);
    #endif


        // Start to decompress
        if( IsCompressMagicNumber == 1 )
        {
            DEBUG_DECOMP( Ld_printf(" Start decomp at %u \n", Loader_MDrv_PIU_Get_Time()); );

            //MemoryAllocatorInit((U8 *)MEMORY_POOL_CACHEABLE, MEMORY_POOL_SIZE);
            MemoryAllocatorInit((U8 *)u32DecompTmpMemAddr, MEMORY_POOL_SIZE);

            //MsDecompress((U8 *)DECOMPRESS_BUF_CACHEABLE, (U8 *)((u32RAM_START + UNCOMPRESSED_LENGTH) & NON_CACHEABLE_TO_CACHEABLE_MASK), CompressedFileLength, OriginalFileLength, &u32OutputLength);
            MsDecompress(   (U8 *)u32DecompSrcMemAddr, // src
                            //(U8 *)((u32RAM_START + UNCOMPRESSED_LENGTH) & NON_CACHEABLE_TO_CACHEABLE_MASK), // Dst
                            //(U8 *)((u32DecompDstMemAddr) & NON_CACHEABLE_TO_CACHEABLE_MASK), // Dst
                            (U8 *)u32DecompDstMemAddr, // Dst
                            CompressedFileLength, // srcLen
                            OriginalFileLength, // DstLen
                            &u32OutputLength);

            if ( u32OutputLength != OriginalFileLength )
            {
                //Decompression fail! Jump to boot loader to recover FW
                //Ld_puts("Decomp fail!\n");
                Ld_printf("\n Decomp fail! u32OutputLength=%u\n", u32OutputLength );
                while(1){}
            }
            else
            {
                //DEBUG_DECOMP( Ld_puts("Decomp OK\n"); );
                DEBUG_DECOMP( Ld_printf(" decomp done at %u \n", Loader_MDrv_PIU_Get_Time()); );
            }
        }
        else
        {
            BOOL bDecomp7Result;

            //DEBUG_DECOMP( Ld_puts("Start decomp7...\n"); );
            DEBUG_DECOMP( Ld_printf(" Start decomp7 at %u \n", Loader_MDrv_PIU_Get_Time()); );

            //MemoryAllocatorInit((U8 *)MEMORY_POOL_CACHEABLE, MEMORY_POOL_SIZE);
            MemoryAllocatorInit((U8 *)u32DecompTmpMemAddr, MEMORY_POOL_SIZE);

            bDecomp7Result = MsDecompress7(
                //(U8 *)DECOMPRESS_BUF_CACHEABLE, // src
                (U8 *)u32DecompSrcMemAddr, // src
                //(U8 *)((u32RAM_START + UNCOMPRESSED_LENGTH) & NON_CACHEABLE_TO_CACHEABLE_MASK), // dst
                (U8 *)u32DecompDstMemAddr, // dst
                CompressedFileLength+4-UNCOMPRESSED_LENGTH // srcLen
            );

            if ( bDecomp7Result != TRUE )
            {
                //Decompression fail! Jump to boot loader to recover FW
                Ld_puts("Decomp7 fail!\n");
                while(1){}
            }
            else
            { //do d-cache flush
                U32 u32DstAddr, u32Len;
                //DEBUG_DECOMP( Ld_puts("Decomp7 OK\n"); );

                //u32DstAddr = (U32)((u32RAM_START + UNCOMPRESSED_LENGTH) & NON_CACHEABLE_TO_CACHEABLE_MASK);
                u32DstAddr = u32DecompDstMemAddr;
                u32Len = (U32)(CompressedFileLength+4-UNCOMPRESSED_LENGTH);
                mhal_dcache_flush(u32DstAddr,u32Len);

                DEBUG_DECOMP( Ld_printf(" Decomp7 done at %u \n", Loader_MDrv_PIU_Get_Time()); );
            }
        }
    }
    else
    {
        DEBUG_DECOMP( Ld_puts("\n----- AP is not compressed format: -----\n"); );

        DEBUG_DECOMP( Ld_printf(" Start CRC check at %u \n", Loader_MDrv_PIU_Get_Time()); );

        //Calculate CRC32 from flash
        u32CRC32_HW = Loader2_MDrv_BDMA_CalCRC(u32ROM_START, u32ROM_END-u32ROM_START, BDMA_CRC_TYPE_FLASH);
        if ( u32CRC32_HW != u32CRC32_SW )
        {
            //AP in flash is bad, load boot loader instead
            goto LOAD_CODE_POINT;
        }
        DEBUG_DECOMP( Ld_printf(" CRC check done at %u \n", Loader_MDrv_PIU_Get_Time()); );

        // Copy ap.bin from flash to dram
        // Uncompressed format
        Loader2_MDrv_BDMA_CopyFlash2Dram(u32ROM_START, u32RAM_START, u32RAM_END - u32RAM_START );
    }

   // init_version();

    //Ld_puts("Reboot to AP...\n");
    Ld_printf("\n===== Reboot to AP... at %u =====\n", Loader_MDrv_PIU_Get_Time());

#ifdef __AEONR2__

  #if 1//(ENABLE_MSTAR_NASA||ENABLE_MSTAR_EDEN)
    ReBootAtDRAM(u32RAM_START);
  #elif (ENABLE_MSTAR_MARIA10 || ENABLE_MSTAR_MACAW12 || ENABLE_MSTAR_EDEN || ENABLE_MSTAR_EULER || ENABLE_MSTAR_NASA || ENABLE_MSTAR_MAYA  || ENABLE_MSTAR_WHISKY)
    Boot2DRAM();
  #else
    // Move from bootaeonsysinit.c
    // wait 51 polling this register and then reset to new vector table address.
    if(u32RAM_START==0)
    {
        __asm__ __volatile__ (
            "\tl.syncwritebuffer\n"
            "\tl.jr     %0\n"
            "\tl.syncwritebuffer\n"
            : : "r" (0x100));
    }
    else
    {
        RIU[0x1002B4] = u32RAM_START >> 16;
    }
  #endif

    while(1);
#endif

}

#endif
#endif
