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

#ifndef _API_MVOP_PRIV_H_
#define _API_MVOP_PRIV_H_


////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct __attribute__((packed))
{
    MS_U32 Dummy;
}MVOP_RESOURCE_PRIVATE;

typedef void                         (*IOCTL_MVOP_INIT)                                 (void*);
typedef void                         (*IOCTL_MVOP_EXIT)                                 (void*);
typedef void                         (*IOCTL_MVOP_ENABLE)                                 (void*, MS_BOOL);
typedef MVOP_Result                         (*IOCTL_MVOP_SETINPUTCFG)                                 (void*, MVOP_InputSel, MVOP_InputCfg*);
typedef MVOP_Result                         (*IOCTL_MVOP_SETOTPUTCFG)                                 (void*, MVOP_VidStat*, MS_BOOL);
typedef void                         (*IOCTL_MVOP_ENABLEBLACKBG)                                 (void*);
typedef void                         (*IOCTL_MVOP_ENABLEUVSHIFT)                                 (void*, MS_BOOL);
typedef void                         (*IOCTL_MVOP_SETMONOMODE)                                 (void*, MS_BOOL);

typedef MS_U16                         (*IOCTL_MVOP_GETHSIZE)                                 (void*);
typedef MS_U16                         (*IOCTL_MVOP_GETVSIZE)                                 (void*);
typedef MS_U16                         (*IOCTL_MVOP_GETHSTART)                                 (void*);
typedef MS_U16                         (*IOCTL_MVOP_GETVSTART)                                 (void*);
typedef MS_BOOL                         (*IOCTL_MVOP_GETISINTERLACE)                                 (void*);
typedef MS_BOOL                         (*IOCTL_MVOP_GETISHDUPLICATE)                                 (void*);
typedef MVOP_Result                         (*IOCTL_MVOP_GETISENABLE)                                 (void*, MS_BOOL*);
typedef MVOP_Result                         (*IOCTL_MVOP_GETOUTPUTTIMING)                                 (void*, MVOP_Timing*);
typedef MVOP_Result                         (*IOCTL_MVOP_GETLIBVER)                                 (void*, const MSIF_Version **);
typedef MS_BOOL                         (*IOCTL_MVOP_CHECKCAPABILITY)                                 (void*, MS_U16, MS_U16, MS_U16);
typedef MS_U16                         (*IOCTL_MVOP_GETMAXHOFFSET)                                 (void*, MS_U16, MS_U16, MS_U16);
typedef MS_U16                         (*IOCTL_MVOP_GETMAXVOFFSET)                                 (void*, MS_U16, MS_U16, MS_U16);

typedef void                         (*IOCTL_MVOP_SETDBGLEVEL)                                 (void*, MS_U8);
typedef const MVOP_DrvInfo*                         (*IOCTL_MVOP_GETINFO)                     (void*);
typedef MS_BOOL                         (*IOCTL_MVOP_GETSTATUS)                                 (void*, MVOP_DrvStatus*);
typedef void                      (*IOCTL_MVOP_SETCLK)                                 (void*, MS_BOOL);
typedef void                         (*IOCTL_MVOP_SETPATTERN)                                 (void*, MVOP_Pattern);
typedef MS_BOOL                         (*IOCTL_MVOP_SETTILEFORMAT)                                 (void*, MVOP_TileFormat);
typedef MS_BOOL                         (*IOCTL_MVOP_GETDSTINFO)                                 (void*, MVOP_DST_DispInfo*, MS_U32);
typedef MS_BOOL                         (*IOCTL_MVOP_SETFIXVTT)                                 (void*, MS_U16);
typedef MS_BOOL                         (*IOCTL_MVOP_SETMMIOMAPBASE)                                 (void*);
typedef MVOP_Result                         (*IOCTL_MVOP_MIUSWITCH)                                 (void*, MS_U8);
typedef MS_BOOL                         (*IOCTL_MVOP_SETBASEADD)                                 (void*, MS_PHY, MS_PHY, MS_BOOL, MS_BOOL);
typedef void                         (*IOCTL_MVOP_SELOPFIELD)                                 (void*, MS_BOOL);
typedef void                         (*IOCTL_MVOP_SETREGSIZEFROMMVD)                                 (void*, MS_BOOL);
typedef void                         (*IOCTL_MVOP_SETSTARTPOS)                                 (void*, MS_U16, MS_U16);
typedef MS_BOOL                         (*IOCTL_MVOP_SETIMAGEWIDTHHIGHT)                                 (void*, MS_U16, MS_U16);
typedef void                         (*IOCTL_MVOP_SETVOPMIRRORMODE)                                 (void*, MS_BOOL, MVOP_DrvMirror);
typedef void                         (*IOCTL_MVOP_INVOPVS)                                 (void*, MS_BOOL);
typedef void                         (*IOCTL_MVOP_FORCETOP)                                 (void*, MS_BOOL);
typedef MS_BOOL                         (*IOCTL_MVOP_ENABLEFREERUNMODE)                                 (void*, MS_BOOL);
typedef void                         (*IOCTL_MVOP_GETBASEADD)                                 (void*, MS_PHY*, MS_PHY*);

