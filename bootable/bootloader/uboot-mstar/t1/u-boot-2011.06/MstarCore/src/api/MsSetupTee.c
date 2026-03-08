/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
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

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------
#include <common.h>
#include <drvCPU.h>
#include <MsTypes.h>
#include <MsDebug.h>
#include <MsSetupTee.h>
#include <MsSysUtility.h>
#include <uboot_mmap.h>

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#if defined (CONFIG_R2_BRINGUP)
#define BIT2		0x04
#define BIT11       0x0800
#define BIT12       0x1000
#define BIT13       0x2000

#if (CONFIG_TV_CHIP==0)

#define UART_DBG_SECR2          0x00
#else
#define UART_DBG_SECR2          0x0D
#endif

#if (CONFIG_TV_CHIP==0)
#define REG_R2_BASE             0x103500
#else
#define REG_R2_BASE             0x122A00
#endif
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Public Functions
//-------------------------------------------------------------------------------------------------
void MApi_Disable_R2(void)
{
    UBOOT_TRACE("IN\n");

    WriteByte(REG_R2_BASE+0x40*2, 0x00); //disable R2

    UBOOT_TRACE("OK\n");
}

void MApi_R2_StartFrDRAM(MS_U32 addr)
{
    MS_U32 AeonBufAdr=addr;
    MS_U16 value;
    char *pEnv=NULL;
    UBOOT_TRACE("IN\n");
    printf("R2 Boot from 0x%08x\n", (unsigned int)AeonBufAdr);

    // select UART source
    //WriteByte(0x101EA6, UART_DBG_SRC);

    pEnv=getenv("uart2R2");
    if(pEnv!=NULL)
    {
        if(strcmp(pEnv,"1")==0)
        {
            printf("switch uart message to uart\n");
            WriteByte(0x101EA6, UART_DBG_SECR2);
        }
    }

//switch UART to dig_mux (instead of MCU51)
    value = Read2Byte(0x0E12);
#if 1//HK message
    value &= (~BIT12); //0: dig_mux (reg_uart_sel0)
    value |= BIT11; //1: enable uart rx via  PAD_DDCA for dig_mux (reg_uart_sel0)
#else //PM message
    value |= (BIT12); //1: HK MCU51 uart0
    value |= BIT11; //1: enable uart rx via  PAD_DDCA for dig_mux (reg_uart_sel0)
#endif
    Write2Byte(0x0E12,value);

    #if defined (CONFIG_NUTTX_BRINGUP_TEST)

    MDrv_COPRO_GetBase();
    MDrv_COPRO_Init_Front();
    MDrv_COPRO_Init_End(0xa200000);
    #else
    MDrv_COPRO_GetBase();
    MDrv_COPRO_Init_Front();
    MDrv_COPRO_Init_End(addr);
    #endif

    UBOOT_TRACE("OK\n");
}
#endif //CONFIG_R2_BRINGUP
