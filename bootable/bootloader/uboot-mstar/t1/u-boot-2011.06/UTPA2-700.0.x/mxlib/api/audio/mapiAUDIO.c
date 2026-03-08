/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
/**
 *
 *
 */

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   dmx.c
/// @brief  Demux (DMX) API Interface
/// @attention
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsTypes.h"
//#include <stdio.h>
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "apiAUDIO.h"
#include "apiAUDIO_v2.h"
#include "apiAUDIO_private.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "MsOS.h"

#include "drvAUDIO_if.h"
#include "drvAUDIO.h"

#include "../drv/audio/internal/drvAUDIO_internal.h"

//extern void SerPrintf(char *fmt,...);

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Extern Variables
//-------------------------------------------------------------------------------------------------
extern AUDIO_SHARED_VARS2 * g_AudioVars2;

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
enum
{
    AUDIO_POOL_ID_AUDIO0,
} eAUDIOPoolID;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

#define PATCH_SN
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_BOOL _AUDIOProbeResourceToUtopia(AUDIO_INSTANT_PRIVATE *_pInstantAudio)
{
    //============================================================
    // AUDIO_SYSTEM RELATIONAL API FUNCTION
    //============================================================
    _pInstantAudio->fpAUDIOInitialize =                                 _MApi_AUDIO_Initialize;
    _pInstantAudio->fpAUDIOSetPowerOn =                                 _MApi_AUDIO_SetPowerOn;
#ifdef PATCH_SN
    _pInstantAudio->fpAUDIOGetDSPMadBaseAddr =                          _MDrv_AUDIO_GetDspMadBaseAddr;
    _pInstantAudio->fpAUDIOSetDSPBaseAddr =                             _MDrv_AUDIO_SetDspBaseAddr;
    _pInstantAudio->fpAUDIOWriteDecMailBox =                            _MDrv_AUDIO_WriteDecMailBox;
    _pInstantAudio->fpAUDIOTriggerSifPLL =                              _MDrv_AUDIO_TriggerSifPLL;
#else
    _pInstantAudio->fpAUDIOSetDSPBaseAddr =                             _MApi_AUDIO_SetDspBaseAddr;
    _pInstantAudio->fpAUDIOGetDSPMadBaseAddr =                          _MApi_AUDIO_GetDspMadBaseAddr;
    _pInstantAudio->fpAUDIOWriteDecMailBox =                            _MApi_AUDIO_WriteDecMailBox;
    _pInstantAudio->fpAUDIOTriggerSifPLL =                              _MApi_AUDIO_TriggerSifPLL;
#endif
    _pInstantAudio->fpAUDIOMonitor =                                    _MApi_Audio_Monitor;
    _pInstantAudio->fpAUDIOSetSystemInfo =                              _MApi_AUDIO_SetSystemInfo;
    //_pInstantAudio->fpAUDIOASFDataToDDR =                               _MApi_AUDIO_ASF_Data_To_DDR;
    //_pInstantAudio->fpAUDIOR2DebugMSG =                                 _MApi_AUDIO_R2_DEBUG_MSG;
    //_pInstantAudio->fpAUDIOR2DDRLetencyMSG =                            _MApi_AUDIO_R2_DDR_LATENCY_MSG;
    _pInstantAudio->fpAUDIOSetOutConnectivity =                         _MApi_AUDIO_SetOutConnectivity;
    _pInstantAudio->fpAUDIOSetPathInfo =                                _MApi_AUDIO_SetPathInfo;
    _pInstantAudio->fpAUDIOGetPathInfo =                                _MApi_AUDIO_GetPathInfo;
    _pInstantAudio->fpAUDIOSetOutputInfo =                              _MApi_AUDIO_SetOutputInfo;
    _pInstantAudio->fpAUDIOSetSourceInfo =                              _MApi_AUDIO_SetSourceInfo;
    _pInstantAudio->fpAUDIOInputSwitch =                                _MApi_AUDIO_InputSwitch;
    //_pInstantAudio->fpAUDIOSetInputScale =                              _MApi_AUDIO_SetInputScale;
    //_pInstantAudio->fpAUDIOSetScartScale =                              _MApi_AUDIO_SetScartScale;
    //_pInstantAudio->fpAUDIOAudysseyVXTSetThreshLevelTable =             _MApi_AUDIO_Audyssey_VXT_Set_THRESH_LEVEL_TABLE;
    //_pInstantAudio->fpAUDIOSPDIFSetChannelStauts =                      _MApi_AUDIO_SPDIF_SetChannelStatus;
    //_pInstantAudio->fpAUDIOSPDIFGetChannelStatus =                      _MApi_AUDIO_SPDIF_GetChannelStatus;

    _pInstantAudio->fpAUDIOSetDataCaptureSource =                       _MApi_AUDIO_SetDataCaptureSource;
    _pInstantAudio->fpAUDIOSetOutputSourceInfo =                        _MApi_AUDIO_SetOutputSourceInfo;
    _pInstantAudio->fpAUDIOGetOutputSourceInfo =                        _MApi_AUDIO_GetOutputSourceInfo;
    _pInstantAudio->fpAUDIOReadMailBox =                                _MApi_AUDIO_ReadMailBox;
    _pInstantAudio->fpAUDIOWriteMailBox =                               _MApi_AUDIO_WriteMailBox;
    _pInstantAudio->fpAUDIOFwTriggerDsp =                               _MApi_AUDIO_FwTriggerDSP;
    _pInstantAudio->fpAUDIOSetFwStatus =                                _MApi_AUDIO_SetFwStatus;
    _pInstantAudio->fpAUDIOExitAudioSystem =                            _MApi_AUDIO_ExitAudioSystem;
    _pInstantAudio->fpAUDIORebootDSP =                                  _MApi_AUDIO_RebootDsp;
    _pInstantAudio->fpAUDIOSendIntrupt =                                _MApi_AUDIO_SendIntrupt;
    _pInstantAudio->fpAUDIOI2SSetMode =                                 _MApi_AUDIO_I2S_SetMode;
    _pInstantAudio->fpAUDIOWritePreInitTable =                          _MApi_AUDIO_WritePreInitTable;
    _pInstantAudio->fpAUDIOEnaEarphoneLowPowerStage =                   _MApi_AUDIO_EnaEarphone_LowPower_Stage;
    _pInstantAudio->fpAUDIOEnaEarphoneHighDrivingStage =                _MApi_AUDIO_EnaEarphone_HighDriving_Stage;

    _pInstantAudio->fpAUDIOSuspend =                                 _MApi_AUDIO_Suspend;
    _pInstantAudio->fpAUDIOResume =                                 _MApi_AUDIO_Resume;

    //=============================================================
    // AUDIO_SOUND RELATIONAL API FUNCTION
    //=============================================================
    _pInstantAudio->fpSNDProcessEnable =                                _MApi_SND_ProcessEnable;
    _pInstantAudio->fpSNDSetParam1 =                                    _MApi_SND_SetParam1;
    _pInstantAudio->fpSNDGetParam1 =                                    _MApi_SND_GetParam1;
    _pInstantAudio->fpAUDIOSoundSetParam =                              _MApi_AUDIO_SOUND_SetParam;
    _pInstantAudio->fpAUDIOSetAbsoluteVolume =                          _MApi_AUDIO_SetAbsoluteVolume;
    //_pInstantAudio->fpAUDIOSetPreScale =                                _MApi_AUDIO_SetPreScale;
    //_pInstantAudio->fpAUDIOSetMute =                                    _MApi_AUDIO_SetMute;
    _pInstantAudio->fpAUDIOSourceConnect =                              _MApi_AUDIO_SourceConnect;
    //_pInstantAudio->fpAUDIOSetBalance =                                 _MApi_AUDIO_SetBalance;
    //_pInstantAudio->fpAUDIOEnableEQ =                                   _MApi_AUDIO_EnableEQ;
    //_pInstantAudio->fpAUDIOEnableTone =                                 _MApi_AUDIO_EnableTone;
    //_pInstantAudio->fpAUDIOEnableAutoVolume =                           _MApi_AUDIO_EnableAutoVolume;
    //_pInstantAudio->fpAUDIOEnableSurround =                             _MApi_AUDIO_EnableSurround;
    //_pInstantAudio->fpAUDIOEnableBalance =                              _MApi_AUDIO_EnableBalance;
    _pInstantAudio->fpAUDIOSetDynamicBass =                             _MApi_AUDIO_SetDynamicBass;
    //_pInstantAudio->fpAUDIOSetBass =                                    _MApi_AUDIO_SetBass;
    //_pInstantAudio->fpAUDIOAbsoluteBass =                               _MApi_AUDIO_AbsoluteBass;
    //_pInstantAudio->fpAUDIOSetTreble =                                  _MApi_AUDIO_SetTreble;
    _pInstantAudio->fpAUDIOAbsoluteTreble =                             _MApi_AUDIO_AbsoluteTreble;
    //_pInstantAudio->fpAUDIOSetEq =                                      _MApi_AUDIO_SetEq;
    _pInstantAudio->fpAUDIOSetEq7band =                                 _MApi_AUDIO_SetEq_7band;
    _pInstantAudio->fpAUDIOSetAvcThreshold =                            _MApi_AUDIO_SetAvcThreshold;
    _pInstantAudio->fpAUDIOSetAvcMode =                                 _MApi_AUDIO_SetAvcMode;
    _pInstantAudio->fpAUDIOSetAvcAT =                                   _MApi_AUDIO_SetAvcAT;
    _pInstantAudio->fpAUDIOSetAvcRT =                                   _MApi_AUDIO_SetAvcRT;
    _pInstantAudio->fpAUDIOSetBufferProcess =                           _MApi_AUDIO_SetBufferProcess;
    _pInstantAudio->fpAUDIOSetSurroundXA =                              _MApi_AUDIO_SetSurroundXA;
    _pInstantAudio->fpAUDIOSetSurroundXB =                              _MApi_AUDIO_SetSurroundXB;
    _pInstantAudio->fpAUDIOSetSurroundXK =                              _MApi_AUDIO_SetSurroundXK;
    _pInstantAudio->fpAUDIOSetSurroundLPFGain =                         _MApi_AUDIO_SetSurroundLPFGain;
    _pInstantAudio->fpAUDIOConvertVolumeUnit =                          _MApi_AUDIO_ConvertVolumeUnit;
    _pInstantAudio->fpAUDIOSetPEQCoef =                                 _MApi_AUDIO_SetPEQCoef;
    //_pInstantAudio->fpAUDIOEnablePEQ =                                  _MApi_AUDIO_EnablePEQ;
    _pInstantAudio->fpAUDIOEnableDcRemove =                             _MApi_AUDIO_EnableDcRemove;

    //============================================================
    // AUDIO_DTV RELATIONAL API FUNCTION
    //============================================================
    _pInstantAudio->fpAUDIOReleaseDecodeSystem =                        _MApi_AUDIO_ReleaseDecodeSystem;
    _pInstantAudio->fpAUDIOSetDecodeSystem =                            _MApi_AUDIO_SetDecodeSystem;
    _pInstantAudio->fpAUDIOGetDecodeSystem =                            _MApi_AUDIO_GetDecodeSystem;
    _pInstantAudio->fpAUDIOSetDecodeCmd =                               _MApi_AUDIO_SetDecodeCmd;
    _pInstantAudio->fpAUDIOOpenDecodeSystem =                           _MApi_AUDIO_OpenDecodeSystem;
    _pInstantAudio->fpAUDIOSetSystem =                                  _MApi_AUDIO_SetSystem;
    _pInstantAudio->fpAUDIOGetDecSysSupportStatus =                     _MApi_AUDIO_GetDecSysSupportStatus;
    _pInstantAudio->fpAUDIOIsMadLock =                                  _MApi_AUDIO_IsMadLock;
    _pInstantAudio->fpAUDIOGetDecStatus =                               _MApi_AUDIO_GetDecStatus;
    _pInstantAudio->fpAUDIOGetCommand =                                 _MApi_AUDIO_GetCommand;
    _pInstantAudio->fpAUDIOSetCommand =                                 _MApi_AUDIO_SetCommand;
    _pInstantAudio->fpAUDIOGetMADLock =                                 _MApi_AUDIO_GetMAD_LOCK;
    _pInstantAudio->fpAUDIOSetADOutputMode =                            _MApi_AUDIO_SetADOutputMode;
    _pInstantAudio->fpAUDIOSetADAbsoluteVoluem =                        _MApi_AUDIO_SetADAbsoluteVolume;
    //_pInstantAudio->fpAUDIOSetMainVolumeForAD =                         _MApi_AUDIO_SetMainVolumeForAD;
    _pInstantAudio->fpAUDIOADSetMute =                                  _MApi_AUDIO_ADSetMute;

    //============================================================
    // AUDIO_PVR RELATED FUNCTIONS
    //============================================================
    _pInstantAudio->fpAUDIOSetEncInit =                                 _MApi_AUDIO_SetEncInit;
    _pInstantAudio->fpAUDIOSetEncCommand =                              _MApi_AUDIO_SetEncCommand;
    _pInstantAudio->fpAUDIOGetEncodeFrameInfo =                         _MApi_AUDIO_GetEncodeFrameInfo;
    _pInstantAudio->fpAUDIOGetEncodeDoneFlag =                          _MApi_AUDIO_GetEncodeDoneFlag;

    //============================================================
    // AUDIO_ATV RELATIONAL API FUNCTION
    //============================================================
    _pInstantAudio->fpAUDIOSIFSetStandard =                             _MApi_AUDIO_SIF_SetStandard;
    _pInstantAudio->fpAUDIOSIFGetSoundMode =                            _MApi_AUDIO_SIF_GetSoundMode;
    _pInstantAudio->fpAUDIOSIFSetSoundMode =                            _MApi_AUDIO_SIF_SetSoundMode;
    _pInstantAudio->fpAUDIOSIFGetAudioStatus =                          _MApi_AUDIO_SIF_GetAudioStatus;
    _pInstantAudio->fpAUDIOSIFStartAutoStandardDetection =              _MApi_AUDIO_SIF_StartAutoStandardDetection;
    _pInstantAudio->fpAUDIOSIFGetResultOfAutoStandardDetection =        _MApi_AUDIO_SIF_GetResultOfAutoStandardDetection;
    _pInstantAudio->fpAUDIOSIFConvertToBasicAudioStandard =             _MApi_AUDIO_SIF_ConvertToBasicAudioStandard;
    _pInstantAudio->fpAUDIOSIFSetThreshold =                            _MApi_AUDIO_SIF_SetThreshold;
    _pInstantAudio->fpAUDIOSIFSetPrescale =                             _MApi_AUDIO_SIF_SetPrescale;
    _pInstantAudio->fpAUDIOSIFIsPALType =                               _MApi_AUDIO_SIF_IsPALType;
    _pInstantAudio->fpAUDIOSIFSetPALType =                              _MApi_AUDIO_SIF_SetPALType;
    _pInstantAudio->fpAUDIOSIFSendCmd =                                 _MApi_AUDIO_SIF_SendCmd;
    _pInstantAudio->fpAUDIOSIFShift =                                   _MApi_AUDIO_SIF_Shift;

#if 1
    //============================================================
    // AUDIO_FM_RADIO RELATIONAL API FUNCTION
    //============================================================
    _pInstantAudio->fpAUDIOFMRADIOGetSoundMode =                        _MApi_AUDIO_FM_RADIO_GetSoundMode;
    _pInstantAudio->fpAUDIOFMRADIOSetSoundMode =                        _MApi_AUDIO_FM_RADIO_SetSoundMode;
    _pInstantAudio->fpAUDIOFMRADIODeEmphassisOption =                   _MApi_AUDIO_FM_RADIO_DeEmphassisOption;
    _pInstantAudio->fpAUDIOFMRADIOGetDcAmp =                            _MApi_AUDIO_FM_RADIO_GET_DC_AMP;
    _pInstantAudio->fpAUDIOFMRADIOGetNSR =                              _MApi_AUDIO_FM_RADIO_GET_NSR;
    _pInstantAudio->fpAUDIOFMRADIOReset =                               _MApi_AUDIO_FM_RADIO_RESET;
#endif

    //=============================================================
    // AUDIO_SPDIF RELATIONAL API FUNCTION
    //=============================================================
    _pInstantAudio->fpAUDIOSPDIFHWEN =                                  _MApi_AUDIO_SPDIF_HWEN;
    _pInstantAudio->fpAUDIOSPDIFSetMute =                               _MApi_AUDIO_SPDIF_SetMute;
    _pInstantAudio->fpAUDIOSPDIFSetMode =                               _MApi_AUDIO_SPDIF_SetMode;
    _pInstantAudio->fpAUDIOSPDIFGetMode =                               _MApi_AUDIO_SPDIF_GetMode;
    _pInstantAudio->fpAUDIOSPDIFSetSCMS =                               _MApi_AUDIO_SPDIF_SetSCMS;
    _pInstantAudio->fpAUDIOSPDIFGetSCMS =                               _MApi_AUDIO_SPDIF_GetSCMS;
    _pInstantAudio->fpAUDIOSPDIFMonitor =                               _MApi_Audio_SPDIF_Monitor;
    _pInstantAudio->fpAUDIOSPDIFChannelStatusCTRL =                     _MApi_AUDIO_SPDIF_ChannelStatus_CTRL;
    _pInstantAudio->fpAUDIODigitalOut_SetChannelStatus =                _MApi_AUDIO_DigitalOut_SetChannelStatus;
    _pInstantAudio->fpAUDIODigitalOut_SetDeviceCapabilty =              _MApi_AUDIO_DigitalOut_SetDeviceCapability;
    //_pInstantAudio->fpAUDIODigitalOut_GetChannelStatus =                _MApi_AUDIO_DigitalOut_GetChannelStatus;


    //=============================================================
    // AUDIO_HDMI RELATIONAL API FUNCTION
    //=============================================================
    _pInstantAudio->fpAUDIOHDMITxSetMute =                              _MApi_AUDIO_HDMI_Tx_SetMute;
    _pInstantAudio->fpAUDIOHDMITxGetStatus =                            _MApi_AUDIO_HDMI_Tx_GetStatus;
    _pInstantAudio->fpAUDIOHDMIMonitor =                                _MApi_AUDIO_HDMI_Monitor;
    _pInstantAudio->fpAUDIOHDMIGetNonpcmFlag =                          _MApi_AUDIO_HDMI_GetNonpcmFlag;
    _pInstantAudio->fpAUDIOHDMISetNonpcm =                              _MApi_AUDIO_HDMI_SetNonpcm;
    _pInstantAudio->fpAUDIOHDMIRxSetNonPCM =                            _MApi_AUDIO_HDMI_RX_SetNonpcm;
    _pInstantAudio->fpAUDIOHDMIRxGetNonPCM =                            _MApi_AUDIO_HDMI_RX_GetNonPCM;
    _pInstantAudio->fpAUDIODTVHDMICFG =                                 _MApi_AUDIO_DTV_HDMI_CFG;
    _pInstantAudio->fpAUDIOHDMIGetSynthFreg =                           _MApi_AUDIO_HDMI_GetSynthFreq;
    _pInstantAudio->fpAUDIOHDMISetDownSample =                          _MApi_AUDIO_HDMI_SetDownSample;
    _pInstantAudio->fpAUDIOHDMITxSetMode =                              _MApi_AUDIO_HDMI_TX_SetMode;
    _pInstantAudio->fpAUDIOHDMIRxGetHDMIInAUDIOStatus =                 _MApi_AUDIO_HDMI_RX_GetHdmiInAudioStatus;

    //=============================================================
    // AUDIO_ADVANCE_SOUND RELATIONAL API FUNCTION
    //=============================================================
    _pInstantAudio->fpAUDIOADVSOUNDProcessEnable =                      _MApi_AUDIO_ADVSOUND_ProcessEnable;
    _pInstantAudio->fpAUDIOADVSOUNDSubProcessEnable =                   _MApi_AUDIO_ADVSOUND_SubProcessEnable;
    _pInstantAudio->fpAUDIOADVSOUNDSetParam =                           _MApi_AUDIO_ADVSOUND_SetParam;
    _pInstantAudio->fpAUDIOADVSNDSetParam =                             _MApi_AUDIO_ADVSND_SetParam;
    //_pInstantAudio->fpAUDIOADVSOUNDGetParam =                           _MApi_AUDIO_ADVSOUND_GetParam;
    //_pInstantAudio->fpAUDIOADVSOUNDSetASFParam =                        _MApi_AUDIO_ADVSOUND_SetASFParam;
    //_pInstantAudio->fpAUDIOADVSOUNDGetASFParam =                        _MApi_AUDIO_ADVSOUND_GetASFParam;
    _pInstantAudio->fpAUDIOADVSOUNDGetInfo =                            _MApi_AUDIO_ADVSOUND_GetInfo;
    _pInstantAudio->fpAUDIODBXTV_SetMode =                              _MApi_DBXTV_SetMode;
    _pInstantAudio->fpAUDIOSeInit =                                     _MApi_AUDIO_SeInit;
    _pInstantAudio->fpAUDIOSetAdvSndSys =                               _MApi_AUDIO_SetAdvSndSys;
    //_pInstantAudio->fpAUDIOAdvSndSetSoundParam =                        _MApi_AUDIO_ADVSND_Set_Sound_Param;
    _pInstantAudio->fpAUDIOSetVDS =                                     _MApi_AUDIO_SetVDS;
    _pInstantAudio->fpAUDIOSetVSPK =                                    _MApi_AUDIO_SetVSPK;
    _pInstantAudio->fpAUDIOSetSRS=                                      _MApi_AUDIO_SetSRS;
    _pInstantAudio->fpAUDIOSetBBE =                                     _MApi_AUDIO_SetBBE;
    _pInstantAudio->fpAUDIOVSPKWMod =                                   _MApi_AUDIO_VSPK_WMod;
    _pInstantAudio->fpAUDIOVSPKSMod =                                   _MApi_AUDIO_VSPK_SMod;
    _pInstantAudio->fpAUDIOSRSDC =                                      _MApi_AUDIO_SRS_DC;
    _pInstantAudio->fpAUDIOSRSTruBass =                                 _MApi_AUDIO_SRS_TruBass;
    _pInstantAudio->fpAUDIOSRSSetTsxtPara =                             _MApi_AUDIO_SRS_SetTsxtPara;
    _pInstantAudio->fpAUDIOSetSRSTSHD =                                 _MApi_AUDIO_SetSRSTSHD;
    _pInstantAudio->fpAUDIOTSHDTruBass =                                _MApi_AUDIO_TSHD_TruBass;
    _pInstantAudio->fpAUDIOTSHDDefinition =                             _MApi_AUDIO_TSHD_Definition;
    _pInstantAudio->fpAUDIOTSHDClarity =                                _MApi_AUDIO_TSHD_Clarity;
    _pInstantAudio->fpAUDIOSRSSetTshdPara =                             _MApi_AUDIO_SRS_SetTshdPara;
    _pInstantAudio->fpAUDIOCopyParameter =                              _MApi_AUDIO_COPY_Parameter;

    //=============================================================
    // AUDIO_KTV RELATIONAL API FUNCTION
    //=============================================================
    _pInstantAudio->fpAUDIOSetKTVInfo =                                 _MApi_AUDIO_SetKTVInfo;
    _pInstantAudio->fpAUDIOGetKTVInfo =                                 _MApi_AUDIO_GetKTVInfo;
    _pInstantAudio->fpAUDIOSetMixModeVolume =                           _MApi_AUDIO_SetMixModeVolume;
    _pInstantAudio->fpAUDIOSetMixModeMute =                             _MApi_AUDIO_SetMixModeMute;
    _pInstantAudio->fpAUDIOPlayMenuSound =                              _MApi_AUDIO_PlayMenuSound;

    //=============================================================
    // AUDIO_MM RELATIONAL API FUNCTION
    //=============================================================
    _pInstantAudio->fpAUDIOSetCommAudioInfo =                           _MApi_AUDIO_SetCommAudioInfo;
    //_pInstantAudio->fpAUDIOSetDecodeParam =                             _MApi_ADEC_SetDecodeParam;
    _pInstantAudio->fpAUDIOSetMpegInfo =                                _MApi_AUDIO_SetMpegInfo;
    _pInstantAudio->fpAUDIOSetAC3Info =                                 _MApi_AUDIO_SetAC3Info;
    _pInstantAudio->fpAUDIOSetAC3PInfo =                                _MApi_AUDIO_SetAC3PInfo;
    _pInstantAudio->fpAUDIOSetAACInfo =                                 _MApi_AUDIO_SetAACInfo;
    _pInstantAudio->fpAUDIOSetWMAInfo =                                 _MApi_AUDIO_SetWmaInfo;
    _pInstantAudio->fpAUDIOSetDTSCommonCtrl =                           _MApi_AUDIO_SetDTSCommonCtrl;
    _pInstantAudio->fpAUDIOGetCommAudioInfo =                           _MApi_AUDIO_GetCommAudioInfo;
    _pInstantAudio->fpAUDIOGetMpegInfo =                                _MApi_AUDIO_GetMpegInfo;
    _pInstantAudio->fpAUDIOGetAC3Info =                                 _MApi_AUDIO_GetAC3Info;
    _pInstantAudio->fpAUDIOGetAC3PInfo =                                _MApi_AUDIO_GetAC3PInfo;
    _pInstantAudio->fpAUDIOGetAACInfo =                                 _MApi_AUDIO_GetAACInfo;
    _pInstantAudio->fpAUDIOGetWmaInfo =                                 _MApi_AUDIO_GetWmaInfo;
    _pInstantAudio->fpAUIDOGetDTSInfo =                                 _MApi_AUDIO_GetDTSInfo;
    _pInstantAudio->fpAUDIOXPCMParam =                                  _MApi_AUDIO_XPCM_Param;
    _pInstantAudio->fpAUDIOXPCM2Param =                                 _MApi_AUDIO_XPCM2_Param;
    _pInstantAudio->fpAUDIOXPCM2CheckIntStatus =                        _MApi_AUDIO_XPCM2_CheckIntStatus;
    _pInstantAudio->fpAUDIORA8Param =                                   _MApi_AUDIO_RA8_Param;
    _pInstantAudio->fpAUDIOInit =                                       _MApi_AUDIO_Init;
    _pInstantAudio->fpAUDIOStartDecode =                                _MApi_AUDIO_StartDecode;
    _pInstantAudio->fpAUDIOStartBrower =                                _MApi_AUDIO_StartBrowse;
    _pInstantAudio->fpAUDIOStopDecode =                                 _MApi_AUDIO_StopDecode;
    _pInstantAudio->fpAUDIOPauseDecode =                                _MApi_AUDIO_PauseDecode;
    _pInstantAudio->fpAUDIOCheckPlayDone =                              _MApi_AUDIO_CheckPlayDone;
    _pInstantAudio->fpAUDIOGetResidualBufferSize =                      _MApi_AUDIO_GetResidualBufferSize;
    _pInstantAudio->fpAUDIOGetPCMBufferSize =                           _MApi_AUDIO_GetPCMBufferSize;
    _pInstantAudio->fpAUDIOGetPCMBufferSize2 =                          _MApi_AUDIO_GetPCMBufferSize2;
    _pInstantAudio->fpAUDIOGetCurrentFrameNumber =                      _MApi_AUDIO_GetCurrentFrameNumber;
    _pInstantAudio->fpAUDIOGetSampleRate =                              _MApi_AUDIO_GetSampleRate;
    _pInstantAudio->fpAUDIOGetBitRate =                                 _MApi_AUDIO_GetBitRate;
    _pInstantAudio->fpAUDIOGetLayer =                                   _MApi_AUDIO_GetLayer;
    _pInstantAudio->fpAUDIOCheckInputRequest =                          _MApi_AUDIO_CheckInputRequest;
    _pInstantAudio->fpAUDIOSetInput =                                   _MApi_AUDIO_SetInput;
    _pInstantAudio->fpAUDIOSetSampleRateIndex =                         _MApi_AUDIO_SetSampleRateIndex;
    _pInstantAudio->fpAUDIOSetXPCMParam =                               _MApi_AUDIO_SetXPCMParam;
    _pInstantAudio->fpAUDIOFileEndNotification =                        _MApi_AUDIO_FileEndNotification;
    _pInstantAudio->fpAUDIOFileEndDataHandle =                          _MApi_AUDIO_FileEndDataHandle;
    _pInstantAudio->fpAUDIOGetPlayTick =                                _MApi_AUDIO_GetPlayTick;
    _pInstantAudio->fpAUDIOGetEsMEMCnt =                                _MApi_AUDIO_GetEsMEMCnt;
    _pInstantAudio->fpAUDIOSetASFParm =                                 _MApi_AUDIO_SetASFParm;
    _pInstantAudio->fpAUDIOMMSetInput =                                 _MApi_AUDIO_MM_SetInput;
    _pInstantAudio->fpAUDIOMMCheckPlayDone =                            _MApi_AUDIO_MM_CheckPlayDone;
    _pInstantAudio->fpAUDIOMMCheckInputRequest =                        _MApi_AUDIO_MM_CheckInputRequest;
    _pInstantAudio->fpAUDIODmaReaderInit =                              _MApi_AUDIO_DmaReader_Init;
    _pInstantAudio->fpAUDIODmaReaderAllInputInit =                      _MApi_AUDIO_DmaReader_AllInput_Init;
    _pInstantAudio->fpAUDIODmaReaderWritePCM =                          _MApi_AUDIO_DmaReader_WritePCM;
    _pInstantAudio->fpAUDIODmaWriterInit =                              _MApi_AUDIO_DmaWriter_Init;
    _pInstantAudio->fpAUDIOUSBPCMEnable =                               _MApi_AUDIO_USBPCM_Enable;
    _pInstantAudio->fpAUDIOUSBPCMSetFlag =                              _MApi_AUDIO_USBPCM_SetFlag;
    _pInstantAudio->fpAUDIOUSBPCMGetFlag =                              _MApi_AUDIO_USBPCM_GetFlag;
    _pInstantAudio->fpAUDIOUSBPCMGetMemInfo =                           _MApi_AUDIO_USBPCM_GetMemInfo;
    _pInstantAudio->fpAUDIOPCMCaptureInit =                             _MApi_AUDIO_PCMCapture_Init;
    _pInstantAudio->fpAUDIOPCMCaptureStart =                            _MApi_AUDIO_PCMCapture_Start;
    _pInstantAudio->fpAUDIOPCMCaptureStop =                             _MApi_AUDIO_PCMCapture_Stop;
    _pInstantAudio->fpAUDIOPCMCaptureRead =                             _MApi_AUDIO_PCMCapture_Read;
    _pInstantAudio->fpAUDIOVoIPConfig =                                 _MApi_AUDIO_VoIP_Config;
    _pInstantAudio->fpAUDIOALSACheck =                                  _MApi_AUDIO_ALSA_Check;
    _pInstantAudio->fpAUDIOALSAEnable =                                 _MApi_AUDIO_ALSA_Enable;
    _pInstantAudio->fpAUDIOUNICheckDecodeDone =                         _MApi_AUDIO_UNI_CheckDecodeDone;
    _pInstantAudio->fpAUDIOUNISetOutput =                               _MApi_AUDIO_UNI_SetOutput;
    _pInstantAudio->fpAUDIOUNISetPCMInputWriteAddr =                    _MApi_AUDIO_UNI_Set_PCMInputWriteAddr;
    _pInstantAudio->fpAUDIOUNIGetOutPCMLevel =                          _MApi_AUDIO_UNI_Get_OutPCMLevel;
    _pInstantAudio->fpAUDIORingTask =                                   _MApi_AUDIO_RingTask;
    _pInstantAudio->fpAUDIORingDataTransfer =                           _MApi_AUDIO_Ring_DataTransfer;
    _pInstantAudio->fpAUDIOMM2InitAesInfo =                             _MApi_AUDIO_MM2_initAesInfo;
    _pInstantAudio->fpAUDIOMM2CheckAesInfo =                            _MApi_AUDIO_MM2_checkAesInfo;
    _pInstantAudio->fpAUDIOMM2InputAesFinished =                        _MApi_AUDIO_MM2_inputAesFinished;
    _pInstantAudio->fpAUDIOSetAudioParam2 =                             _MApi_AUDIO_SetAudioParam2;
    _pInstantAudio->fpAUDIOGetAudioInfo2 =                              _MApi_AUDIO_GetAudioInfo2;
    _pInstantAudio->fpAUDIOGetDDRInfo =                                 _MApi_AUDIO_GetDDRInfo;
    _pInstantAudio->fpAUDIOGetCaps =                                    _MApi_AUDIO_GetCaps;
    //To Do

    return TRUE;

}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void AUDIORegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    void* pUtopiaModule = NULL;
    UtopiaModuleCreate(MODULE_AUDIO, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)AUDIOOpen, (FUtopiaClose)AUDIOClose, (FUtopiaIOctl)AUDIOIoctl);