//Functions for Sub MVOP
//Basic control functions
typedef void                         (*IOCTL_MVOP_SUBINIT)                                 (void*);
typedef void                         (*IOCTL_MVOP_SUBEXIT)                                 (void*);
typedef void                         (*IOCTL_MVOP_SUBENABLE)                                 (void*, MS_BOOL);
typedef MVOP_Result                         (*IOCTL_MVOP_SUBSETINPUTCFG)                                 (void*, MVOP_InputSel, MVOP_InputCfg*);
typedef MVOP_Result                         (*IOCTL_MVOP_SUBSETOTPUTCFG)                                 (void*, MVOP_VidStat *, MS_BOOL);

//Advanced control functions
typedef void                         (*IOCTL_MVOP_SUBENABLEBLACKBG)                                 (void*);
typedef void                         (*IOCTL_MVOP_SUBENABLEUVSHIFT)                                 (void*, MS_BOOL);
typedef void                         (*IOCTL_MVOP_SUBSETMONOMODE)                                 (void*, MS_BOOL);
typedef MS_BOOL                         (*IOCTL_MVOP_SUBGETSTATUS)                                 (void*, MVOP_DrvStatus*);
typedef MS_U16                         (*IOCTL_MVOP_SUBGETHSIZE)                                 (void*);
typedef MS_U16                         (*IOCTL_MVOP_SUBGETVSIZE)                                 (void*);
typedef MS_U16                         (*IOCTL_MVOP_SUBGETHSTART)                                 (void*);
typedef MS_U16                         (*IOCTL_MVOP_SUBGETVSTART)                                 (void*);
typedef MS_BOOL                         (*IOCTL_MVOP_SUBGETISINTERLACE)                                 (void*);
typedef MS_BOOL                         (*IOCTL_MVOP_SUBGETISHDUPLICATE)                                 (void*);
typedef MVOP_Result                         (*IOCTL_MVOP_SUBGETISENABLE)                                 (void*, MS_BOOL*);
typedef MVOP_Result                         (*IOCTL_MVOP_SUBGETOUTPUTTIMING)                                 (void*, MVOP_Timing*);
typedef MS_BOOL                         (*IOCTL_MVOP_SUBCHECKCAPABILITY)                                 (void*, MS_U16, MS_U16, MS_U16);
typedef MS_U16                         (*IOCTL_MVOP_SUBGETMAXHOFFSET)                                 (void*, MS_U16, MS_U16, MS_U16);
typedef MS_U16                         (*IOCTL_MVOP_SUBGETMAXVOFFSET)                                 (void*, MS_U16, MS_U16, MS_U16);
typedef void                        (*IOCTL_MVOP_SUBSETCLK)                                 (void*, MS_BOOL);
typedef void                         (*IOCTL_MVOP_SUBSETPATTERN)                                 (void*, MVOP_Pattern);
typedef MS_BOOL                         (*IOCTL_MVOP_SUBSETTILEFORMAT)                                 (void*, MVOP_TileFormat);
typedef MS_BOOL                         (*IOCTL_MVOP_SUBGETDSTINFO)                                 (void*, MVOP_DST_DispInfo*, MS_U32);
typedef MS_BOOL                         (*IOCTL_MVOP_SUBSETFIXVTT)                                 (void*, MS_U16);
typedef MS_BOOL                         (*IOCTL_MVOP_SUBSETMMIOMAPBASE)                                 (void*);
typedef MVOP_Result                         (*IOCTL_MVOP_SUBMIUSWITCH)                                 (void*, MS_U8);
typedef MS_BOOL                         (*IOCTL_MVOP_SUBSETBASEADD)                                 (void*, MS_PHY, MS_PHY, MS_BOOL, MS_BOOL);
typedef void                         (*IOCTL_MVOP_SUBSETVOPMIRRORMODE)                                 (void*, MS_BOOL, MVOP_DrvMirror);
typedef MS_BOOL                         (*IOCTL_MVOP_SUBENABLEFREERUNMODE)                                 (void*, MS_BOOL);
typedef void                         (*IOCTL_MVOP_SUBGETBASEADD)                             (void*, MS_PHY*, MS_PHY*);

// { Functions for the 3rd MVOP
//Basic control functions
typedef MVOP_Result                         (*IOCTL_MVOP_EXINIT)                                 (void*, MVOP_DevID, MS_U32);
typedef MVOP_Result                         (*IOCTL_MVOP_EXEXIT)                                 (void*, MVOP_DevID, MS_U32);
typedef MVOP_Result                         (*IOCTL_MVOP_EXENABLE)                                 (void*, MVOP_DevID, MS_BOOL);
typedef MVOP_Result                         (*IOCTL_MVOP_EXSETINPUTCFG)                                 (void*, MVOP_DevID, MVOP_InputSel, MVOP_InputCfg*);
typedef MVOP_Result                         (*IOCTL_MVOP_EXSETOTPUTCFG)                                 (void*, MVOP_DevID, MVOP_VidStat*, MS_BOOL);

