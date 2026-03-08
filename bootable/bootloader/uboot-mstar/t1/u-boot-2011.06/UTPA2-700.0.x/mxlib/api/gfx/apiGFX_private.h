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
////////////////////////////////////////////////////////////////////////////////

#ifndef _APIGOP_PRIV_H_
#define _APIGOP_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
/// @file drvBDMA.h
/// @brief Byte DMA control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

//Local define
#define PATCH_T3_SCK_FAIL 1 //[2009.07.02]T3/U3 GE HW issue:set source alpha is max for filtering the background color

#define GFX_DEBUGINFO_LEVEL(a, x)           if (g_apiGFXLocal.u32dbglvl <= a) {MS_DEBUG_MSG(x);}
#define GFX_DBG_HDR     "[GFXDBG]"

#define GFX_DEBUGINFO(x)                    //MS_DEBUG_MSG(x)

#undef GFX_ASSERT
#ifdef GFX_DEBUG
#define GFX_ASSERT(_bool, _f)                 if (!(_bool)) { MS_CRITICAL_MSG(_f); while (1); }
#else
#define GFX_ASSERT(_bool, _f)                 while (0)
#endif // #ifdef SCL_DEBUG

#include "ULog.h"

// Debug Logs, level form low(INFO) to high(FATAL, always show)
// Function information, ex function entry
#define GFX_INFO(x, args...) ULOGI("GE API", x, ##args)
// Warning, illegal paramter but can be self fixed in functions
#define GFX_WARN(x, args...) ULOGW("GE API", x, ##args)
//  Need debug, illegal paramter.
#define GFX_DBUG(x, args...) ULOGD("GE API", x, ##args)
// Error, function will be terminated but system not crash
#define GFX_ERR(x, args...) ULOGE("GE API", x, ##args)
// Critical, system crash. (ex. assert)
#define GFX_FATAL(x, args...) ULOGF("GE API", x, ##args)

typedef struct
{
    GFX_Set_Colorkey*           pColorKey;              //////////ColorKey
    GFX_Set_ROP*                pROP;                   //////////ROP
}GFX_Function;

typedef struct _GFX_SetConfig
{
    MS_BOOL                     bOnePixelMode;          //////////OnePixelMode
    MS_BOOL                     bDither;                //////////Dither
    MS_BOOL                     bVQEnable;              //////////Virual Cmd Queue
    MS_BOOL                     bABLEnable;             //////////Alpha Blending Enable
    GFX_RotateAngle             eRotate;                //////////Rotate
    GFX_Set_Mirror*             pMirror;                //////////Mirror
    MS_U32                      u32TagID;               //////////TagID
    GFX_Set_Clip_Property*      pClip;                  //////////Clip
}GFX_SetConfig;

typedef struct
{
    GFX_FireInfo*               pFireInfo;
    GFX_OvalFillInfo*           pDrawOvalInfo;
}GFX_Set_DrawOvalInfo;

void            GFXRegisterToUtopia                 (FUtopiaOpen ModuleType);
MS_U32          GFXOpen                             (void** ppInstance, const void* const pAttribute);
MS_U32          GFXClose                            (void* pInstance);
MS_U32          GFXIoctl                            (void* pInstance, MS_U32 u32Cmd, void* pArgs);

#ifdef __cplusplus
}
#endif
#endif
