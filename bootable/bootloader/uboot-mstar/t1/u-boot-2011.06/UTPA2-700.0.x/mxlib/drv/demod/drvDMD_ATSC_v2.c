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
//    The place of arbitra«–cordance
//
//******************************************************************************
////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvDMD_ATSC_v2.c
/// @brief  DMD ATSC Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#include <stdio.h>
#endif

#include "utopia.h"
#include "utopia_dapi.h"

#include "drvDMD_ATSC_v2.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#ifdef MS_DEBUG
#define DMD_DBG(x)          (x)
#else
#define DMD_DBG(x)          //(x)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

typedef enum _ATSC_POOL_ID
{
    ATSC_POOL_ID_DMD0 = 0
} ATSC_POOL_ID;

typedef struct _ATSC_RESOURCE_PRIVATE
{
    DMD_ATSC_ResData sDMD_ATSC_ResData[DMD_ATSC_MAX_DEMOD_NUM];
} ATSC_RESOURCE_PRIVATE, *PATSC_RESOURCE_PRIVATE;

typedef MS_BOOL                   (*IOCTL_ATSC_SetDbgLevel)(DMD_ATSC_DbgLv);
typedef DMD_ATSC_Info*            (*IOCTL_ATSC_GetInfo)(void);
typedef MS_BOOL                   (*IOCTL_ATSC_GetLibVer)(const MSIF_Version**);
typedef MS_BOOL                   (*IOCTL_ATSC_Init)(MS_U8, DMD_ATSC_InitData*, MS_U32);
typedef MS_BOOL                   (*IOCTL_ATSC_Exit)(MS_U8);
typedef MS_BOOL                   (*IOCTL_ATSC_SetConfig)(MS_U8, DMD_ATSC_DEMOD_TYPE, MS_BOOL);
typedef MS_BOOL                   (*IOCTL_ATSC_SetReset)(MS_U8);
typedef MS_BOOL                   (*IOCTL_ATSC_Set_QAM_SR)(MS_U8, DMD_ATSC_DEMOD_TYPE, MS_U16);
typedef MS_BOOL                   (*IOCTL_ATSC_SetActive)(MS_U8, MS_BOOL);
typedef MS_U32                    (*IOCTL_ATSC_SetPowerState)(MS_U8, EN_POWER_MODE);
typedef DMD_ATSC_LOCK_STATUS      (*IOCTL_ATSC_GetLock)(MS_U8, DMD_ATSC_GETLOCK_TYPE);
typedef DMD_ATSC_DEMOD_TYPE       (*IOCTL_ATSC_GetModulationMode)(MS_U8);
typedef MS_BOOL                   (*IOCTL_ATSC_GetSignalStrength)(MS_U8, MS_U16*);
typedef DMD_ATSC_SIGNAL_CONDITION (*IOCTL_ATSC_GetSignalQuality)(MS_U8);
typedef MS_U8                     (*IOCTL_ATSC_GetSNRPercentage)(MS_U8);
typedef MS_BOOL                   (*IOCTL_ATSC_GET_QAM_SNR)(MS_U8, DMD_ATSC_SNR_DATA*);
typedef MS_BOOL                   (*IOCTL_ATSC_Read_uCPKT_ERR)(MS_U8, MS_U16*);
typedef MS_BOOL                   (*IOCTL_ATSC_GetPreViterbiBer)(MS_U8, DMD_ATSC_BER_DATA*);
typedef MS_BOOL                   (*IOCTL_ATSC_GetPostViterbiBer)(MS_U8, DMD_ATSC_BER_DATA*);
typedef MS_BOOL                   (*IOCTL_ATSC_ReadFrequencyOffset)(MS_U8, DMD_ATSC_CFO_DATA*);
typedef MS_BOOL                   (*IOCTL_ATSC_SetSerialControl)(MS_U8, MS_U8);
typedef MS_BOOL                   (*IOCTL_ATSC_IIC_BYPASS_MODE)(MS_U8, MS_BOOL);
typedef MS_BOOL                   (*IOCTL_ATSC_SWITCH_SSPI_GPIO)(MS_U8, MS_BOOL);
typedef MS_BOOL                   (*IOCTL_ATSC_GPIO_GET_LEVEL)(MS_U8, MS_U8, MS_BOOL*);
typedef MS_BOOL                   (*IOCTL_ATSC_GPIO_SET_LEVEL)(MS_U8, MS_U8, MS_BOOL);
typedef MS_BOOL                   (*IOCTL_ATSC_GPIO_OUT_ENABLE)(MS_U8, MS_U8, MS_BOOL);
typedef MS_BOOL                   (*IOCTL_ATSC_DO_IQ_SWAP)(MS_U8, MS_BOOL);
typedef MS_BOOL                   (*IOCTL_ATSC_GetReg)(MS_U8, MS_U16, MS_U8*);
typedef MS_BOOL                   (*IOCTL_ATSC_SetReg)(MS_U8, MS_U16, MS_U8);