#if defined(MSOS_TYPE_LINUX_KERNEL)
    UtopiaModuleSetupSTRFunctionPtr(pUtopiaModule,(FUtopiaSTR)AUDIOStr);
    //UtopiaModuleSetSTRPrivate(pUtopiaModule, STRPrivateDataSize);
#endif

    // 2. deal with resource
    void* psResource = NULL;
    UtopiaModuleAddResourceStart(pUtopiaModule, AUDIO_POOL_ID_AUDIO0);
    UtopiaResourceCreate("AUDIO", sizeof(AUDIO_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, AUDIO_POOL_ID_AUDIO0);
    UtopiaModuleAddResourceEnd(pUtopiaModule, AUDIO_POOL_ID_AUDIO0);
}

MS_U32 AUDIOOpen(void** ppInstance, void* pAttribute)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIO open \n");

    if(ppInstance == 0)
    {
        return UTOPIA_STATUS_PARAMETER_ERROR;
    }

    AUDIO_INSTANT_PRIVATE*    psAudioInstPri = NULL;

    UtopiaInstanceCreate(sizeof(AUDIO_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psAudioInstPri);

    if(_AUDIOProbeResourceToUtopia(psAudioInstPri) == FALSE)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "Audio module version is not supported \n");
        return UTOPIA_STATUS_NOT_SUPPORTED;
    }

    return UTOPIA_STATUS_SUCCESS;

}

