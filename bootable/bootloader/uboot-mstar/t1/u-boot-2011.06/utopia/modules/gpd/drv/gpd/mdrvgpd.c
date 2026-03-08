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
#include "MsCommon.h"
#include "gpd.h"
#include "mdrvgpd.h"
#include "gif_lib.h"
#include "png.h"
#include "drvgpd.h"

#define ERROR_HANDLE_AFTER_DECODE     TRUE
#define MAX_ERROR_HANDLE_COUNT        10

static gpd_addr_info g_gpd_addr;

extern MS_VIRT GPD_REG_BASE;
extern MS_VIRT READBITBASE;

void mdrv_gpd_setbuf(gpd_addr_info* addr_info)
{

    console_printf("mdrv_gpd_setbuf\n");


    GPD_REG_BASE = addr_info->RegBaseAddr;
    READBITBASE = addr_info->ReadBitBaseAddr;
    console_printf("GPD_REG_BASE = 0x%tx\n", (ptrdiff_t)GPD_REG_BASE);
    console_printf("READBITBASE = 0x%tx\n", (ptrdiff_t)READBITBASE);

    memcpy((void*)&g_gpd_addr, (void*)addr_info, sizeof(gpd_addr_info));
}

MS_S32 mdrv_gpd_init(gpd_addr_info* addr_info)
{

    console_printf("mdrv_gpd_init\n");

    mdrv_gpd_setbuf(addr_info);
    drv_gpd_set_zbuf_bas((addr_info->u32PA_DepthBufferAddr)>>3);
    drv_gpd_set_cbuf_bas((addr_info->u32PA_ColorBufferAddr)>>3);
    drv_gpd_init_chip_specific_reg();
    return 0;
}

MS_S32 mdrv_gpd_decode(gpd_pic_info *pic_info, MS_U32 Ocolor, MS_U32 mode, gpd_roi_info *roi)
{
    MS_U8 sig[5] = {0};
    GifFileType *GifFile = NULL;
    PngInfop pnginfo = NULL;


    console_printf("mdrv_gpd_decode\n");

    if(mode != 3)
    {
        sig[0] = MMIO8(g_gpd_addr.u32VA_ReadBufferAddr);
        sig[1] = MMIO8(g_gpd_addr.u32VA_ReadBufferAddr+1);
        sig[2] = MMIO8(g_gpd_addr.u32VA_ReadBufferAddr+2);
        sig[3] = MMIO8(g_gpd_addr.u32VA_ReadBufferAddr+3);
    }

    console_printf("Signaute = %x,%x,%x,%x\n",sig[0],sig[1],sig[2],sig[3]);


    if ((sig[0] == 'G' && sig[1] == 'I' && sig[2] == 'F') || (mode == 3))
    {
        /* GIF Decode*/
        console_printf("GIF Decode!!\n");

        GifFile = MstDGifProcess((void*)&g_gpd_addr, Ocolor, mode, (void *)roi);
        if (GifFile == NULL)
        {
            console_printf("GIF Decode error!!\n");
        }
        if(!GifFile)
        {
            pic_info->enGPDStatus = -1;
        }
        else
        {
            //pic_info->enGPDStatus = 0;
            pic_info->enGPDStatus = GifFile->GifState;
            pic_info->u32Width = GifFile->SWidth;
            pic_info->u32Height = GifFile->SHeight;
            if(GifFile->SColorMap != NULL)
                pic_info->u8BitPP = GifFile->SColorMap->BitsPerPixel;
            pic_info->u8Interlace = GifFile->Image.Interlace;
            pic_info->u8MGIF = GifFile->isMGIF;
        }
    }
    else
    {
        /* PNG Decode*/
        if ((sig[0] == 0x89) && (sig[1] == 'P') && (sig[2] == 'N') && (sig[3] == 'G'))
        {

            console_printf("PNG Decode!!\n");

#if ERROR_HANDLE_AFTER_DECODE
            MS_U8 u8Cnt;
            for(u8Cnt=0; u8Cnt< MAX_ERROR_HANDLE_COUNT; u8Cnt++)
            {
                pnginfo = PngProcess((void *)&g_gpd_addr, Ocolor, mode, (void *)roi);
                if(pnginfo)
                {
                    console_printf("Decode Done!!!");
                    break;
                }
                else
                {
                    console_printf("Decode error!!! re-decode again!!!");
                }
            }
#else
            pnginfo = PngProcess((void *)&g_gpd_addr, Ocolor, mode, (void *)roi);
#endif

        }
        else
        {
            console_printf("Unknown decode format!!\n");
            pnginfo = NULL;
        }
        if(!pnginfo)
        {
            pic_info->enGPDStatus = -1;
        }
        else
        {
            pic_info->enGPDStatus   = 0;
            pic_info->u32Width      = pnginfo->png_width;
            pic_info->u32Height     = pnginfo->png_height;
            pic_info->u8BitPP       = pnginfo->bpp;
            pic_info->u8Interlace   = pnginfo->interlace;
            pic_info->u8MGIF        = 0;
        }
    }

    return pic_info->enGPDStatus;
}

void mdrv_gpd_scaling_init(MS_U32 enable, MS_U32 mode)
{
    drv_gpd_set_scaling(enable, mode);
}
