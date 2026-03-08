/**
* Copyright (c) 2006 – 2018 MStar Semiconductor, Inc.
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
//******************************************************************************
//******************************************************************************
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


#ifndef _DRV_MFE_ST_H_
#define _DRV_MFE_ST_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_OUTBUFFER 10

typedef enum
{
BITS_INFO_UNKNOW=0,
BITS_INFO_FRAMEDONE,
BITS_INFO_BUFFULL,
BITS_INFO_IMG_BUFFULL,
} BITS_STATUS_INFO;

typedef enum
{
GETBITS_NOT_READY=0,
GETBITS_READY=1,
} MFE_HW_STATUS;

typedef struct {
    unsigned long miuAddress;    //hardware physical
    unsigned long miuPointer;    //need to OR 0xA0000000
    unsigned long virtual_addr;
    long size;
    BITS_STATUS_INFO status;	// 1: frame_done. 2: buffer full,3: image buffer full.
    int is_more_bits; //1: there are more bitstream packet;  0: this is the last bitstream packet
    unsigned char  voptype;
    unsigned long IVOP_address;
} BITSFRAME_INFO;

/* Linear memory area descriptor */
typedef struct MEMMAP
{
    //U32 *miuPointer;
    unsigned char *miuPointer;
    unsigned int miuAddress;
    unsigned int size;
} MEMMAP_t;

typedef struct{
    unsigned int is_off;
    unsigned int clock_level;
} POWERON_t;

// MADP-layer "simplified" encoder configuration
// NOTE: "Advanced" configurations are hidden inside DRV-layer.
typedef struct{
	// MFE-compliant
	int width;
	int height;
	unsigned long BitsBuffStart;
	unsigned long BitsBuffSize;
       int nOutBuffer;
       int OutBufferSize;
	MEMMAP_t BitsOutBuffer[MAX_OUTBUFFER];
	// MFE new settings
	int nCodecType;
	int nDispWidth, nDispHeight;	// Display resolution
	int nBufWidth, nBufHeight;		// Image buffer resolution (aligned to 16 or 32 byte, depends on nCodecType)
	unsigned char bInterlace;
	// GOP
	int nPbetweenI;
	int nBbetweenP;
       int nP_is_infinite;
	// Rate control
	int FrameRatex100;
	int nBitrate;

	// Video length
	int nTotalFrames;	// -1 means infinite. 0 is not allowed.
	char file_name[50];
	int test_case_format;
	int test_case_num;
	unsigned char CModel_pattern[20*8];//20 frames
}PVR_Info;


#ifdef __cplusplus
}
#endif

#endif
