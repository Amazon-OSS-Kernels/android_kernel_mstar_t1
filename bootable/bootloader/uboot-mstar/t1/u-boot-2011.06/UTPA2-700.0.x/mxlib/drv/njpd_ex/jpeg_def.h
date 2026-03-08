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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    jpeg_def.h
/// @brief  JPEG internal definition
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _JPEG_DEF_H_
#define _JPEG_DEF_H_

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
#define JPEG_STATIC     static

#define MSOS_GET_SYSTEM_TIME    1


#if defined(CHIP_EDISON)   // need to modify njpeg_def.h and jpeg_def.h simontaneously
#define SUPPORT_OJPD                    TRUE
#else
#define SUPPORT_OJPD                    FALSE
#endif

//JPD IP supporting feature
#define JPD_SUPPORT_3_HUFFMAN_TABLE                             FALSE//TRUE
#define JPD_SUPPORT_AUTO_PROTECT                                FALSE//TRUE

#define ENABLE_TEST_09_JPEGWriteProtectTest                     FALSE
#define ENABLE_TEST_11_JPEGScaleDownFunctionTest_2              FALSE
#define ENABLE_TEST_11_JPEGScaleDownFunctionTest_4              FALSE
#define ENABLE_TEST_11_JPEGScaleDownFunctionTest_8              FALSE
#define ENABLE_TEST_16_JPEGEnablePsaveModeTest                  FALSE
#define ENABLE_TEST_18_miu_sel_128M                             FALSE
#define ENABLE_TEST_18_miu_sel_64M                              FALSE
#define ENABLE_TEST_18_miu_sel_32M                              FALSE
#define ENABLE_TEST_22_AutoProtectFailTest                      FALSE

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define JPEG_BLOCK_TYPE         MS_S16
//------------------------------------------------------------------------------
#define JPEG_QUANT_TYPE         MS_S16
//------------------------------------------------------------------------------
// May need to be adjusted if support for other colorspaces/sampling factors is added
#define JPEG_MAXBLOCKSPERMCU    10
//------------------------------------------------------------------------------
#define JPEG_MAXHUFFTABLES      8
//------------------------------------------------------------------------------
#define JPEG_MAXQUANTTABLES     4
//------------------------------------------------------------------------------
#define JPEG_MAXCOMPONENTS      4
//------------------------------------------------------------------------------
#define JPEG_MAXCOMPSINSCAN     4
//------------------------------------------------------------------------------
//Increase this if you increase the max width!
#define JPEG_MAXBLOCKSPERROW    6144

#if 0//(defined (MSOS_TYPE_NOS) || defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_ECOS))
#define JPEG_memcpy(pDstAddr, pSrcAddr, u32Size)                \
    do {                                                        \
        MS_U32 idx = 0;                                         \
        volatile MS_U8 *Dest = (volatile MS_U8 *)(pDstAddr);    \
        volatile MS_U8 *Src = (volatile MS_U8 *)(pSrcAddr);     \
        for (idx = 0; idx < (u32Size); idx++)                   \
        {                                                       \
            Dest[idx] = Src[idx];                               \
        }                                                       \
    }while(0)

#define JPEG_memset(pDstAddr, u8value, u32Size)                 \
    do {                                                        \
        MS_U32 idx = 0;                                         \
        volatile MS_U8 *Dest = (volatile MS_U8 *)(pDstAddr);    \
        for (idx = 0; idx < (u32Size); idx++)                   \
        {                                                       \
            Dest[idx] = (u8value);                              \
        }                                                       \
    }while(0)
#else
#define JPEG_memcpy(pDstAddr, pSrcAddr, u32Size) memcpy((pDstAddr), (pSrcAddr), (u32Size))
#define JPEG_memset(pDstAddr, u8value, u32Size)  memset((pDstAddr), (u8value), (u32Size))
#endif

#define JPEG_AtoU32(pData, u32value) \
    do { \
        u32value = 0; \
        while(('0' <= *pData) && ('9' >= *pData)) \
        { \
            u32value = (10 * u32value) + (MS_U32)(*pData - '0'); \
            pData++; \
        } \
    }while(0)

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Description: JPEG scan type
typedef enum
{
    E_JPEG_GRAYSCALE    = 0
  , E_JPEG_YH1V1        = 1
  , E_JPEG_YH2V1        = 2
  , E_JPEG_YH1V2        = 3
  , E_JPEG_YH2V2        = 4
  , E_JPEG_YH4V1        = 5
//#if SW_JPD_RGB_CMYK
  , E_JPEG_CMYK         = 6
  , E_JPEG_RGB          = 7
//#endif
}JPEG_ScanType;
//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: JPEG_QuantTbl
/// @brief \b Struct \b Description: Structure for JPEG Quantization Table
//-----------------------------------------------------------------------------
typedef struct //same as JPD_QuanTbl in drvJPD.h
{
    JPEG_QUANT_TYPE s16Value[64];   ///< value of Q table
    MS_BOOL bValid;                 ///< has Q table or not.
} JPEG_QuantTbl;

//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: JPEG_HuffInfo
/// @brief \b Struct \b Description: Structure for JPEG huffman information
//-----------------------------------------------------------------------------
typedef struct
{
    MS_U8 u8Huff_num[17];   ///< number of Huffman codes per bit size
    MS_U8 u8Huff_val[256];  ///< Huffman codes per bit size

    MS_U8 u8Symbol[17];     ///< u8Huff_num in reverse order
    MS_U16 u16Code[17];     ///< Minimun code word
    MS_BOOL bValid;         ///< has huffman table or not
} JPEG_HuffInfo;
//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: JPEG_HuffTbl
/// @brief \b Struct \b Description: Structure for JPEG huffman table
//-----------------------------------------------------------------------------
typedef struct
{
    MS_S16 s16Look_up[256]; ///< the lookup of huffman code
    // FIXME: Is 512 tree entries really enough to handle _all_ possible
    // code sets? I think so but not 100% positive.
    MS_S16 s16Tree[512];    ///< huffman tree of huffman code
    MS_U8 u8Code_size[256]; ///< code size of huffman code
} JPEG_HuffTbl;


#endif //_JPEG_DEF_H_

