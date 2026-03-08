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
/// file    halMBXINT.c
/// @brief  MStar MailBox interrupt DDI
///////////////////////////////////////////////////////////////////////////////////////////////////

#define _MHAL_MBX_INTERRUPT_C

//=============================================================================
// Include Files
//=============================================================================
#include "MsCommon.h"
#include "drvMBX.h"
#include "regMBXINT.h"
#include "halMBXINT.h"

//=============================================================================
// Compile options
//=============================================================================


//=============================================================================
// Local Defines
//=============================================================================

//=============================================================================
// Debug Macros
//=============================================================================
//#define MBXINT_DEBUG
#ifdef MBXINT_DEBUG
    #define MBXINT_ERROR(fmt, args...)           printf("[MBX INT Driver USER ERR][%06d]     " fmt, __LINE__, ## args)
    #define MBXINT_WARN(fmt, args...)            printf("[MBX INT Driver WARN][%06d]    " fmt, __LINE__, ## args)
    #define MBXINT_PRINT(fmt, args...)           printf("[MBX INT Driver][%06d]     " fmt, __LINE__, ## args)
    #define MBXINT_ASSERT(_cnd, _fmt, _args...)    \
                                    if (!(_cnd)) {              \
                                        MBXINT_PRINT(_fmt, ##_args);  \
                                    }
#else
    #define MBXINT_ERROR(fmt, args...)
    #define MBXINT_WARN(fmt, args...)
    #define MBXINT_PRINT(fmt, args...)
    #define MBXINT_ASSERT(_cnd, _fmt, _args...)
#endif

//=============================================================================
// Macros
//=============================================================================

//=============================================================================
// Local Variables
//=============================================================================
static MBX_MSGRECV_CB_FUNC _pMBXMsgRecvCbFunc = NULL;
static MS_VIRT _virtRIUBaseAddrMBXINT = 0;

//=============================================================================
// Global Variables
//=============================================================================

//=============================================================================
// Local Function Prototypes
//=============================================================================
static void _MHAL_MBXINT_INTHandler(InterruptNum vector);
static MBX_Result _MHAL_MBXINT_SetHostCPU(MBX_CPU_ID eHostCPUID);

//=============================================================================
// Local Function
//=============================================================================

//-------------------------------------------------------------------------------------------------
/// Handle Interrupt, schedule tasklet
/// @param  irq                  \b IN: interrupt number
/// @param  dev_id                  \b IN: dev id
/// @return irqreturn_t: IRQ_HANDLED
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
void _MHAL_MBXINT_INTHandler(InterruptNum eIntNum)
{
    if(NULL == _pMBXMsgRecvCbFunc)
    {
        return;
    }

    _pMBXMsgRecvCbFunc(eIntNum);

    MsOS_EnableInterrupt(eIntNum);
}

//-------------------------------------------------------------------------------------------------
/// Set Interrupt to Host CPU ID: Enable related interrupt and attached related callback.
/// @param  eHostCPUID                  \b IN: The Host CPU ID
/// @return E_MBX_SUCCESS
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result _MHAL_MBXINT_SetHostCPU(MBX_CPU_ID eHostCPUID)
{
  #if defined(CONFIG_FRC)//frcr2_integration###
    switch(eHostCPUID)
    {
        case E_MBX_CPU_R2FRC:
            MsOS_AttachInterrupt(E_FRCINT_FIQ_HST0_TO_HST1, (InterruptCb)_MHAL_MBXINT_INTHandler);
            MsOS_EnableInterrupt(E_FRCINT_FIQ_HST0_TO_HST1); //enable host0(HKCPU/Non-PM Intr) to host1(frc-r2)
            MsOS_EnableInterrupt(E_FRCINT_FIQ_HST1_TO_HST0); //enable host1(frc-r2) to host0(HKCPU/Non-PM Intr)
            break;
           
        default:
            return E_MBX_ERR_INVALID_CPU_ID;
    }
  #else
    switch(eHostCPUID)
    {
        case E_MBX_CPU_PM:
            MsOS_AttachInterrupt(E_INT_FIQ_AEON_TO_8051, (InterruptCb)_MHAL_MBXINT_INTHandler);
            MsOS_AttachInterrupt(E_INT_FIQ_BEON_TO_8051, (InterruptCb)_MHAL_MBXINT_INTHandler);
            MsOS_EnableInterrupt(E_INT_FIQ_AEON_TO_8051);
            MsOS_EnableInterrupt(E_INT_FIQ_BEON_TO_8051);
            break;
        case E_MBX_CPU_AEON:
            //MsOS_AttachInterrupt(E_INT_FIQ_8051_TO_AEON, (InterruptCb)_MHAL_MBXINT_INTHandler);
            MsOS_AttachInterrupt(E_INT_FIQ_BEON_TO_AEON, (InterruptCb)_MHAL_MBXINT_INTHandler);
            //MsOS_EnableInterrupt(E_INT_FIQ_8051_TO_AEON);
            MsOS_EnableInterrupt(E_INT_FIQ_BEON_TO_AEON);
            break;
        case E_MBX_CPU_MIPS:
            MsOS_AttachInterrupt(E_INT_FIQ_8051_TO_BEON, (InterruptCb)_MHAL_MBXINT_INTHandler);
            MsOS_AttachInterrupt(E_INT_FIQ_AEON_TO_BEON, (InterruptCb)_MHAL_MBXINT_INTHandler);
            MsOS_AttachInterrupt(E_INT_IRQ_FRC_INT_FIQ2HST0, (InterruptCb)_MHAL_MBXINT_INTHandler);//frcr2_integration###

            MsOS_EnableInterrupt(E_INT_FIQ_8051_TO_BEON);
            MsOS_EnableInterrupt(E_INT_FIQ_AEON_TO_BEON);
            MsOS_EnableInterrupt(E_INT_IRQ_FRC_INT_FIQ2HST0);//frcr2_integration###
            break;
           
        default:
            return E_MBX_ERR_INVALID_CPU_ID;
    }
#endif
    return E_MBX_SUCCESS;
}

//=============================================================================
// Mailbox HAL Interrupt Driver Function
//=============================================================================

//-------------------------------------------------------------------------------------------------
/// Handle Interrupt INIT
/// @param  eHostCPU                  \b IN: interrupt owner
/// @param  pMBXRecvMsgCBFunc                  \b IN: callback func by driver
/// @param  u32RIUBaseAddrMBXINT                  \b IN: RIU Base Addr with platform
/// @return E_MBX_ERR_INVALID_CPU_ID: the cpu id is wrong
/// @return E_MBX_UNKNOW_ERROR: request_irq failed;
/// @return E_MBX_SUCCESS: success;
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result MHAL_MBXINT_Init (MBX_CPU_ID eHostCPU, MBX_MSGRECV_CB_FUNC pMBXRecvMsgCBFunc, MS_VIRT virtRIUBaseAddrMBXINT)
{
    _pMBXMsgRecvCbFunc = pMBXRecvMsgCBFunc;
    _virtRIUBaseAddrMBXINT = virtRIUBaseAddrMBXINT;

    return _MHAL_MBXINT_SetHostCPU(eHostCPU);
}

//-------------------------------------------------------------------------------------------------
/// Handle Interrupt DeINIT
/// @param  eHostCPU                  \b IN: interrupt owner
/// @return void;
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
void MHAL_MBXINT_DeInit (MBX_CPU_ID eHostCPU)
{
  #if defined(CONFIG_FRC)//frcr2_integration###
      switch(eHostCPU)
      {
          case E_MBX_CPU_R2FRC:
              MsOS_DisableInterrupt(E_FRCINT_FIQ_HST1_TO_HST0); //disable host1(frc-r2) to host0(HKCPU/Non-PM Intr)
              MsOS_DisableInterrupt(E_FRCINT_FIQ_HST0_TO_HST1); //disable host0(HKCPU/Non-PM Intr) to host1(frc-r2)
              MsOS_DetachInterrupt(E_FRCINT_FIQ_HST0_TO_HST1);
              break;
             
          default:
              break;
      }
  #else
    switch(eHostCPU)
    {
        case E_MBX_CPU_PM:
            MsOS_DisableInterrupt(E_INT_FIQ_AEON_TO_8051);
            MsOS_DisableInterrupt(E_INT_FIQ_BEON_TO_8051);
            MsOS_DetachInterrupt(E_INT_FIQ_AEON_TO_8051);
            MsOS_DetachInterrupt(E_INT_FIQ_BEON_TO_8051);
            break;
        case E_MBX_CPU_AEON:
            //MsOS_DisableInterrupt(E_INT_FIQ_8051_TO_AEON);
            MsOS_DisableInterrupt(E_INT_FIQ_BEON_TO_AEON);
            //MsOS_DetachInterrupt(E_INT_FIQ_8051_TO_AEON);
            MsOS_DetachInterrupt(E_INT_FIQ_BEON_TO_AEON);
            break;
        case E_MBX_CPU_MIPS:
            MsOS_DisableInterrupt(E_INT_FIQ_8051_TO_BEON);
            MsOS_DisableInterrupt(E_INT_FIQ_AEON_TO_BEON);
            MsOS_DisableInterrupt(E_INT_IRQ_FRC_INT_FIQ2HST0);//frcr2_integration###
            MsOS_DetachInterrupt(E_INT_FIQ_8051_TO_BEON);
            MsOS_DetachInterrupt(E_INT_FIQ_AEON_TO_BEON);
            MsOS_DetachInterrupt(E_INT_IRQ_FRC_INT_FIQ2HST0);//frcr2_integration###
            break;
        default:
            break;
    }
#endif
}

//-------------------------------------------------------------------------------------------------
/// Reset Host CPU for MBX Interrupt
/// @param  ePrevCPU                  \b IN: previous host cpu id
/// @param  eConfigCpu                  \b IN: new configed cpu id
/// @return E_MBX_SUCCESS: success;
/// @return E_MBX_INVALID_CPU_ID
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result MHAL_MBXINT_ResetHostCPU (MBX_CPU_ID ePrevCPU, MBX_CPU_ID eConfigCpu)
{
    MHAL_MBXINT_DeInit(ePrevCPU);

    return _MHAL_MBXINT_SetHostCPU(eConfigCpu);
}

//-------------------------------------------------------------------------------------------------
/// Fire Interrupt
/// @param  dstCPUID                  \b IN: dst cpu of interrupt
/// @param  srcCPUID                  \b IN: src cpu of interrupt
/// @return E_MBX_SUCCESS: success;
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result MHAL_MBXINT_Fire (MBX_CPU_ID dstCPUID, MBX_CPU_ID srcCPUID)
{
    MBXINT_ASSERT((dstCPUID!=srcCPUID),"dst cpu is the same as src cpu!\n");

#if defined(MSOS_TYPE_NUTTX)
	srcCPUID = E_MBX_CPU_AEON;
	dstCPUID = E_MBX_CPU_MIPS; 


	CPU_INT_REG(REG_INT_AEONFIRE) |= INT_AEON_H1;
	CPU_INT_REG(REG_INT_AEONFIRE) &= ~INT_AEON_H1;
	return  E_MBX_SUCCESS; 
#endif

#if defined(CONFIG_FRC)//frcr2_integration###
    switch(srcCPUID)
    {
        case E_MBX_CPU_R2FRC:
            if(dstCPUID==E_MBX_CPU_MIPS)
            {
                FRC_CPU_INT_REG(REG_FRCINT_FRCR2FIRE) |= INT_FRCR2_HKCPU;
                FRC_CPU_INT_REG(REG_FRCINT_FRCR2FIRE) &= ~(INT_FRCR2_HKCPU);
                MBXINT_PRINT("FIA [0-15]M = 0x%x\n", FIQ_FRCREG(REG_FRCFIQ_H0_0_15));
                MBXINT_PRINT("FIQ [0-15]S = 0x%x\n", FIQ_FRCREG(REG_FRCFIQS_H0_0_15));
            }
            
            break;
        default:
            MBXINT_ASSERT(FALSE,"wrong src cpu!\n");
            break;
    }
	return  E_MBX_SUCCESS; 

#endif

    switch(srcCPUID)
    {
        case E_MBX_CPU_PM:
            if(dstCPUID==E_MBX_CPU_AEON)
            {
                //FIQ_REG(REG_FIQ_H1_32_47) &= ~(INT_FIQMASK_PM_AEON);
                CPU_INT_REG(REG_INT_PMFIRE) |= INT_PM_AEON;
                //MBXINT_PRINT("[RIU ADDR] = 0x%x\n", FIQ_REG(REG_FIQ_H1_32_47));
                CPU_INT_REG(REG_INT_PMFIRE) &= ~(INT_PM_AEON);
                MBXINT_PRINT("FIA [32-47]M = 0x%x\n", FIQ_REG(REG_FIQ_H1_32_47));
                MBXINT_PRINT("FIQ [32-47]S = 0x%x\n", FIQ_REG(REG_FIQS_H1_32_47));
            }
            else
            {   // PM 2 MIPS
                //FIQ_REG(REG_FIQ_H1_32_47) &= ~(INT_FIQMASK_PM_H3);
                CPU_INT_REG(REG_INT_PMFIRE) |= INT_PM_H1;
                CPU_INT_REG(REG_INT_PMFIRE) &= ~(INT_PM_H1);
                MBXINT_PRINT("FIA [32-47]M = 0x%x\n", FIQ_REG(REG_FIQ_H1_32_47));
                MBXINT_PRINT("FIQ [32-47]S = 0x%x\n", FIQ_REG(REG_FIQS_H1_32_47));
            }

            break;
        case E_MBX_CPU_AEON:
            if(dstCPUID==E_MBX_CPU_PM)
            {
                // AEON 2 PM
                //FIQ_REG(REG_FIQ_H1_32_47) &= ~(INT_FIQMASK_AEON_PM);
                CPU_INT_REG(REG_INT_AEONFIRE) |= INT_AEON_PM;
                CPU_INT_REG(REG_INT_AEONFIRE) &= ~(INT_AEON_PM);
                MBXINT_PRINT("FIA [32-47]M = 0x%x\n", FIQ_REG(REG_FIQ_H1_32_47));
                MBXINT_PRINT("FIQ [32-47]S = 0x%x\n", FIQ_REG(REG_FIQS_H1_32_47));
            }
            else
            {
                // AEON 2 MIPS
                //FIQ_REG(REG_FIQ_H1_32_47) &= ~(INT_FIQMASK_AEON_H3);
                CPU_INT_REG(REG_INT_AEONFIRE) |= INT_AEON_H1;
                CPU_INT_REG(REG_INT_AEONFIRE) &= ~(INT_AEON_H1);
                MBXINT_PRINT("FIA [32-47]M = 0x%x\n", FIQ_REG(REG_FIQ_H1_32_47));
                MBXINT_PRINT("FIQ [32-47]S = 0x%x\n", FIQ_REG(REG_FIQS_H1_32_47));
            }

            break;
        case E_MBX_CPU_MIPS:
            if(dstCPUID==E_MBX_CPU_PM)
            {
                //FIQ_REG(REG_FIQ_H1_48_63) &= ~(INT_FIQMASK_H3_PM);
                CPU_INT_REG(REG_INT_H1FIRE) |= INT_H1_PM;
                CPU_INT_REG(REG_INT_H1FIRE) &= ~(INT_H1_PM);
                MBXINT_PRINT("FIA [48-63]M = 0x%x\n", FIQ_REG(REG_FIQ_H1_48_63));
                MBXINT_PRINT("FIQ [48-63]S = 0x%x\n", FIQ_REG(REG_FIQS_H1_48_63));
            }
            else if(dstCPUID==E_MBX_CPU_AEON)
            {
                //FIQ_REG(REG_FIQ_H1_48_63) &= ~(INT_FIQMASK_H3_AEON);
                CPU_INT_REG(REG_INT_H1FIRE) |= INT_H1_AEON;
                CPU_INT_REG(REG_INT_H1FIRE) &= ~(INT_H1_AEON);
                MBXINT_PRINT("FIA [48-63]M = 0x%x\n", FIQ_REG(REG_FIQ_H1_48_63));
                MBXINT_PRINT("FIQ [48-63]S = 0x%x\n", FIQ_REG(REG_FIQS_H1_48_63));
            }
            #if 1//frcr2_integration###
            else if(dstCPUID==E_MBX_CPU_R2FRC)
            {
                FRC_CPU_INT_REG(REG_FRCINT_HKCPUFIRE) |= INT_HKCPU_FRCR2;
                FRC_CPU_INT_REG(REG_FRCINT_HKCPUFIRE) &= ~(INT_HKCPU_FRCR2);
                MBXINT_PRINT("FIA [0-15]M = 0x%x\n", FIQ_FRCREG(REG_FRCFIQ_H1_0_15));
                MBXINT_PRINT("FIQ [0-15]S = 0x%x\n", FIQ_FRCREG(REG_FRCFIQS_H1_0_15));
            }
            #endif

            break;
        default:
            MBXINT_ASSERT(FALSE,"wrong src cpu!\n");
            break;
    }

    return E_MBX_SUCCESS;
}

MBX_Result MHAL_MBXINT_CpuInit(MBX_CPU_ID eHKCPU)
{
    if((eHKCPU == E_MBX_CPU_AEON) || (eHKCPU == E_MBX_CPU_MIPS) || 
      (eHKCPU == E_MBX_CPU_MIPS_VPE1) || (eHKCPU == E_MBX_CPU_R2FRC)) //frcr2_integration###
    {
        return E_MBX_SUCCESS;
    }
    MBXINT_ASSERT(FALSE, "[MHAL_MBXINT_CpuInit] Invalid HK CPU ID \n");
    return E_MBX_ERR_INVALID_CPU_ID;
}