typedef struct _ATSC_INSTANT_PRIVATE
{
    IOCTL_ATSC_SetDbgLevel              fpSetDbgLevel;
    IOCTL_ATSC_GetInfo                  fpGetInfo;
    IOCTL_ATSC_GetLibVer                fpGetLibVer;
    IOCTL_ATSC_Init                     fpInit;
    IOCTL_ATSC_Exit                     fpExit;
    IOCTL_ATSC_SetConfig                fpSetConfig;
    IOCTL_ATSC_SetReset                 fpSetReset;
    IOCTL_ATSC_Set_QAM_SR               fpSet_QAM_SR;
    IOCTL_ATSC_SetActive                fpSetActive;
    #if DMD_ATSC_STR_EN
    IOCTL_ATSC_SetPowerState            fpSetPowerState;
    #endif
    IOCTL_ATSC_GetLock                  fpGetLock;
    IOCTL_ATSC_GetModulationMode        fpGetModulationMode;
    IOCTL_ATSC_GetSignalStrength        fpGetSignalStrength;
    IOCTL_ATSC_GetSignalQuality         fpGetSignalQuality;
    IOCTL_ATSC_GetSNRPercentage         fpGetSNRPercentage;
    IOCTL_ATSC_GET_QAM_SNR              fpGET_QAM_SNR;
    IOCTL_ATSC_Read_uCPKT_ERR           fpRead_uCPKT_ERR;
    IOCTL_ATSC_GetPreViterbiBer         fpGetPreViterbiBer;
    IOCTL_ATSC_GetPostViterbiBer        fpGetPostViterbiBer;
    IOCTL_ATSC_SetSerialControl         fpSetSerialControl;
    IOCTL_ATSC_ReadFrequencyOffset      fpReadFrequencyOffset;
    IOCTL_ATSC_IIC_BYPASS_MODE          fpIIC_BYPASS_MODE;
    IOCTL_ATSC_SWITCH_SSPI_GPIO         fpSWITCH_SSPI_GPIO;
    IOCTL_ATSC_GPIO_GET_LEVEL           fpGPIO_GET_LEVEL;
    IOCTL_ATSC_GPIO_SET_LEVEL           fpGPIO_SET_LEVEL;
    IOCTL_ATSC_GPIO_OUT_ENABLE          fpGPIO_OUT_ENABLE;
    IOCTL_ATSC_DO_IQ_SWAP               fpDO_IQ_SWAP;
    IOCTL_ATSC_GetReg                   fpGetReg;
    IOCTL_ATSC_SetReg                   fpSetReg;
} ATSC_INSTANT_PRIVATE;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

extern DMD_ATSC_ResData *psDMD_ATSC_ResData;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

