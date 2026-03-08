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

#ifndef _VER_JPD_H_
#define _VER_JPD_H_
#include <MsTypes.h>

//#include <display_logo/MsCommon.h>

#define TEST_02_Baseline1600x1200 1
#define TEST_03_Baseline352x512 2
#define TEST_04_BaselineYUV400  3
#define TEST_05_BaselineYUV411  4
#define TEST_06_BaselineYUV420  5
#define TEST_07_BaselineYUV422  6
#define TEST_08_BaselineYUV440  7
#define TEST_09_BaselineYUV444  8
#define TEST_RST_3Q             9
#define TEST_10_OtherTest       10

#define BASELINE    TEST_07_BaselineYUV422
typedef enum
{
    E_JPD_MAIN_DECODE       = 1,
    E_JPD_THUMBNAIL_DECODE
} EN_JPEG_DECODE_TYPE;

typedef enum
{
    E_JPEG_IDLE = 0x01,
    E_JPEG_DECODE_ERR = 0x02,
    E_JPEG_DECODE_HEADER = 0x03,
    E_JPEG_DECODE_BEGIN = 0x04,
    E_JPEG_DECODE_DONE = 0x05,
    E_JPEG_WAITDONE = 0x06,
    E_JPEG_INIT = 0x07,
} EN_JPEG_DECODE_STATE;




//void MsDrv_Main(MS_U32 u32SrcAddr, MS_U32 u32SrcSize);
int MsDrv_JPD_Decode(MS_U32 u32SrcAddr, MS_U32 u32SrcSize);

int JPG_GetAlignmentWidth(void);
int JPG_GetAlignmentHeight(void);
int JPG_GetAlignmentPitch(void);
MS_U32 JPG_GetOutRawDataAddr(void);
void MsDrv_JpdInit(void);
void MsDrv_JpdStartDecode(EN_JPEG_DECODE_TYPE decode_type);

#endif /* _VER_JPD_H_ */

