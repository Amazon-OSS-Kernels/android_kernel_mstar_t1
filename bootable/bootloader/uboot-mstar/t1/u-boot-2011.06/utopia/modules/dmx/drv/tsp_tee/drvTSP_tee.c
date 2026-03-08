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
///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvTSP_tee.c
/// @brief  Transport Stream Processer (TSP) Driver Interface for TEE
/// @attention
/// All TSP DDI are not allowed to use in any interrupt context other than TSP ISR and Callback
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include <string.h>

#include "asmCPU.h"

// Internal Definition
#include "halTSP_tee.h"
#include "drvTSP_tee.h"
#include "drvMMIO.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
static MS_U8 u8TSPFirmware[] = {
    #include "fwTSP_tee.dat"
};

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
/// Init
/// @return TURE or FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_TSP_Tee_Init(void)
{
    MS_VIRT virtBank = 0;
    MS_PHY  u32BankSize = 0;

    if (FALSE == MDrv_MMIO_GetBASE(&virtBank, &u32BankSize, MS_MODULE_HW))
    {
        printf("MDrv_MMIO_GetBASE fail\n");
        return FALSE;
    }

    //printf("[%s] virtBank %x\n", __FUNCTION__, (int)virtBank);
    HAL_TSP_Tee_SetBank(virtBank);

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Copy fwtsp.dat to FW Buffer
/// @param  virtAddr                    \b IN: virtual address of FW buffer
/// @param  phyAddr                    \b IN: Physical address of VQ buffer
/// @param  u32Size                    \b IN: FW buffer size
/// @return TURE or FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_TSP_Tee_CopyFw(MS_VIRT virtAddr, MS_PHY phyAddr, MS_U32 u32Size)
{
    //MS_U8 *pu8 = 0;

    //printf("[%s][%d] virtAddr %x , phyAddr %x, u32Size %x \n", __FUNCTION__, __LINE__, (int)virtAddr, (int)phyAddr, (int)u32Size);

    if(u32Size < sizeof(u8TSPFirmware))
    {
        return FALSE;
    }

    memcpy((void*)virtAddr, (void*)u8TSPFirmware, sizeof(u8TSPFirmware));
    MAsm_CPU_Sync();
    MsOS_FlushMemory();

    #if 0
    pu8 = (MS_U8*)virtAddr;
    printf("[%s][%d] \n", __FUNCTION__, __LINE__);
    printf("%02x %02x %02x %02x %02x %02x %02x %02x \n", pu8[256], pu8[257],pu8[258],pu8[259],pu8[260],pu8[261],pu8[262],pu8[263]);
    printf("%02x %02x %02x %02x %02x %02x %02x %02x \n", pu8[264], pu8[265],pu8[266],pu8[267],pu8[268],pu8[269],pu8[270],pu8[271]);
    printf("%02x %02x %02x %02x %02x %02x %02x %02x \n", u8TSPFirmware[256], u8TSPFirmware[257],u8TSPFirmware[258],u8TSPFirmware[259],u8TSPFirmware[260],u8TSPFirmware[261],u8TSPFirmware[262],u8TSPFirmware[263]);
    printf("%02x %02x %02x %02x %02x %02x %02x %02x \n", u8TSPFirmware[264], u8TSPFirmware[265],u8TSPFirmware[266],u8TSPFirmware[267],u8TSPFirmware[268],u8TSPFirmware[269],u8TSPFirmware[270],u8TSPFirmware[271]);
    #endif

    HAL_TSP_Tee_Set_FWBuf(phyAddr, u32Size);

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Set VQ buffer
/// @param  phyAddr                    \b IN: Physical address of VQ buffer
/// @param  u32Size                     \b IN: VQ buffer size
/// @return  TURE or FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_TSP_Tee_Set_VQBuffer(MS_PHY phyAddr, MS_U32 u32Size)
{
    return HAL_TSP_Tee_Set_VQBuf(phyAddr, u32Size);
}

//-------------------------------------------------------------------------------------------------
/// Set PVR buffer
/// @param  u8PvrId                     \b IN: PVR engine ID
/// @param  phyAddr0                   \b IN: Physical address of PVR buffer 0
/// @param  u32Size0                    \b IN: Buffer size of PVR buffer 0
/// @param  phyAddr1                   \b IN: Physical address of PVR buffer 1
/// @param  u32Size1                    \b IN: Buffer size of PVR buffer 1
/// @return  TURE or FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_TSP_Tee_Set_PVRBuffer(MS_U8 u8PvrId, MS_PHY phyAddr0, MS_U32 u32Size0, MS_PHY phyAddr1, MS_U32 u32Size1)
{
    return HAL_TSP_Tee_Set_PvrBuf(u8PvrId, phyAddr0, u32Size0, phyAddr1, u32Size1);
}

//-------------------------------------------------------------------------------------------------
/// Set PVR buffer
/// @param  u8PvrId                      \b IN: PVR engine ID
/// @param  pphyAddr                    \b IN: Pointer to Physical address of PVR buffer 0
/// @return  TURE or FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_TSP_Tee_Get_PVRWriteAddr(MS_U8 u8PvrId, MS_PHY *pphyAddr)
{
    return HAL_TSP_Tee_Get_PVRWriteAddr(u8PvrId, pphyAddr);
}

//-------------------------------------------------------------------------------------------------
/// Transfer virtual filein engine ID to physical ID
/// @param  u8VirEngId                      \b IN: Virtual filein engine ID
/// @return  Physical engine ID
//-------------------------------------------------------------------------------------------------
MS_U32  MDrv_TSP_Tee_FileEngId_Vir2Phy(MS_U8 u8VirEngId)
{
    return HAL_TSP_Tee_FilePath2Tsif_Mapping(u8VirEngId);
}

//-------------------------------------------------------------------------------------------------
/// Set Filein buffer
/// @param  u8EngId                     \b IN: Filein engine ID
/// @param  phyAddr                     \b IN: Physical address of Filein buffer
/// @param  u32Size                      \b IN: Buffer size of Filein buffer
/// @return  TURE or FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_TSP_Tee_Set_FileinBuffer(MS_U8 u8EngId, MS_PHY phyAddr, MS_U32 u32Size)
{
    return HAL_TSP_Tee_Set_FileinBuf(u8EngId, phyAddr, u32Size);
}

//-------------------------------------------------------------------------------------------------
/// Set MMFI buffer
/// @param  u8EngId                     \b IN: Filein engine ID
/// @param  phyAddr                     \b IN: Physical address of Filein buffer
/// @param  u32Size                      \b IN: Buffer size of Filein buffer
/// @return  TURE or FALSE
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_TSP_Tee_Set_MMFIBuffer(MS_U8 u8EngId, MS_PHY phyAddr, MS_U32 u32Size)
{
    return HAL_TSP_Tee_Set_MMFIBuf(u8EngId, phyAddr, u32Size);
}