MS_U32 ATSCOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    ATSC_INSTANT_PRIVATE *pAtscPri = NULL;
    void *pAtscPriVoid = NULL;

	printf("\n[ATSC INFO] atsc open");

    // instance is allocated here, also can allocate private for internal use, ex, BDMA_INSTANT_PRIVATE
    UtopiaInstanceCreate(sizeof(ATSC_INSTANT_PRIVATE), ppInstance);
    // setup func in private and assign the calling func in func ptr in instance private
    UtopiaInstanceGetPrivate(*ppInstance, &pAtscPriVoid);
    pAtscPri = (ATSC_INSTANT_PRIVATE*)pAtscPriVoid;

    pAtscPri->fpSetDbgLevel         = _MDrv_DMD_ATSC_SetDbgLevel;
    pAtscPri->fpGetInfo             = _MDrv_DMD_ATSC_GetInfo;
    pAtscPri->fpGetLibVer           = _MDrv_DMD_ATSC_GetLibVer;
    pAtscPri->fpInit                = _MDrv_DMD_ATSC_MD_Init;
    pAtscPri->fpExit                = _MDrv_DMD_ATSC_MD_Exit;
    pAtscPri->fpSetConfig           = _MDrv_DMD_ATSC_MD_SetConfig;
    pAtscPri->fpSetReset            = _MDrv_DMD_ATSC_MD_SetReset;
    pAtscPri->fpSet_QAM_SR          = _MDrv_DMD_ATSC_MD_Set_QAM_SR;
    pAtscPri->fpSetActive           = _MDrv_DMD_ATSC_MD_SetActive;
    #if DMD_ATSC_STR_EN
    pAtscPri->fpSetPowerState       = _MDrv_DMD_ATSC_MD_SetPowerState;
    #endif
    pAtscPri->fpGetLock             = _MDrv_DMD_ATSC_MD_GetLock;
    pAtscPri->fpGetModulationMode   = _MDrv_DMD_ATSC_MD_GetModulationMode;
    pAtscPri->fpGetSignalStrength   = _MDrv_DMD_ATSC_MD_GetSignalStrength;
    pAtscPri->fpGetSignalQuality    = _MDrv_DMD_ATSC_MD_GetSignalQuality;
    pAtscPri->fpGetSNRPercentage    = _MDrv_DMD_ATSC_MD_GetSNRPercentage;
    pAtscPri->fpGET_QAM_SNR         = _MDrv_DMD_ATSC_MD_GET_QAM_SNR;
    pAtscPri->fpRead_uCPKT_ERR      = _MDrv_DMD_ATSC_MD_Read_uCPKT_ERR;
    pAtscPri->fpGetPreViterbiBer    = _MDrv_DMD_ATSC_MD_GetPreViterbiBer;
    pAtscPri->fpGetPostViterbiBer   = _MDrv_DMD_ATSC_MD_GetPostViterbiBer;
    pAtscPri->fpReadFrequencyOffset = _MDrv_DMD_ATSC_MD_ReadFrequencyOffset;
    pAtscPri->fpSetSerialControl    = _MDrv_DMD_ATSC_MD_SetSerialControl;
    pAtscPri->fpIIC_BYPASS_MODE     = _MDrv_DMD_ATSC_MD_IIC_BYPASS_MODE;
    pAtscPri->fpSWITCH_SSPI_GPIO    = _MDrv_DMD_ATSC_MD_SWITCH_SSPI_GPIO;
    pAtscPri->fpGPIO_GET_LEVEL      = _MDrv_DMD_ATSC_MD_GPIO_GET_LEVEL;
    pAtscPri->fpGPIO_SET_LEVEL      = _MDrv_DMD_ATSC_MD_GPIO_SET_LEVEL;
    pAtscPri->fpGPIO_OUT_ENABLE     = _MDrv_DMD_ATSC_MD_GPIO_OUT_ENABLE;
    pAtscPri->fpDO_IQ_SWAP          = _MDrv_DMD_ATSC_MD_DoIQSwap;
    pAtscPri->fpGetReg              = _MDrv_DMD_ATSC_MD_GetReg;
    pAtscPri->fpSetReg              = _MDrv_DMD_ATSC_MD_SetReg;

	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 ATSCIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    void* pModule = NULL;
	UtopiaInstanceGetModule(pInstance, &pModule);
	void* pResource = NULL;
    ATSC_INSTANT_PRIVATE* psATSCInstPri = NULL;
    void* psATSCInstPriVoid = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void**)&psATSCInstPriVoid);
    psATSCInstPri = (ATSC_INSTANT_PRIVATE*)psATSCInstPriVoid;

    MS_U32 u32Ret = FALSE;

    if (UtopiaResourceObtain(pModule, ATSC_POOL_ID_DMD0, &pResource) != 0)
    {
        printf("UtopiaResourceObtainToInstant fail\n");
	    return UTOPIA_STATUS_ERR_RESOURCE;
    }

    psDMD_ATSC_ResData = ((PATSC_RESOURCE_PRIVATE)pResource)->sDMD_ATSC_ResData;

    switch (u32Cmd)
    {
        case DMD_ATSC_DRV_CMD_SetDbgLevel:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_SetDbgLevel\n"));
            u32Ret = psATSCInstPri->fpSetDbgLevel(((PATSC_DBG_LEVEL_PARAM)pArgs)->u8DbgLevel);
            break;
        case DMD_ATSC_DRV_CMD_GetInfo:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_GetInfo\n"));
            ((PATSC_GET_INFO_PARAM)pArgs)->pInfo = psATSCInstPri->fpGetInfo();
            u32Ret = TRUE;
            break;
        case DMD_ATSC_DRV_CMD_GetLibVer:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_GetLibVer\n"));
            u32Ret = psATSCInstPri->fpGetLibVer(((PATSC_GET_LIB_VER_PARAM)pArgs)->ppVersion);
            break;
        case DMD_ATSC_DRV_CMD_Init:
            ((PATSC_INIT_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_Init:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_Init\n"));
            u32Ret = psATSCInstPri->fpInit(((PATSC_INIT_PARAM)pArgs)->id, ((PATSC_INIT_PARAM)pArgs)->pDMD_ATSC_InitData, ((PATSC_INIT_PARAM)pArgs)->u32InitDataLen);
            break;
        case DMD_ATSC_DRV_CMD_Exit:
            ((PATSC_INIT_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_Exit:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_Exit\n"));
            u32Ret = psATSCInstPri->fpExit(((PATSC_INIT_PARAM)pArgs)->id);
            break;
        case DMD_ATSC_DRV_CMD_SetConfig:
            ((PATSC_SET_CONFIG_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_SetConfig:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_SetConfig\n"));
            u32Ret = psATSCInstPri->fpSetConfig(((PATSC_SET_CONFIG_PARAM)pArgs)->id, ((PATSC_SET_CONFIG_PARAM)pArgs)->eType, ((PATSC_SET_CONFIG_PARAM)pArgs)->bEnable);
            break;
        case DMD_ATSC_DRV_CMD_SetReset:
            ((PATSC_INIT_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_SetReset:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_SetReset\n"));
            u32Ret = psATSCInstPri->fpSetReset(((PATSC_INIT_PARAM)pArgs)->id);
            break;
        case DMD_ATSC_DRV_CMD_Set_QAM_SR:
            ((PATSC_SET_QAM_SR_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_Set_QAM_SR:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_Set_QAM_SR\n"));
            u32Ret = psATSCInstPri->fpSet_QAM_SR(((PATSC_SET_QAM_SR_PARAM)pArgs)->id, ((PATSC_SET_QAM_SR_PARAM)pArgs)->eType, ((PATSC_SET_QAM_SR_PARAM)pArgs)->symbol_rate);
            break;
        case DMD_ATSC_DRV_CMD_SetActive:
            ((PATSC_SET_ACTIVE_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_SetActive:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_SetActive\n"));
            u32Ret = psATSCInstPri->fpSetActive(((PATSC_SET_ACTIVE_PARAM)pArgs)->id, ((PATSC_SET_ACTIVE_PARAM)pArgs)->bEnable);
            break;
        case DMD_ATSC_DRV_CMD_SetPowerState:
            ((PATSC_SET_POWER_STATE_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_SetPowerState:
            #if DMD_ATSC_STR_EN
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_SetPowerState\n"));
            if(psATSCInstPri->fpSetPowerState(((PATSC_SET_POWER_STATE_PARAM)pArgs)->id, ((PATSC_SET_POWER_STATE_PARAM)pArgs)->u16PowerState) == UTOPIA_STATUS_SUCCESS)
                u32Ret = TRUE;
            else
                u32Ret = FALSE;
            #else
            DMD_DBG(printf("ATSCIoctl - Don't support MDrv_DMD_ATSC_MD_SetPowerState\n"));
            #endif
            break;
        case DMD_ATSC_DRV_CMD_GetLock:
            ((PATSC_GET_LOCK_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_GetLock:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_GetLock\n"));
            ((PATSC_GET_LOCK_PARAM)pArgs)->status = psATSCInstPri->fpGetLock(((PATSC_GET_LOCK_PARAM)pArgs)->id, ((PATSC_GET_LOCK_PARAM)pArgs)->eType);
            u32Ret = TRUE;
            break;
        case DMD_ATSC_DRV_CMD_GetModulationMode:
            ((PATSC_GET_MODULATION_MODE_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_GetModulationMode:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_GetModulationMode\n"));
            ((PATSC_GET_MODULATION_MODE_PARAM)pArgs)->eType = psATSCInstPri->fpGetModulationMode(((PATSC_GET_MODULATION_MODE_PARAM)pArgs)->id);
            u32Ret = TRUE;
            break;
        case DMD_ATSC_DRV_CMD_GetSignalStrength:
            ((PATSC_GET_SIGNAL_STRENGTH_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_GetSignalStrength:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_GetSignalStrength\n"));
            u32Ret = psATSCInstPri->fpGetSignalStrength(((PATSC_GET_SIGNAL_STRENGTH_PARAM)pArgs)->id, &(((PATSC_GET_SIGNAL_STRENGTH_PARAM)pArgs)->u16Strength));
            break;
        case DMD_ATSC_DRV_CMD_GetSignalQuality:
            ((PATSC_GET_SIGNAL_QUALITY_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_GetSignalQuality:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_GetSignalQuality\n"));
            ((PATSC_GET_SIGNAL_QUALITY_PARAM)pArgs)->eQuality = psATSCInstPri->fpGetSignalQuality(((PATSC_GET_SIGNAL_QUALITY_PARAM)pArgs)->id);
            u32Ret = TRUE;
            break;
        case DMD_ATSC_DRV_CMD_GetSNRPercentage:
            ((PATSC_GET_SNR_PERCENTAGE_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_GetSNRPercentage:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_GetSNRPercentage\n"));
            ((PATSC_GET_SNR_PERCENTAGE_PARAM)pArgs)->u8Percentage = psATSCInstPri->fpGetSNRPercentage(((PATSC_GET_SNR_PERCENTAGE_PARAM)pArgs)->id);
            u32Ret = TRUE;
            break;
        case DMD_ATSC_DRV_CMD_GET_QAM_SNR:
            ((PATSC_GET_SNR_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_GET_QAM_SNR:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_GET_QAM_SNR\n"));
            u32Ret = psATSCInstPri->fpGET_QAM_SNR(((PATSC_GET_SNR_PARAM)pArgs)->id, &(((PATSC_GET_SNR_PARAM)pArgs)->snr));
            break;
        case DMD_ATSC_DRV_CMD_Read_uCPKT_ERR:
            ((PATSC_GET_UCPKT_ERR_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_Read_uCPKT_ERR:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_Read_uCPKT_ERR\n"));
            u32Ret = psATSCInstPri->fpRead_uCPKT_ERR(((PATSC_GET_UCPKT_ERR_PARAM)pArgs)->id, &(((PATSC_GET_UCPKT_ERR_PARAM)pArgs)->u16PacketErr));
            break;
        case DMD_ATSC_DRV_CMD_GetPreViterbiBer:
            ((PATSC_GET_BER_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_GetPreViterbiBer:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_GetPreViterbiBer\n"));
            u32Ret = psATSCInstPri->fpGetPreViterbiBer(((PATSC_GET_BER_PARAM)pArgs)->id, &(((PATSC_GET_BER_PARAM)pArgs)->ber));
            break;
        case DMD_ATSC_DRV_CMD_GetPostViterbiBer:
            ((PATSC_GET_BER_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_GetPostViterbiBer:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_GetPostViterbiBer\n"));
            u32Ret = psATSCInstPri->fpGetPostViterbiBer(((PATSC_GET_BER_PARAM)pArgs)->id, &(((PATSC_GET_BER_PARAM)pArgs)->ber));
            break;
        case DMD_ATSC_DRV_CMD_ReadFrequencyOffset:
            ((PATSC_READ_FREQ_OFFSET_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_ReadFrequencyOffset:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_ReadFrequencyOffset\n"));
            u32Ret = psATSCInstPri->fpReadFrequencyOffset(((PATSC_READ_FREQ_OFFSET_PARAM)pArgs)->id, &(((PATSC_READ_FREQ_OFFSET_PARAM)pArgs)->cfo));
            break;
        case DMD_ATSC_DRV_CMD_SetSerialControl:
            ((PATSC_SET_SERIAL_CONTROL_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_SetSerialControl:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_SetSerialControl\n"));
            u32Ret = psATSCInstPri->fpSetSerialControl(((PATSC_SET_SERIAL_CONTROL_PARAM)pArgs)->id, ((PATSC_SET_SERIAL_CONTROL_PARAM)pArgs)->u8TsConfigData);
            break;
        case DMD_ATSC_DRV_CMD_IIC_BYPASS_MODE:
            ((PATSC_IIC_BYPASS_MODE_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_IIC_BYPASS_MODE:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_IIC_BYPASS_MODE\n"));
            u32Ret = psATSCInstPri->fpIIC_BYPASS_MODE(((PATSC_IIC_BYPASS_MODE_PARAM)pArgs)->id, ((PATSC_IIC_BYPASS_MODE_PARAM)pArgs)->bEnable);
            break;
        case DMD_ATSC_DRV_CMD_SWITCH_SSPI_GPIO:
            ((PATSC_SWITCH_SSPI_GPIO_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_SWITCH_SSPI_GPIO:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_SWITCH_SSPI_GPIO\n"));
            u32Ret = psATSCInstPri->fpSWITCH_SSPI_GPIO(((PATSC_SWITCH_SSPI_GPIO_PARAM)pArgs)->id, ((PATSC_SWITCH_SSPI_GPIO_PARAM)pArgs)->bEnable);
            break;
        case DMD_ATSC_DRV_CMD_GPIO_GET_LEVEL:
            ((PATSC_GPIO_LEVEL_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_GPIO_GET_LEVEL:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_GPIO_GET_LEVEL\n"));
            u32Ret = psATSCInstPri->fpGPIO_GET_LEVEL(((PATSC_GPIO_LEVEL_PARAM)pArgs)->id, ((PATSC_GPIO_LEVEL_PARAM)pArgs)->u8Pin, &(((PATSC_GPIO_LEVEL_PARAM)pArgs)->bLevel));
            break;
        case DMD_ATSC_DRV_CMD_GPIO_SET_LEVEL:
            ((PATSC_GPIO_LEVEL_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_GPIO_SET_LEVEL:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_GPIO_SET_LEVEL\n"));
            u32Ret = psATSCInstPri->fpGPIO_SET_LEVEL(((PATSC_GPIO_LEVEL_PARAM)pArgs)->id, ((PATSC_GPIO_LEVEL_PARAM)pArgs)->u8Pin, ((PATSC_GPIO_LEVEL_PARAM)pArgs)->bLevel);
            break;
        case DMD_ATSC_DRV_CMD_GPIO_OUT_ENABLE:
            ((PATSC_GPIO_OUT_ENABLE_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_GPIO_OUT_ENABLE:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_GPIO_OUT_ENABLE\n"));
            u32Ret = psATSCInstPri->fpGPIO_OUT_ENABLE(((PATSC_GPIO_OUT_ENABLE_PARAM)pArgs)->id, ((PATSC_GPIO_OUT_ENABLE_PARAM)pArgs)->u8Pin, ((PATSC_GPIO_OUT_ENABLE_PARAM)pArgs)->bEnableOut);
            break;
        case DMD_ATSC_DRV_CMD_DoIQSwap:
            ((PATSC_DO_IQ_SWAP_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_DoIQSwap:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_DO_IQ_SWAP\n"));
            u32Ret = psATSCInstPri->fpDO_IQ_SWAP(((PATSC_DO_IQ_SWAP_PARAM)pArgs)->id, ((PATSC_DO_IQ_SWAP_PARAM)pArgs)->bIsQPad);
            break;
        case DMD_ATSC_DRV_CMD_GetReg:
            ((PATSC_REG_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_GetReg:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_GetReg\n"));
            u32Ret = psATSCInstPri->fpGetReg(((PATSC_REG_PARAM)pArgs)->id, ((PATSC_REG_PARAM)pArgs)->u16Addr, &(((PATSC_REG_PARAM)pArgs)->u8Data));
            break;
        case DMD_ATSC_DRV_CMD_SetReg:
            ((PATSC_REG_PARAM)pArgs)->id = 0;
        case DMD_ATSC_DRV_CMD_MD_SetReg:
            DMD_DBG(printf("ATSCIoctl - MDrv_DMD_ATSC_MD_SetReg\n"));
            u32Ret = psATSCInstPri->fpSetReg(((PATSC_REG_PARAM)pArgs)->id, ((PATSC_REG_PARAM)pArgs)->u16Addr, ((PATSC_REG_PARAM)pArgs)->u8Data);
            break;
        default:
            break;
    }

    UtopiaResourceRelease(pResource);

    return (u32Ret ? UTOPIA_STATUS_SUCCESS : UTOPIA_STATUS_FAIL);
}

MS_U32 ATSCClose(void* pInstance)
{
    UtopiaInstanceDelete(pInstance);

	return UTOPIA_STATUS_SUCCESS;
}

void ATSCRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    void* pUtopiaModule = NULL;
	UtopiaModuleCreate(MODULE_ATSC, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)ATSCOpen, (FUtopiaClose)ATSCClose, (FUtopiaIOctl)ATSCIoctl);
    // register func for module, after register here, then ap call UtopiaSTR can call to registered standard func
    #if DMD_ATSC_STR_EN
    UtopiaModuleSetupSTRFunctionPtr(pUtopiaModule,(FUtopiaSTR)_MDrv_DMD_ATSC_MD_SetPowerState);
    #endif

    // 2. deal with resource
    void* psResource = NULL;
    // start func to add res, call once will create 2 access in resource. Also can declare BDMA_POOL_ID_BDMA1 for another channel depend on driver owner.
	UtopiaModuleAddResourceStart(pUtopiaModule, ATSC_POOL_ID_DMD0);
    // resource can alloc private for internal use, ex, BDMA_RESOURCE_PRIVATE
    UtopiaResourceCreate("atsc0", sizeof(ATSC_RESOURCE_PRIVATE), &psResource);
    // func to reg res
    UtopiaResourceRegister(pUtopiaModule, psResource, ATSC_POOL_ID_DMD0);
    // end function to add res
	UtopiaModuleAddResourceEnd(pUtopiaModule, ATSC_POOL_ID_DMD0);
}
