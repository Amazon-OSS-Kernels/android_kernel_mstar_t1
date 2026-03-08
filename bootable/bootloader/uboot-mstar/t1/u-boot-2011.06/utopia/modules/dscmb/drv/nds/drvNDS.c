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
/// file    drvNDS.c
/// @brief  NDS Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"               // MStar common .h
#include "drvMMIO.h"

#include "regNDS.h"                 // MStar NDS  HAL
#include "drvNDS.h"
#include "halNDS.h"

#include "nds.h"                    // internal .h

#include "ddiNDS_HDI.h"             // NDS CAHDI driver .h


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


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
void NDS_DBG_PrintData(MS_U8 *pu8Data, MS_U16 u16DataLen)
{
    int                 i;
    //printf("(%d)>>", u16DataLen);
    for (i = 0; i < u16DataLen; i++)
    {
        if ((i % 16) == 0)
        {
            NDS_DBG("\n    ");
        }
        NDS_DBG(" %02X ", pu8Data[i]);
    }
    NDS_DBG("\n");
}


NDS_Result MDrv_NDS_Init(NDS_Param *param)
{
    MS_U32              u32BaseAddr, u32BaseSize;

    if (FALSE == MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_NDS))
    {
        NDS_ERR("MDrv_MMIO_GetBASE fail\n");
        return E_NDS_FAIL;
    }
    NDS_RegMap(u32BaseAddr);
    if (FALSE == MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_NSK))
    {
        NDS_ERR("MDrv_MMIO_GetBASE fail\n");
        return E_NDS_FAIL;
    }
    NDS_NSK_RegMap(u32BaseAddr);
    if (FALSE == MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_HW))
    {
        NDS_ERR("MDrv_MMIO_GetBASE fail\n");
        return E_NDS_FAIL;
    }
    NDS_HW_RegMap(u32BaseAddr);

    NDS_CAM_Init();
    NDS_FLT_Init(&(param->flt));    // ECM / EMM
    NDS_SC_Init(&(param->sc));      // SC

    #if 0
    NDS_RASP_Init(&(param->rasp));
    #endif


    NDS_SC_Open();

    //[NOTE] add for J2 to open NSK clk
    HAL_NDS_PowCtrl(0, TRUE);

    // Set Chip Magic Value //
    HAL_NDS_InitMagic();


    return E_NDS_OK;
}


NDS_Result MDrv_NDS_Exit(void)
{
    NDS_SC_Close();
    NDS_SC_Exit();
    NDS_FLT_Exit();
    NDS_CAM_Exit();

    return E_NDS_OK;
}


NDS_Result MDrv_NDS_PowerOff(void)
{
    MS_U32              u32BaseAddr, u32BaseSize;
    int                 i;

    NDS_ERR("%s : Force Power OFF [BEGIN]\n", __FUNCTION__);
    NDS_ERR("%s : All MDrv_NDS_ is not accessible now\n", __FUNCTION__);

    if (FALSE == MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_HW))
    {
        NDS_ERR("MDrv_MMIO_GetBASE fail\n");
        return E_NDS_FAIL;
    }
    NDS_HW_RegMap(u32BaseAddr);

    for (i = 0; i < NSK_DEV_NUM; i++)
    {
        HAL_NDS_PowCtrl(i, TRUE);
    }

    NDS_ERR("%s : Force Power OFF [COMPLETED]\n", __FUNCTION__);

    return E_NDS_OK;
}


MS_U32 MDrv_NDS_GetCaps(NDS_Caps cap)
{
//    return HAL_NDS_GetCaps(cap);
    switch (cap)
    {
    case E_NDS_CAP_ECM_NUM :
        return NDS_CAM_ECM_FLT_NUM;

    case E_NDS_CAP_EMM_NUM :
        return NDS_CAM_EMM_BUF_NUM;

    case E_NDS_CAP_BUF_PITCH :
        return NDS_CAM_FLT_BUF_SIZE;

    case E_NDS_CAP_BUF_ALIGN :
        return NDS_CAM_FLT_BUF_ALIGN;

    case E_NDS_CAP_BUF_TOTALSIZE :
        return NDS_CAM_FLT_BUFS_SIZE;

    case E_NDS_CAP_RASP_BUF_TOTALSIZE :
        return NDS_RASP_ALL_BUF_SIZE;

    default:
        return 0;
    }
}


NDS_Result MDrv_NDS_SetMagicValue(MS_U8 idx, MS_U16 word_0, MS_U16 word_1, MS_U16 word_2, MS_U16 word_3)
{
    if (idx > 4)
    {
        return E_NDS_FAIL;
    }

    NDS_REG(REG_MAGIC_V(idx, 0)) = word_0;
    NDS_REG(REG_MAGIC_V(idx, 1)) = word_1;
    NDS_REG(REG_MAGIC_V(idx, 2)) = word_2;
    NDS_REG(REG_MAGIC_V(idx, 3)) = word_3;

    return E_NDS_OK;
}


NDS_Result MDrv_NDS_NSK_Open(MS_U32 nds_id)
{
    HAL_NDS_PowCtrl(nds_id, TRUE);

    NDS_NSK_Init(nds_id);

    return E_NDS_OK;
}


NDS_Result MDrv_NDS_NSK_Close(MS_U32 nds_id)
{
    NDS_NSK_Exit(nds_id);

    HAL_NDS_PowCtrl(nds_id, FALSE);

    return E_NDS_OK;
}