//Advanced control functions
typedef void                        (*IOCTL_MVOP_EXSETCLK)                                 (void*, MVOP_DevID, MS_BOOL);
typedef MVOP_Result                         (*IOCTL_MVOP_EXSETPATTERN)                                 (void*, MVOP_DevID, MVOP_Pattern);
typedef MVOP_Result                         (*IOCTL_MVOP_EXSETTILEFORMAT)                                 (void*, MVOP_DevID, MVOP_TileFormat);
typedef MVOP_Result                         (*IOCTL_MVOP_EXENABLEUVSHIFT)                                 (void*, MVOP_DevID, MS_BOOL);
typedef MVOP_Result                         (*IOCTL_MVOP_EXENABLEBLACKBG)                                 (void*, MVOP_DevID);
typedef MVOP_Result                         (*IOCTL_MVOP_EXSETMONOMODE)                                 (void*, MVOP_DevID, MS_BOOL);
typedef MVOP_Result                         (*IOCTL_MVOP_EXSETFIXVTT)                                 (void*, MVOP_DevID, MS_U16);
typedef MVOP_Result                         (*IOCTL_MVOP_EXMIUSWITCH)                                 (void*, MVOP_DevID, MS_U8);
typedef MVOP_Result                         (*IOCTL_MVOP_EXSETVOPMIRRORMODE)                                 (void*, MVOP_DevID, MS_BOOL, MVOP_DrvMirror);
typedef MVOP_Result                         (*IOCTL_MVOP_EXENABLEFREERUNMODE)                                 (void*, MVOP_DevID, MS_BOOL);

// Query functions
typedef MVOP_Result                         (*IOCTL_MVOP_EXGETOUTPUTTIMING)                                 (void*, MVOP_DevID, MVOP_Timing*);
typedef MVOP_Result                         (*IOCTL_MVOP_EXGETISENABLE)                                 (void*, MVOP_DevID, MS_BOOL*);
typedef MS_U16                         (*IOCTL_MVOP_EXGETHSIZE)                                 (void*, MVOP_DevID);
typedef MS_U16                         (*IOCTL_MVOP_EXGETVSIZE)                                 (void*, MVOP_DevID);
typedef MS_U16                         (*IOCTL_MVOP_EXGETHSTART)                                 (void*, MVOP_DevID);
typedef MS_U16                         (*IOCTL_MVOP_EXGETVSTART)                                 (void*, MVOP_DevID);
typedef MS_BOOL                         (*IOCTL_MVOP_EXGETISINTERLACE)                                 (void*, MVOP_DevID);
typedef MS_BOOL                         (*IOCTL_MVOP_EXGETISHDUPLICATE)                                 (void*, MVOP_DevID);
typedef MS_BOOL                         (*IOCTL_MVOP_EXGETSTATUS)                                 (void*, MVOP_DevID, MVOP_DrvStatus*);
typedef MS_BOOL                         (*IOCTL_MVOP_EXCHECKCAPABILITY)                                 (void*, MVOP_DevID, MS_U32);
typedef MS_BOOL                         (*IOCTL_MVOP_EXGETDSTINFO)                                 (void*, MVOP_DevID, MVOP_DST_DispInfo*, MS_U32);

typedef MVOP_Result                         (*IOCTL_MVOP_SENDBLUESCREEN)                                 (void*, MS_U16, MS_U16);
typedef MVOP_Result                         (*IOCTL_MVOP_SETCOMMAND)                                 (void*, MVOP_Handle*, MVOP_Command, void*);
typedef MVOP_Result                         (*IOCTL_MVOP_GETCOMMAND)                                 (void*, MVOP_Handle*, MVOP_Command, void*, MS_U32);
typedef void                         (*IOCTL_MVOP_SETFREQUENCY)                                 (void*, MVOP_FREQUENCY);

typedef MS_BOOL                         (*IOCTL_MVOP_ENABLEINTERRUPT)                                 (void*, MS_U8);
typedef MS_U8                         (*IOCTL_MVOP_GETINTSTATUS)                                 (void*);
typedef MS_BOOL                         (*IOCTL_MVOP_SUBENABLEINTERRUPT)                                 (void*, MVOP_IntType);
typedef MS_U8                         (*IOCTL_MVOP_SUBGETINTSTATUS)                                 (void*);
typedef MS_BOOL                         (*IOCTL_MVOP_EXENABLEINTERRUPT)                                 (void*, MVOP_DevID, MVOP_IntType);
typedef MS_U8                         (*IOCTL_MVOP_EXGETINTSTATUS)                                 (void*, MVOP_DevID);

typedef MS_U32                         (*IOCTL_MVOP_SETPOWERSTATE)                                 (void*, EN_POWER_MODE);


// OBSOLETE FUNCTION POINTER END



