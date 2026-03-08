/**
* Copyright (c) 2006-2019 MediaTek Inc.
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

/*
 @file   xc_private.h
 @brief  xc private
 @author MStar Semiconductor Inc.
*/

#ifndef _XC_PRIVATE_H_
#define _XC_PRIVATE_H_

#ifdef _MDRV_XC_C_
#ifndef INTERFACE
#define INTERFACE
#endif
#else
#ifndef INTERFACE
#define INTERFACE   extern
#endif
#endif


/*=============================================================================
 Includs
=============================================================================*/


/* -------------------------------------------------------------------------------------------------
  Global Variables
------------------------------------------------------------------------------------------------- */
INTERFACE MS_U32 gu32DbgLevel;
INTERFACE MS_U32 gu32DbgMode;

/* -------------------------------------------------------------------------------------------------
  Macro and Define
------------------------------------------------------------------------------------------------- */

/* Please add help info in the functon of KDrv_XC_DebugLevelRead() in mdrv_xc_debugfs.c, when debug level is changed. */
#define XC_DBG_LEVEL_ERR                            BIT(0)
#define XC_DBG_LEVEL_INFO                           BIT(1)
#define XC_DBG_LEVEL_PERFORMANCE                    BIT(2)
#define XC_DBG_LEVEL_CFD_CTRL                       BIT(3)
#define XC_DBG_LEVEL_SEAMLESS                       BIT(4)
#define XC_DBG_LEVEL_DOLBY_SETT                     BIT(5)
#define XC_DBG_LEVEL_DOLBY_GD                       BIT(6)
#define XC_DBG_LEVEL_WRAPPER_ALG                    BIT(7)
#define XC_DBG_LEVEL_WRAPPER_ALG_CTRL_POINT         BIT(8)
#define XC_DBG_LEVEL_HDR_EMP_RECEIVE_STATUS         BIT(9)
#define XC_DBG_LEVEL_HDR_EMP_PERFORMANCE            BIT(10)
#define XC_DBG_LEVEL_IO_CTOL_INFO                   BIT(11)

#define XC_DBG_LEVEL_HDR10_PLUS_GOLDEN              BIT(16)

/* Default control point customer debugfs setting(controlled by TMO Qmap) */
#define XC_DBG_DEFAULT_CUSTOMER_MANUAL_SET          (0xFF)

/* Please add help info in the functon of KDrv_XC_EmpDebugLevelRead() in mdrv_xc_debugfs.c, when debug level is changed. */
#define EMP_DBG_HDREMPRECEIVESTATUS                  BIT(0)
#define EMP_DBG_EMPIPIRQSEQ                          BIT(1)
#define EMP_DBG_HDREMPPERFORMANCE                    BIT(2)

