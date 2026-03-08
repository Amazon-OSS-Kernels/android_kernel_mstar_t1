/**
* Copyright (c) 2006 ¡V 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef _MMAP_ID_H_
#define _MMAP_ID_H_
//-------------------------Note !!-------------------------------
// This file was generated in Middleware/Makefile automatically
// Do not modify it ...
//---------------------------------------------------------------

/// MMapp item enum
typedef enum
{
    E_MMAP_ID_VDEC_CPU=0,
    E_MMAP_ID_VDEC_FRAMEBUFFER,
    E_MMAP_ID_VDEC_BITSTREAM,
    E_MMAP_ID_VDEC_SUB_FRAMEBUFFER,
    E_MMAP_ID_VDEC_SUB_BITSTREAM,
    E_MMAP_ID_VDEC_MVC_BITSTREAM,
    E_MMAP_ID_VDEC_MVC_FRAMEBUFFER,
    E_MMAP_ID_VDEC_FRAMEBUFFER_SD,
    E_MMAP_ID_VDEC_BITSTREAM_SD,
    E_MMAP_ID_VDEC_SHARE_MEM,
    E_MMAP_ID_MAD_DEC,
    E_MMAP_ID_MAD_SE,
    E_MMAP_ID_MAD_R2,
    E_MMAP_ID_ARMFW_MEM,
    E_MMAP_ID_OPTEE_MEM,
    E_MMAP_ID_HW_AES_BUF,
    E_MMAP_ID_NUTTX_RAMLOG,
    E_SECURE_SHM,
    E_SECURE_UPDATE_AREA,
    E_SECURE_TSP,
    E_MMAP_ID_HW_SECURE_BUFFER,
    E_MMAP_ID_DIP_MEM,
    E_DFB_JPD_WRITE,
    E_DFB_FRAMEBUFFER,
    E_MMAP_ID_PVR_DOWNLOAD,
    E_MMAP_ID_PVR_UPLOAD,
    E_MMAP_ID_PVR_SECURE_DOWNLOAD,
    E_MMAP_ID_PVR_SECURE_UPLOAD,
    E_MMAP_ID_DMX_VQUEUE,
    E_MIU1_INTERVAL,
    E_MIU2_INTERVAL,
    E_MMAP_ID_MAX   //Must be placed at the end
} EN_MMAP_ID;

#endif // _MMAP_ID_H_