typedef struct
{
    IOCTL_MVOP_INIT                                 fpMVOP_Init;
    IOCTL_MVOP_EXIT                                 fpMVOP_Exit;
    IOCTL_MVOP_ENABLE                               fpMVOP_Enable;
    IOCTL_MVOP_SETINPUTCFG                          fpMVOP_SetInputCfg;
    IOCTL_MVOP_SETOTPUTCFG                          fpMVOP_SetOutputCfg;
    IOCTL_MVOP_ENABLEBLACKBG                        fpMVOP_EnableBlackBG;
    IOCTL_MVOP_ENABLEUVSHIFT                        fpMVOP_EnableUVShift;
    IOCTL_MVOP_SETMONOMODE                          fpMVOP_SetMonoMode;

    IOCTL_MVOP_GETHSIZE                             fpMVOP_GetHSize;
    IOCTL_MVOP_GETVSIZE                             fpMVOP_GetVSize;
    IOCTL_MVOP_GETHSTART                            fpMVOP_GetHStart;
    IOCTL_MVOP_GETVSTART                            fpMVOP_GetVStart;
    IOCTL_MVOP_GETISINTERLACE                       fpMVOP_GetIsInterlace;
    IOCTL_MVOP_GETISHDUPLICATE                      fpMVOP_GetIsHDuplicate;
    IOCTL_MVOP_GETISENABLE                          fpMVOP_GetIsEnable;
    IOCTL_MVOP_GETOUTPUTTIMING                      fpMVOP_GetOutputTiming;
    IOCTL_MVOP_GETLIBVER                            fpMVOP_GetLibVer;
    IOCTL_MVOP_CHECKCAPABILITY                      fpMVOP_CheckCapability;
    IOCTL_MVOP_GETMAXHOFFSET                        fpMVOP_GetMaxHOffset;
    IOCTL_MVOP_GETMAXVOFFSET                        fpMVOP_GetMaxVOffset;

    IOCTL_MVOP_SETDBGLEVEL                          fpMVOP_SetDbgLevel;
    IOCTL_MVOP_GETINFO                              fpMVOP_GetInfo;
    IOCTL_MVOP_GETSTATUS                            fpMVOP_GetStatus;
    IOCTL_MVOP_SETCLK                               fpMVOP_SetClk;
    IOCTL_MVOP_SETPATTERN                           fpMVOP_SetPattern;
    IOCTL_MVOP_SETTILEFORMAT                        fpMVOP_SetTileFormat;
    IOCTL_MVOP_GETDSTINFO                           fpMVOP_GetDstInfo;
    IOCTL_MVOP_SETFIXVTT                            fpMVOP_SetFixVtt;
    IOCTL_MVOP_SETMMIOMAPBASE                       fpMVOP_SetMMIOMapBase;
    IOCTL_MVOP_MIUSWITCH                            fpMVOP_MiuSwitch;
    IOCTL_MVOP_SETBASEADD                           fpMVOP_SetBaseAdd;
    IOCTL_MVOP_SELOPFIELD                           fpMVOP_SelOPField;
    IOCTL_MVOP_SETREGSIZEFROMMVD                    fpMVOP_SetRegSizeFromMVD;
    IOCTL_MVOP_SETSTARTPOS                          fpMVOP_SetStartPos;
    IOCTL_MVOP_SETIMAGEWIDTHHIGHT                   fpMVOP_SetImageWidthHight;
    IOCTL_MVOP_SETVOPMIRRORMODE                     fpMVOP_SetVOPMirrorMode;
    IOCTL_MVOP_INVOPVS                              fpMVOP_InvOPVS;
    IOCTL_MVOP_FORCETOP                             fpMVOP_ForceTop;
    IOCTL_MVOP_ENABLEFREERUNMODE                    fpMVOP_EnableFreerunMode;
    IOCTL_MVOP_GETBASEADD                           fpMVOP_GetBaseAdd;

    //Sub MVOP
    IOCTL_MVOP_SUBINIT                              fpMVOP_SubInit;
    IOCTL_MVOP_SUBEXIT                              fpMVOP_SubExit;
    IOCTL_MVOP_SUBENABLE                            fpMVOP_SubEnable;
    IOCTL_MVOP_SUBSETINPUTCFG                       fpMVOP_SubSetInputCfg;
    IOCTL_MVOP_SUBSETOTPUTCFG                       fpMVOP_SubSetOutputCfg;

    IOCTL_MVOP_SUBENABLEBLACKBG                     fpMVOP_SubEnableBlackBG;
    IOCTL_MVOP_SUBENABLEUVSHIFT                     fpMVOP_SubEnableUVShift;
    IOCTL_MVOP_SUBSETMONOMODE                       fpMVOP_SubSetMonoMode;
    IOCTL_MVOP_SUBGETSTATUS                         fpMVOP_SubGetStatus;
    IOCTL_MVOP_SUBGETHSIZE                          fpMVOP_SubGetHSize;
    IOCTL_MVOP_SUBGETVSIZE                          fpMVOP_SubGetVSize;
    IOCTL_MVOP_SUBGETHSTART                         fpMVOP_SubGetHStart;
    IOCTL_MVOP_SUBGETVSTART                         fpMVOP_SubGetVStart;
    IOCTL_MVOP_SUBGETISINTERLACE                    fpMVOP_SubGetIsInterlace;
    IOCTL_MVOP_SUBGETISHDUPLICATE                   fpMVOP_SubGetIsHDuplicate;
    IOCTL_MVOP_SUBGETISENABLE                       fpMVOP_SubGetIsEnable;
    IOCTL_MVOP_SUBGETOUTPUTTIMING                   fpMVOP_SubGetOutputTiming;
    IOCTL_MVOP_SUBCHECKCAPABILITY                   fpMVOP_SubCheckCapability;
    IOCTL_MVOP_SUBGETMAXHOFFSET                     fpMVOP_SubGetMaxHOffset;
    IOCTL_MVOP_SUBGETMAXVOFFSET                     fpMVOP_SubGetMaxVOffset;
    IOCTL_MVOP_SUBSETCLK                            fpMVOP_SubSetClk;
    IOCTL_MVOP_SUBSETPATTERN                        fpMVOP_SubSetPattern;
    IOCTL_MVOP_SUBSETTILEFORMAT                     fpMVOP_SubSetTileFormat;
    IOCTL_MVOP_SUBGETDSTINFO                        fpMVOP_SubGetDstInfo;
    IOCTL_MVOP_SUBSETFIXVTT                         fpMVOP_SubSetFixVtt;
    IOCTL_MVOP_SUBSETMMIOMAPBASE                    fpMVOP_SubSetMMIOMapBase;
    IOCTL_MVOP_SUBMIUSWITCH                         fpMVOP_SubMiuSwitch;
    IOCTL_MVOP_SUBSETBASEADD                        fpMVOP_SubSetBaseAdd;
    IOCTL_MVOP_SUBSETVOPMIRRORMODE                  fpMVOP_SubSetVOPMirrorMode;
    IOCTL_MVOP_SUBENABLEFREERUNMODE                 fpMVOP_SubEnableFreerunMode;
    IOCTL_MVOP_SUBGETBASEADD                        fpMVOP_SubGetBaseAdd;

    //Third MVOP
    IOCTL_MVOP_EXINIT                               fpMVOP_ExInit;
    IOCTL_MVOP_EXEXIT                               fpMVOP_ExExit;
    IOCTL_MVOP_EXENABLE                             fpMVOP_ExEnable;
    IOCTL_MVOP_EXSETINPUTCFG                        fpMVOP_ExInputCfg;
    IOCTL_MVOP_EXSETOTPUTCFG                        fpMVOP_ExOutputCfg;

    IOCTL_MVOP_EXSETCLK                             fpMVOP_ExSetClk;
    IOCTL_MVOP_EXSETPATTERN                         fpMVOP_ExSetPattern;
    IOCTL_MVOP_EXSETTILEFORMAT                      fpMVOP_ExSetTileFormat;
    IOCTL_MVOP_EXENABLEUVSHIFT                      fpMVOP_ExEnableUVShift;
    IOCTL_MVOP_EXENABLEBLACKBG                      fpMVOP_ExEnableBlackBG;
    IOCTL_MVOP_EXSETMONOMODE                        fpMVOP_ExSetMonoMode;
    IOCTL_MVOP_EXSETFIXVTT                          fpMVOP_ExSetFixVtt;
    IOCTL_MVOP_EXMIUSWITCH                          fpMVOP_ExMiuSwitch;
    IOCTL_MVOP_EXSETVOPMIRRORMODE                   fpMVOP_ExSetVOPMirrorMode;
    IOCTL_MVOP_EXENABLEFREERUNMODE                  fpMVOP_ExEnableFreerunMode;

    IOCTL_MVOP_EXGETOUTPUTTIMING                    fpMVOP_ExGetOutputTiming;
    IOCTL_MVOP_EXGETISENABLE                        fpMVOP_ExGetIsEnable;
    IOCTL_MVOP_EXGETHSIZE                           fpMVOP_ExGetHSize;
    IOCTL_MVOP_EXGETVSIZE                           fpMVOP_ExGetVSize;
    IOCTL_MVOP_EXGETHSTART                          fpMVOP_ExGetHStart;
    IOCTL_MVOP_EXGETVSTART                          fpMVOP_ExGetVStart;
    IOCTL_MVOP_EXGETISINTERLACE                     fpMVOP_ExGetIsInterlace;
    IOCTL_MVOP_EXGETISHDUPLICATE                    fpMVOP_ExGetIsHDuplicate;
    IOCTL_MVOP_EXGETSTATUS                          fpMVOP_ExGetStatus;
    IOCTL_MVOP_EXCHECKCAPABILITY                    fpMVOP_ExCheckCapability;
    IOCTL_MVOP_EXGETDSTINFO                         fpMVOP_ExGetDstInfo;

    IOCTL_MVOP_SENDBLUESCREEN                       fpMVOP_SendBlueScreen;
    IOCTL_MVOP_SETCOMMAND                           fpMVOP_SetCommand;
    IOCTL_MVOP_GETCOMMAND                           fpMVOP_GetCommand;
    IOCTL_MVOP_SETFREQUENCY                         fpMVOP_SetFrequency;

    IOCTL_MVOP_ENABLEINTERRUPT                      fpMVOP_EnableInterrupt;
    IOCTL_MVOP_GETINTSTATUS                         fpMVOP_GetIntStatus;
    IOCTL_MVOP_SUBENABLEINTERRUPT                   fpMVOP_SubEnableInterrupt;
    IOCTL_MVOP_SUBGETINTSTATUS                      fpMVOP_SubGetIntStatus;
    IOCTL_MVOP_EXENABLEINTERRUPT                    fpMVOP_ExEnableInterrupt;
    IOCTL_MVOP_EXGETINTSTATUS                       fpMVOP_ExGetIntStatus;

    IOCTL_MVOP_SETPOWERSTATE                        fpMVOP_SetPowerState_Kernel;

    // OBSOLETE FUNCTION POINTER END
    // avoid using these functions pointers!!!!
}MVOP_INSTANCE_PRIVATE;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void MDrv_MVOP_Init_V2 (void* pInstance);
void MDrv_MVOP_Exit_V2(void* pInstance);
void MDrv_MVOP_Enable_V2(void* pInstance, MS_BOOL bEnable);
MVOP_Result MDrv_MVOP_SetInputCfg_V2 (void* pInstance, MVOP_InputSel in, MVOP_InputCfg* pCfg);
MVOP_Result MDrv_MVOP_SetOutputCfg_V2(void* pInstance, MVOP_VidStat *pstVideoStatus, MS_BOOL bEnHDup);
void MDrv_MVOP_EnableBlackBG_V2 (void* pInstance);
void MDrv_MVOP_EnableUVShift_V2(void* pInstance, MS_BOOL bEnable);
void MDrv_MVOP_SetMonoMode_V2(void* pInstance, MS_BOOL bEnable);

