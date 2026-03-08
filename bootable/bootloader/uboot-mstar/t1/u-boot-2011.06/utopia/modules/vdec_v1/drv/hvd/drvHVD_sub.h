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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvHVD_sub.h
/// @brief  SubHVD Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_HVD_SUB_H_
#define _DRV_HVD_SUB_H_

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void MDrv_HVD_Sub_PowerCtrl( MS_BOOL bEnable );
void MDrv_HVD_Sub_SetOSRegBase( MS_U32 u32RegBaseAddr );
HVD_Result MDrv_HVD_Sub_Init(HVD_MemCfg *pStMemCfg, HVD_Init_Settings *pStInitSettings);
HVD_Result MDrv_HVD_Sub_Rst(MS_BOOL bErrHandle);
HVD_Result MDrv_HVD_Sub_Play(void);
HVD_Result MDrv_HVD_Sub_Exit(void);
HVD_Result MDrv_HVD_Sub_Pause(void);
HVD_Result MDrv_HVD_Sub_Flush( MS_BOOL bShowLast);
HVD_Result MDrv_HVD_Sub_StepDisp( void);
HVD_Result MDrv_HVD_Sub_StepDecode(void);
HVD_Result MDrv_HVD_Sub_PushQueue(HVD_Packet_Info* pInfo);
HVD_Result MDrv_HVD_Sub_PushQueue_Fire(void);
HVD_Result MDrv_HVD_Sub_DecodeIFrame(MS_PHYADDR u32SrcSt , MS_U32 u32SrcSize);

HVD_Result MDrv_HVD_Sub_SetDataEnd( MS_BOOL bEnd );
HVD_Result MDrv_HVD_Sub_SetDispErrFrm(MS_BOOL bEnable);
HVD_Result MDrv_HVD_Sub_SetDispRepeatField(MS_BOOL bEnable);
HVD_Result MDrv_HVD_Sub_SetSkipDecMode(HVD_Skip_Decode eDecType);
HVD_Result MDrv_HVD_Sub_SetDispSpeed(HVD_Drv_Disp_Speed eSpeed);
HVD_Result MDrv_HVD_Sub_SetSyncActive( MS_BOOL bEnable);
HVD_Result MDrv_HVD_Sub_SetDropMode( HVD_Drop_Disp eMode , MS_U32 u32Arg);
HVD_Result MDrv_HVD_Sub_RstPTS(MS_U32 u32PTS);
HVD_Result MDrv_HVD_Sub_SetFrcMode(HVD_FrmRateConv_Mode eMode );
HVD_Result MDrv_HVD_Sub_SetSyncTolerance(MS_U32 u32Arg );
HVD_Result MDrv_HVD_Sub_SetSyncVideoDelay( MS_U32 u32Arg);
HVD_Result MDrv_HVD_Sub_SetSyncFreeRunTH(MS_U32 u32Arg );
HVD_Result MDrv_HVD_Sub_SetSyncRepeatTH( MS_U32 u32Arg);
HVD_Result MDrv_HVD_Sub_SetErrConceal(MS_BOOL u32Arg );
void MDrv_HVD_Sub_SetDbgLevel( HVD_Uart_Level elevel );
HVD_Result MDrv_HVD_Sub_SeekToPTS(MS_U32 u32PTS );
HVD_Result MDrv_HVD_Sub_SkipToPTS(MS_U32 u32PTS );
HVD_Result MDrv_HVD_Sub_SetFreezeImg(MS_BOOL bEnable );
HVD_Result MDrv_HVD_Sub_SetBlueScreen(MS_BOOL bEnable );
HVD_Result MDrv_HVD_Sub_SetDispOneField(MS_BOOL bEnable );
HVD_Result MDrv_HVD_Sub_SetISREvent(MS_U32 u32Event, HVD_InterruptCb fnISRHandler );
MS_BOOL MDrv_HVD_Sub_SetEnableISR(MS_BOOL bEnable);
MS_BOOL MDrv_HVD_Sub_SetForceISR(MS_BOOL bEnable);
MS_BOOL MDrv_HVD_Sub_SetMVOPDone(void);
HVD_Result MDrv_HVD_Sub_SetVirtualBox(MS_U16 u16Width, MS_U16 u16Height);
HVD_Result MDrv_HVD_Sub_SetDynScalingParam(  void *pStAddr , MS_U32 u32Size );
HVD_Result MDrv_HVD_Sub_SetAutoRmLstZeroByte(MS_BOOL bOn);
HVD_Result MDrv_HVD_Sub_SetDispInfoTH(HVD_Disp_Info_Threshold* DispInfoTH );