#define XC_LOG_TRACE(_dbgLevel_, _fmt, _args...) {\
    if ((_dbgLevel_ & gu32DbgLevel) != 0) {\
	if (_dbgLevel_ & XC_DBG_LEVEL_DOLBY_GD) {\
		printk("[GD] "_fmt, ##_args);\
	} \
	else if (_dbgLevel_ & XC_DBG_LEVEL_DOLBY_SETT) {\
		printk("[SETT] "_fmt, ##_args);\
	} \
	else if (_dbgLevel_ & XC_DBG_LEVEL_ERR) {\
		printk(KERN_EMERG "[Error] [%s,%5d]"_fmt, __FUNCTION__, __LINE__, ##_args);\
	} \
	else{\
		printk("[%s,%5d]"_fmt, __FUNCTION__, __LINE__, ##_args);\
	} \
    } \
}

#define TIME_DIFF_MS(startTs, endTs) ((endTs.tv_nsec / 1000000 + endTs.tv_sec * 1000) - (startTs.tv_nsec / 1000000 + startTs.tv_sec * 1000))

#define XC_PERFORMANCE_STAT_AVG_CNT  30  /* calculate avg per 30 frame */

#define TIME_DIFF_MS_BEGIN(stTimeStart)\
{\
    if (XC_DBG_LEVEL_PERFORMANCE & gu32DbgLevel) {\
	getnstimeofday(&stTimeStart);\
    } \
}

#define TIME_DIFF_MS_END(stTimeStart, stTimeEnd, stPerformance)\
{\
    if ((XC_DBG_LEVEL_PERFORMANCE & gu32DbgLevel) || (XC_DBG_LEVEL_HDR_EMP_PERFORMANCE & gu32DbgLevel)) {\
	static MS_U32 u32TimeDiffTotal;\
	MS_U32 u32TimeDiff = 0;\
	getnstimeofday(&stTimeEnd);\
	u32TimeDiff = TIME_DIFF_MS(stTimeStart, stTimeEnd);\
	stPerformance.u32TimeMSCur = u32TimeDiff;\
	stPerformance.u32Count++;\
\
	if ((stPerformance.u32TimeMSMax == 0)\
	&& (stPerformance.u32TimeMSMin == 0)\
	&& (stPerformance.u32TimeMSAvg == 0)) {\
		stPerformance.u32TimeMSMax = u32TimeDiff;\
		stPerformance.u32TimeMSMin = u32TimeDiff;\
		stPerformance.u32TimeMSAvg = u32TimeDiff;\
	} \
\
	if (u32TimeDiff >= stPerformance.u32TimeMSMax) {\
		stPerformance.u32TimeMSMax = u32TimeDiff;\
	} \
\
	if (u32TimeDiff <= stPerformance.u32TimeMSMin) {\
		stPerformance.u32TimeMSMin = u32TimeDiff;\
	} \
\
	u32TimeDiffTotal += u32TimeDiff;\
	if (stPerformance.u32Count == XC_PERFORMANCE_STAT_AVG_CNT) {\
		stPerformance.u32TimeMSAvg = (u32TimeDiffTotal / XC_PERFORMANCE_STAT_AVG_CNT);\
		stPerformance.u32Count = 0;\
		u32TimeDiffTotal = 0;\
	} \
    } \
}

#define CFD_IS_HDMI(x)         ((x) == E_CFD_INPUT_SOURCE_HDMI)
#define CFD_IS_DVI(x)          ((x) == E_CFD_INPUT_SOURCE_DVI)
#define CFD_IS_MM(x)           (((x) == E_CFD_INPUT_SOURCE_STORAGE) || ((x) == E_CFD_INPUT_SOURCE_JPEG))
#define CFD_IS_DTV(x)          ((x) == E_CFD_INPUT_SOURCE_DTV)
#define CFD_IS_VGA(x)          ((x) == E_CFD_INPUT_SOURCE_VGA)
#define CFD_IS_COMPONENT(x)    ((x) == E_CFD_INPUT_SOURCE_YPBPR)
#define CFD_IS_ATV(x)          ((x) == E_CFD_INPUT_SOURCE_TV)
#define CFD_IS_CVBS(x)         ((x) == E_CFD_INPUT_SOURCE_CVBS)
#define CFD_IS_SCART(x)        ((x) == E_CFD_INPUT_SOURCE_SCART)
#define CFD_IS_VD(x)           (CFD_IS_ATV(x) || CFD_IS_CVBS(x) || CFD_IS_SCART(x))
#define CFD_IS_ANALOG(X)       (((x) == E_CFD_INPUT_SOURCE_VGA) || ((x) == E_CFD_INPUT_SOURCE_TV) || ((x) == E_CFD_INPUT_SOURCE_CVBS) \
				|| ((x) == E_CFD_INPUT_SOURCE_SVIDEO) || ((x) == E_CFD_INPUT_SOURCE_YPBPR) || ((x) == E_CFD_INPUT_SOURCE_SCART) \
				|| ((x) == E_CFD_INPUT_SOURCE_DVI))
#define CFD_IS_NONE(x)         ((x) == E_CFD_INPUT_SOURCE_NONE)


/* =============================================================================
 For xc debug mode
============================================================================= */
/* Please add help info in the functon of KDrv_XC_DebugModeRead in mdrv_xc_debugfs.c, when debug mode is changed. */
#define XC_DBG_MODE_DUMP_MEM_INFO               BIT(0)
#define XC_DBG_MODE_FIRE_CFD_RUN_TEST_CASE      BIT(1)
#define XC_DBG_MODE_FIRE_CFD_WITH_RIU           BIT(2)
#define XC_DBG_MODE_FIRE_CFD_WITH_MLOAD         BIT(3)
#define XC_DBG_MODE_FIRE_CFD_BYPASS_PRE_SDR     BIT(4)
#define XC_DBG_MODE_FIRE_CFD_BYPASS_HDR         BIT(5)
#define XC_DBG_MODE_FIRE_CFD_BYPASS_POST_SDR    BIT(6)
#define XC_DBG_MODE_FIRE_CFD_FORCE_RUN          BIT(7)



/* =============================================================================
 Type and Structure Declaration
============================================================================= */
typedef enum {
    E_CHIP_MIU_0 = 0,
    E_CHIP_MIU_1,
    E_CHIP_MIU_2,
    E_CHIP_MIU_3,
    E_CHIP_MIU_NUM,
} CHIP_MIU_ID;

typedef enum {
    E_KDRV_XC_WRITE_REG_RIU,
    E_KDRV_XC_WRITE_REG_MLOAD,
    E_KDRV_XC_WRITE_REG_DS,
} EN_KDRV_XC_WRITE_REG_METHOD;

typedef struct {
    EN_KDRV_XC_WRITE_REG_METHOD eMethod;
    K_XC_DS_CMDCNT *pstCmdCnt;
    MS_BOOL bExternalBuf;
    MS_U32 u32Client; /* refer to E_DS_CLIENT & EN_MLOAD_CLIENT_TYPE */
    MS_U8 u8DsIdx;    /* when bExternalBuf = FALSE */
} ST_KDRV_XC_WRITE_REG_METHOD_INFO;

typedef struct {
    MS_BOOL bAdlImmediate;
    MS_BOOL bExternalBuf;
} ST_KDRV_XC_WRITE_ADL_METHOD_INFO;

typedef struct {
    ST_KDRV_XC_WRITE_REG_METHOD_INFO stWriteRegInfo;
    ST_KDRV_XC_WRITE_ADL_METHOD_INFO stWriteAdlInfo;
    /* force to calc PreSDR/HDR/PostSDR */
    MS_BOOL bForced;
    MS_BOOL bAtomic;
    MS_BOOL bSkipPostSdr;
} ST_KDRV_XC_FIRE_METHOD_INFO;

typedef struct {
    MS_U32 u32TimeMSMax;
    MS_U32 u32TimeMSMin;
    MS_U32 u32TimeMSAvg;
    MS_U32 u32TimeMSCur;
    MS_U32 u32Count;
} ST_KDRV_XC_PERFORMANCE;

typedef enum {
    E_KDRV_XC_PATH_SC0,
    E_KDRV_XC_PATH_SC1,
    E_KDRV_XC_PATH_MAX,
} EN_KDRV_XC_PATH;

typedef struct {
    MS_U32 u32Version;
    MS_U16 u16Length;
    MS_U16 u16UpdateStatus; /* Check whitch IPs need to update */
} ST_KDRV_XC_CFD_OUTPUT_STATUS;

typedef struct {
    MS_U32 u32Depth;
    MS_U32 *pu32Address;
    MS_U16 *pu16Value;
    MS_U16 *pu16Mask;
    MS_U16 *pu16Client;
} ST_KDRV_XC_REG_TABLE;

typedef struct {
    MS_U16 u16Client;
    MS_U8 *pu8Data;
    MS_U32 u32Size;
} ST_KDRV_XC_ADL_TABLE;

typedef struct {
    MS_U32 u32Version;
    MS_U16 u16Length;
    ST_KDRV_XC_REG_TABLE stRegTable;
    ST_KDRV_XC_ADL_TABLE stAdlTable;
    ST_KDRV_XC_CFD_OUTPUT_STATUS stStatus;
} ST_KDRV_XC_GENERAL_OUTPUT;

#endif /* _XC_PRIVATE_H_ */