MS_U16 MDrv_MVOP_GetHSize_V2(void* pInstance);
MS_U16 MDrv_MVOP_GetVSize_V2(void* pInstance);
MS_U16 MDrv_MVOP_GetHStart_V2(void* pInstance);
MS_U16 MDrv_MVOP_GetVStart_V2(void* pInstance);
MS_BOOL MDrv_MVOP_GetIsInterlace_V2(void* pInstance);
MS_BOOL MDrv_MVOP_GetIsHDuplicate_V2(void* pInstance);
MVOP_Result MDrv_MVOP_GetIsEnable_V2 ( void* pInstance, MS_BOOL* pbEnable );
MVOP_Result MDrv_MVOP_GetOutputTiming_V2(void* pInstance, MVOP_Timing* pMVOPTiming);
MVOP_Result MDrv_MVOP_GetLibVer_V2(void* pInstance, const MSIF_Version **ppVersion);
MS_BOOL MDrv_MVOP_CheckCapability_V2(void* pInstance, MS_U16 u16HSize, MS_U16 u16VSize, MS_U16 u16Fps);
MS_U16 MDrv_MVOP_GetMaxHOffset_V2(void* pInstance, MS_U16 u16HSize, MS_U16 u16VSize, MS_U16 u16Fps);
MS_U16 MDrv_MVOP_GetMaxVOffset_V2(void* pInstance, MS_U16 u16HSize, MS_U16 u16VSize, MS_U16 u16Fps);

