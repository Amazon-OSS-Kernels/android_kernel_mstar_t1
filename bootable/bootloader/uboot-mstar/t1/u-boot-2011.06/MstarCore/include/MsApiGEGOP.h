/**
* Copyright (c) 2006 ¡V 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of
* the GNU General Public License as published by the Free Software Foundation;
* either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program;
* if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
* MA 02111-1307, USA.
*/

#ifndef _MS_API_GEGOP_
#define _MS_API_GEGOP_
#include <apiGFX.h>
#include <apiGOP.h>
#include <ShareType.h>

typedef enum
{
   eTextAlignLeft,
   eTextAlignMiddle,
   eTextAlignRight

} TextAttrib;

typedef enum
{
	font32X32 = 0,
	font16X16,
	font_max = 7
}FontSize;

typedef struct
{
	MS_U8 font_width;
	MS_U8 font_height;
	MS_U8 space_width;
	MS_U8 interval_width;
	unsigned char *pCharTable;
	unsigned char *pFontWidthTable;

}FontInfo;

void MsApiSelectFont(MS_U8 font_choose);
void MsApiDisplay_Logo(MS_U8 u8logoGopIdx, GFX_DrawRect *rect,GFX_BufferInfo *src_info,GFX_BufferInfo *dst_info, MS_BOOL bHorStretch, MS_BOOL bVerStretch,MS_U16 u16DispX,MS_U16 u16DispY);

void MsApiOsdCreate(MS_U8 u8logoGopIdx, GFX_Block* pBlk, U32 u32GopBuffer);

void MsApiOsdDestroy(void);

void MsApiFlushCanvas2Screen(void);

void MsApiDrawRect(GFX_Block* pBlk, GFX_RgbColor color);

void MsApiDrawPixel(GFX_Point p, GFX_RgbColor color);

void MsApiDrawStrText(const char* pStrText, MS_U16 X, MS_U16 Y, GFX_RgbColor color, TextAttrib eTextAttrib);

void MsApiDrawProgress(MS_U16 X, MS_U16 Y, GFX_RgbColor fcolor, MS_U8 per);

void MsApiDrawJPG(GFX_BufferInfo src_info,GFX_Block Blk);

#endif

