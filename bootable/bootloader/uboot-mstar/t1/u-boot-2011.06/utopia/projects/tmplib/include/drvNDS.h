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
/// @file   drvNDS.h
/// @brief  NDS Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_NDS_H_
#define _DRV_NDS_H_


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
#define NDS_ENFLAGS_CTRL_ENC_SUPPORTED  0x01
#define NDS_ENFLAGS_CTRL_ENC_HW_FORCED  0x02    // permanently
#define NDS_ENFLAGS_CTRL_ENC_SW_FORCED  0x04    // force until reset
#define NDS_ENFLAGS_JTAG_PWD            0x08
#define NDS_ENFLAGS_BOOTROM             0x10
#define NDS_ENFLAGS_DATA_ENC            0x20
#define NDS_ENFLAGS_GETRESP2CHALLENGE   0x40


// Predefined by NDS
// ref. icq063
#define NDS_KTE_ESA_SEL_AES             0x0
#define NDS_KTE_ESA_SEL_CSA             0x1
#define NDS_KTE_ESA_SEL_DES             0x2
#define NDS_KTE_ESA_SEL_TDES            0x3
#define NDS_KTE_ESA_SEL_MULTI2          0x4
#define NDS_KTE_ESA_SEL_DFAST           0x5
#define NDS_KTE_ESA_SEL_USER            0xF

#define NDS_KTE_ESA_SUB_ECB             0x0
#define NDS_KTE_ESA_SUB_CBC             0x1
#define NDS_KTE_ESA_SUB_NSA             0x7



//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum
{
    E_NDS_OK,
    E_NDS_FAIL,

} NDS_Result;


typedef enum
{
    E_NDS_SC_EVENT_CARD_REMOVED     =   0x00000000,
    E_NDS_SC_EVENT_CARD_INSERTED    =   0x00000001,

} NDS_SC_Event;


typedef enum
{
    E_NDS_SC_CLK_27M_D6,
    E_NDS_SC_CLK_27M_D2,
    E_NDS_SC_CLK_27M_D4,
    E_NDS_SC_CLK_27M_D8,

} NDS_SC_Clk;


typedef enum
{
    E_NDS_SC_BAUD_RATE_9600,
    E_NDS_SC_BAUD_RATE_19200,
    E_NDS_SC_BAUD_RATE_38400,
    E_NDS_SC_BAUD_RATE_76800,
    E_NDS_SC_BAUD_RATE_153600,
    E_NDS_SC_BAUD_RATE_223200,
} NDS_SC_BaudRate;


typedef enum
{
    E_NDS_CAP_ECM_NUM               =   0x00,

    E_NDS_CAP_EMM_NUM               =   0x10,

    E_NDS_CAP_BUF_PITCH             =   0x20,
    E_NDS_CAP_BUF_ALIGN,
    E_NDS_CAP_BUF_TOTALSIZE,
    E_NDS_CAP_RASP_BUF_TOTALSIZE,

} NDS_Caps;


typedef MS_BOOL                     (*P_NDS_SC_CbSetVcc)(MS_BOOL b5V);
typedef void                        (*P_NDS_SC_CbEvent)(NDS_SC_Event event);

typedef struct _NDS_SC_Param
{
    P_NDS_SC_CbSetVcc               cbSetVcc;

    P_NDS_SC_CbEvent                cbEvent;                            // for HDI certification
    MS_BOOL                         bCommDump;                          // for card debugging // @TODO: remove

} NDS_SC_Param;


typedef struct _NDS_FLT_Param
{
    MS_PHYADDR                      ecm_emm_addr;                       // NDS_CAP_BUF_SIZE*(NDS_CAP_ECM_NUM+NDS_CAP_EMM_NUM) at least
                                                                        // NDS_CAP_BUF_ALIGN aligned
    MS_U8*                          pu8ecm_emm_buf;                     // virtual address of ecm_emm_addr
    MS_U32                          ecm_emm_size;                       // total buffer size from emm_ecm_addr
} NDS_FLT_Param;

typedef struct _NDS_RASP_Param
{
    MS_PHYADDR                      ecm_addr;                       // NDS_CAP_BUF_SIZE * NDS_CAP_ECM_NUM
    MS_U8                          *pu8ecm_buf;                     // virtual address of ecm address
    MS_U32                          ecm_size;                       // total buffer size from emm_ecm_addr

    MS_PHYADDR                      payload_addr;                   // NDS_CAP_BUF_SIZE * NDS_CAP_ECM_NUM
    MS_U8                          *pu8playload_buf;                // virtual address of ecm address
    MS_U32                          payload_size;                   // total buffer size from emm_ecm_addr

} NDS_RASP_Param;

typedef struct _NDS_Param
{
    NDS_FLT_Param                   flt;
    NDS_SC_Param                    sc;
    NDS_RASP_Param                  rasp;
} NDS_Param;


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

NDS_Result                          MDrv_NDS_Init(NDS_Param *param);
NDS_Result                          MDrv_NDS_Exit(void);
NDS_Result                          MDrv_NDS_PowerOff(void);
NDS_Result                          MDrv_NDS_SetMagicValue(MS_U8 idx, MS_U16 word_0, MS_U16 word_1, MS_U16 word_2, MS_U16 word_3);
MS_U32                              MDrv_NDS_GetCaps(NDS_Caps cap);

NDS_Result                          MDrv_NDS_NSK_Open(MS_U32 nsk_id);
NDS_Result                          MDrv_NDS_NSK_Close(MS_U32 nsk_id);


#ifdef __cplusplus
}
#endif

#endif // _DRV_NDS_H_