void MDrv_MVOP_SetDbgLevel_V2(void* pInstance, MS_U8 level);
const MVOP_DrvInfo* MDrv_MVOP_GetInfo_V2(void* pInstance);
MS_BOOL MDrv_MVOP_GetStatus_V2(void* pInstance, MVOP_DrvStatus *pMVOPStat);
void MDrv_MVOP_SetClk_V2 (void* pInstance, MS_BOOL bEnable );
void MDrv_MVOP_SetPattern_V2(void* pInstance, MVOP_Pattern enMVOPPattern);
MS_BOOL MDrv_MVOP_SetTileFormat_V2(void* pInstance, MVOP_TileFormat eTileFmt);
MS_BOOL MDrv_MVOP_GetDstInfo_V2(void* pInstance, MVOP_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo);
MS_BOOL MDrv_MVOP_SetFixVtt_V2(void* pInstance, MS_U16 u16FixVtt);
MS_BOOL MDrv_MVOP_SetMMIOMapBase_V2(void* pInstance);
MVOP_Result MDrv_MVOP_MiuSwitch_V2(void* pInstance, MS_U8 u8Miu);
MS_BOOL MDrv_MVOP_SetBaseAdd_V2(void* pInstance, MS_PHY u32YOffset, MS_PHY u32UVOffset, MS_BOOL bProgressive, MS_BOOL b422pack);
void MDrv_MVOP_SEL_OP_FIELD_V2(void* pInstance, MS_BOOL bEnable);
void MDrv_MVOP_SetRegSizeFromMVD_V2(void* pInstance, MS_BOOL bEnable);
void MDrv_MVOP_SetStartPos_V2(void* pInstance, MS_U16 u16Xpos ,MS_U16 u16Ypos);
MS_BOOL MDrv_MVOP_SetImageWidthHight_V2(void* pInstance, MS_U16 u16Width ,MS_U16 u16Height);
void MDrv_MVOP_SetVOPMirrorMode_V2(void* pInstance, MS_BOOL bEnable,MVOP_DrvMirror eMirrorMode);
void MDrv_MVOP_INV_OP_VS_V2(void* pInstance, MS_BOOL bEnable);
void MDrv_MVOP_FORCE_TOP_V2(void* pInstance, MS_BOOL bEnable);
MS_BOOL MDrv_MVOP_EnableFreerunMode_V2(void* pInstance, MS_BOOL bEnable);
void MDrv_MVOP_GetBaseAdd_V2(void* pInstance, MS_PHY* u32YOffset, MS_PHY* u32UVOffset);