MS_U32 AUDIOIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    AUDIO_INSTANT_PRIVATE* psAudioInstPri = NULL;
    void* pModule = NULL;
    void* pResource = NULL;
    void* pResPrivate = NULL;
    AUDIO_STATUS tRes = AUDIO_STATUS_OK;
    MS_BOOL bStatus = FALSE;
    MS_U8 u8Status = 0;
    MS_S8 s8Status = 0;
    MS_U16 u16Status = 0;
    MS_U32 u32Status = 0;
    long long ulong = 0;
    AUDIO_DEC_ID eDecId = 0;
    AU_DVB_DECCMD eDecCmd = 0;
    En_DVB_decCmdType eDecCmdType = 0;
    AUDIOSTANDARD_TYPE eAUDIOStand_Type = 0;
    AUDIO_HDMI_RX_TYPE eHDMI_RX_Type = 0;
    Audio_MP3_LAYER eMP3_Layer = 0;

    MS_U32*                                 pu32Param                       = NULL;
    MS_S32*                                 ps32Param                       = NULL;
    //MS_U64*                                 pu64Param                       = NULL;
    PAUDIO_ST_RX_HDMI_AUDIO_STATUS          pRxHdmiSts                      = NULL;
    PAUDIO_ST_SOUND_SETPARAM                pSoundSetParam                  = NULL;
    //PAUDIO_ST_DECODESYSTEM                  pDecodeSystem                   = NULL;
    PAUDIO_ST_GET_DECODESYSTEM              pGetDecodeSystem                = NULL;
    PAUDIO_ST_OPEN_DECODESYSTEM             pOpenDecodeSystem               = NULL;
    PAUDIO_ST_SET_DECODESYSTEM              pSetDecodeSystem                = NULL;
    PAUDIO_ST_RELEASE_DECODESYSTEM          pReleaseDecodeSystem            = NULL;
    PAUDIO_ST_SIF_GET_AUDIO_STATUS          pSifGetAudioStatus              = NULL;
    PAUDIO_ST_DIGITALOUT_SETCHANNELSTATUS   pDigitalOut_SetChannelStatus    = NULL;
    PAUDIO_ST_DIGITALOUT_GETCHANNELSTATUS   pDigitalOut_GetChannelStatus    = NULL;
    PAUDIO_ST_DIGITALOUT_SETDEVICECAP       pDigitalOut_SetDeviceCap        = NULL;
    PAUDIO_HDMI_TX_GETSTATUS                pHDMITx_GetStatus               = NULL;
    PAUDIO_ST_COPY_PARAMETER                pCopyParameter                  = NULL;
    PAUDIO_ST_RA8_Param                     pRA8Param                       = NULL;
    PAUDIO_ST_CHECK_INPUT_REQ               pCheckInputRequest              = NULL;
    PAUDIO_ST_MM_CHECK_INPUT_REQ            pMMCheckInputRequest            = NULL;
    PAUDIO_ST_DMAREADER_WRITEPCM            pDmaReaderWritePCM              = NULL;
    PAUDIO_ST_PCMCAPTURE_INIT               pPCMCapture_Init                = NULL;
    PAUDIO_ST_PCMCAPTURE_READ               pPCMCapture_Read                = NULL;
    PAUDIO_ST_UNI_CHECK_DECODEDONE          pUNICheck_DecodeDone            = NULL;
    PAUDIO_ST_RING_DATA_TRANS               pRing_Data_Trans                = NULL;
    PAUDIO_ST_MM2_CHECKAESINFO              pMM2_CheckAesInfo               = NULL;
    PAUDIO_ST_MM2_INPUTAES_FINISH           pMM2_InputAes_Finish            = NULL;
    PAUDIO_ST_GETAUDIOINFO2                 pGetAudioInfo2                  = NULL;
    //PAUDIO_ST_ADV_SOUND_SETPARAM            pAdvSnd_Sound_Set_Param         = NULL;
    PAUDIO_ST_OUTPUT_SOURCE                 pOutputSourceInfo               = NULL;
    PAUDIO_ST_SET_AD                        pSetAD                          = NULL;
    PAUDIO_ST_DECODER_COMMAND               pDecoderCommand                 = NULL;
    PAUDIO_ST_SPDIF_OP                      pSpdif_op                       = NULL;
    PAUDIO_ST_SPDIF_SCMS                    pSpdif_scms                     = NULL;
    PAUDIO_ST_HDMI_NONPCM_OP                pHDMI_Nonpcm_op                 = NULL;
    PAUDIO_ST_KTV_OP                        pKTV_OP                         = NULL;
    //PAUDIO_ST_SETDECODEPARAM                pSetDecodeParam                 = NULL;
    //PAUDIO_ST_AUDYSSEY_VXT_SET_THRESH_LEVEL_TABLE pAudyssey_VXT_setTHRESH_Level_Table = NULL;
    PAUDIO_ST_SPDIF_CHANNELSTATUS           pSPDIF_ChannelStatus            = NULL;
    PAUDIO_ST_GETCAPS                       pGetCaps                        = NULL;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
    PAUDIO_ST_GET_DSPBASEADDR               pGet_DspBaseAddr                = NULL;
    PAUDIO_ST_SET_DSPBASEADDR	            pSet_DspBaseAddr                = NULL;
    PAUDIO_ST_WRITE_DECMAILBOX              pWrite_DecMailBox               = NULL;
    PAUDIO_ST_INPUTSWITCH                   pInputSwitch                    = NULL;
    PAUDIO_ST_SET_DATACAPTURESOURCE         pSet_DataCaptureResource        = NULL;
    PAUDIO_ST_READ_MAILBOX                  pRead_MailBox                   = NULL;
    PAUDIO_ST_WRITE_MAILBOX                 pWrite_MailBox                  = NULL;
    PAUDIO_ST_SEND_INTERRUPT                pSend_Interrupt                 = NULL;
    PAUDIO_ST_I2S_SETMODE                   pI2s_SetMode                    = NULL;
    PAUDIO_ST_SND_PROCESSENABLE             pSnd_ProcessEnable              = NULL;
    PAUDIO_ST_SND_SETPARAM1                 pSnd_SetParam1                  = NULL;
    PAUDIO_ST_SND_GETPARAM1                 pSnd_GetParam1                  = NULL;
    PAUDIO_ST_SETEQ_7BAND                   pSet_7band                      = NULL;
    PAUDIO_ST_SET_ABSOLUTEVOLUME            pSet_AbsoluteVolume	            = NULL;
    PAUDIO_ST_SOURCECONNECT                 pSourceConnect                  = NULL;
    PAUDIO_ST_CONVERT_VOLUMEUNIT            pConvertVolumeUnit              = NULL;
    PAUDIO_ST_ENABLE_DCREMOVE               pEnableDcRemove                 = NULL;
    PAUDIO_ST_SET_DECCMD                    pSetDecCmd                      = NULL;
    PAUDIO_ST_SET_SYSTEM                    pSetSystem                      = NULL;
    PAUDIO_ST_GET_DECSYSSUPPORTSTATUS       pGetDecSysSupportStatus         = NULL;
    PAUDIO_ST_SET_ENCINIT                   pSetEncInit                     = NULL;
    PAUDIO_ST_SIF_SET_SOUNDMODE	            pSifSetSoundMode                = NULL;
    PAUDIO_ST_SIF_CONVERTTOBASICAUDIOSTANDARD pSifConvertToBasicAudioStandard = NULL;
    PAUDIO_ST_SIF_SETPRESCALE               pSifSetPrescale                 = NULL;
    PAUDIO_ST_SIF_ISPALTYPE                 pSifIsPalType                   = NULL;
    PAUDIO_ST_SIF_SENDCMD                   pSifSendCmd                     = NULL;
    PAUDIO_ST_SPDIF_CHANNELSTATUS_CTRL      pSpdifChannelStatusCtrl         = NULL;
    PAUDIO_ST_ADVSOUND_PROCESSENABLE        pAdvSoundProcessEnable          = NULL;
    PAUDIO_ST_ADVSOUND_SUBPROCESSENABLE     pAdvSoundSubProcessEnable       = NULL;
    PAUDIO_ST_ADVSOUND_SETPARAM             pAdvSoundSetParam               = NULL;
    PAUDIO_ST_ADVSND_SETPARAM               pAdvSndSetParam                 = NULL;
    PAUDIO_ST_SETADVSNDSYS                  pSetAdvSndSys                   = NULL;
    PAUDIO_ST_SETBBE                        pSetBBE                         = NULL;
    PAUDIO_ST_SRS_SETTSXTPARA               pSrsSetTsxtPara                 = NULL;
    PAUDIO_ST_SRS_SETTSHDPARA               pSrsSetTshdPara                 = NULL;
    PAUDIO_ST_SRS_ADVSOUND_GETINFO          pAdvSoundGetInfo                = NULL;
    PAUDIO_ST_DBXTV_SETMODE                 pDbxTvSetMode                   = NULL;
    PAUDIO_ST_SET_COMMAUDIOINFO             pSetCommAudioInfo               = NULL;
    PAUDIO_ST_SET_MPEGINFO                  pSetMpegInfo                    = NULL;
    PAUDIO_ST_SET_AC3INFO                   pSetAc3Info                     = NULL;
    PAUDIO_ST_SET_AC3PINFO                  pSetAc3pInfo                    = NULL;
    PAUDIO_ST_SET_AACINFO                   pSetAacInfo                     = NULL;
    PAUDIO_ST_SET_WMAINFO                   pSetWmaInfo                     = NULL;
    PAUDIO_ST_SET_DTSCOMMONCTRL             pSetDtsCommonCtrl               = NULL;
    PAUDIO_ST_MM2_INITAESINFO               pMm2InitAesInfo                 = NULL;
    PAUDIO_ST_SET_XPCMPARAM                 pSetXpcmParam                   = NULL;
    PAUDIO_ST_GET_COMMAUDIOINFO	            pGetCommAudioInfo               = NULL;
    PAUDIO_ST_GET_MPEGINFO                  pGetMpegInfo                    = NULL;
    PAUDIO_ST_GET_AC3INFO                   pGetAc3Info	                    = NULL;
    PAUDIO_ST_GET_AC3PINFO                  pGetAc3pInfo                    = NULL;
    PAUDIO_ST_GET_AACINFO                   pGetAacInfo                     = NULL;
    PAUDIO_ST_GET_WMAINFO                   pGetWmaInfo	                    = NULL;
    PAUDIO_ST_GET_DTSINFO                   pGetDtsInfo	                    = NULL;
    PAUDIO_ST_XPCMPARAM	                    pXpcmParam                      = NULL;
    PAUDIO_ST_GET_PCMBUFFERSIZE	            pGetPcmBufferSize               = NULL;
    PAUDIO_ST_GET_PCMBUFFERSIZE2            pGetPcmBufferSize2              = NULL;
    PAUDIO_ST_SET_ASFPARM                   pSetAsfParm	                    = NULL;
    PAUDIO_ST_MM_SETINPUT                   pMmSetInput	                    = NULL;
    PAUDIO_ST_MM_CHECKPLAYDONE              pMmCheckPlayDone                = NULL;
    PAUDIO_ST_VOIP_CONFIG                   pVoipConfig	                    = NULL;
    PAUDIO_ST_ALSA_ENABLE                   pAlsaEnable	                    = NULL;
    PAUDIO_ST_SET_AUDIOPARAM2               pSetAudioParam2                 = NULL;
    PAUDIO_ST_GET_DDRINFO                   pGetDdrInfo	                    = NULL;
    PAUDIO_ST_ADVSND_SETASFPARAM pAdvSndSetAsfParam = NULL;

    if(pInstance == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "pInstance is NULL!!! \n");
        return UTOPIA_STATUS_FAIL;
    }

    if(UtopiaInstanceGetPrivate(pInstance, (void*)&psAudioInstPri) != 0)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "UtopiaInstanceGetPrivate fail\n");
        return UTOPIA_STATUS_FAIL;
    }
    if(UtopiaInstanceGetModule(pInstance, &pModule) != 0)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "UtopiaInstanceGetModule fail\n");
        return UTOPIA_STATUS_FAIL;
    }

    if(UtopiaResourceObtain(pModule, AUDIO_POOL_ID_AUDIO0, &pResource) != 0)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "UtopiaResourceObtain fail\n");
        return UTOPIA_STATUS_FAIL;
    }

    if(UtopiaResourceGetPrivate(pResource, &pResPrivate) != 0)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "UtopiaResourceGetPrivatefail\n");
        goto IOCTRL_END;
    }

    #if 0   //[Q] Need to check sharead mem
    // init share resource of driver and assign share mem pointer to process
    if(psDmxInstPri->fpDMXInitLibRes(pResPrivate) != DMX_FILTER_STATUS_OK)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "DMX init share memory failed \n");
        tRes = DMX_FILTER_STATUS_ERROR;
        goto IOCTRL_END;
    }
    #endif

    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[Audio][mapiAudio.c] cmd %d  switch\n", (unsigned int)u32Cmd);

    switch(u32Cmd)
    {
        //============================================================
        // AUDIO_SYSTEM RELATIONAL API FUNCTION
        //============================================================
        case MApi_CMD_AUDIO_Initialize:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_Init\n");
            psAudioInstPri->fpAUDIOInitialize();
            break;
        case MApi_CMD_AUDIO_SetPowerOn:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetPowerOn\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetPowerOn((MS_BOOL)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_GetDSPBaseAddr:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetDSPBaseAddr\n");
            pGet_DspBaseAddr = (PAUDIO_ST_GET_DSPBASEADDR)pArgs;
            u32Status= psAudioInstPri->fpAUDIOGetDSPMadBaseAddr(pGet_DspBaseAddr->u8Index);
            pGet_DspBaseAddr->u32Addr = u32Status;
            break;
        case MApi_CMD_AUDIO_SetDSPBaseAddr:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetDSPBaseAddr\n");
            pSet_DspBaseAddr = (PAUDIO_ST_SET_DSPBASEADDR)pArgs;
            psAudioInstPri->fpAUDIOSetDSPBaseAddr(pSet_DspBaseAddr->u8Index, pSet_DspBaseAddr->u32Bin_Base_Address, pSet_DspBaseAddr->u32Mad_Base_Buffer_Adr);
            break;
        case MApi_CMD_AUDIO_WriteDecMailBox:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_WriteDecMailBox\n");
            pWrite_DecMailBox = (PAUDIO_ST_WRITE_DECMAILBOX)pArgs;
            psAudioInstPri->fpAUDIOWriteDecMailBox(pWrite_DecMailBox->u8ParamNum, pWrite_DecMailBox->u16Data);
            break;
        case MApi_CMD_AUDIO_TriggerSIFPLL:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_TriggerSIFPLL\n");
            psAudioInstPri->fpAUDIOTriggerSifPLL();
            break;
        case MApi_CMD_AUDIO_Monitor:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_Monitor\n");
            psAudioInstPri->fpAUDIOMonitor();
            break;
        case MApi_CMD_AUDIO_SetSystemInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetSystemInfo\n");
            psAudioInstPri->fpAUDIOSetSystemInfo((AUDIO_INIT_INFO*)pArgs);
            break;
        case MApi_CMD_AUDIO_ASF_Data_To_DDR:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_AUDIO_ASF_Data_To_DDR\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOASFDataToDDR((MS_BOOL)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_R2_DEBUG_MSG:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_AUDIO_R2_Debug_MSG\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOR2DebugMSG((MS_BOOL)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_R2_DDR_LETENCY_MSG:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_R2_DDR_LETENCY_MSG\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOR2DDRLetencyMSG((MS_BOOL)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SetOutConnectivity:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetOutConnectivity\n");
            psAudioInstPri->fpAUDIOSetOutConnectivity();
            break;
        case MApi_CMD_AUDIO_SetPathInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetPathInfo\n");
            psAudioInstPri->fpAUDIOSetPathInfo((AUDIO_PATH_INFO*)pArgs);
            break;
        case MApi_CMD_AUDIO_GetPathInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetPathInfo\n");
            psAudioInstPri->fpAUDIOGetPathInfo((AUDIO_PATH_INFO*)pArgs);
            break;
        case MApi_CMD_AUDIO_SetOutputInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetOutputInfo\n");
            psAudioInstPri->fpAUDIOSetOutputInfo((AUDIO_OUT_INFO*)pArgs);
            break;
        case MApi_CMD_AUDIO_SetSourceInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetSourceInfo\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetSourceInfo((MS_BOOL)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_InputSwitch:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_InputSwitch\n");
            pInputSwitch = (PAUDIO_ST_INPUTSWITCH)pArgs;
            psAudioInstPri->fpAUDIOInputSwitch(pInputSwitch->eSource, pInputSwitch->eGroup);
            break;

        case MApi_CMD_AUDIO_Set_Input_Scale:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_Set_Input_Scale\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetInputScale((MS_BOOL)pu32Param[0]);
            break;

#if 0
        case MApi_CMD_AUDIO_AUDYSSEY_VXT_SET_THRESH_LEVEL_TABLE:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_AUDYSSEY_VXT_SET_THRESH_LEVEL_TABLE\n");
            pAudyssey_VXT_setTHRESH_Level_Table = (PAUDIO_ST_AUDYSSEY_VXT_SET_THRESH_LEVEL_TABLE)pArgs;
            bStatus = psAudioInstPri->fpAUDIOAudysseyVXTSetThreshLevelTable((int*)pAudyssey_VXT_setTHRESH_Level_Table->pthresh_level_table);
            pAudyssey_VXT_setTHRESH_Level_Table->bStatus = bStatus;
            break;
#endif
        case MApi_CMD_AUDIO_SPDIF_CHANNELSTATUS:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SPDIF_CHANNELSTATUS\n");
            pSPDIF_ChannelStatus = (PAUDIO_ST_SPDIF_CHANNELSTATUS)pArgs;
            if(pSPDIF_ChannelStatus->bSet == TRUE)
            {
                psAudioInstPri->fpAUDIOSPDIFSetChannelStauts(pSPDIF_ChannelStatus->eType, pSPDIF_ChannelStatus->eValue);
            }
            else
            {
                u16Status = psAudioInstPri->fpAUDIOSPDIFGetChannelStatus(pSPDIF_ChannelStatus->eType);
                pSPDIF_ChannelStatus->u32Ret = (MS_U32)u16Status;
            }
            break;

        case MApi_CMD_AUDIO_Set_Scart_Scale:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_Set_Scart_Scale\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetScartScale((MS_BOOL)pu32Param[0]);
            break;

        case MApi_CMD_AUDIO_SetDataCaptureSource:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetDataCaptureSource\n");
            pSet_DataCaptureResource = (PAUDIO_ST_SET_DATACAPTURESOURCE)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSetDataCaptureSource(pSet_DataCaptureResource->eID, pSet_DataCaptureResource->eSource);
            pSet_DataCaptureResource->bRet = bStatus;
            break;
        case MApi_CMD_AUDIO_OutputSourceInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_OutputSourceInfo\n");
            pOutputSourceInfo = (PAUDIO_ST_OUTPUT_SOURCE)pArgs;
            if(pOutputSourceInfo->bSet == TRUE)
            {
                psAudioInstPri->fpAUDIOSetOutputSourceInfo((OUTPUT_SOURCE_INFO *)pOutputSourceInfo->stOutputSourceInfo);
            }
            else
            {
                psAudioInstPri->fpAUDIOGetOutputSourceInfo((OUTPUT_SOURCE_INFO *)pOutputSourceInfo->stOutputSourceInfo);
            }
            break;
#if 0
        case MApi_CMD_AUDIO_SetOutputSourceInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetOutputSourceInfo\n");
            psAudioInstPri->fpAUDIOSetOutputSourceInfo((OUTPUT_SOURCE_INFO *)pArgs);
            break;
        case MApi_CMD_AUDIO_GetOutputSourceInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMS_AUDIO_GetOutputSourceInfo\n");
            psAudioInstPri->fpAUDIOGetOutputSourceInfo((OUTPUT_SOURCE_INFO *)pArgs);
            break;
#endif
        case MApi_CMD_AUDIO_ReadMailBox:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_ReadMailBox\n");
            pRead_MailBox = (PAUDIO_ST_READ_MAILBOX)pArgs;
            u16Status = psAudioInstPri->fpAUDIOReadMailBox(pRead_MailBox->bDspType, pRead_MailBox->u8ParamNum);
            pRead_MailBox->u16Ret = u16Status;
            break;
        case MApi_CMD_AUDIO_WriteMailBox:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_WriteMailBox\n");
            pWrite_MailBox = (PAUDIO_ST_WRITE_MAILBOX)pArgs;
            psAudioInstPri->fpAUDIOWriteMailBox(pWrite_MailBox->bDspType, pWrite_MailBox->u8ParamNum, pWrite_MailBox->u16Data);
            break;
        case MApi_CMD_AUDIO_FwTriggerDSP:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_FwTriggerDSP\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOFwTriggerDsp((MS_U16)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SetFwStatus:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetFwStatus\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetFwStatus((MS_BOOL)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_ExitAudioSystem:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_ExitAudioSystem\n");
            psAudioInstPri->fpAUDIOExitAudioSystem();
            break;
        case MApi_CMD_AUDIO_RebootDSP:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_RebootDSP\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIORebootDSP((MS_BOOL)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SendIntrupt:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SendIntrupt\n");
            pSend_Interrupt = (PAUDIO_ST_SEND_INTERRUPT)pArgs;
            psAudioInstPri->fpAUDIOSendIntrupt(pSend_Interrupt->bDspType, pSend_Interrupt->u8Cmd);
            break;
        case MApi_CMD_AUDIO_I2S_SetMode:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_I2S_SetMode\n");
            pI2s_SetMode = (PAUDIO_ST_I2S_SETMODE)pArgs;
            psAudioInstPri->fpAUDIOI2SSetMode(pI2s_SetMode->eMode, pI2s_SetMode->eVal);
            break;
        case MApi_CMD_AUDIO_WritePreInitTable:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_WritePreInitTable\n");
            psAudioInstPri->fpAUDIOWritePreInitTable();
            break;
        case MApi_CMD_AUDIO_EnaEarphone_LowPower_Stage:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_EnaEarphone_LowPower_Stage\n");
            psAudioInstPri->fpAUDIOEnaEarphoneLowPowerStage();
            break;
        case MApi_CMD_AUDIO_EnaEarphone_HighDriving_Stage:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_EnaEarphone_HighDriving_Stage\n");
            psAudioInstPri->fpAUDIOEnaEarphoneHighDrivingStage();
            break;
        case MApi_CMD_AUDIO_Suspend:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_Suspend\n");
            bStatus = psAudioInstPri->fpAUDIOSuspend();
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;
        case MApi_CMD_AUDIO_Resume:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_Resume\n");
            bStatus = psAudioInstPri->fpAUDIOResume();
            *((MS_BOOL*)&(pu32Param[0])) = bStatus;
            break;

        //=============================================================
        // AUDIO_SOUND RELATIONAL API FUNCTION
        //=============================================================
        case MApi_CMD_SND_ProcessEnable:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_SND_ProcessEnable\n");
            pSnd_ProcessEnable = (PAUDIO_ST_SND_PROCESSENABLE)pArgs;
            bStatus = psAudioInstPri->fpSNDProcessEnable(pSnd_ProcessEnable->eType, pSnd_ProcessEnable->bEnable);
            pSnd_ProcessEnable->bRet = bStatus;
            break;
        case MApi_CMD_SND_SetParam1:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_SND_SetParam1\n");
            pSnd_SetParam1 = (PAUDIO_ST_SND_SETPARAM1)pArgs;
            bStatus = psAudioInstPri->fpSNDSetParam1(pSnd_SetParam1->eType, pSnd_SetParam1->u16Param1, pSnd_SetParam1->u16Param2);
            pSnd_SetParam1->bRet = bStatus;
            break;
        case MApi_CMD_SND_GetParam1:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_SND_GetParam1\n");
            pSnd_GetParam1 = (PAUDIO_ST_SND_GETPARAM1)pArgs;
            u16Status = psAudioInstPri->fpSNDGetParam1(pSnd_GetParam1->eType, pSnd_GetParam1->u16Param1);
            pSnd_GetParam1->u16Ret = u16Status;
            break;
        case MApi_CMD_AUDIO_Sound_SetParam:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_Sound_SetParam\n");
            pSoundSetParam = (PAUDIO_ST_SOUND_SETPARAM)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSoundSetParam((Sound_SET_Type)pSoundSetParam->eType, (MS_U32*)pSoundSetParam->pParam);
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[Type 0x%x] [0]: 0x%x [1]: 0x%x, [2]:0x%x, [3]:0x%x\n",(int)pSoundSetParam->eType, (int)*(pSoundSetParam->pParam), (int)*(pSoundSetParam->pParam+1), (int)*(pSoundSetParam->pParam+2), (int)*(pSoundSetParam->pParam+3));
            pSoundSetParam->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_SetAbsoluteVolume:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetAbsoluteVolume\n");
            pSet_AbsoluteVolume = (PAUDIO_ST_SET_ABSOLUTEVOLUME)pArgs;
            psAudioInstPri->fpAUDIOSetAbsoluteVolume(pSet_AbsoluteVolume->u8Path, pSet_AbsoluteVolume->u8Vol1, pSet_AbsoluteVolume->u8Vol2);
            break;
#if 0
        case MApi_CMD_AUDIO_SetPreScale:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetPreScale\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetPreScale((MS_U8)pu32Param[0], (MS_U8)pu32Param[1]);
            break;
        case MApi_CMD_AUDIO_SetMute:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetPreScale\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetMute((MS_U8)pu32Param[0], (MS_BOOL)pu32Param[1]);
            break;
#endif
        case MApi_CMD_AUDIO_SourceConnect:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SourceConnect\n");
            pSourceConnect = (PAUDIO_ST_SOURCECONNECT)pArgs;
            psAudioInstPri->fpAUDIOSourceConnect(pSourceConnect->eConnectSourceType, pSourceConnect->bConnect);
            break;
#if 0
        case MApi_CMD_AUDIO_SetBalance:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetBalance\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetBalance((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_EnableEQ:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_EnableEQ\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOEnableEQ((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_EnableTone:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_EnableTone\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOEnableTone((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_EnableAutoVolume:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_EnableAutoVolume\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOEnableAutoVolume((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_EnableSurround:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_EnableSurround\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOEnableSurround((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_EnableBalance:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_EnableBalance\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOEnableBalance((MS_U8)pu32Param[0]);
            break;
#endif
        case MApi_CMD_AUDIO_SetDynamicBass:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetDynamicBass\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetDynamicBass((MS_U8)pu32Param[0]);
            break;
#if 0
        case MApi_CMD_AUDIO_SetBass:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetBass\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetBass((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_AbsoluteBass:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_AbsoluteBass\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOAbsoluteBass((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SetTreble:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetTreble\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetTreble((MS_U8)pu32Param[0]);
            break;
#endif
        case MApi_CMD_AUDIO_AbsoluteTreble:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_AbsoluteTreble\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOAbsoluteTreble((MS_U8)pu32Param[0]);
            break;
#if 0
        case MApi_CMD_AUDIO_SetEq:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetEq\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetEq((MS_U8)pu32Param[0], (MS_U8)pu32Param[1]);
            break;
#endif

        case MApi_CMD_AUDIO_SetEq_7band:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetEq_7band\n");
            pSet_7band = (PAUDIO_ST_SETEQ_7BAND)pArgs;
            psAudioInstPri->fpAUDIOSetEq7band(pSet_7band->u8Band, pSet_7band->u8Level);
            break;

        case MApi_CMD_AUDIO_SetAvcThreshold:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetAvcThreshold\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetAvcThreshold((MS_U8)pu32Param[0]);
            break;

        case MApi_CMD_AUDIO_SetAvcMode:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetAvcMode\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetAvcMode((MS_U8)pu32Param[0]);
            break;

        case MApi_CMD_AUDIO_SetAvcAT:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetAvcAT\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetAvcAT((MS_U8)pu32Param[0]);
            break;

        case MApi_CMD_AUDIO_SetAvcRT:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetAvcRT\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetAvcRT((MS_U8)pu32Param[0]);
            break;

        case MApi_CMD_AUDIO_SetBufferProcess:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetBufferProcess\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetBufferProcess((MS_U8)pu32Param[0]);
            break;

        case MApi_CMD_AUDIO_SetSurroundXA:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetSurroundXA\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetSurroundXA((MS_U8)pu32Param[0]);
            break;

        case MApi_CMD_AUDIO_SetSurroundXB:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetSurroundXB\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetSurroundXB((MS_U8)pu32Param[0]);
            break;

        case MApi_CMD_AUDIO_SetSurroundXK:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetSurroundXK\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetSurroundXK((MS_U8)pu32Param[0]);
            break;

        case MApi_CMD_AUDIO_SetSurroundLPFGain:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetSurrounLPFGain\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetSurroundLPFGain((MS_U8)pu32Param[0]);
            break;

        case MApi_CMD_AUDIO_ConvertVolumeUnit:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_ConvertVolumeUnit\n");
            pConvertVolumeUnit = (PAUDIO_ST_CONVERT_VOLUMEUNIT)pArgs;
            u8Status= psAudioInstPri->fpAUDIOConvertVolumeUnit(pConvertVolumeUnit->u8Uivolume);
            pConvertVolumeUnit->u8Ret = u8Status;
            break;
        case MApi_CMD_AUDIO_SetPEQCoef:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetPEQCoef\n");
            psAudioInstPri->fpAUDIOSetPEQCoef((AUDIO_PEQ_COEF*)pArgs);
            break;
#if 0
        case MApi_CMD_AUDIO_EnablePEQ:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_EnablePEQ\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOEnablePEQ((MS_U8)pu32Param[0]);
            break;
#endif
        case MApi_CMD_AUDIO_EnableDcRemove:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_EnableDcRemove\n");
            pEnableDcRemove = (PAUDIO_ST_ENABLE_DCREMOVE)pArgs;
            bStatus = psAudioInstPri->fpAUDIOEnableDcRemove(pEnableDcRemove->bEndcremove);
            pEnableDcRemove->bRet = bStatus;
            break;

        //============================================================
        // AUDIO_DTV RELATIONAL API FUNCTION
        //============================================================
#if 0
        case MApi_CMD_AUDIO_DecodeSystem:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_DecodeSystem\n");
            pDecodeSystem = (PAUDIO_ST_DECODESYSTEM)pArgs;
            if(pDecodeSystem->eType == DECODE_SYS_RELEASE)
            {
                bStatus= psAudioInstPri->fpAUDIOReleaseDecodeSystem(pDecodeSystem->DecId);
                pDecodeSystem->bStatus = bStatus;
            }
            else if(pDecodeSystem->eType == DECODE_SYS_SET)
            {
                bStatus = psAudioInstPri->fpAUDIOSetDecodeSystem(pDecodeSystem->DecId, pDecodeSystem->p_AudioDecStatus);
                pDecodeSystem->bStatus = bStatus;
            }
            else if(pDecodeSystem->eType == DECODE_SYS_GET)
            {
                bStatus = psAudioInstPri->fpAUDIOGetDecodeSystem(pDecodeSystem->DecId, pDecodeSystem->p_AudioDecStatus);
                pDecodeSystem->bStatus = bStatus;
            }
            else if(pDecodeSystem->eType == DECODE_SYS_OPEN)
            {
                eDecId = psAudioInstPri->fpAUDIOOpenDecodeSystem(pDecodeSystem->p_AudioDecStatus);
                pDecodeSystem->DecId = eDecId;
            }
            break;
#endif
#if 1
        case MApi_CMD_AUDIO_ReleaseDecodeSystem:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_ReleaseDecodeSystem\n");
            pReleaseDecodeSystem = (PAUDIO_ST_RELEASE_DECODESYSTEM)pArgs;
            bStatus= psAudioInstPri->fpAUDIOReleaseDecodeSystem(pReleaseDecodeSystem->DecId);
            pReleaseDecodeSystem->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_SetDecodeSystem:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetDecodeSystem\n");
            pSetDecodeSystem = (PAUDIO_ST_SET_DECODESYSTEM)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSetDecodeSystem(pSetDecodeSystem->DecId, pSetDecodeSystem->p_AudioDecStatus);
            pSetDecodeSystem->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_GetDecodeSystem:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetDecodeSystem\n");
            pGetDecodeSystem = (PAUDIO_ST_GET_DECODESYSTEM)pArgs;
            bStatus = psAudioInstPri->fpAUDIOGetDecodeSystem(pGetDecodeSystem->DecId, pGetDecodeSystem->p_AudioDecStatus);
            pGetDecodeSystem->bStatus = bStatus;
            break;
#endif
        case MApi_CMD_AUDIO_SetDecodeCmd:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetDecodeCmd\n");
            pSetDecCmd = (PAUDIO_ST_SET_DECCMD)pArgs;
            bStatus= psAudioInstPri->fpAUDIOSetDecodeCmd(pSetDecCmd->eDecId, pSetDecCmd->eDecComamnd);
            pSetDecCmd->bRet = bStatus;
            break;
#if 1
        case MApi_CMD_AUDIO_OpenDecodeSystem:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_OpenDecodeSystem\n");
            pOpenDecodeSystem = (PAUDIO_ST_OPEN_DECODESYSTEM)pArgs;
            eDecId = psAudioInstPri->fpAUDIOOpenDecodeSystem(pOpenDecodeSystem->p_AudioDecStatus);
            pOpenDecodeSystem->DecId = eDecId;
            break;
#endif
        case MApi_CMD_AUDIO_SetSystem:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetSystem\n");
            pSetSystem = (PAUDIO_ST_SET_SYSTEM)pArgs;
            bStatus= psAudioInstPri->fpAUDIOSetSystem(pSetSystem->eDecSystem);
            pSetSystem->bRet = bStatus;
            break;
        case MApi_CMD_AUDIO_GetDecSysSupportStatus:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetDecSysSupportStatus\n");
            pGetDecSysSupportStatus = (PAUDIO_ST_GET_DECSYSSUPPORTSTATUS)pArgs;
            bStatus = psAudioInstPri->fpAUDIOGetDecSysSupportStatus(pGetDecSysSupportStatus->eDecSystem);
            pGetDecSysSupportStatus->bRet = bStatus;
            break;
        case MApi_CMD_AUDIO_IsMadLock:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_IsMadLock\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOIsMadLock();
            *((MS_BOOL*)&(pu32Param[1])) = bStatus;
            break;
        case MApi_CMD_AUDIO_GetDecStatus:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetDecStatus\n");
            pu32Param = (MS_U32*)pArgs;
            eDecCmdType = psAudioInstPri->fpAUDIOGetDecStatus();
            *((En_DVB_decCmdType*)&(pu32Param[1])) = eDecCmdType;
            break;
        case MApi_CMD_AUDIO_Decode_Command:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_Decode_Command\n");
            pDecoderCommand = (PAUDIO_ST_DECODER_COMMAND)pArgs;
            if(pDecoderCommand->bSet == TRUE)
            {
                psAudioInstPri->fpAUDIOSetCommand(pDecoderCommand->enCommandType);
            }
            else
            {
                eDecCmd = psAudioInstPri->fpAUDIOGetCommand(pDecoderCommand->eAdoDecoderNo);
                pDecoderCommand->eDecCmd = eDecCmd;
            }
            break;
#if 0
        case MApi_CMD_AUDIO_GetCommand:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetCommand\n");
            pu32Param = (MS_U32*)pArgs;
            eDecCmd = psAudioInstPri->fpAUDIOGetCommand((En_DVB_AudioDecoder)pu32Param[1]);
            *((AU_DVB_DECCMD*)&(pu32Param[0])) = eDecCmd;
            break;
        case MApi_CMD_AUDIO_SetCommand:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetCommand\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetCommand((En_DVB_decCmdType)pu32Param[0]);
            break;
#endif
        case MApi_CMD_AUDIO_GetMAD_Lock:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetMAD_Lock\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOGetMADLock();
            *((MS_BOOL*)&(pu32Param[1])) = bStatus;
            break;
        case MApi_CMD_AUDIO_SETAD:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SETAD\n");
            pSetAD = (PAUDIO_ST_SET_AD)pArgs;
            if(pSetAD->eType == AD_OUTPUT_MODE)
            {
                psAudioInstPri->fpAUDIOSetADOutputMode(pSetAD->u8Data);
            }
            else if(pSetAD->eType == AD_ABSOLUTE_VOLUME)
            {
                psAudioInstPri->fpAUDIOSetADAbsoluteVoluem(pSetAD->u8Data);
            }
            else if(pSetAD->eType == AD_SET_MUTE)
            {
                psAudioInstPri->fpAUDIOADSetMute(pSetAD->bMute);
            }
            else if(pSetAD->eType == AD_SET_MAIN_VOL)
            {
                psAudioInstPri->fpAUDIOSetMainVolumeForAD(pSetAD->u8Data, pSetAD->u8Data2);
            }
            break;
#if 0
        case MApi_CMD_AUDIO_SetADOutputMode:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetADOutputMode\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetADOutputMode((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SetADAbsoluteVolume:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetADAbsoluteVolume\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetADAbsoluteVoluem((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_ADSetMute:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_ADSetMute\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOADSetMute((MS_BOOL)pu32Param[0]);
            break;
#endif
        //============================================================
        // AUDIO_PVR RELATED FUNCTIONS
        //============================================================
        case MApi_CMD_AUDIO_SetEncInit:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetEncInit\n");
            pSetEncInit = (PAUDIO_ST_SET_ENCINIT)pArgs;
            psAudioInstPri->fpAUDIOSetEncInit(pSetEncInit->eBitRate, pSetEncInit->u8EncFrameNum);
            break;
        case MApi_CMD_AUDIO_SetEncCommand:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetEncCommand\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetEncCommand((AU_DVB_ENCCMD)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_GetEncodeFrameInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetEncodeFrameInfo\n");
            psAudioInstPri->fpAUDIOGetEncodeFrameInfo((AU_DVB_ENC_FRAME_INFO*)pArgs);
            break;
        case MApi_CMD_AUDIO_GetEncodeDoneFlag:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetEncodeDoneFlag\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOGetEncodeDoneFlag();
            *((MS_BOOL*)&(pu32Param[1])) = bStatus;
            break;

        //============================================================
        // AUDIO_ATV RELATIONAL API FUNCTION
        //============================================================
        case MApi_CMD_AUDIO_SIF_SetStandard:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SIF_SetStandard\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSIFSetStandard((AUDIOSTANDARD_TYPE)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SIF_GetSoundMode:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SIF_GetSoundMode\n");
            pu32Param = (MS_U32*)pArgs;
            u8Status = psAudioInstPri->fpAUDIOSIFGetSoundMode();
            *((MS_U8*)&(pu32Param[1])) = u8Status;
            break;
        case MApi_CMD_AUDIO_SIF_SetSoundMode:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SIF_SetSoundMode\n");
            pSifSetSoundMode = (PAUDIO_ST_SIF_SET_SOUNDMODE)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSIFSetSoundMode(pSifSetSoundMode->u8SifSoundMode);
            pSifSetSoundMode->bRet = bStatus;
            break;
        case MApi_CMD_AUDIO_SIF_GetAudioStatus:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SIF_GetAudioStatus\n");
            pSifGetAudioStatus = (PAUDIO_ST_SIF_GET_AUDIO_STATUS)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSIFGetAudioStatus(pSifGetAudioStatus->p_eAudioStatus);
            pSifGetAudioStatus->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_SIF_StartAutoStandardDetection:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SIF_StartAutoStandardDetection\n");
            psAudioInstPri->fpAUDIOSIFStartAutoStandardDetection();
            break;
        case MApi_CMD_AUDIO_SIF_GetResultOfAutoStandardDetection:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SIF_GetResultOfAutoStandardDetection\n");
            pu32Param = (MS_U32*)pArgs;
            eAUDIOStand_Type = psAudioInstPri->fpAUDIOSIFGetResultOfAutoStandardDetection();
            *((AUDIOSTANDARD_TYPE*)&(pu32Param[1])) = eAUDIOStand_Type;
            break;
        case MApi_CMD_AUDIO_SIF_ConvertToBasicAudioStandard:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SIF_ConvertToBasicAudioStandard\n");
            pSifConvertToBasicAudioStandard = (PAUDIO_ST_SIF_CONVERTTOBASICAUDIOSTANDARD)pArgs;
            eAUDIOStand_Type = psAudioInstPri->fpAUDIOSIFConvertToBasicAudioStandard(pSifConvertToBasicAudioStandard->eStandard);
            pSifConvertToBasicAudioStandard->eRet = eAUDIOStand_Type;
            break;
        case MApi_CMD_AUDIO_SIF_SetThreshold:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SIF_SetThreshold\n");
            psAudioInstPri->fpAUDIOSIFSetThreshold((THR_TBL_TYPE code*)pArgs);
            break;
        case MApi_CMD_AUDIO_SIF_SetPrescale:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SIF_SetPrescale\n");
            pSifSetPrescale = (PAUDIO_ST_SIF_SETPRESCALE)pArgs;
            psAudioInstPri->fpAUDIOSIFSetPrescale(pSifSetPrescale->u8GainType, pSifSetPrescale->s32DbValue);
            break;
        case MApi_CMD_AUDIO_SIF_IsPALType:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SIF_IsPALType\n");
            pSifIsPalType = (PAUDIO_ST_SIF_ISPALTYPE)pArgs;
            bStatus= psAudioInstPri->fpAUDIOSIFIsPALType(pSifIsPalType->ePalType);
            pSifIsPalType->bRet = bStatus;
            break;
        case MApi_CMD_AUDIO_SIF_SetPALType:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SIF_SetPALType\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSIFSetPALType((AUDIO_SIF_PAL_TYPE)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SIF_SendCmd:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SIF_SendCmd\n");
            pSifSendCmd = (PAUDIO_ST_SIF_SENDCMD)pArgs;
            psAudioInstPri->fpAUDIOSIFSendCmd(pSifSendCmd->eAudSifCommand, pSifSendCmd->u8CommArg1, pSifSendCmd->u8CommArg2);
            break;
        case MApi_CMD_AUDIO_SIF_Shift:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SIF_Shift\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSIFShift((En_AUD_VIF_Type)pu32Param[0]);
            break;

#if 1
        //============================================================
        // AUDIO_FM_RADIO RELATIONAL API FUNCTION
        //============================================================
        case MApi_CMD_AUDIO_FM_RADIO_GetSoundMode:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_FM_RADIO_GetSoundMode\n");
            pu32Param = (MS_U32*)pArgs;
            u8Status = psAudioInstPri->fpAUDIOFMRADIOGetSoundMode();
            *((MS_U8*)&(pu32Param[1])) = u8Status;
            break;
        case MApi_CMD_AUDIO_FM_RADIO_SetSoundMode:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_FM_RADIO_SetSoundMode\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOFMRADIOSetSoundMode((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_FM_RADIO_DeEmphassisOption:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_FM_RADIO_DeEmphassisOption\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOFMRADIODeEmphassisOption((MS_BOOL)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_FM_RADIO_GetDC_Amp:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_FM_RADIO_GetDC_Amp\n");
            ps32Param = (MS_S32*)pArgs;
            s8Status = psAudioInstPri->fpAUDIOFMRADIOGetDcAmp();
            *((MS_S8*)&(ps32Param[1])) = s8Status;
            break;
        case MApi_CMD_AUDIO_FM_RADIO_Get_NSR:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_FM_RADIO_Get_NSR\n");
            pu32Param = (MS_U32*)pArgs;
            u8Status = psAudioInstPri->fpAUDIOFMRADIOGetNSR();
            *((MS_U8*)&(pu32Param[1])) = u8Status;
            break;
        case MApi_CMD_AUDIO_FM_RADIO_Reset:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_FM_RADIO_Reset\n");
            psAudioInstPri->fpAUDIOFMRADIOReset();
            break;
#endif

        //=============================================================
        // AUDIO_SPDIF RELATIONAL API FUNCTION
        //=============================================================
        case MApi_CMD_AUDIO_SPDIF_OP:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SPDIF_OP\n");
            pSpdif_op = (PAUDIO_ST_SPDIF_OP)pArgs;

            if(pSpdif_op->eType == SPDIF_HWEN)
            {
                psAudioInstPri->fpAUDIOSPDIFHWEN(pSpdif_op->spdif_hwen);
            }
            else if(pSpdif_op->eType == SPDIF_SETMUTE)
            {
                psAudioInstPri->fpAUDIOSPDIFSetMute(pSpdif_op->mute_en);
            }
            else if(pSpdif_op->eType == SPDIF_SETMODE)
            {
                psAudioInstPri->fpAUDIOSPDIFSetMode(pSpdif_op->spdif_mode);
            }
            else if(pSpdif_op->eType == SPDIF_GETMODE)
            {
                u8Status = psAudioInstPri->fpAUDIOSPDIFGetMode();
                pSpdif_op->spdif_mode = u8Status;
            }
            break;
        case MApi_CMD_AUDIO_SPDIF_SCMS:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SPDIF_SCMS\n");
            pSpdif_scms = (PAUDIO_ST_SPDIF_SCMS)pArgs;
            if(pSpdif_scms->bSetSCMS == TRUE)
            {
                psAudioInstPri->fpAUDIOSPDIFSetSCMS(pSpdif_scms->C_bit_en, pSpdif_scms->L_bit_en);
            }
            else
            {
                u8Status = psAudioInstPri->fpAUDIOSPDIFGetSCMS();
                pSpdif_scms->Ret_bit = u8Status;
            }
            break;
#if 0
        case MApi_CMD_AUDIO_SPDIF_HWEN:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SPDIF_HWEN\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSPDIFHWEN((MS_BOOL)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SPDIF_SetMute:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SPDIF_SetMute\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSPDIFSetMute((MS_BOOL)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SPDIF_SetMode:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SPDIF_SetMode\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSPDIFSetMode((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SPDIF_GetMode:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SPDIF_GetMode\n");
            pu32Param = (MS_U32*)pArgs;
            u8Status = psAudioInstPri->fpAUDIOSPDIFGetMode();
            *((MS_U8*)&(pu32Param[1])) = u8Status;
            break;
        case MApi_CMD_AUDIO_SPDIF_SetSCMS:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - IOCTL_AUDIO_SPDIF_SETSCMS\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSPDIFSetSCMS((MS_U8)pu32Param[0], (MS_U8)pu32Param[1]);
            break;
        case MApi_CMD_AUDIO_SPDIF_GetSCMS:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SPDIF_GetSCMS\n");
            pu32Param = (MS_U32*)pArgs;
            u8Status = psAudioInstPri->fpAUDIOSPDIFGetSCMS();
            *((MS_U8*)&(pu32Param[1])) = u8Status;
            break;
#endif
        case MApi_CMD_AUDIO_SPDIF_Monitor:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SPDIF_Monitor\n");
            psAudioInstPri->fpAUDIOSPDIFMonitor();
            break;
        case MApi_CMD_AUDIO_SPDIF_ChannelStatus_CTRL:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SPDIF_ChannelStatus_CTRL\n");
            pSpdifChannelStatusCtrl = (PAUDIO_ST_SPDIF_CHANNELSTATUS_CTRL)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSPDIFChannelStatusCTRL(pSpdifChannelStatusCtrl->eCsMode, pSpdifChannelStatusCtrl->eStatus);
            pSpdifChannelStatusCtrl->bRet = bStatus;
            break;
        case MApi_CMD_AUDIO_DigitalOut_SetChannelStatus:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_DigitalOut_SetChannelStatus\n");
            pDigitalOut_SetChannelStatus = (PAUDIO_ST_DIGITALOUT_SETCHANNELSTATUS)pArgs;
            bStatus = psAudioInstPri->fpAUDIODigitalOut_SetChannelStatus((DIGITAL_OUTPUT_TYPE)pDigitalOut_SetChannelStatus->ePath, (Digital_Out_Channel_Status_t*)pDigitalOut_SetChannelStatus->stChannelStatus);
            pDigitalOut_SetChannelStatus->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_DigitalOut_GetChannelStatus:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_DigitalOut_GetChannelStatus\n");
            pDigitalOut_GetChannelStatus = (PAUDIO_ST_DIGITALOUT_GETCHANNELSTATUS)pArgs;
            psAudioInstPri->fpAUDIODigitalOut_GetChannelStatus((DIGITAL_OUTPUT_TYPE)pDigitalOut_GetChannelStatus->ePath, (Digital_Out_Channel_Status_t*)pDigitalOut_GetChannelStatus->stChannelStatus);
            break;
        case MApi_CMD_AUDIO_DigitalOut_SetDeviceCapability:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_DigitalOut_SetDeviceCapability\n");
            pDigitalOut_SetDeviceCap = (PAUDIO_ST_DIGITALOUT_SETDEVICECAP)pArgs;
            bStatus = psAudioInstPri->fpAUDIODigitalOut_SetDeviceCapabilty((DIGITAL_OUTPUT_TYPE)pDigitalOut_SetDeviceCap->eoutType, (AUDIO_FORMAT_CODE)pDigitalOut_SetDeviceCap->ecodecType, (Digital_Out_Device_Capability_t*)pDigitalOut_SetDeviceCap->p_codecCapability);
            pDigitalOut_SetDeviceCap->bStatus = bStatus;
            break;
        //=============================================================
        // AUDIO_HDMI RELATIONAL API FUNCTION
        //=============================================================
        case MApi_CMD_AUDIO_HDMI_Tx_SetMute:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_HDMI_Tx_SetMute\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOHDMITxSetMute((MS_BOOL)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_HDMI_Tx_GetStatus:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_HDMI_Tx_GetStatus\n");
            pHDMITx_GetStatus = (PAUDIO_HDMI_TX_GETSTATUS)pArgs;
            psAudioInstPri->fpAUDIOHDMITxGetStatus(&(pHDMITx_GetStatus->onOff), &(pHDMITx_GetStatus->hdmi_smpFreq), &(pHDMITx_GetStatus->outType));
            break;
        case MApi_CMD_AUDIO_HDMI_Monitor:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_HDMI_Monitor\n");
            pu32Param = (MS_U32*)pArgs;
            u8Status= psAudioInstPri->fpAUDIOHDMIMonitor();
            *((MS_U8*)&(pu32Param[1])) = u8Status;
            break;
        case MApi_CMD_AUDIO_HDMI_GetNonpcmFlag:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_HDMI_GetNonpcmFlag\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOHDMIGetNonpcmFlag();
            *((MS_BOOL*)&(pu32Param[1])) = bStatus;
            break;
        case MApi_CMD_AUDIO_HDMI_SetNonpcm:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_HDMI_SetNonpcm\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOHDMISetNonpcm((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_HDMI_Nonpcm_OP:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_HDMI_Nonpcm_OP\n");
            pHDMI_Nonpcm_op = (PAUDIO_ST_HDMI_NONPCM_OP)pArgs;
            if(pHDMI_Nonpcm_op->bSet == TRUE)
            {
                eHDMI_RX_Type = psAudioInstPri->fpAUDIOHDMIRxSetNonPCM(pHDMI_Nonpcm_op->nonPCM_en);
            }
            else
            {
                eHDMI_RX_Type = psAudioInstPri->fpAUDIOHDMIRxGetNonPCM();
            }
            pHDMI_Nonpcm_op->eRet = eHDMI_RX_Type;
            break;
#if 0
        case MApi_CMD_AUDIO_HDMI_Rx_SetNonPcm:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_HDMI_Rx_SetNonPcm\n");
            pu32Param = (MS_U32*)pArgs;
            eHDMI_RX_Type = psAudioInstPri->fpAUDIOHDMIRxSetNonPCM((MS_U8)pu32Param[1]);
            *((AUDIO_HDMI_RX_TYPE*)&(pu32Param[0])) = eHDMI_RX_Type;
            break;

        case MApi_CMD_AUDIO_HDMI_RX_GetNonpcm:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_HDMI_RX_GetNonpcm\n");
            pu32Param = (MS_U32*)pArgs;
            eHDMI_RX_Type = psAudioInstPri->fpAUDIOHDMIRxGetNonPCM();
            *((AUDIO_HDMI_RX_TYPE*)&(pu32Param[1])) = eHDMI_RX_Type;
            break;
#endif
        case MApi_CMD_AUDIO_DTV_HDMI_CFG:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_DTV_HDMI_CFG\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIODTVHDMICFG((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_HDMI_GetSynthFreq:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_HDMI_GetSynthFreq\n");
            pu32Param = (MS_U32*)pArgs;
            u16Status = psAudioInstPri->fpAUDIOHDMIGetSynthFreg();
            *((MS_U16*)&(pu32Param[1])) = u16Status;
            break;
        case MApi_CMD_AUDIO_HDMI_SetDownSample:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_HDMI_SetDownSample\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOHDMISetDownSample((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_HDMI_TX_SetMode:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_HDMI_TX_SetMode\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOHDMITxSetMode((HDMI_TX_OUTPUT_TYPE)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_HDMI_RX_GetHdmiInAudioStatus:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_HDMI_RX_GetHdmiInAudioStatus\n");
            pRxHdmiSts = (PAUDIO_ST_RX_HDMI_AUDIO_STATUS)pArgs;
            bStatus = psAudioInstPri->fpAUDIOHDMIRxGetHDMIInAUDIOStatus(pRxHdmiSts->pHdmi_status);
            pRxHdmiSts->bStatus = bStatus;
            break;

        //=============================================================
        // AUDIO_ADVANCE_SOUND RELATIONAL API FUNCTION
        //=============================================================
        case MApi_CMD_AUDIO_ADVSOUND_ProcessEnable:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_ADVSOUND_ProcessEnable\n");
            pAdvSoundProcessEnable = (PAUDIO_ST_ADVSOUND_PROCESSENABLE)pArgs;
            bStatus = psAudioInstPri->fpAUDIOADVSOUNDProcessEnable(pAdvSoundProcessEnable->eType);
            pAdvSoundProcessEnable->bRet = bStatus;
            break;
        case MApi_CMD_AUDIO_ADVSOUND_SubProcessEnable:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_ADVSOUND_SubProcessEnable\n");
            pAdvSoundSubProcessEnable = (PAUDIO_ST_ADVSOUND_SUBPROCESSENABLE)pArgs;
            bStatus = psAudioInstPri->fpAUDIOADVSOUNDSubProcessEnable(pAdvSoundSubProcessEnable->eProc, pAdvSoundSubProcessEnable->bEnable);
            pAdvSoundSubProcessEnable->bRet = bStatus;
            break;
        case MApi_CMD_AUDIO_ADVSOUND_SetParam:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_ADVSOUND_SubProcessEnable\n");
            pAdvSoundSetParam = (PAUDIO_ST_ADVSOUND_SETPARAM)pArgs;
            bStatus = psAudioInstPri->fpAUDIOADVSOUNDSetParam(pAdvSoundSetParam->eParam, pAdvSoundSetParam->u16Value1, pAdvSoundSetParam->u16Value2);
            pAdvSoundSetParam->bRet = bStatus;
            break;
        case MApi_CMD_AUDIO_ADVSND_SetParam:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_ADVSND_SetParam\n");
            pAdvSndSetParam = (PAUDIO_ST_ADVSND_SETPARAM)pArgs;
            bStatus = psAudioInstPri->fpAUDIOADVSNDSetParam(pAdvSndSetParam->eParam, pAdvSndSetParam->u32Value1, pAdvSndSetParam->u16Value2);
            pAdvSndSetParam->bRet = bStatus;
            break;
#if 0
        case MApi_CMD_AUDIO_ADVSOUND_GetParam:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_ADVSOUND_GetParam\n");
            pu32Param = (MS_U32*)pArgs;
            u32Status = psAudioInstPri->fpAUDIOADVSOUNDGetParam((ADVSND_GET_PARAM)pu32Param[1]);
            *((MS_U32*)&(pu32Param[0])) = u32Status;
            break;
#endif
        case MApi_CMD_AUDIO_ADVSOUND_SET_ASF_PARAM:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_ADVSOUND_SET_ASF_PARAM\n");
            pAdvSndSetAsfParam = (PAUDIO_ST_ADVSND_SETASFPARAM)pArgs;
            bStatus = psAudioInstPri->fpAUDIOADVSOUNDSetASFParam(pAdvSndSetAsfParam->eParam, pAdvSndSetAsfParam->s16Value1, pAdvSndSetAsfParam->s16Value2);
            pAdvSndSetAsfParam->bRet = bStatus;
            break;
#if 0
        case MApi_CMD_AUDIO_ADVSOUND_GET_ASF_PARAM:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_ADVSOUND_GET_ASF_PARAM\n");
            pu32Param = (MS_U32*)pArgs;
            u32Status = psAudioInstPri->fpAUDIOADVSOUNDGetASFParam((ADVSND_GET_PARAM)pu32Param[1]);
            *((MS_U32*)&(pu32Param[0])) = u32Status;
            break;
#endif
        case MApi_CMD_AUDIO_ADVSOUND_GetInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_ADVSOUND_GetInfo\n");
            pAdvSoundGetInfo = (PAUDIO_ST_SRS_ADVSOUND_GETINFO)pArgs;
            u32Status = psAudioInstPri->fpAUDIOADVSOUNDGetInfo(pAdvSoundGetInfo->eType);
            pAdvSoundGetInfo->u32Ret = u32Status;
            break;
        case MApi_CMD_AUDIO_DBXTV_SetMode:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_DBXTV_SetMode\n");
            pDbxTvSetMode = (PAUDIO_ST_DBXTV_SETMODE)pArgs;
            psAudioInstPri->fpAUDIODBXTV_SetMode(pDbxTvSetMode->eTotSonMode, pDbxTvSetMode->eTotVolMode, pDbxTvSetMode->eTotSurMode, pDbxTvSetMode->u32Enable);
            break;
        case MApi_CMD_AUDIO_SeInit:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SeInit\n");
            psAudioInstPri->fpAUDIOSeInit();
            break;
        case MApi_CMD_AUDIO_SetAdvSndSys:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetAdvSndSys\n");
            pSetAdvSndSys = (PAUDIO_ST_SETADVSNDSYS)pArgs;
            psAudioInstPri->fpAUDIOSetAdvSndSys(pSetAdvSndSys->eType);
            break;
#if 0
        case MApi_CMD_AUDIO_ADV_SOUND_SET:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_ADV_SOUND_SET\n");
            pAdvSnd_Sound_Set_Param = (PAUDIO_ST_ADV_SOUND_SETPARAM)pArgs;
            psAudioInstPri->fpAUDIOAdvSndSetSoundParam((AdvSound_SET_Type)pAdvSnd_Sound_Set_Param->eType, (MS_U32*)pAdvSnd_Sound_Set_Param->pParam);
            break;
#else
        case MApi_CMD_AUDIO_SetVDS:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetVDS\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetVDS((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SetVSPK:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetVSPK\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetVSPK((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SetSRS:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetSRS\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetSRS((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SetBBE:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetBBE\n");
            pSetBBE = (PAUDIO_ST_SETBBE)pArgs;
            psAudioInstPri->fpAUDIOSetBBE(pSetBBE->u8EnBBE, pSetBBE->u8BBEMode);
            break;
        case MApi_CMD_AUDIO_VSPK_WMod:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_VSPK_WMod\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOVSPKWMod((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_VSPK_SMod:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_VSPK_SMod\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOVSPKSMod((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SRS_DC:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SRS_DC\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSRSDC((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SRS_TruBass:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SRS_TruBass\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSRSTruBass((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SRSTsxtPara:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SRSTsxtPara\n");
            pSrsSetTsxtPara = (PAUDIO_ST_SRS_SETTSXTPARA)pArgs;
            psAudioInstPri->fpAUDIOSRSSetTsxtPara(pSrsSetTsxtPara->u8Mode, pSrsSetTsxtPara->u8Value);
            break;
        case MApi_CMD_AUDIO_SetSRSTSHD:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetSRSTSHD\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetSRSTSHD((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_TSHD_Clarity:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_TSHD_Clarity\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOTSHDClarity((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_TSHD_TruBass:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_TSHD_TruBass\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOTSHDTruBass((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_TSHD_Definition:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_TSHD_Definition\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOTSHDDefinition((MS_U8)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_SRSSetTshdPara:
            //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SRSSetTshdPara\n");
            pSrsSetTshdPara = (PAUDIO_ST_SRS_SETTSHDPARA)pArgs;
            psAudioInstPri->fpAUDIOSRSSetTshdPara(pSrsSetTshdPara->u8Mode, pSrsSetTshdPara->u8Value);
            break;
#endif
        case MApi_CMD_AUDIO_Copy_Parameter:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_Copy_Parameter\n");
            pCopyParameter = (PAUDIO_ST_COPY_PARAMETER)pArgs;
            psAudioInstPri->fpAUDIOCopyParameter((ADVSND_PARAM)pCopyParameter->type, pCopyParameter->Parameter_ptr, pCopyParameter->size);
            break;
        //=============================================================
        // AUDIO_KTV RELATIONAL API FUNCTION
        //=============================================================
        case MApi_CMD_AUDIO_KTV_OP:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_KTV_OP\n");
            pKTV_OP = (PAUDIO_ST_KTV_OP)pArgs;
            if(pKTV_OP->eType == KTV_SET_INFO)
            {
                bStatus = psAudioInstPri->fpAUDIOSetKTVInfo(pKTV_OP->modelType, pKTV_OP->paramType, pKTV_OP->param1, pKTV_OP->param2);
                pKTV_OP->bRet = bStatus;
            }
            else if(pKTV_OP->eType == KTV_GET_INFO)
            {
                u32Status = psAudioInstPri->fpAUDIOGetKTVInfo(pKTV_OP->modelType, pKTV_OP->infoType);
                pKTV_OP->u32Ret = u32Status;
            }
            else if(pKTV_OP->eType == KTV_SET_MIX_VOL)
            {
                psAudioInstPri->fpAUDIOSetMixModeVolume(pKTV_OP->eSourceType, pKTV_OP->VolType, pKTV_OP->u8Vol1, pKTV_OP->u8Vol2);
            }
            else if (pKTV_OP->eType == KTV_SET_MIX_MUTE)
            {
                psAudioInstPri->fpAUDIOSetMixModeMute(pKTV_OP->eSourceType, pKTV_OP->VolType, pKTV_OP->EnMute);
            }
            break;
#if 0
        case MApi_CMD_AUDIO_SetKTVInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetKTVInfo\n");
            pu32Param = (MS_U32*)pArgs;
            *((MS_BOOL*)&(pu32Param[0])) = psAudioInstPri->fpAUDIOSetKTVInfo((AUDIO_KTV_MODEL)pu32Param[1], (AUDIO_KTV_PARAMTYPE)pu32Param[2], (MS_U32)pu32Param[3], (MS_U32)pu32Param[4]);
            break;
        case MApi_CMD_AUDIO_GetKTVInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetKTVInfo\n");
            pu32Param = (MS_U32*)pArgs;
            *((MS_U32*)&(pu32Param[0])) = psAudioInstPri->fpAUDIOGetKTVInfo((AUDIO_KTV_MODEL)pu32Param[1], (AUDIO_KTV_INFOTYPE)pu32Param[2]);
            break;
        case MApi_CMD_AUDIO_SetMixModeVolume:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetMixModeVolume\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetMixModeVolume((AUDIO_SOURCE_INFO_TYPE)pu32Param[0], (AUDIO_MIX_VOL_TYPE)pu32Param[1], (MS_U8)pu32Param[2], (MS_U8)pu32Param[3]);
            break;
        case MApi_CMD_AUDIO_SetMixModeMute:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetMixModeMute\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetMixModeMute((AUDIO_SOURCE_INFO_TYPE)pu32Param[0], (AUDIO_MIX_VOL_TYPE)pu32Param[1], (MS_BOOL)pu32Param[2]);
            break;
#endif
        case MApi_CMD_AUDIO_PlayMenuSound:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_PlayMenuSound\n");
            psAudioInstPri->fpAUDIOPlayMenuSound();
            break;

        //=============================================================
        // AUDIO_MM RELATIONAL API FUNCTION
        //=============================================================
        case MApi_CMD_AUDIO_SetCommAudioInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetCommAudioInfo\n");
            pSetCommAudioInfo = (PAUDIO_ST_SET_COMMAUDIOINFO)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSetCommAudioInfo(pSetCommAudioInfo->eInfoType, pSetCommAudioInfo->Param1, pSetCommAudioInfo->Param2);
            pSetCommAudioInfo->bRet = bStatus;
            break;
#if 0
        case MApi_CMD_AUDIO_SetDecodeParam:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetDecodeParam\n");
            pSetDecodeParam = (PAUDIO_ST_SETDECODEPARAM)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSetDecodeParam(pSetDecodeParam->DecID, pSetDecodeParam->paramType, pSetDecodeParam->Param);
            pSetDecodeParam->bStatus = bStatus;
            break;
#endif
        case MApi_CMD_AUDIO_SetMpegInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetMpegInfo\n");
            pSetMpegInfo = (PAUDIO_ST_SET_MPEGINFO)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSetMpegInfo(pSetMpegInfo->eInfoType, pSetMpegInfo->Param1, pSetMpegInfo->Param2);
            pSetMpegInfo->bRet = bStatus;
            break;
        case MApi_CMD_AUDIO_SetAC3Info:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetAC3Info\n");
            pSetAc3Info = (PAUDIO_ST_SET_AC3INFO)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSetAC3Info(pSetAc3Info->eInfoType, pSetAc3Info->Param1, pSetAc3Info->Param2);
            pSetAc3Info->bRet = bStatus;
            break;
        case MApi_CMD_AUDIO_SetAC3PInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetAC3PInfo\n");
            pSetAc3pInfo = (PAUDIO_ST_SET_AC3PINFO)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSetAC3PInfo(pSetAc3pInfo->eInfoType, pSetAc3pInfo->Param1, pSetAc3pInfo->Param2);
            pSetAc3pInfo->bRet = bStatus;
            break;
        case MApi_CMD_AUDIO_SetAACInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetAACInfo\n");
            pSetAacInfo = (PAUDIO_ST_SET_AACINFO)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSetAACInfo(pSetAacInfo->eInfoType, pSetAacInfo->Param1, pSetAacInfo->Param2);
            pSetAacInfo->bRet = bStatus;
            break;
        case MApi_CMD_AUDIO_SetWMAInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetWMAInfo\n");
            pSetWmaInfo = (PAUDIO_ST_SET_WMAINFO)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSetWMAInfo(pSetWmaInfo->eInfoType, pSetWmaInfo->Param1, pSetWmaInfo->Param2);
            pSetWmaInfo->bRet = bStatus;
            break;
        case MApi_CMD_AUDIO_SetDTSCommonCtrl:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetDTSCommonCtrl\n");
            pSetDtsCommonCtrl = (PAUDIO_ST_SET_DTSCOMMONCTRL)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSetDTSCommonCtrl(pSetDtsCommonCtrl->eInfoType, pSetDtsCommonCtrl->eCtrlType);
            pSetDtsCommonCtrl->bRet = bStatus;
            break;

        case MApi_CMD_AUDIO_GetCommAudioInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetCommAudioInfo\n");
            pGetCommAudioInfo = (PAUDIO_ST_GET_COMMAUDIOINFO)pArgs;
            ulong= psAudioInstPri->fpAUDIOGetCommAudioInfo(pGetCommAudioInfo->eInfoType);
            pGetCommAudioInfo->u64Ret = ulong;
            break;
        case MApi_CMD_AUDIO_GetMpegInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetMpegInfo\n");
			pGetMpegInfo = (PAUDIO_ST_GET_MPEGINFO)pArgs;
            u32Status= psAudioInstPri->fpAUDIOGetMpegInfo(pGetMpegInfo->eInfoType);
            pGetMpegInfo->u32Ret = u32Status;
            break;
        case MApi_CMD_AUDIO_GetAC3Info:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetAC3Info\n");
			pGetAc3Info = (PAUDIO_ST_GET_AC3INFO)pArgs;
            u32Status= psAudioInstPri->fpAUDIOGetAC3Info(pGetAc3Info->eInfoType);
            pGetAc3Info->u32Ret = u32Status;
            break;
        case MApi_CMD_AUDIO_GetAC3PInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetAC3PInfo\n");
			pGetAc3pInfo = (PAUDIO_ST_GET_AC3PINFO)pArgs;
            u32Status= psAudioInstPri->fpAUDIOGetAC3PInfo(pGetAc3pInfo->eInfoType);
            pGetAc3pInfo->u32Ret = u32Status;	//todo stanley
            break;
        case MApi_CMD_AUDIO_GetAACInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetAACInfo\n");
			pGetAacInfo = (PAUDIO_ST_GET_AACINFO)pArgs;
            u32Status= psAudioInstPri->fpAUDIOGetAACInfo(pGetAacInfo->eInfoType);
            pGetAacInfo->u32Ret = u32Status;
            break;
        case MApi_CMD_AUDIO_GetWmaInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetWmaInfo\n");
            pGetWmaInfo = (PAUDIO_ST_GET_WMAINFO)pArgs;
            u32Status= psAudioInstPri->fpAUDIOGetWmaInfo(pGetWmaInfo->eInfoType);
            pGetWmaInfo->u32Ret = u32Status;	//todo stanley
            break;
        case MApi_CMD_AUDIO_GetDTSInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetDTSInfo\n");
            pGetDtsInfo = (PAUDIO_ST_GET_DTSINFO)pArgs;
            u32Status= psAudioInstPri->fpAUIDOGetDTSInfo(pGetDtsInfo->eInfoType);
            pGetDtsInfo->u32Ret = u32Status;	//todo stanley
            break;

        case MApi_CMD_AUDIO_XPCM_Param:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_XPCM_Param\n");
			pXpcmParam = (PAUDIO_ST_XPCMPARAM)pArgs;
            u8Status = psAudioInstPri->fpAUDIOXPCMParam(pXpcmParam->eAudioType, pXpcmParam->u8Channels, pXpcmParam->u16SampleRate, pXpcmParam->u8BitsPerSample, pXpcmParam->u16BlockSize, pXpcmParam->u16SamplePerBlock);
            pXpcmParam->u8Ret = u8Status;	//todo stanley
            break;

        case MApi_CMD_AUDIO_XPCM2_Param:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_XPCM2_Param\n");
            pSetXpcmParam = (PAUDIO_ST_SET_XPCMPARAM)pArgs;
            psAudioInstPri->fpAUDIOXPCM2Param(pSetXpcmParam->eAudioType,pSetXpcmParam->u8Channels,pSetXpcmParam->u16SampleRate,pSetXpcmParam->u8BitsPerSample,pSetXpcmParam->u16BlockSize,pSetXpcmParam->u16SamplePerBlock);
            break;

        case MApi_CMD_AUDIO_XPCM2_CheckIntStatus:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_XPCM2_CheckIntStatus\n");
            pu32Param = (MS_U32*)pArgs;
            u16Status = psAudioInstPri->fpAUDIOXPCM2CheckIntStatus();
            *((MS_U16*)&(pu32Param[1])) = u16Status;
            break;
        case MApi_CMD_AUDIO_RA8_Param:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_RA8_Param\n");
            pRA8Param = (PAUDIO_ST_RA8_Param)pArgs;
            u8Status = psAudioInstPri->fpAUDIORA8Param((MS_U16)pRA8Param->mNumCodecs, (MS_U16)pRA8Param->mSamples, (MS_U16)pRA8Param->mSampleRate,
                        (MS_U16*)pRA8Param->pChannels, (MS_U16*)pRA8Param->pRegions, (MS_U16*)pRA8Param->pcplStart, (MS_U16*)pRA8Param->pcplQbits, (MS_U16*)pRA8Param->pFrameSize);
            pRA8Param->u8Status = u8Status;
            break;
        case MApi_CMD_AUDIO_Init:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_Init\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOInit((En_DVB_decSystemType)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_StartDecode:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_StartDecode\n");
            psAudioInstPri->fpAUDIOStartDecode();
            break;
        case MApi_CMD_AUDIO_StartBroswe:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_StartBroswe\n");
            psAudioInstPri->fpAUDIOStartBrower();
            break;
        case MApi_CMD_AUDIO_StopDecode:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_StopDecode\n");
            psAudioInstPri->fpAUDIOStopDecode();
            break;
        case MApi_CMD_AUDIO_PauseDecode:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_PauseDecode\n");
            psAudioInstPri->fpAUDIOPauseDecode();
            break;
        case MApi_CMD_AUDIO_CheckPlayDone:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_CheckPlayDone\n");
            pu32Param = (MS_U32*)pArgs;
            u8Status = psAudioInstPri->fpAUDIOCheckPlayDone();
            *((MS_U8*)&(pu32Param[1])) = u8Status;
            break;
        case MApi_CMD_AUDIO_GetResidualBufferSize:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - IOCTL_AUDIO_GETRESIDUALBUFFERSIZE\n");
            pu32Param = (MS_U32*)pArgs;
            u16Status = psAudioInstPri->fpAUDIOGetResidualBufferSize();
            *((MS_U16*)&(pu32Param[1])) = u16Status;
            break;
        case MApi_CMD_AUDIO_GetPCMBufferSize:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetPCMBufferSize\n");
            pGetPcmBufferSize = (PAUDIO_ST_GET_PCMBUFFERSIZE)pArgs;
            u16Status = psAudioInstPri->fpAUDIOGetPCMBufferSize(pGetPcmBufferSize->u16BitRate);
            pGetPcmBufferSize->u16Ret = u16Status;
            break;
        case MApi_CMD_AUDIO_GetPCMBufferSize2:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetPCMBufferSize2\n");
            pGetPcmBufferSize2 = (PAUDIO_ST_GET_PCMBUFFERSIZE2)pArgs;
            u16Status = psAudioInstPri->fpAUDIOGetPCMBufferSize2(pGetPcmBufferSize2->u16BitRate, pGetPcmBufferSize2->u16SampleRate);
            pGetPcmBufferSize2->u16Ret = u16Status;	//todo stanley
            break;
        case MApi_CMD_AUDIO_GetCurrentFrameNumber:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetCurrentFrameNumber\n");
            pu32Param = (MS_U32*)pArgs;
            u32Status = psAudioInstPri->fpAUDIOGetCurrentFrameNumber();
            *((MS_U32*)&(pu32Param[1])) = u32Status;
            break;
        case MApi_CMD_AUDIO_GetSampleRates:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetSampleRates\n");
            pu32Param = (MS_U32*)pArgs;
            u16Status = psAudioInstPri->fpAUDIOGetSampleRate();
            *((MS_U16*)&(pu32Param[1])) = u16Status;
            break;
        case MApi_CMD_AUDIO_GetBitRates:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetBitRates\n");
            pu32Param = (MS_U32*)pArgs;
            u32Status = psAudioInstPri->fpAUDIOGetBitRate();
            *((MS_U32*)&(pu32Param[1])) = u32Status;
            break;
        case MApi_CMD_AUDIO_GetLayer:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetLayer\n");
            pu32Param = (MS_U32*)pArgs;
            eMP3_Layer = psAudioInstPri->fpAUDIOGetLayer();
            *((Audio_MP3_LAYER*)&(pu32Param[1])) = eMP3_Layer;
            break;

        case MApi_CMD_AUDIO_CheckInputRequest:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_CheckInputRequest\n");
            pCheckInputRequest = (PAUDIO_ST_CHECK_INPUT_REQ)pArgs;
            u8Status = psAudioInstPri->fpAUDIOCheckInputRequest((MS_PHY*)pCheckInputRequest->pPHYWrtAddr, (MS_U32*)pCheckInputRequest->pU32WrtBytes);
            pCheckInputRequest->u8Status = u8Status;
            break;

        case MApi_CMD_AUDIO_SetInput:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetInput\n");
            psAudioInstPri->fpAUDIOSetInput();
            break;
        case MApi_CMD_AUDIO_SetSampleRateIndex:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetSampleRateIndex\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOSetSampleRateIndex((MS_U16)pu32Param[0]);
            break;

        case MApi_CMD_AUDIO_SetXPCMParam:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetXPCMParam\n");
            pSetXpcmParam = (PAUDIO_ST_SET_XPCMPARAM)pArgs;
            psAudioInstPri->fpAUDIOSetXPCMParam(pSetXpcmParam->eAudioType,pSetXpcmParam->u8Channels,pSetXpcmParam->u16SampleRate,pSetXpcmParam->u8BitsPerSample,pSetXpcmParam->u16BlockSize,pSetXpcmParam->u16SamplePerBlock);
            break;

        case MApi_CMD_AUDIO_FileEndNotification:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_FileEndNotification\n");
            psAudioInstPri->fpAUDIOFileEndNotification();
            break;
        case MApi_CMD_AUDIO_FileEndDataHandle:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_FileEndDataHandle\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOFileEndDataHandle((MS_U32)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_GetPlayTick:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetPlayTick\n");
            pu32Param = (MS_U32*)pArgs;
            u32Status = psAudioInstPri->fpAUDIOGetPlayTick();
            *((MS_U32*)&(pu32Param[1])) = u32Status;
            break;
        case MApi_CMD_AUDIO_GetEsMEMCnt:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetEsMEMCnt\n");
            pu32Param = (MS_U32*)pArgs;
            u16Status= psAudioInstPri->fpAUDIOGetEsMEMCnt();
            *((MS_U16*)&(pu32Param[1])) = u16Status;
            break;
        case MApi_CMD_AUDIO_SetASFParm:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetASFParm\n");
            pSetAsfParm = (PAUDIO_ST_SET_ASFPARM)pArgs;
            psAudioInstPri->fpAUDIOSetASFParm(pSetAsfParm->eParmType, pSetAsfParm->u32Value);
            break;
        case MApi_CMD_AUDIO_MM_SetInput:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_MM_SetInput\n");
            pMmSetInput = (PAUDIO_ST_MM_SETINPUT)pArgs;
            psAudioInstPri->fpAUDIOMMSetInput(pMmSetInput->eAudDecoderNo, pMmSetInput->u8IntTag);
            break;
        case MApi_CMD_AUDIO_MM_CheckPlayDone:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_MM_CheckPlayDone\n");
			pMmCheckPlayDone = (PAUDIO_ST_MM_CHECKPLAYDONE)pArgs;
            u32Status = psAudioInstPri->fpAUDIOMMCheckPlayDone(pMmCheckPlayDone->eAudDecoderNo);
            pMmCheckPlayDone->u32Ret = u32Status;
            break;
        case MApi_CMD_AUDIO_MM_CheckInputRequest:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_MM_CheckInputRequest\n");
            pMMCheckInputRequest = (PAUDIO_ST_MM_CHECK_INPUT_REQ)pArgs;
            u8Status = psAudioInstPri->fpAUDIOMMCheckInputRequest((En_DVB_AudioDecoder)pMMCheckInputRequest->AUDDecoderNo, (MS_PHY*)pMMCheckInputRequest->pPHYWrtAddr, (MS_U32*)pMMCheckInputRequest->pU32WrtBytes);
            pMMCheckInputRequest->u8Status = u8Status;
            break;
        case MApi_CMD_AUDIO_DmaReader_Init:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_DmaReader_Init\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIODmaReaderInit((SAMPLE_RATE)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_DmaReader_AllInput_Init:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_DmaReader_AllInput_Init\n");
            psAudioInstPri->fpAUDIODmaReaderAllInputInit();
            break;
        case MApi_CMD_AUDIO_DmaReader_WritePCM:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_DmaReader_WritePCM\n");
            pDmaReaderWritePCM = (PAUDIO_ST_DMAREADER_WRITEPCM)pArgs;
            bStatus = psAudioInstPri->fpAUDIODmaReaderWritePCM((void*)pDmaReaderWritePCM->pBuffer, (MS_U32)pDmaReaderWritePCM->bytes);
            pDmaReaderWritePCM->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_DmaWrite_Init:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_DmaWrite_Init\n");
            psAudioInstPri->fpAUDIODmaWriterInit();
            break;
        case MApi_CMD_AUDIO_USBPCM_Enable:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_USBPCM_Enable\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOUSBPCMEnable((MS_BOOL)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_USBPCM_SetFlag:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_USBPCM_SetFlag\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOUSBPCMSetFlag((MS_BOOL)pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_USBPCM_GetFlag:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_USBPCM_GetFlag\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOUSBPCMGetFlag();
            *((MS_BOOL*)&(pu32Param[1])) = bStatus;
            break;
        case MApi_CMD_AUDIO_USBPCM_GetMemInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_USBPCM_GetMemInfo\n");
            psAudioInstPri->fpAUDIOUSBPCMGetMemInfo((AUDIO_UPLOAD_INFO*)pArgs);
            break;
        case MApi_CMD_AUDIO_PCMCapture_Init:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_PCMCapture_Init\n");
            pPCMCapture_Init = (PAUDIO_ST_PCMCAPTURE_INIT)pArgs;
            bStatus = psAudioInstPri->fpAUDIOPCMCaptureInit((const AUDIO_DEVICE_TYPE)pPCMCapture_Init->eID, (const AUDIO_CAPTURE_SOURCE_TYPE)pPCMCapture_Init->eSource);
            pPCMCapture_Init->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_PCMCapture_Start:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_PCMCapture_Start\n");
            pPCMCapture_Init = (PAUDIO_ST_PCMCAPTURE_INIT)pArgs;
            bStatus = psAudioInstPri->fpAUDIOPCMCaptureStart((const AUDIO_DEVICE_TYPE)pPCMCapture_Init->eID);
            pPCMCapture_Init->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_PCMCapture_Stop:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_PCMCapture_Stop\n");
            pPCMCapture_Init = (PAUDIO_ST_PCMCAPTURE_INIT)pArgs;
            bStatus = psAudioInstPri->fpAUDIOPCMCaptureStop((const AUDIO_DEVICE_TYPE)pPCMCapture_Init->eID);
            pPCMCapture_Init->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_PCMCapture_Read:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_PCMCapture_Stop\n");
            pPCMCapture_Read = (PAUDIO_ST_PCMCAPTURE_READ)pArgs;
            bStatus = psAudioInstPri->fpAUDIOPCMCaptureRead((const AUDIO_DEVICE_TYPE)pPCMCapture_Read->eID, (void *)pPCMCapture_Read->pBuffer, (const MS_U32)pPCMCapture_Read->bytes);
            pPCMCapture_Read->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_VoIP_Config:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_VoIP_Config\n");
            pVoipConfig = (PAUDIO_ST_VOIP_CONFIG)pArgs;
            bStatus = psAudioInstPri->fpAUDIOVoIPConfig(pVoipConfig->eConfigType, pVoipConfig->u32Param1, pVoipConfig->u32Param2);
            pVoipConfig->bRet = bStatus;
            break;
        case MApi_CMD_AUDIO_ALSA_Check:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_ALSA_Check\n");
            pu32Param = (MS_U32*)pArgs;
            bStatus = psAudioInstPri->fpAUDIOALSACheck();
            *((MS_BOOL*)&(pu32Param[1])) = bStatus;
            break;
        case MApi_CMD_AUDIO_ALSA_Enable:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_ALSA_Enable\n");
            pAlsaEnable = (PAUDIO_ST_ALSA_ENABLE)pArgs;
            bStatus = psAudioInstPri->fpAUDIOALSAEnable(pAlsaEnable->bEnable);
            pAlsaEnable->bRet = bStatus;	//todo stanley
            break;
        case MApi_CMD_AUDIO_UNI_CheckDecodeDone:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_UNI_CheckDecodeDone\n");
            pUNICheck_DecodeDone = (PAUDIO_ST_UNI_CHECK_DECODEDONE)pArgs;
            bStatus = psAudioInstPri->fpAUDIOUNICheckDecodeDone((MS_PHY*)pUNICheck_DecodeDone->pPHYWrtAddr, (MS_U32*)pUNICheck_DecodeDone->pU32WrtBytes);
            pUNICheck_DecodeDone->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_UNI_SetOutput:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_UNI_SetOutput\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOUNISetOutput(pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_UNI_Set_PCMInputWriteAddr:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_UNI_Set_PCMInputWriteAddr\n");
            pu32Param = (MS_U32*)pArgs;
            psAudioInstPri->fpAUDIOUNISetPCMInputWriteAddr(pu32Param[0]);
            break;
        case MApi_CMD_AUDIO_UNI_Get_OutPCMLevel:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_UNI_Get_OutPCMLevel\n");
            pu32Param = (MS_U32*)pArgs;
            u32Status= psAudioInstPri->fpAUDIOUNIGetOutPCMLevel();
            *((MS_U32*)&(pu32Param[1])) = u32Status;
            break;
        case MApi_CMD_AUDIO_RingTask:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_RingTask\n");
            psAudioInstPri->fpAUDIORingTask((audioRingTask*)pArgs);
            break;
        case MApi_CMD_AUDIO_Ring_DataTransfer:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_Ring_DataTransfer\n");
            pRing_Data_Trans = (PAUDIO_ST_RING_DATA_TRANS)pArgs;
            psAudioInstPri->fpAUDIORingDataTransfer((audioRingTask*)pRing_Data_Trans->pauRingTask, (audioRingTask*)pRing_Data_Trans->pauRingTask_1);
            break;

        case MApi_CMD_AUDIO_MM2_InitAesInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_MM2_InitAesInfo\n");
            pMm2InitAesInfo = (PAUDIO_ST_MM2_INITAESINFO)pArgs;
            bStatus = psAudioInstPri->fpAUDIOMM2InitAesInfo(pMm2InitAesInfo->eDecId);
            pMm2InitAesInfo->bRet = bStatus;
            break;

        case MApi_CMD_AUDIO_MM2_CheckAesInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_MM2_CheckAesInfo\n");
            pMM2_CheckAesInfo = (PAUDIO_ST_MM2_CHECKAESINFO)pArgs;
            bStatus = psAudioInstPri->fpAUDIOMM2CheckAesInfo((AUDIO_DEC_ID)pMM2_CheckAesInfo->dec_id, (AES_INFO*)pMM2_CheckAesInfo->paes_info);
            break;
        case MApi_CMD_AUDIO_MM2_InputAesFinished:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_MM2_InputAesFinished\n");
            pMM2_InputAes_Finish = (PAUDIO_ST_MM2_INPUTAES_FINISH)pArgs;
            bStatus = psAudioInstPri->fpAUDIOMM2InputAesFinished((AUDIO_DEC_ID)pMM2_InputAes_Finish->dec_id, (MS_U32)pMM2_InputAes_Finish->es_size,
                                                                    (MS_BOOL)pMM2_InputAes_Finish->ptsExist, (MS_U64)pMM2_InputAes_Finish->pts);
            pMM2_InputAes_Finish->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_SetAudioParam2:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_SetAudioParam2\n");
            pSetAudioParam2 = (PAUDIO_ST_SET_AUDIOPARAM2)pArgs;
            bStatus = psAudioInstPri->fpAUDIOSetAudioParam2(pSetAudioParam2->eDecId, pSetAudioParam2->eParamType,pSetAudioParam2->Param);
            pSetAudioParam2->bRet = bStatus;
            break;
        case MApi_CMD_AUDIO_GetAudioInfo2:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetAudioInfo2\n");
            pGetAudioInfo2 = (PAUDIO_ST_GETAUDIOINFO2)pArgs;
            bStatus = psAudioInstPri->fpAUDIOGetAudioInfo2((AUDIO_DEC_ID)pGetAudioInfo2->DecId, (Audio_InfoType)pGetAudioInfo2->infoType, (void*)pGetAudioInfo2->pInfo);
            pGetAudioInfo2->bStatus = bStatus;
            break;
        case MApi_CMD_AUDIO_GetDDRInfo:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetDDRInfo\n");
            pGetDdrInfo = (PAUDIO_ST_GET_DDRINFO)pArgs;
            u32Status = psAudioInstPri->fpAUDIOGetDDRInfo(pGetDdrInfo->eDecId, pGetDdrInfo->eDdrInfo);
            pGetDdrInfo->Ret = u32Status;
            break;

        case MApi_CMD_AUDIO_GetCaps:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "AUDIOIoctl - MApi_CMD_AUDIO_GetDDRInfo\n");
            pGetCaps = (PAUDIO_ST_GETCAPS)pArgs;
            u32Status = psAudioInstPri->fpAUDIOGetCaps((AUDIO_DEC_ID)pGetCaps->eDecId, (MS_U32*)pGetCaps->pu32Caps);
            pGetCaps->u32Ret = u32Status;
            break;
            // To   Do

        default:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "AUDIOIoctl - Unknown commend!!!\n");
            return UTOPIA_STATUS_FAIL;

    }

IOCTRL_END:

    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[Audio][mapiAudio.c] cmd %d  Release\n", (unsigned int)u32Cmd);

    UtopiaResourceRelease(pResource);

    if(tRes == AUDIO_STATUS_OK)
        return UTOPIA_STATUS_SUCCESS;
    else
        return UTOPIA_STATUS_FAIL;

}

MS_U32 AUDIOClose(void* pInstance)
{

    UtopiaInstanceDelete(pInstance);

    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 AUDIOStr(MS_U32 u32PowerState, void* pModule)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    MS_BOOL bStatus = FALSE;

    //UtopiaModuleGetSTRPrivate(pModule, (void**));

    switch (u32PowerState)
    {
        case E_POWER_SUSPEND:
        {
            /* Please Implement Module Suspend Flow Here. */
            //printk("\033[32m [Utopia2K] %s : %d suspending \033[m\n", __FUNCTION__,__LINE__);

            bStatus = _MApi_AUDIO_Suspend();
            //bStatus = psAudioInstPri->fpAUDIOSuspend();

            u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;

            break;
        }

        case E_POWER_RESUME:
        {
            /* Please Implement Module Resume Flow Here. */
            //printk("\033[32m [Utopia2K] %s : %d resume \033[m\n", __FUNCTION__,__LINE__);

            bStatus = _MApi_AUDIO_Resume();
            //bStatus = psAudioInstPri->fpAUDIOResume();

            u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;

            break;
        }

        default:
        {
            u32Return = UTOPIA_STATUS_FAIL;

            break;
        }
    }

    return u32Return;// for success
}

