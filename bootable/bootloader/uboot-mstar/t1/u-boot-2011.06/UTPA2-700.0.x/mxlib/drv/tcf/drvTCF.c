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
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// file    drvTCF.c
// @brief  TCF Driver
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsVersion.h"

#include "drvMMIO.h"
#include "drvTCF.h"
#include "halTCF.h"
#include "regTCF.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Local Defines
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Global Variable
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Internal Variable
//--------------------------------------------------------------------------------------------------
static MS_BOOL      _bTCF_Init     = FALSE;
static MS_S32       _s32TCFMutexId = -1;

#define TCF_TIMEOUT_VALUE (500)


//--------------------------------------------------------------------------------------------------
//  Debug Function
//--------------------------------------------------------------------------------------------------
#define _TCF_DBG(fmt, args...)  //printf("\033[32m""[DBG]"fmt"\033[m", ## args)
#define _TCF_ERR(fmt, args...)  printf("\033[31m""[ERR]"fmt"\033[m", ## args)


//--------------------------------------------------------------------------------------------------
//  Internal Function
//--------------------------------------------------------------------------------------------------
static MS_BOOL _MDrv_CF_Ready(void)
{
    _TCF_DBG("[%s][%d] Tracing...........\n", __FUNCTION__, __LINE__);
    CF_TRANS_STATUS_VALUES eTransStatus = E_CF_TRANS_STATUS_BUSY_INIT;
    MS_U32                 u32Retries   = 0;
    MS_U8                  u8Ret        = DRV_TCF_OK;

    eTransStatus = HAL_CF_Get_Trans_Status();
    switch(eTransStatus)
    {
    case E_CF_TRANS_STATUS_INPUT:
        u32Retries = CF_CMD_WORDS;
        do {
            HAL_CF_Write_Input( 0);
            eTransStatus = HAL_CF_Get_Trans_Status();

            if( 0>=u32Retries-- )
            {
                _TCF_ERR("[%s][%d] Busy!!\n", __FUNCTION__, __LINE__);
                u8Ret = DRV_TCF_FAIL;
                break;
            }
        } while( E_CF_TRANS_STATUS_INPUT==eTransStatus );
    case E_CF_TRANS_STATUS_BUSY_OP:
        u32Retries = 30;
        while( E_CF_TRANS_STATUS_BUSY_OP==eTransStatus )
        {
            MsOS_DelayTask(1);
            eTransStatus = HAL_CF_Get_Trans_Status();

            if( 0>=u32Retries-- )
            {
                _TCF_ERR("[%s][%d] Busy!!\n", __FUNCTION__, __LINE__);
                u8Ret = DRV_TCF_FAIL;
                break;
            }
        }
    case E_CF_TRANS_STATUS_DONE_OK:
    case E_CF_TRANS_STATUS_DONE_ERROR:
    case E_CF_TRANS_STATUS_DONE_MFR_TEST:
        u32Retries = CF_CWC_WORDS;
        do {
            HAL_CF_Read_Output();
            HAL_CF_Write_Output( 0);
            eTransStatus = HAL_CF_Get_Trans_Status();

            if( 0>=u32Retries-- )
            {
                _TCF_ERR("[%s][%d] Busy!!\n", __FUNCTION__, __LINE__);
                u8Ret = DRV_TCF_FAIL;
                break;
            }
        } while( (E_CF_TRANS_STATUS_DONE_OK==eTransStatus) ||
                 (E_CF_TRANS_STATUS_DONE_ERROR==eTransStatus) ||
                 (E_CF_TRANS_STATUS_DONE_MFR_TEST==eTransStatus) );
    case E_CF_TRANS_STATUS_BUSY_INIT:
    case E_CF_TRANS_STATUS_BUSY_SETUP:
        u32Retries = 30;
        while( E_CF_TRANS_STATUS_READY > eTransStatus )
        {
            MsOS_DelayTask(1);
            eTransStatus = HAL_CF_Get_Trans_Status();

            if( 0>=u32Retries-- )
            {
                _TCF_ERR("[%s][%d] Invalid state!!\n", __FUNCTION__, __LINE__);
                u8Ret = DRV_TCF_FAIL;
                break;
            }
        }
    default:
        break;
    }

    return u8Ret;
}


static MS_BOOL _MDrv_CFKE_Ready(void)
{
    _TCF_DBG("[%s][%d] Tracing...........\n", __FUNCTION__, __LINE__);
    CFKE_TRANS_STATUS_VALUES eTransStatus = E_CFKE_TRANS_STATUS_BUSY_INIT;
    MS_U32                   u32Retries   = 0;
    MS_U8                    u8Ret        = DRV_TCF_OK;


    eTransStatus = HAL_CFKE_Get_OperationStatus();
    switch(eTransStatus){
        case E_CFKE_TRANS_STATUS_DONE_ERROR:
        case E_CFKE_TRANS_STATUS_DONE_TEST:
            _TCF_DBG("[%s][%d] Perform hard-coded \"CFKE_OP_CLRKE_CMD\"\n", __FUNCTION__, __LINE__);
            HAL_CFKE_Write_Command( CFKE_OP_CLRKE_CMD);
            eTransStatus = HAL_CFKE_Get_OperationStatus();
        case E_CFKE_TRANS_STATUS_BUSY_OP:
            u32Retries = 30;
            while( E_CFKE_TRANS_STATUS_BUSY_OP==eTransStatus ){
                MsOS_DelayTask(1);
                eTransStatus = HAL_CFKE_Get_OperationStatus();

                if( 0>=u32Retries-- )
                {
                    _TCF_ERR("[%s][%d] Busy!!\n", __FUNCTION__, __LINE__);
                    u8Ret = DRV_TCF_FAIL;
                    break;
                }
            }
        case E_CFKE_TRANS_STATUS_BUSY_INIT:
            u32Retries = 30;
            while( (CFKE_TRANS_STATUS_VALUES)E_CF_TRANS_STATUS_READY>eTransStatus ){
                MsOS_DelayTask(1);
                eTransStatus = HAL_CFKE_Get_OperationStatus();

                if( 0>=u32Retries-- ){
                    _TCF_ERR("[%s][%d] Invalid state!!\n", __FUNCTION__, __LINE__);
                    u8Ret = DRV_TCF_FAIL;
                    break;
                }
            }
        default:
            break;
    }

    return u8Ret;
}