//Functions for Sub MVOP
void MDrv_MVOP_SubInit_V2(void* pInstance);
void MDrv_MVOP_SubExit_V2(void* pInstance);
void MDrv_MVOP_SubEnable_V2(void* pInstance, MS_BOOL bEnable);
MVOP_Result MDrv_MVOP_SubSetInputCfg_V2 (void* pInstance, MVOP_InputSel in, MVOP_InputCfg* pCfg);
MVOP_Result MDrv_MVOP_SubSetOutputCfg_V2(void* pInstance, MVOP_VidStat *pstVideoStatus, MS_BOOL bEnHDup);

void MDrv_MVOP_SubEnableBlackBG_V2(void* pInstance);
void MDrv_MVOP_SubEnableUVShift_V2(void* pInstance, MS_BOOL bEnable);
void MDrv_MVOP_SubSetMonoMode_V2(void* pInstance, MS_BOOL bEnable);
MS_BOOL MDrv_MVOP_SubGetStatus_V2(void* pInstance, MVOP_DrvStatus *pMVOPStat);
MS_U16 MDrv_MVOP_SubGetHSize_V2(void* pInstance);
MS_U16 MDrv_MVOP_SubGetVSize_V2(void* pInstance);
MS_U16 MDrv_MVOP_SubGetHStart_V2(void* pInstance);
MS_U16 MDrv_MVOP_SubGetVStart_V2(void* pInstance);
MS_BOOL MDrv_MVOP_SubGetIsInterlace_V2(void* pInstance);
MS_BOOL MDrv_MVOP_SubGetIsHDuplicate_V2(void* pInstance);
MVOP_Result MDrv_MVOP_SubGetIsEnable_V2 (void* pInstance, MS_BOOL* pbEnable);
MVOP_Result MDrv_MVOP_SubGetOutputTiming_V2(void* pInstance, MVOP_Timing* pMVOPTiming);
MS_BOOL MDrv_MVOP_SubCheckCapability_V2(void* pInstance, MS_U16 u16HSize, MS_U16 u16VSize, MS_U16 u16Fps);
MS_U16 MDrv_MVOP_SubGetMaxHOffset_V2(void* pInstance, MS_U16 u16HSize, MS_U16 u16VSize, MS_U16 u16Fps);
MS_U16 MDrv_MVOP_SubGetMaxVOffset_V2(void* pInstance, MS_U16 u16HSize, MS_U16 u16VSize, MS_U16 u16Fps);
void MDrv_MVOP_SubSetClk_V2 (void* pInstance, MS_BOOL bEnable );
void MDrv_MVOP_SubSetPattern_V2(void* pInstance, MVOP_Pattern enMVOPPattern);
MS_BOOL MDrv_MVOP_SubSetTileFormat_V2(void* pInstance, MVOP_TileFormat eTileFmt);
MS_BOOL MDrv_MVOP_SubGetDstInfo_V2(void* pInstance, MVOP_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo);
MS_BOOL MDrv_MVOP_SubSetFixVtt_V2(void* pInstance, MS_U16 u16FixVtt);
MS_BOOL MDrv_MVOP_SubSetMMIOMapBase_V2(void* pInstance);
MVOP_Result MDrv_MVOP_SubMiuSwitch_V2(void* pInstance, MS_U8 u8Miu);
MS_BOOL MDrv_MVOP_SubSetBaseAdd_V2(void* pInstance, MS_PHY u32YOffset, MS_PHY u32UVOffset, MS_BOOL bProgressive, MS_BOOL b422pack);
void MDrv_MVOP_SubGetBaseAdd_V2(void* pInstance, MS_PHY* u32YOffset, MS_PHY* u32UVOffset);
void MDrv_MVOP_SubSetVOPMirrorMode_V2(void* pInstance, MS_BOOL bEnable, MVOP_DrvMirror eMirrorMode);
MS_BOOL MDrv_MVOP_SubEnableFreerunMode_V2(void* pInstance, MS_BOOL bEnable);

// { Functions for the 3rd MVOP
//Basic control functions
MVOP_Result MDrv_MVOP_EX_Init_V2(void* pInstance, MVOP_DevID eID, MS_U32 u32InitParam);
MVOP_Result MDrv_MVOP_EX_Exit_V2(void* pInstance, MVOP_DevID eID, MS_U32 u32ExitParam);
MVOP_Result MDrv_MVOP_EX_Enable_V2(void* pInstance, MVOP_DevID eID, MS_BOOL bEnable);
MVOP_Result MDrv_MVOP_EX_SetInputCfg_V2 (void* pInstance, MVOP_DevID eID, MVOP_InputSel in, MVOP_InputCfg* pCfg);
MVOP_Result MDrv_MVOP_EX_SetOutputCfg_V2(void* pInstance, MVOP_DevID eID, MVOP_VidStat *pstVideoStatus, MS_BOOL bEnHDup);