MS_BOOL MDrv_HVD_Sub_IsISROccured(void);
MS_BOOL MDrv_HVD_Sub_IsDispFinish(void);
MS_BOOL MDrv_HVD_Sub_IsFrameShowed(void);
MS_BOOL MDrv_HVD_Sub_IsStepDecodeDone(void);
HVD_Result MDrv_HVD_Sub_CheckDispInfoRdy(void);
MS_BOOL MDrv_HVD_Sub_IsDispInfoChg(void);
MS_BOOL MDrv_HVD_Sub_IsIdle(void);
MS_BOOL MDrv_HVD_Sub_IsSyncStart(void);
MS_BOOL MDrv_HVD_Sub_IsSyncReach(void);
MS_BOOL MDrv_HVD_Sub_IsLowDelay(void);
MS_BOOL MDrv_HVD_Sub_IsIFrmFound(void);
MS_BOOL MDrv_HVD_Sub_Is1stFrmRdy(void);
MS_BOOL MDrv_HVD_Sub_IsAllBufferEmpty(void);
HVD_Result MDrv_HVD_Sub_IsAlive(void);

MS_U32 MDrv_HVD_Sub_GetBBUVacancy(void);
HVD_Result MDrv_HVD_Sub_GetDispInfo( HVD_Disp_Info *pinfo );
MS_U32 MDrv_HVD_Sub_GetPTS(void);
MS_U32 MDrv_HVD_Sub_GetNextPTS(void);
MS_U32 MDrv_HVD_Sub_GetDataErrCnt(void);
MS_U32 MDrv_HVD_Sub_GetDecErrCnt(void);
MS_U32 MDrv_HVD_Sub_GetESWritePtr(void);
MS_U32 MDrv_HVD_Sub_GetESReadPtr(void);
MS_BOOL MDrv_HVD_Sub_GetCaps( HVD_Codec u32Type);
MS_U32 MDrv_HVD_Sub_GetErrCode(void);
MS_U32 MDrv_HVD_Sub_GetPlayMode(HVD_Get_Mode_Status eMode);
HVD_Get_Play_State MDrv_HVD_Sub_GetPlayState(void);
MS_U32 MDrv_HVD_Sub_GetDecodeCnt(void);
MS_U8 MDrv_HVD_Sub_GetActiveFormat(void);
const HVD_DrvInfo* MDrv_HVD_Sub_GetInfo( void );
MS_BOOL MDrv_HVD_Sub_GetStatus( HVD_DrvStatus *pstatus);
HVD_Result MDrv_HVD_Sub_GetFrmInfo( HVD_Get_Frm_Info_Type eType  ,  HVD_Frame_Info *pInfo);
MS_BOOL MDrv_HVD_Sub_GetISRInfo( MS_U32* eType );
MS_U32 MDrv_HVD_Sub_CalLumaSum(  HVD_Get_Frm_Info_Type eType  );
MS_U32 MDrv_HVD_Sub_GetUserData_Wptr( void );
MS_U32 MDrv_HVD_Sub_GetUserData_Packet( MS_U32 u32Idx , MS_U32* u32Size );
HVD_Result MDrv_HVD_Sub_GenPattern( HVD_PatternType eType ,  MS_U32 u32VAddr  ,  MS_U32* u32Size  );
MS_U32 MDrv_HVD_Sub_GetPatternInfo(   HVD_Pattern_Info eType );
MS_U32 MDrv_HVD_Sub_GetDynamicScalingInfo(   HVD_DynamicScaling_Info eType );

MS_U32 MDrv_HVD_Sub_GetData(HVD_GData_Type eType);
MS_U32 MDrv_HVD_Sub_GetMem_Dbg(MS_U32 u32Addr);
void MDrv_HVD_Sub_DbgDumpStatus(HVD_DumpStatus eFlag);
void MDrv_HVD_Sub_SetMem_Dbg(MS_U32 u32Addr , MS_U32 u32Arg);
HVD_Result MDrv_HVD_Sub_SetCmd_Dbg( MS_U32 u32Cmd , MS_U32 u32Arg);
HVD_Result MDrv_HVD_Sub_SetSettings_Pro( HVD_SSettings_Type eType , MS_U32 u32Arg);
HVD_Result MDrv_HVD_Sub_SetBalanceBW(MS_U8 qp_cnt, MS_U8 db_cnt, MS_U8 upper);

#ifdef __cplusplus
}
#endif

#endif // _DRV_HVD_SUB_H_