static void _MDrv_CFKE_WriteSlots(DRV_CFKE_SOURCE *pSrc)
{
    MS_U8    u8i = 0;

    switch(pSrc->eSrcType){
        case E_CFKE_SRC_SLOTA:
            for( u8i=0 ; u8i<CFKE_INPUT_WORDS ; u8i++ ){
                HAL_CFKE_Write_SlotA( pSrc->pu32KeySlotData[u8i]);
            }
            break;
        case E_CFKE_SRC_SLOTB:
            for( u8i=0 ; u8i<CFKE_INPUT_WORDS ; u8i++ ){
                HAL_CFKE_Write_SlotB( pSrc->pu32KeySlotData[u8i]);
            }
            break;
        case E_CFKE_SRC_SLOTC:
            for( u8i=0 ; u8i<CFKE_INPUT_WORDS ; u8i++ ){
                HAL_CFKE_Write_SlotC( pSrc->pu32KeySlotData[u8i]);
            }
            break;
        case E_CFKE_SRC_SLOTD:
            for( u8i=0 ; u8i<CFKE_INPUT_WORDS ; u8i++ ){
                HAL_CFKE_Write_SlotD( pSrc->pu32KeySlotData[u8i]);
            }
            break;
        default:
            break;
    }

}


//--------------------------------------------------------------------------------------------------
//  Global Function
//--------------------------------------------------------------------------------------------------
DRV_TCF_RET SYMBOL_WEAK MDrv_TCF_Init(void)
{
    MS_U32 u32Bank     = 0;
    MS_U32 u32BankSize = 0;

    if( !_bTCF_Init ){
#ifdef TEST_PM_BASE
        if (FALSE == MDrv_MMIO_GetBASE(&u32Bank, &u32BankSize, MS_MODULE_PM)){
#else
        if (FALSE == MDrv_MMIO_GetBASE(&u32Bank, &u32BankSize, MS_MODULE_HW)){
#endif
            _TCF_ERR("[%s][%d] Get MMIO base fail\n", __FUNCTION__, __LINE__);
            return DRV_TCF_FAIL;
        }
        //printf("[%s] >>>>>> u32Bank= 0x%08lx\n", __FUNCTION__, u32Bank);

        HAL_TCF_SetBank( u32Bank);
        HAL_TCF_Clk(TRUE);

        _s32TCFMutexId = MsOS_CreateMutex(E_MSOS_FIFO, (char*)"TCF mutex", MSOS_PROCESS_SHARED);
        _bTCF_Init = TRUE;
        _TCF_DBG("[%s][%d] MDrv_TCF_Init Done!\n", __FUNCTION__, __LINE__);
    }
    else{
        _TCF_DBG("[%s][%d] TCF was already inited!\n", __FUNCTION__, __LINE__);
    }

    return DRV_TCF_OK;
}


// CF
//
DRV_TCF_RET SYMBOL_WEAK MDrv_CF_Version_Info(DRV_CF_VERSION *pstVersion)
{
    if( !_bTCF_Init ){
        _TCF_ERR("[%s][%d] TCF is NOT inited\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }
    MsOS_ObtainMutex(_s32TCFMutexId, MSOS_WAIT_FOREVER);

    HAL_CF_VERSION_INFO *_pstVersion= (HAL_CF_VERSION_INFO *) pstVersion;

    HAL_CF_Version_Info( _pstVersion);

    MsOS_ReleaseMutex(_s32TCFMutexId);
    return DRV_TCF_OK;
}

DRV_TCF_RET SYMBOL_WEAK MDrv_CF_Trans_Status(DRV_CF_TRANS_STATUS *pstTransStatus)
{
    if( !_bTCF_Init ){
        _TCF_ERR("[%s][%d] TCF is NOT inited\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }
    MsOS_ObtainMutex(_s32TCFMutexId, MSOS_WAIT_FOREVER);

    HAL_CF_TRANS_STATUS *_pstTransStatus= (HAL_CF_TRANS_STATUS *) pstTransStatus;

    HAL_CF_Trans_Status( _pstTransStatus);

    MsOS_ReleaseMutex(_s32TCFMutexId);
    return DRV_TCF_OK;
}

DRV_TCF_RET SYMBOL_WEAK MDrv_CF_Cf_Status(DRV_CF_CF_STATUS *pstCfStatus)
{
    if( !_bTCF_Init ){
        _TCF_ERR("[%s][%d] TCF is NOT inited\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }
    MsOS_ObtainMutex(_s32TCFMutexId, MSOS_WAIT_FOREVER);

    HAL_CF_CF_STATUS *_pstCfStatus= (HAL_CF_CF_STATUS *) pstCfStatus;

    HAL_CF_Cf_Status( _pstCfStatus);

    MsOS_ReleaseMutex(_s32TCFMutexId);
    return DRV_TCF_OK;
}

DRV_TCF_RET SYMBOL_WEAK MDrv_CF_FeatureVector(DRV_CF_FEATURE_VECTOR *pstCfFeature)
{
    if( !_bTCF_Init ){
        _TCF_ERR("[%s][%d] TCF is NOT inited\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }
    MsOS_ObtainMutex(_s32TCFMutexId, MSOS_WAIT_FOREVER);

    HAL_CF_FEATURE_VECTOR *_pstCfFeature= (HAL_CF_FEATURE_VECTOR *) pstCfFeature;

    HAL_CF_Cf_FeatureVector( _pstCfFeature);

    MsOS_ReleaseMutex(_s32TCFMutexId);
    return DRV_TCF_OK;
}

DRV_TCF_RET SYMBOL_WEAK MDrv_CF_Issue_Op(DRV_CF_OPERATION *pCfOperations)
{
    CF_TRANS_STATUS_VALUES eTransStatus = E_CF_TRANS_STATUS_BUSY_INIT;
    MS_U32                 u32Cmd       = 0;
    MS_U8                  u8Ret        = DRV_TCF_OK;
    MS_U32                 u32i         = 0;

    if( !_bTCF_Init ){
        _TCF_ERR("[%s][%d] TCF is NOT inited\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }
    MsOS_ObtainMutex(_s32TCFMutexId, MSOS_WAIT_FOREVER);

    eTransStatus = HAL_CF_Get_Trans_Status();
    switch(eTransStatus)
    {
    case E_CF_TRANS_STATUS_BUSY_INIT:
    case E_CF_TRANS_STATUS_BUSY_SETUP:
    case E_CF_TRANS_STATUS_INPUT:         // note: same as vendor
    case E_CF_TRANS_STATUS_BUSY_OP:
    case E_CF_TRANS_STATUS_DONE_OK:
        if( DRV_TCF_OK!=(u8Ret= _MDrv_CF_Ready()) )
        {
            _TCF_ERR("[%s][%d] Set CF to E_CF_TRANS_STATUS_READY fail!!\n", __FUNCTION__, __LINE__);
            goto CF_ISSUE_OP_FINISH;
        }
    case E_CF_TRANS_STATUS_READY:
        for( u32i=0 ; u32i<CF_CMD_WORDS ; u32i++ )
        {
            HAL_CF_Write_Input( pCfOperations->Op[u32i]);
            u32Cmd = HAL_CF_Read_Input();
            if( u32Cmd!=pCfOperations->Op[u32i] )
            {
                _TCF_ERR("[%s][%d] Write cmd fail!!\n", __FUNCTION__, __LINE__);
                u8Ret = DRV_TCF_FAIL;
            }
        }
        break;
    default:
        _TCF_ERR("[%s][%d] CF is busy!!\n", __FUNCTION__, __LINE__);
        u8Ret = DRV_TCF_FAIL;
        break;
    }

CF_ISSUE_OP_FINISH:
    MsOS_ReleaseMutex(_s32TCFMutexId);
    return u8Ret;
}

DRV_TCF_RET SYMBOL_WEAK MDrv_CF_IsFinished(void)
{
    MS_U8 u8Ret = DRV_TCF_OK;

    if( !_bTCF_Init ){
        _TCF_ERR("[%s][%d] TCF is NOT inited\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }
    MsOS_ObtainMutex(_s32TCFMutexId, MSOS_WAIT_FOREVER);

    if( HAL_CF_Get_CF_IsFinished()==TRUE )
    {
        u8Ret = DRV_TCF_OK;
    }
    else
    {
        u8Ret = DRV_TCF_FAIL;
    }

    MsOS_ReleaseMutex(_s32TCFMutexId);
    return u8Ret;
}

DRV_TCF_RET SYMBOL_WEAK MDrv_CF_Read_Op_Result(DRV_CF_RESULT *pCfResult)
{
    MS_U32 u32i  = 0;
    MS_U8  u8Ret = DRV_TCF_OK;

    if( !_bTCF_Init ){
        _TCF_ERR("[%s][%d] TCF is NOT inited\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }
    MsOS_ObtainMutex(_s32TCFMutexId, MSOS_WAIT_FOREVER);

    memset( pCfResult, 0x00, sizeof(DRV_CF_RESULT));
    pCfResult->eTransStatus   = HAL_CF_Get_Trans_Status();
    pCfResult->eOperationType = HAL_CF_Get_Operation_Type();
    pCfResult->u8CwcValid     = HAL_CF_Get_CwcValid();

    if( E_CF_TRANS_STATUS_BUSY_OP < pCfResult->eTransStatus ) // E_CF_TRANS_STATUS_DONE_*
    {
        for( u32i=0 ; u32i<CF_CWC_WORDS ; u32i++ )
        {
            pCfResult->Shv[u32i] = HAL_CF_Read_Output();
            HAL_CF_Write_Output( 0);
        }
    }
    else
    {
        switch(pCfResult->eTransStatus)
        {
        case E_CF_TRANS_STATUS_BUSY_INIT:
        case E_CF_TRANS_STATUS_BUSY_SETUP:
        case E_CF_TRANS_STATUS_READY:
        case E_CF_TRANS_STATUS_INPUT:
            _TCF_ERR("[%s][%d] Invalid state!!\n", __FUNCTION__, __LINE__);
            u8Ret = DRV_TCF_FAIL;
            break;
        default:
            _TCF_ERR("[%s][%d] Timed out!!\n", __FUNCTION__, __LINE__);
            u8Ret = DRV_TCF_FAIL;
            break;
        }
    }

    MsOS_ReleaseMutex(_s32TCFMutexId);
    return u8Ret;
}


// CFKE
//
DRV_TCF_RET SYMBOL_WEAK MDrv_CFKE_Cmd_Exe(DRV_CFKE_OPERATION *pCfkeOp)
{
    CFKE_TRANS_STATUS_VALUES eTransStatus  = E_CFKE_TRANS_STATUS_BUSY_INIT;
    MS_U32                   u32Cmd        = 0;
//    MS_U8                    uDesireStatus = CFKE_STATUS_DONE;
//    MS_U8                    u8i           = 0;
    MS_U8                    u8Ret         = DRV_TCF_OK;


    if( !_bTCF_Init ){
        _TCF_ERR("[%s][%d] TCF is NOT inited\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }
    MsOS_ObtainMutex(_s32TCFMutexId, MSOS_WAIT_FOREVER);


    // check CFKE_STATUS
    eTransStatus = HAL_CFKE_Get_OperationStatus();
    switch(eTransStatus){
        case E_CFKE_TRANS_STATUS_BUSY_INIT:
        case E_CFKE_TRANS_STATUS_DONE_ERROR:
        case E_CFKE_TRANS_STATUS_DONE_TEST:
            if( DRV_TCF_OK!=(u8Ret= _MDrv_CFKE_Ready()) ){
                _TCF_ERR("[%s][%d] Set CF to E_CF_TRANS_STATUS_READY fail!!\n", __FUNCTION__, __LINE__);
                goto CFKE_CMD_EXE_FINISH;
            }
        case E_CFKE_TRANS_STATUS_READY:
        case E_CFKE_TRANS_STATUS_DONE_OK:
            // write the input data based on operation request
            switch(pCfkeOp->eOpType){
                case E_CFKE_OP_DCF:
                    _TCF_DBG("[%s][%d] Direct CF operation.\n", __FUNCTION__, __LINE__);

                    //~ Conditions to check ~//
                    // i)   operation type field is valid
                    // ii)  src1 and src2 should be null. dest should not be null.
                    if( E_CFKE_OP_DCF!=pCfkeOp->eOpType){
                        _TCF_ERR("[%s][%d] <OP_DCF> Operation type requested is not correct\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }
                    if( E_CFKE_SRC_NULL!=pCfkeOp->eOpSrc1.eSrcType || E_CFKE_SRC_NULL!=pCfkeOp->eOpSrc2.eSrcType || E_CFKE_DEST_NULL==pCfkeOp->eOpDest ){
                        _TCF_ERR("[%s][%d] <OP_DCF> SRC1 & SRC2 values should be NULL, DEST should not be NULL.\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }

                    //~ Populate the command data ~//
                    u32Cmd = 0;
                    u32Cmd |= pCfkeOp->eOpType << CFKE_CMD_OP_POS;
                    u32Cmd |= pCfkeOp->eOpDest << CFKE_CMD_DEST_POS;
                    u32Cmd |= pCfkeOp->eOpSrc1.eSrcType << CFKE_CMD_SRC1_POS;
                    u32Cmd |= pCfkeOp->eOpSrc2.eSrcType << CFKE_CMD_SRC2_POS;
                    break;

                case E_CFKE_OP_XOR:
                    _TCF_DBG("[%s][%d] XOR operation.\n", __FUNCTION__, __LINE__);

                    //~ Conditions to check ~//
                    // i)   operation type field is valid
                    // ii)  src1, src2 and dest are not null
                    if( E_CFKE_OP_XOR!=pCfkeOp->eOpType){
                        _TCF_ERR("[%s][%d] <OP_XOR> Operation type requested is not correct\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }
                    if( E_CFKE_SRC_NULL==pCfkeOp->eOpSrc1.eSrcType || E_CFKE_SRC_NULL==pCfkeOp->eOpSrc2.eSrcType || E_CFKE_DEST_NULL==pCfkeOp->eOpDest ){
                        _TCF_ERR("[%s][%d] <OP_XOR> SRC1, SRC2 and DEST values should not be NULL.\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }

                    //~ Write the data to the key registers for both sources ~//
                    // Src1
                    switch(pCfkeOp->eOpSrc1.eSrcType){
                        case E_CFKE_SRC_CF:
                            break;
                        case E_CFKE_SRC_SLOTAL:
                        case E_CFKE_SRC_SLOTBL:
                        case E_CFKE_SRC_SLOTCL:
                        case E_CFKE_SRC_SLOTDL:
                            pCfkeOp->eOpSrc1.eSrcType = (pCfkeOp->eOpSrc1.eSrcType) - (E_CFKE_SRC_SLOTAL-E_CFKE_SRC_SLOTA);
                            _MDrv_CFKE_WriteSlots( (DRV_CFKE_SOURCE*)&pCfkeOp->eOpSrc1);
                            break;
                        case E_CFKE_SRC_SLOTA:
                        case E_CFKE_SRC_SLOTB:
                        case E_CFKE_SRC_SLOTC:
                        case E_CFKE_SRC_SLOTD:
                            // Don't load the key slots. Use previously loaded values as sources.
                            break;
                        default:
                            _TCF_ERR("[%s][%d] <OP_XOR> Invalid Src1 type requested!\n", __FUNCTION__, __LINE__);
                            u8Ret = DRV_TCF_INVALID_INPUT;
                        	break;
                    }

                    // Do the same for Src2
                    switch(pCfkeOp->eOpSrc2.eSrcType){
                        case E_CFKE_SRC_CF:
                            break;
                        case E_CFKE_SRC_SLOTAL:
                        case E_CFKE_SRC_SLOTBL:
                        case E_CFKE_SRC_SLOTCL:
                        case E_CFKE_SRC_SLOTDL:
                            pCfkeOp->eOpSrc2.eSrcType = (pCfkeOp->eOpSrc2.eSrcType) - (E_CFKE_SRC_SLOTAL-E_CFKE_SRC_SLOTA);
                            _MDrv_CFKE_WriteSlots( (DRV_CFKE_SOURCE*)&pCfkeOp->eOpSrc2);
                            break;
                        case E_CFKE_SRC_SLOTA:
                        case E_CFKE_SRC_SLOTB:
                        case E_CFKE_SRC_SLOTC:
                        case E_CFKE_SRC_SLOTD:
                            // Don't load the key slots. Use previously loaded values as sources.
                            break;
                        default:
                            _TCF_ERR("[%s][%d] <OP_XOR> Invalid Src2 type requested!\n", __FUNCTION__, __LINE__);
                            u8Ret = DRV_TCF_INVALID_INPUT;
                        	break;
                    }

                    //~ Populate the command data ~//
                    u32Cmd = 0;
                    u32Cmd |= pCfkeOp->eOpType << CFKE_CMD_OP_POS;
                    u32Cmd |= pCfkeOp->eOpDest << CFKE_CMD_DEST_POS;
                    u32Cmd |= pCfkeOp->eOpSrc1.eSrcType << CFKE_CMD_SRC1_POS;
                    u32Cmd |= pCfkeOp->eOpSrc2.eSrcType << CFKE_CMD_SRC2_POS;
                    break;

                case E_CFKE_OP_AES:
                    _TCF_DBG("[%s][%d] AES operation.\n", __FUNCTION__, __LINE__);

                    //~ Conditions to check ~//
                    // i)   operation type field is valid
                    // ii)  src1, src2 and dest are not null
                    if( E_CFKE_OP_AES!=pCfkeOp->eOpType){
                        _TCF_ERR("[%s][%d] <OP_AES> Operation type requested is not correct\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }
                    if( E_CFKE_SRC_NULL==pCfkeOp->eOpSrc1.eSrcType || E_CFKE_SRC_NULL==pCfkeOp->eOpSrc2.eSrcType || E_CFKE_DEST_NULL==pCfkeOp->eOpDest ){
                        _TCF_ERR("[%s][%d] <OP_AES> SRC1, SRC2 and DEST values should not be NULL.\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }

                    //~ Write the data to the key registers for both sources ~//
                    // Src1
                    switch(pCfkeOp->eOpSrc1.eSrcType){
                        case E_CFKE_SRC_CF:
                            break;
                        case E_CFKE_SRC_SLOTAL:
                        case E_CFKE_SRC_SLOTBL:
                        case E_CFKE_SRC_SLOTCL:
                        case E_CFKE_SRC_SLOTDL:
                            pCfkeOp->eOpSrc1.eSrcType = (pCfkeOp->eOpSrc1.eSrcType) - (E_CFKE_SRC_SLOTAL-E_CFKE_SRC_SLOTA);
                            _MDrv_CFKE_WriteSlots( (DRV_CFKE_SOURCE*)&pCfkeOp->eOpSrc1);
                            break;
                        case E_CFKE_SRC_SLOTA:
                        case E_CFKE_SRC_SLOTB:
                        case E_CFKE_SRC_SLOTC:
                        case E_CFKE_SRC_SLOTD:
                            // Don't load the key slots. Use previously loaded values as sources.
                            break;
                        default:
                            _TCF_ERR("[%s][%d] <OP_AES> Invalid Src1 type requested!\n", __FUNCTION__, __LINE__);
                            u8Ret = DRV_TCF_INVALID_INPUT;
                        	break;
                    }

                    // Do the same for Src2
                    switch(pCfkeOp->eOpSrc2.eSrcType){
                        case E_CFKE_SRC_CF:
                            break;
                        case E_CFKE_SRC_SLOTAL:
                        case E_CFKE_SRC_SLOTBL:
                        case E_CFKE_SRC_SLOTCL:
                        case E_CFKE_SRC_SLOTDL:
                            pCfkeOp->eOpSrc2.eSrcType = (pCfkeOp->eOpSrc2.eSrcType) - (E_CFKE_SRC_SLOTAL-E_CFKE_SRC_SLOTA);
                            _MDrv_CFKE_WriteSlots( (DRV_CFKE_SOURCE*)&pCfkeOp->eOpSrc2);
                            break;
                        case E_CFKE_SRC_SLOTA:
                        case E_CFKE_SRC_SLOTB:
                        case E_CFKE_SRC_SLOTC:
                        case E_CFKE_SRC_SLOTD:
                            // Don't load the key slots. Use previously loaded values as sources.
                            break;
                        default:
                            _TCF_ERR("[%s][%d] <OP_AES> Invalid Src2 type requested!\n", __FUNCTION__, __LINE__);
                            u8Ret = DRV_TCF_INVALID_INPUT;
                        	break;
                    }

                    //~ Populate the command data ~//
                    u32Cmd = 0;
                    u32Cmd |= pCfkeOp->eOpType << CFKE_CMD_OP_POS;
                    u32Cmd |= pCfkeOp->eOpDest << CFKE_CMD_DEST_POS;
                    u32Cmd |= pCfkeOp->eOpSrc1.eSrcType << CFKE_CMD_SRC1_POS;
                    u32Cmd |= pCfkeOp->eOpSrc2.eSrcType << CFKE_CMD_SRC2_POS;
                    break;

                case E_CFKE_OP_UNW:
                    _TCF_DBG("[%s][%d] UNW operation.\n", __FUNCTION__, __LINE__);

                    //~ Conditions to check ~//
                    // i)   operation type field is valid
                    // ii)  src1, src2 and dest are not null
                    if( E_CFKE_OP_UNW!=pCfkeOp->eOpType){
                        _TCF_ERR("[%s][%d] <OP_UNW> Operation type requested is not correct\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }
                    if( E_CFKE_SRC_NULL==pCfkeOp->eOpSrc1.eSrcType || E_CFKE_SRC_NULL==pCfkeOp->eOpSrc2.eSrcType || E_CFKE_DEST_NULL==pCfkeOp->eOpDest ){
                        _TCF_ERR("[%s][%d] <OP_UNW> SRC1, SRC2 and DEST values should not be NULL.\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }

                    //~ Write the data to the key registers for both sources ~//
                    // Src1
                    switch(pCfkeOp->eOpSrc1.eSrcType){
                        case E_CFKE_SRC_CF:
                            break;
                        case E_CFKE_SRC_SLOTAL:
                        case E_CFKE_SRC_SLOTBL:
                        case E_CFKE_SRC_SLOTCL:
                        case E_CFKE_SRC_SLOTDL:
                            pCfkeOp->eOpSrc1.eSrcType = (pCfkeOp->eOpSrc1.eSrcType) - (E_CFKE_SRC_SLOTAL-E_CFKE_SRC_SLOTA);
                            _MDrv_CFKE_WriteSlots( (DRV_CFKE_SOURCE*)&pCfkeOp->eOpSrc1);
                            break;
                        case E_CFKE_SRC_SLOTA:
                        case E_CFKE_SRC_SLOTB:
                        case E_CFKE_SRC_SLOTC:
                        case E_CFKE_SRC_SLOTD:
                            // Don't load the key slots. Use previously loaded values as sources.
                            break;
                        default:
                            _TCF_ERR("[%s][%d] <OP_UNW> Invalid Src1 type requested!\n", __FUNCTION__, __LINE__);
                            u8Ret = DRV_TCF_INVALID_INPUT;
                        	break;
                    }

                    // Do the same for Src2
                    switch(pCfkeOp->eOpSrc2.eSrcType){
                        case E_CFKE_SRC_CF:
                            break;
                        case E_CFKE_SRC_SLOTAL:
                        case E_CFKE_SRC_SLOTBL:
                        case E_CFKE_SRC_SLOTCL:
                        case E_CFKE_SRC_SLOTDL:
                            pCfkeOp->eOpSrc2.eSrcType = (pCfkeOp->eOpSrc2.eSrcType) - (E_CFKE_SRC_SLOTAL-E_CFKE_SRC_SLOTA);
                            _MDrv_CFKE_WriteSlots( (DRV_CFKE_SOURCE*)&pCfkeOp->eOpSrc2);
                            break;
                        case E_CFKE_SRC_SLOTA:
                        case E_CFKE_SRC_SLOTB:
                        case E_CFKE_SRC_SLOTC:
                        case E_CFKE_SRC_SLOTD:
                            // Don't load the key slots. Use previously loaded values as sources.
                            break;
                        default:
                            _TCF_ERR("[%s][%d] <OP_UNW> Invalid Src2 type requested!\n", __FUNCTION__, __LINE__);
                            u8Ret = DRV_TCF_INVALID_INPUT;
                        	break;
                    }

                    //~ Write unwrap data to the registers ~//
                    // (unwrap data) Write the value to the unwrap path register
                    HAL_CFKE_Write_UnwPath( pCfkeOp->UnwData.u32UnwPathData);
                    // Read back the data to verify whether the data is written successfully
                    if( pCfkeOp->UnwData.u32UnwPathData!=HAL_CFKE_Read_UnwPath() ){
                        _TCF_ERR("[%s][%d] <OP_UNW> CFKE Write UNW Path Register Error!!\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_FAIL;
                        goto CFKE_CMD_EXE_FINISH;
                    }

                    // Write the value [31:0] to the validator register
                    HAL_CFKE_Write_Validator0( pCfkeOp->UnwData.pu832ValidatorData[0]);
                    if( pCfkeOp->UnwData.pu832ValidatorData[0]!=HAL_CFKE_Read_Validator0() ){
                        _TCF_ERR("[%s][%d] <OP_UNW> CFKE Write Validator 0 Register Error!!\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_FAIL;
                        goto CFKE_CMD_EXE_FINISH;
                    }

                    // Write the value [63:32]to the validator1 register
                    HAL_CFKE_Write_Validator1( pCfkeOp->UnwData.pu832ValidatorData[1]);
                    if( pCfkeOp->UnwData.pu832ValidatorData[1]!=HAL_CFKE_Read_Validator1() ){
                        _TCF_ERR("[%s][%d] <OP_UNW> CFKE Write Validator 1 Register Error!!\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_FAIL;
                        goto CFKE_CMD_EXE_FINISH;
                    }

                    //~ Populate the command data ~//
                    u32Cmd = 0;
                    u32Cmd |= pCfkeOp->eOpType << CFKE_CMD_OP_POS;
                    u32Cmd |= pCfkeOp->eOpDest << CFKE_CMD_DEST_POS;
                    u32Cmd |= pCfkeOp->eOpSrc1.eSrcType << CFKE_CMD_SRC1_POS;
                    u32Cmd |= pCfkeOp->eOpSrc2.eSrcType << CFKE_CMD_SRC2_POS;
                    break;

                case E_CFKE_OP_HWDECM:
                    _TCF_DBG("[%s][%d] HWDECM operation.\n", __FUNCTION__, __LINE__);

                    //~ Conditions to check ~//
                    // i)   operation type field is valid
                    // ii)  src1 should not be null. dest and src2 must be null
                    if( E_CFKE_OP_HWDECM!=pCfkeOp->eOpType){
                        _TCF_ERR("[%s][%d] <OP_HWDECM> Operation type requested is not correct\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }
                    if( E_CFKE_SRC_NULL==pCfkeOp->eOpSrc1.eSrcType || E_CFKE_SRC_NULL!=pCfkeOp->eOpSrc2.eSrcType || E_CFKE_DEST_NULL!=pCfkeOp->eOpDest ){
                        _TCF_ERR("[%s][%d] <OP_HWDECM> SRC1/SRC2/Dest values are not valid.\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }

                    // Write the data to the key registers for src1
                    switch(pCfkeOp->eOpSrc1.eSrcType){
                        case E_CFKE_SRC_CF:
                            break;
                        case E_CFKE_SRC_SLOTAL:
                        case E_CFKE_SRC_SLOTBL:
                        case E_CFKE_SRC_SLOTCL:
                        case E_CFKE_SRC_SLOTDL:
                            pCfkeOp->eOpSrc1.eSrcType = (pCfkeOp->eOpSrc1.eSrcType) - (E_CFKE_SRC_SLOTAL-E_CFKE_SRC_SLOTA);
                            _MDrv_CFKE_WriteSlots( (DRV_CFKE_SOURCE*)&pCfkeOp->eOpSrc1);
                            break;
                        case E_CFKE_SRC_SLOTA:
                        case E_CFKE_SRC_SLOTB:
                        case E_CFKE_SRC_SLOTC:
                        case E_CFKE_SRC_SLOTD:
                            // Don't load the key slots. Use previously loaded values as sources.
                            break;
                        default:
                            _TCF_ERR("[%s][%d] <OP_HWDECM> Invalid Src1 type requested!\n", __FUNCTION__, __LINE__);
                            u8Ret = DRV_TCF_INVALID_INPUT;
                        	break;
                    }

                    //~ Populate the command data ~//
                    u32Cmd = 0;
                    u32Cmd |= pCfkeOp->eOpType << CFKE_CMD_OP_POS;
                    u32Cmd |= pCfkeOp->eOpDest << CFKE_CMD_DEST_POS;
                    u32Cmd |= pCfkeOp->eOpSrc1.eSrcType << CFKE_CMD_SRC1_POS;
                    u32Cmd |= pCfkeOp->eOpSrc2.eSrcType << CFKE_CMD_SRC2_POS;
                    break;

                case E_CFKE_OP_TEST:
                    _TCF_DBG("[%s][%d] TEST operation.\n", __FUNCTION__, __LINE__);

                    //~ Conditions to check ~//
                    // i)   operation type field is valid
                    // ii)  src1 src2 and dest must be null
                    if( E_CFKE_OP_TEST!=pCfkeOp->eOpType){
                        _TCF_ERR("[%s][%d] <OP_TEST> Operation type requested is not correct\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }
                    if( E_CFKE_SRC_NULL!=pCfkeOp->eOpSrc1.eSrcType || E_CFKE_SRC_NULL!=pCfkeOp->eOpSrc2.eSrcType || E_CFKE_DEST_NULL!=pCfkeOp->eOpDest ){
                        _TCF_ERR("[%s][%d] <OP_TEST> SRC1/SRC2/Dest values should be NULL.\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }

                    //~ Populate the command data ~//
                    u32Cmd = 0;
                    u32Cmd |= pCfkeOp->eOpType << CFKE_CMD_OP_POS;
                    u32Cmd |= pCfkeOp->eOpDest << CFKE_CMD_DEST_POS;
                    u32Cmd |= pCfkeOp->eOpSrc1.eSrcType << CFKE_CMD_SRC1_POS;
                    u32Cmd |= pCfkeOp->eOpSrc2.eSrcType << CFKE_CMD_SRC2_POS;
                    break;

                case E_CFKE_OP_CLRKE:
                    _TCF_DBG("[%s][%d] CLRKE operation.\n", __FUNCTION__, __LINE__);

                    //~ Conditions to check ~//
                    // i)   operation type field is valid
                    // ii)  src1 src2 and dest are ignored
                    if( E_CFKE_OP_CLRKE!=pCfkeOp->eOpType){
                        _TCF_ERR("[%s][%d] <OP_CLRKE> Operation type requested is not correct\n", __FUNCTION__, __LINE__);
                        u8Ret = DRV_TCF_INVALID_INPUT;
                        goto CFKE_CMD_EXE_FINISH;
                    }

                    //~ Populate the command data ~//
                    u32Cmd = 0;
                    u32Cmd |= pCfkeOp->eOpType << CFKE_CMD_OP_POS;
                    u32Cmd |= pCfkeOp->eOpDest << CFKE_CMD_DEST_POS;
                    break;

                default: // default of 'switch(pCfkeOp->eOpType)'
                    _TCF_ERR("[%s][%d] Invalid operation type requested!!\n", __FUNCTION__, __LINE__);
                    u8Ret = DRV_TCF_INVALID_INPUT;
                	break;
            }// end of 'switch(pCfkeOp->eOpType)'


            // Write the command data to the command register, Read back the data to verify whether the command is written successfully
            HAL_CFKE_Write_Command( u32Cmd);
            if( u32Cmd!=HAL_CFKE_Read_Command() ){
                _TCF_ERR("[%s][%d] CFKE Write Command Error!!\n", __FUNCTION__, __LINE__);
                u8Ret = DRV_TCF_FAIL;
                goto CFKE_CMD_EXE_FINISH;
            }
            _TCF_DBG("[%s][%d] CFKE Write Command OK-- 0x%08lx\n\n", __FUNCTION__, __LINE__, u32Cmd);
            break;

        default: // default of 'switch(eTransStatus)'
            _TCF_ERR("[%s][%d] Busy..\n", __FUNCTION__, __LINE__);
           	u8Ret = DRV_TCF_BUSY;
           	break;
    }// end of 'switch(eTransStatus)'


CFKE_CMD_EXE_FINISH:
    MsOS_ReleaseMutex(_s32TCFMutexId);
    return u8Ret;
}

DRV_TCF_RET SYMBOL_WEAK MDrv_CFKE_IsDone(CFKE_ERROR_CODE *eCfkeErr)
{
    CFKE_TRANS_STATUS_VALUES eTransStatus  = E_CFKE_TRANS_STATUS_BUSY_INIT;
    MS_U8                    u8Ret         = DRV_TCF_OK;

    if( !_bTCF_Init ){
        _TCF_ERR("[%s][%d] TCF is NOT inited\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }
    MsOS_ObtainMutex(_s32TCFMutexId, MSOS_WAIT_FOREVER);

    if( CFKE_STATUS_DONE&(eTransStatus= HAL_CFKE_Get_OperationStatus()) ){
        *eCfkeErr = HAL_CFKE_Get_ErrorCode();
        if( *eCfkeErr ){
            _TCF_ERR("[%s] CFKE Error!! (ErrorCode= 0x%x)\n", __FUNCTION__, *eCfkeErr);
        }
        u8Ret = DRV_TCF_OK;
    }
    else{
        u8Ret = DRV_TCF_FAIL;
    }

    MsOS_ReleaseMutex(_s32TCFMutexId);
    return u8Ret;
}

DRV_TCF_RET SYMBOL_WEAK MDrv_CFKE_IsReady(void)
{
    MS_U8 u8Ret = DRV_TCF_OK;

    if( !_bTCF_Init ){
        _TCF_ERR("[%s][%d] TCF is NOT inited\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }
    MsOS_ObtainMutex(_s32TCFMutexId, MSOS_WAIT_FOREVER);

    if( E_CFKE_TRANS_STATUS_READY==HAL_CFKE_Get_OperationStatus() ){
        u8Ret = DRV_TCF_OK;
    }
    else{
        u8Ret = DRV_TCF_FAIL;
    }

    MsOS_ReleaseMutex(_s32TCFMutexId);
    return u8Ret;
}


DRV_TCF_RET SYMBOL_WEAK MDrv_CFKE_UserHash_Setup(DRV_CFKE_USER_HASH_DATA *pUserHash)
{
    MS_U8    u8i = 0;

    if( !_bTCF_Init ){
        _TCF_ERR("[%s][%d] TCF is NOT inited\n", __FUNCTION__, __LINE__);
        return DRV_TCF_FAIL;
    }
    MsOS_ObtainMutex(_s32TCFMutexId, MSOS_WAIT_FOREVER);

    // Write the value to the user hash register
    for( u8i=0 ; u8i<CFKE_INPUT_WORDS ; u8i++ ){
        HAL_CFKE_Write_UserHash( pUserHash->pu32UsrHashData[u8i]);
    }

    MsOS_ReleaseMutex(_s32TCFMutexId);

    return DRV_TCF_OK;
}


// CRIKL
//
MS_U32 SYMBOL_WEAK MDrv_CRIKL_Mirror(CRIKL_MIRROR_MEM eCandidate)
{
    MS_U32  u32Value = 0;

    switch(eCandidate){
        case E_CRIKL_MIRROR_CF_STATUS:
            u32Value = HAL_CRIKL_Read_CfStatus();
            break;
        case E_CRIKL_MIRROR_CFKE_STATUS:
            u32Value = HAL_CRIKL_Read_CfkeStatus();
            break;
        case E_CRIKL_MIRROR_CF_PLATFORM:
            u32Value = HAL_CRIKL_Read_CfPlatform();
            break;
        case E_CRIKL_MIRROR_CF_FEATURE:
            u32Value = HAL_CRIKL_Read_CfFeature();
            break;
        default:
            _TCF_ERR("[%s][%d] Invalid Mirror Member\n", __FUNCTION__, __LINE__);
            break;
    }

    return u32Value;
}

DRV_TCF_RET SYMBOL_WEAK MDrv_CRIKL_Set_KeyConfig(DRV_CRIKL_KEY_CONFIG *stKeyCfg)
{
    MS_BOOL    _bDscmbKey = stKeyCfg->bDscmbKey;


    /// kt_index | ks_index
    if(_bDscmbKey){
        HAL_CRIKL_Set_KTIndex( stKeyCfg->eScmbCtl, stKeyCfg->u8DscmbId);
    }
    else{
        HAL_CRIKL_Set_KSIndex( stKeyCfg->u8KeySlotIdx);
    }

    /// kt_para | ks_para
    HAL_CRIKL_Reset_KeyPara();

    // High_HalfWord
    HAL_CRIKL_Setup_VendorNo( _bDscmbKey, stKeyCfg->u8VendorNo);
    HAL_CRIKL_Setup_VendorAtt( _bDscmbKey, stKeyCfg->u16VendorAtt);

    // 2nd_HalfWord
    HAL_CRIKL_Setup_OutputAllowed( _bDscmbKey, stKeyCfg->u8OutputAllowed);
    HAL_CRIKL_Setup_bMultiEng( _bDscmbKey, stKeyCfg->bMultiEng);
    HAL_CRIKL_Setup_bLsb64( _bDscmbKey, stKeyCfg->bLsb64);
    HAL_CRIKL_Setup_KeySize( _bDscmbKey, stKeyCfg->eKeySize);

    // 3rd_HalfWord
    HAL_CRIKL_Setup_KeyEngine( _bDscmbKey, stKeyCfg->u8KlEng);
    HAL_CRIKL_Setup_Drop( _bDscmbKey, stKeyCfg->u8Drop);
    HAL_CRIKL_Setup_Cnt( _bDscmbKey, stKeyCfg->u8Cnt);

    // Low_HalfWord
    HAL_CRIKL_Setup_KeyDest( _bDscmbKey, stKeyCfg->u8KlDest);
    HAL_CRIKL_Setup_OpType( _bDscmbKey, stKeyCfg->eOpSel);
    if(_bDscmbKey){
        HAL_CRIKL_Setup_KteSel( _bDscmbKey, stKeyCfg->eKteSel);
    }
    HAL_CRIKL_Setup_bLock( _bDscmbKey, stKeyCfg->bLock);
    HAL_CRIKL_Setup_bParaValid( _bDscmbKey, stKeyCfg->bParaVld);
    HAL_CRIKL_Setup_bKlEn( _bDscmbKey, stKeyCfg->bKlEn);

    HAL_CRIKL_Write_KeyPara( _bDscmbKey);

    return DRV_TCF_OK;
}