//Advanced control functions
void MDrv_MVOP_EX_SetClk_V2(void* pInstance, MVOP_DevID eID,  MS_BOOL bEnable);
MVOP_Result MDrv_MVOP_EX_SetPattern_V2(void* pInstance, MVOP_DevID eID, MVOP_Pattern enMVOPPattern);
MVOP_Result MDrv_MVOP_EX_SetTileFormat_V2(void* pInstance, MVOP_DevID eID, MVOP_TileFormat eTileFmt);
MVOP_Result MDrv_MVOP_EX_EnableUVShift_V2(void* pInstance, MVOP_DevID eID, MS_BOOL bEnable);
MVOP_Result MDrv_MVOP_EX_EnableBlackBG_V2(void* pInstance, MVOP_DevID eID);
MVOP_Result MDrv_MVOP_EX_SetMonoMode_V2(void* pInstance, MVOP_DevID eID, MS_BOOL bEnable);
MVOP_Result MDrv_MVOP_EX_SetFixVtt_V2(void* pInstance, MVOP_DevID eID, MS_U16 u16FixVtt);
MVOP_Result MDrv_MVOP_EX_MiuSwitch_V2(void* pInstance, MVOP_DevID eID, MS_U8 u8Miu);
MVOP_Result MDrv_MVOP_EX_SetVOPMirrorMode_V2(void* pInstance, MVOP_DevID eID, MS_BOOL bEnable, MVOP_DrvMirror eMirrorMode);
MVOP_Result MDrv_MVOP_EX_EnableFreerunMode_V2(void* pInstance, MVOP_DevID eID, MS_BOOL bEnable);

// Query functions
MVOP_Result MDrv_MVOP_EX_GetOutputTiming_V2(void* pInstance, MVOP_DevID eID, MVOP_Timing* pMVOPTiming);
MVOP_Result MDrv_MVOP_EX_GetIsEnable_V2 (void* pInstance, MVOP_DevID eID, MS_BOOL* pbEnable);
MS_U16 MDrv_MVOP_EX_GetHStart_V2(void* pInstance, MVOP_DevID eID);
MS_U16 MDrv_MVOP_EX_GetVStart_V2(void* pInstance, MVOP_DevID eID);
MS_U16 MDrv_MVOP_EX_GetHSize_V2(void* pInstance, MVOP_DevID eID);
MS_U16 MDrv_MVOP_EX_GetVSize_V2(void* pInstance, MVOP_DevID eID);
MS_BOOL MDrv_MVOP_EX_GetIsInterlace_V2(void* pInstance, MVOP_DevID eID);
MS_BOOL MDrv_MVOP_EX_GetIsHDuplicate_V2(void* pInstance, MVOP_DevID eID);
MS_BOOL MDrv_MVOP_EX_GetStatus_V2(void* pInstance, MVOP_DevID eID, MVOP_DrvStatus *pMVOPStat);
MS_BOOL MDrv_MVOP_EX_CheckCapability_V2(void* pInstance, MVOP_DevID eID, MS_U32 u32InParam);
MS_BOOL MDrv_MVOP_EX_GetDstInfo_V2(void* pInstance, MVOP_DevID eID, MVOP_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo);
//   Functions for the 3rd MVOP }

MVOP_Result MDrv_MVOP_SendBlueScreen_V2(void* pInstance, MS_U16 u16Width, MS_U16 u16Height);
MVOP_Result MDrv_MVOP_SetCommand_V2(void* pInstance, MVOP_Handle* stHd, MVOP_Command eCmd, void* pPara);
MVOP_Result MDrv_MVOP_GetCommand_V2(void* pInstance, MVOP_Handle* stHd, MVOP_Command eCmd, void* pPara, MS_U32 u32ParaSize);
void MDrv_MVOP_SetFrequency_V2(void* pInstance, MVOP_FREQUENCY eFreq);

MS_BOOL MDrv_MVOP_EnableInterrupt_V2(void* pInstance, MS_U8 eIntType);
MS_U8 MDrv_MVOP_GetIntStatus_V2(void* pInstance);
MS_BOOL MDrv_MVOP_SubEnableInterrupt_V2(void* pInstance, MVOP_IntType eIntType);
MS_U8 MDrv_MVOP_SubGetIntStatus_V2(void* pInstance);
MS_BOOL MDrv_MVOP_EX_EnableInterrupt_V2(void* pInstance, MVOP_DevID eID, MVOP_IntType eIntType);
MS_U8 MDrv_MVOP_EX_GetIntStatus_V2(void* pInstance, MVOP_DevID eID);
MS_U32 MDrv_MVOP_SetPowerState_Kernel_V2(void* pInstance, EN_POWER_MODE u16PowerState);


// OBSOLETE FUNCTION POINTER END

#ifdef __cplusplus
}
#endif

#undef _API_MVOP_PRIV_H_
#endif // _API_MVOP_PRIV_H_
