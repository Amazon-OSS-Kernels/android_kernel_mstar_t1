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
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/string.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <stddef.h>   /* for size_t */
#include "MsCommon.h"
#include "drvMMIO.h"
#include "gif_lib.h"
#include "apiGPD.h"
#include "drvgpd.h"
#include "gpd.h"
#include "drvMIU.h"
#include "halCHIP.h"
#ifdef CONFIG_MSTAR_CLKM
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/clkm.h>
#else
#include "drvCLKM.h"
#endif
#endif

#include "utopia.h"
#include "apiGPD_priv.h"
#include "apiGPD_v2.h"
void* pInstantGPD = NULL;

gpd_debug_level eEnableDbgMsg = E_GPD_DEBUG_LEVEL_NONE;

//--------------------------------------------------------------------------------------------------
//  Internal define
//--------------------------------------------------------------------------------------------------
#if (defined CONFIG_MLOG)
#include "ULog.h"

#define GPD_API_ERROR(format, args...)      do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) ULOGE("API_GPD", format, ##args);}while(0)
#define GPD_API_INFO(format, args...)       do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) ULOGI("API_GPD", format, ##args);}while(0)
#else

#ifdef ANDROID
#ifndef LOGD
#define GPD_API_ERROR(format, args...)      do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) ALOGE(format, ##args);}while(0)
#define GPD_API_INFO(format, args...)       do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) ALOGI(format, ##args);}while(0)
#else
#define GPD_API_ERROR(format, args...)      do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) LOGE(format, ##args);}while(0)
#define GPD_API_INFO(format, args...)       do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) LOGI(format, ##args);}while(0)
#endif
#else
#ifdef MSOS_TYPE_LINUX_KERNEL
#define GPD_API_ERROR(format, args...)      do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) printk(format, ##args);}while(0)
#define GPD_API_INFO(format, args...)       do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) printk(format, ##args);}while(0)
#else
#define GPD_API_ERROR(format, args...)      do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) printf(format, ##args);}while(0)
#define GPD_API_INFO(format, args...)       do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) printf(format, ##args);}while(0)
#endif
#endif

#endif

#define LOG_DATA_TO_USB         FALSE

#ifdef GPD_UTOPIA_2K
    #define GPD_DRIVER_BASE KERNEL_MODE
#else
    #define GPD_DRIVER_BASE 0
#endif

//#define VERIFY


#ifndef VERIFY
#define CBUFFER_SIZE         (1024UL*1024UL)
#define ZBUFFER_SIZE         (1024UL*1024UL)

#define GPD_XIU_BASE  (0x13F00UL * 2UL)
#define GPD_RIU_BASE  (0x13E00UL * 2UL)


#define MIU_CLI_BASE  (0x01200UL * 2UL)
#define MIU_SEL_BASE  (0x00600UL * 2UL)

#define STB_GPD_XIU_BASE  (0x10F00UL * 2UL)
#define STB_GPD_RIU_BASE  (0x10E00UL * 2UL)

#else

#define CBUFFER_SIZE         (1024UL*1024UL)
#define ZBUFFER_SIZE         (1024UL*1024UL)
#define STREAMBUFFER_SIZE  (4UL*1024UL*1024UL) //4MB
#define FRAMEBUFFER_SIZE  (22UL*1024UL*1024UL) //22MB


#define CBUFFER            (DRAMBASE + 0x11000000UL)
#define ZBUFFER            (CBUFFER + CBUFFER_SIZE)
#define STREAMBUFFER       (ZBUFFER + ZBUFFER_SIZE)
#define FRAMEBUFFER        (STREAMBUFFER + STREAMBUFFER_SIZE)
#define ARGBBUFFER         (FRAMEBUFFER + FRAMEBUFFER_SIZE)

#if (defined(__arm__))

#define HK_REG_BASE   0xFD200000UL

#else

#define HK_REG_BASE   0xBF200000UL

#endif

#define GPD_XIU_BASE  (HK_REG_BASE + 0x13F00UL * 2UL)
#define GPD_RIU_BASE  (HK_REG_BASE + 0x13E00UL * 2UL)

#if 0

#define CLK_GEN_BASE  (HK_REG_BASE + 0x00B00 * 2)

#else

#define CLK_GEN_BASE  (HK_REG_BASE + 0x03300UL * 2UL)

#endif

#define MIU_CLI_BASE  (HK_REG_BASE + 0x01200UL * 2UL)


#endif

#define HW_MAX_SUPPORTED_WIDTH          8192UL
#define HW_MAX_SUPPORTED_HEIGHT         8192UL

//--------------------------------------------------------------------------------------------------
//  Internal macros
//--------------------------------------------------------------------------------------------------
//#define MMIO8(addr) (*((volatile MS_U8*)(addr)))

//--------------------------------------------------------------------------------------------------
//  Internal Variable
//--------------------------------------------------------------------------------------------------
gpd_addr_info GpdAddrInfo;
gpd_pic_info Param;
MS_U32 MIUAllocation = 0;
MS_U32 g_gpd_set_access_region = FALSE;
MS_VIRT g_gpd_clock;
MS_U32 RIU_REG_BASE;
MS_U8 bIsGPDInit = FALSE;
//static MS_U32 paMIUAddress = 0xFFFFFFFF;
//--------------------------------------------------------------------------------------------------
//  Implementation
//--------------------------------------------------------------------------------------------------


//extern void *memset(void *ptr, MS_U32 value, size_t num);
extern void mdrv_gpd_setbuf(gpd_addr_info* addr_info);
extern MS_S32 mdrv_gpd_init(gpd_addr_info* param);
extern MS_S32 mdrv_gpd_decode(gpd_pic_info* pic_info, MS_U32 Ocolor, MS_U32 mode,gpd_roi_info* roi);
extern void mdrv_gpd_scaling_init(MS_U32 enable, MS_U32 mode);
MS_U32 MApi_GPD_Input_GetCRCResult(const MS_U8 *pu8Data, MS_U32 u32Size);

#if (ENABLE_GPD_MESSAGE|| LOG_DATA_TO_USB)
static MS_U32 u32FileIndex=0;
#endif

#if LOG_DATA_TO_USB
void GPD_WRITE_INPUT_2_USB(MS_U32 u32addr, MS_U32 u32Size, MS_U32 u32Index)
{
    u32Size = ALIGN_16(u32Size);
    static MS_U8 fileName2[]={'/','u','s','b','/','s','d','a','1','/','I','n', 'p', 'u', 't','_','0', '0', '0', '.', 'p', 'n', 'g',0};
    MS_U8 u8FilenameIndex = 10;

    fileName2[u8FilenameIndex+6]=(u32Index/100)%10+'0';
    fileName2[u8FilenameIndex+7]=(u32Index/10)%10+'0';
    fileName2[u8FilenameIndex+8]=u32Index%10+'0';

    FILE *inBinfp;
    if((inBinfp = fopen((char *)fileName2, "wb"))==NULL)
    {
        GPD_API_ERROR("Can't open input png file\n");
    }
    else
    {
        GPD_API_ERROR("open input png file success\n");
    }

    fwrite((void *)u32addr, 1, u32Size, inBinfp);
    fclose(inBinfp);

}

void GPD_WRITE_OUTPUT_2_USB(MS_U32 u32addr, MS_U32 u32Size, MS_U32 u32Width, MS_U32 u32Height, MS_U32 u32Index)
{
    u32Size = ALIGN_16(u32Size);
    static MS_U8 fileName2[]={'/','u','s','b','/','s','d','a','1','/','0','0', '0', '0', 'x','0','0', '0', '0', '_','0', '0', '0', '.', 'y', 'u', 'y', 'v',0};
    MS_U8 u8FilenameIndex = 10;
    fileName2[u8FilenameIndex]=u32Width/1000+'0';
    fileName2[u8FilenameIndex+1]=(u32Width/100)%10+'0';
    fileName2[u8FilenameIndex+2]=(u32Width/10)%10+'0';
    fileName2[u8FilenameIndex+3]=u32Width%10+'0';
    fileName2[u8FilenameIndex+5]=u32Height/1000+'0';
    fileName2[u8FilenameIndex+6]=(u32Height/100)%10+'0';
    fileName2[u8FilenameIndex+7]=(u32Height/10)%10+'0';
    fileName2[u8FilenameIndex+8]=u32Height%10+'0';

    fileName2[u8FilenameIndex+10]=(u32Index/100)%10+'0';
    fileName2[u8FilenameIndex+11]=(u32Index/10)%10+'0';
    fileName2[u8FilenameIndex+12]=u32Index%10+'0';


    FILE *outBinfp;
    if((outBinfp = fopen((char *)fileName2, "wb"))==NULL)
    {
        GPD_API_ERROR("Can't open output yuyv file\n");
    }
    else
    {
        GPD_API_ERROR("open output yuyv file success\n");
    }

    fwrite((void *)u32addr, 1, u32Size, outBinfp);
    fclose(outBinfp);

}
#endif

static MS_U32 _GPD_SetCacheable(MS_U32 param)
{
    MS_U32 cacheable;
    cacheable = (param == TRUE) ? TRUE: FALSE;
    drv_gpd_set_cacheable(cacheable);

    return TRUE;
}

static MS_U32 _GPD_SetAccessRegion(gpd_access_region *param)
{
    MS_VIRT BankBaseAddr;
    MS_PHY BankSize;
    MS_U32 u32GPDRIUBase, u32GPDXIUBase;

    MDrv_GPD_Reg_Base(&u32GPDRIUBase, &u32GPDXIUBase);

    if(FALSE == MDrv_MMIO_GetBASE((MS_VIRT*)&BankBaseAddr, &BankSize, MS_MODULE_GPD))
    {
        GPD_API_ERROR("MDrv_MMIO_GetBASE FAILED !! \n");
        return FALSE;
    }

    GpdAddrInfo.RegBaseAddr = BankBaseAddr + (MS_VIRT)u32GPDRIUBase;
    mdrv_gpd_setbuf(&GpdAddrInfo);
    drv_gpd_set_access_region(param->u32PA_StartAddr >> WORDALIGN, param->u32PA_EndAddr >> WORDALIGN);
    g_gpd_set_access_region = TRUE;

    GPD_API_INFO("Set g_gpd_set_access_region to TRUE!!!!\n");
    return TRUE;

}

static MS_U32 _GPD_GetPitch(gpd_get_pitch_info *para)
{
    if(para == NULL)
    {
        GPD_API_ERROR("_GPD_GetPitch FAILED !! para error\n");
        return FALSE;
    }
    if(para->ocolor > GRAY_ALPHA)
    {
        GPD_API_ERROR("_GPD_GetPitch FAILED !! ocolor error\n");
        return FALSE;
    }
    if(para->u32Width == 0)
    {
        GPD_API_ERROR("_GPD_GetPitch FAILED !! u32Width error\n");
        return FALSE;
    }
    switch(para->ocolor)
    {
        case ARGB8888:
            para->u32Pitch = (((para->u32Width) + 1) & ~1);     // 2-align
            break;
        case YUV422:
        case RGB565:
        case YVYU422:
        case GRAY_ALPHA:
            para->u32Pitch = (((para->u32Width) + 3) & ~3);     // 4-align
            break;
        case GRAY:
            para->u32Pitch = (((para->u32Width) + 7) & ~7);     // 8-align
            break;
    }
    return TRUE;

}


#ifndef VERIFY
void verGPD_ISR_CB(void)
{
    GPD_API_ERROR("verGPD_ISR_CB()!!!!!!!!!\n");
}


void MApi_GPD_EnableISR(GPD_IsrFuncCb IsrCb)
{
    if(TRUE == MDrv_GPD_EnableISR((GPD_IsrFuncCb)IsrCb))
    {
    }
    else
    {
        GPD_API_ERROR("%s : Fail!!!!\n", __FUNCTION__);
    }
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Disable GPD ISR and unregister callback function.
///@return \b JPEG_Result
/// - E_JPEG_OKAY : success.
/// - E_JPEG_FAILED : fail.
/********************************************************************/
void MApi_GPD_DisableISR(void)
{
    if(TRUE == MDrv_GPD_DisableISR())
    {
    }
    else
    {
        GPD_API_ERROR("%s : Fail!!!!\n", __FUNCTION__);
    }
    return;
}


void *_MApi_GPD_Init(MS_PHY SrcBuf)
{
    MS_VIRT BankBaseAddr;
    MS_PHY BankSize;
    MS_U32 GPD_CLK_BASE, GPD_CLK_OFFSET;
    MS_U32 u32GPDRIUBase, u32GPDXIUBase;

    if(bIsGPDInit == TRUE)
    {
        GPD_API_ERROR("GPD is in used !! \n");
        return FALSE;
    }
    else
    {
        bIsGPDInit = TRUE;
    }


    GPD_API_INFO("MApi_GPD_Init\n");
    GPD_API_INFO("GPD version:%x\n", GPD_VERSION);

    if(FALSE == MDrv_MMIO_GetBASE(&BankBaseAddr, &BankSize, MS_MODULE_GPD))
    {
        GPD_API_ERROR("MDrv_MMIO_GetBASE FAILED !! \n");
        bIsGPDInit = FALSE;
        return FALSE;
    }

    RIU_REG_BASE = BankBaseAddr;
    GPD_API_INFO("[%s] [%d] BankBaseAddr = %tx, BankSize = %tx\n", __FUNCTION__, __LINE__, (ptrdiff_t)BankBaseAddr, (ptrdiff_t)BankSize);

    //enable gpd clck

    GPD_CLK_BASE = drv_gpd_get_clkbase();

    GPD_CLK_OFFSET = drv_gpd_get_clkoffset();

    GPD_API_INFO("GPD_CLK_BASE=%tx, GPD_CLK_OFFSET=%tx\n", (ptrdiff_t)GPD_CLK_BASE, (ptrdiff_t)GPD_CLK_OFFSET);

    drv_gpd_init_outside_reg(BankBaseAddr);

    if (TRUE == g_gpd_set_access_region)
    {
        g_gpd_clock = (BankBaseAddr + GPD_CLK_BASE + GPD_CLK_OFFSET);
        GPD_API_INFO("Set g_gpd_clock to %tx!!!!(%tx %tx %tx)\n", (ptrdiff_t)g_gpd_clock,
            (ptrdiff_t)BankBaseAddr, (ptrdiff_t)GPD_CLK_BASE, (ptrdiff_t)GPD_CLK_OFFSET);
    }
    else
    {
#ifdef CONFIG_MSTAR_CLKM
        drv_gpd_power_on();
#else
#if 1
        GPD_API_INFO("@@@set gpd_clk to 216MHz\n");
        *((volatile MS_U32 *)(BankBaseAddr + GPD_CLK_BASE + GPD_CLK_OFFSET)) = 0;   // 0x28 << 2
#elif 0
        GPD_API_INFO("@@@set gpd_clk to 192MHz\n");
        *((volatile MS_U32 *)(BankBaseAddr + GPD_CLK_BASE + GPD_CLK_OFFSET)) = 0x4;
#else
        GPD_API_INFO("@@@set gpd_clk to 144MHz\n");
        *((volatile MS_U32 *)(BankBaseAddr + GPD_CLK_BASE + GPD_CLK_OFFSET)) = 0xc;
#endif
#endif

    }
    MDrv_GPD_Reg_Base(&u32GPDRIUBase, &u32GPDXIUBase);

#if 0
    MS_U32 u32Offset = 0;
    _phy_to_miu_offset(MIUAllocation, u32Offset, SrcBuf);
    UNUSED(u32Offset);
    switch(MIUAllocation)
    {
        case E_CHIP_MIU_0:
            MDrv_MIU_SelMIU(MIU_CLIENT_GPD_RW, MIU_SELTYPE_MIU0);
            break;
        case E_CHIP_MIU_1:
            MDrv_MIU_SelMIU(MIU_CLIENT_GPD_RW, MIU_SELTYPE_MIU1);
            break;
        case E_CHIP_MIU_2:
            MDrv_MIU_SelMIU(MIU_CLIENT_GPD_RW, MIU_SELTYPE_MIU2);
            break;
        case E_CHIP_MIU_3:
            MDrv_MIU_SelMIU(MIU_CLIENT_GPD_RW, MIU_SELTYPE_MIU3);
            break;
        default:
            MDrv_MIU_SelMIU(MIU_CLIENT_GPD_RW, MIU_SELTYPE_MIU0);
            break;
    }
#else
    MS_U32 tmp = 0;
    MS_U8 u8MIU_Offset;
    MS_U16 u16MIU_Mask;
    MDrv_GPD_MIU_Client(&u8MIU_Offset, &u16MIU_Mask);

    //check miu sel is controlled by HW or MIU
    //GPD MIU select control is
    tmp = *((volatile MS_U32 *)(BankBaseAddr + MIU_SEL_BASE + (((MS_U32)u8MIU_Offset)<< 1)));
    tmp &= (~u16MIU_Mask);
    *((volatile MS_U32 *)(BankBaseAddr + MIU_SEL_BASE + (((MS_U32)u8MIU_Offset) << 1))) = tmp;

    tmp = *((volatile MS_U32 *)(BankBaseAddr + MIU_CLI_BASE + (((MS_U32)u8MIU_Offset) << 1)));

    GPD_API_INFO("GPD MIU value = %tx\n", (ptrdiff_t)tmp);
    GPD_API_INFO("SrcBuf :0x%tx\n", (ptrdiff_t)SrcBuf);

    MIUAllocation = MDrv_GPD_MIU_Select(SrcBuf);
    if(MIUAllocation)
    {
        tmp |= (u16MIU_Mask);
        GPD_API_INFO("Select MIU1\n");
    }
    else
    {
        tmp &= (~u16MIU_Mask);
        GPD_API_INFO("Select MIU0\n");
    }

    *((volatile MS_U32 *)(BankBaseAddr + MIU_CLI_BASE +(((MS_U32)u8MIU_Offset) << 1))) = tmp;
#endif

    GpdAddrInfo.RegBaseAddr = BankBaseAddr + (MS_VIRT)u32GPDRIUBase;
    GpdAddrInfo.ReadBitBaseAddr = BankBaseAddr + (MS_VIRT)u32GPDXIUBase;

    GpdAddrInfo.u32PA_ColorBufferAddr = SrcBuf;

    if (FALSE == drv_gpd_get_cacheable())
    {
        GpdAddrInfo.u32VA_ColorBufferAddr = MsOS_PA2KSEG1(GpdAddrInfo.u32PA_ColorBufferAddr);
        GpdAddrInfo.u32PA_DepthBufferAddr = GpdAddrInfo.u32PA_ColorBufferAddr + 512UL * 1024UL;
        GpdAddrInfo.u32VA_DepthBufferAddr = MsOS_PA2KSEG1(GpdAddrInfo.u32PA_DepthBufferAddr);
    }
    else
    {
        GpdAddrInfo.u32VA_ColorBufferAddr = MsOS_PA2KSEG0(GpdAddrInfo.u32PA_ColorBufferAddr);
        GpdAddrInfo.u32PA_DepthBufferAddr = GpdAddrInfo.u32PA_ColorBufferAddr + 512UL * 1024UL;
        GpdAddrInfo.u32VA_DepthBufferAddr = MsOS_PA2KSEG0(GpdAddrInfo.u32PA_DepthBufferAddr);
    }


    if(GpdAddrInfo.u32VA_ColorBufferAddr == 0)
    {
        GPD_API_ERROR("Error!! MApi_GPD_Init with SrcBuf(PA)=0x%tx, VA=0\n", (ptrdiff_t)SrcBuf);
        bIsGPDInit = FALSE;
        return FALSE;
    }


    mdrv_gpd_init(&GpdAddrInfo);
#if 0
    MApi_GPD_DisableISR();
    MApi_GPD_EnableISR((GPD_IsrFuncCb)verGPD_ISR_CB);
    MDrv_GPD_SetISRMask();
#endif

    return &GpdAddrInfo;

}

MS_S32 _MApi_GPD_InputSource(gpd_pic_info* param, MS_PHY InputBuf, MS_U32 Size)
{
    MS_U8 u8MiuSel = 0;
    MS_U32 u32Offset = 0;
    _phy_to_miu_offset(u8MiuSel, u32Offset, InputBuf);
    UNUSED(u32Offset);
    if(MIUAllocation != u8MiuSel)
    {
        GPD_API_INFO("Wrong Memory Allocation\n");
        bIsGPDInit = FALSE;
        return -1;
    }

    if(bIsGPDInit == FALSE)
    {
        GPD_API_ERROR("[%s]GPD is not inited\n", __FUNCTION__);
        return -1;
    }
    GPD_API_INFO("MApi_GPD_ParseHeader\n");
    GpdAddrInfo.u32PA_ReadBufferAddr = InputBuf;

    GPD_API_INFO("InputBuf=%tx\n", (ptrdiff_t)InputBuf);

    if (FALSE == drv_gpd_get_cacheable())
    {
        GpdAddrInfo.u32VA_ReadBufferAddr = MsOS_PA2KSEG1(GpdAddrInfo.u32PA_ReadBufferAddr);
        GPD_API_INFO("MsOS_PA2KSEG1(InputBuf)=%tx\n", (ptrdiff_t)MsOS_PA2KSEG1(InputBuf));
    }
    else
    {
        GpdAddrInfo.u32VA_ReadBufferAddr = MsOS_PA2KSEG0(GpdAddrInfo.u32PA_ReadBufferAddr);
        GPD_API_INFO("MsOS_PA2KSEG0(InputBuf)=%tx\n", (ptrdiff_t)MsOS_PA2KSEG0(InputBuf));
    }

    if(GpdAddrInfo.u32VA_ReadBufferAddr == 0)
    {
        GPD_API_ERROR("Error!! MApi_GPD_InputSource with InputBuf(PA)=0x%tx, VA=0\n", (ptrdiff_t)InputBuf);
        bIsGPDInit = FALSE;
        return -1;
    }

    GpdAddrInfo.u32ReadBufferSize = Size;

#if (ENABLE_GPD_MESSAGE|| LOG_DATA_TO_USB)
    u32FileIndex++;
#if LOG_DATA_TO_USB
    GPD_WRITE_INPUT_2_USB((MS_U32)GpdAddrInfo.u32VA_ReadBufferAddr, (MS_U32)Size, u32FileIndex);
#endif

#if ENABLE_GPD_MESSAGE
    GPD_API_INFO("[%s][addr=0x%tx, size=0x%tx] Input CRC result=0x%tx\n", __FUNCTION__,
        (ptrdiff_t)GpdAddrInfo.u32VA_ReadBufferAddr, (ptrdiff_t)GpdAddrInfo.u32ReadBufferSize,
        (ptrdiff_t)MApi_GPD_Input_GetCRCResult((MS_U8 *)(MS_VIRT)GpdAddrInfo.u32VA_ReadBufferAddr,
        GpdAddrInfo.u32ReadBufferSize));
#endif
#endif


    mdrv_gpd_setbuf(&GpdAddrInfo);
    mdrv_gpd_decode(param,0,1,NULL);
    if(param->u32Height > HW_MAX_SUPPORTED_HEIGHT || param->u32Width >HW_MAX_SUPPORTED_WIDTH)
    {
        param->enGPDStatus = -2;
    }
    memcpy((void*)(&Param),(void*)(param),sizeof(gpd_pic_info));
    return param->enGPDStatus;

}

#if ENABLE_GPD_MESSAGE
static void MApi_GPD_PrintMem(MS_VIRT u32Addr, MS_U32 u32Size)
{
    MS_U32 u32i;
    GPD_API_INFO("===========================================================\n");
    GPD_API_INFO("print memory addr=0x%tx, size=0x%tx\n", (ptrdiff_t)u32Addr, (ptrdiff_t)u32Size);
    GPD_API_INFO("===========================================================\n");

#if 0
    for(u32i=0; u32i<u32Size/8+((u32Size%8)? 1:0); u32i++)
    {
        GPD_API_INFO("%02x %02x %02x %02x %02x %02x %02x %02x \n",
        *((MS_U8*)(u32Addr+u32i*8)), *((MS_U8*)(u32Addr+u32i*8+1)), *((MS_U8*)(u32Addr+u32i*8+2)), *((MS_U8*)(u32Addr+u32i*8+3)),
        *((MS_U8*)(u32Addr+u32i*8+4)), *((MS_U8*)(u32Addr+u32i*8+5)), *((MS_U8*)(u32Addr+u32i*8+6)), *((MS_U8*)(u32Addr+u32i*8+7))
        );
    }
#else
    for(u32i=0; u32i<u32Size/16+((u32Size%16)? 1:0); u32i++)
    {
        GPD_API_INFO("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x \n",
        *((MS_U8*)(u32Addr+u32i*16)), *((MS_U8*)(u32Addr+u32i*16+1)), *((MS_U8*)(u32Addr+u32i*16+2)), *((MS_U8*)(u32Addr+u32i*16+3)),
        *((MS_U8*)(u32Addr+u32i*16+4)), *((MS_U8*)(u32Addr+u32i*16+5)), *((MS_U8*)(u32Addr+u32i*16+6)), *((MS_U8*)(u32Addr+u32i*16+7)),
        *((MS_U8*)(u32Addr+u32i*16+8)), *((MS_U8*)(u32Addr+u32i*16+9)), *((MS_U8*)(u32Addr+u32i*16+10)), *((MS_U8*)(u32Addr+u32i*16+11)),
        *((MS_U8*)(u32Addr+u32i*16+12)), *((MS_U8*)(u32Addr+u32i*16+13)), *((MS_U8*)(u32Addr+u32i*16+14)), *((MS_U8*)(u32Addr+u32i*16+15))
        );
    }
#endif
    GPD_API_INFO("===========================================================\n");
}
#endif


MS_S32 _MApi_GPD_OutputDecode(MS_PHY DecodeBuf, MS_U32 ocolor, MS_U32 Size)
{
    //DecodeBuf needs 32-byte alignment
    //gpd_pic_info info;
    //gpd_pic_info* param = &info;
    MS_U8 u8MiuSel = 0;
    MS_U32 u32Offset = 0;
    _phy_to_miu_offset(u8MiuSel, u32Offset, DecodeBuf);
    UNUSED(u32Offset);
    if(MIUAllocation != u8MiuSel)
    {
        GPD_API_INFO("Wrong Memory Allocation\n");
        bIsGPDInit = FALSE;
        return -1;
    }

    if(bIsGPDInit == FALSE)
    {
        GPD_API_ERROR("[%s]GPD is not inited\n", __FUNCTION__);
        return -1;
    }
    gpd_roi_info roi;
    GPD_API_INFO("MApi_GPD_Decode\n");
    GpdAddrInfo.u32PA_WriteBufferAddr = DecodeBuf;
    if (FALSE == drv_gpd_get_cacheable())
    {
        GpdAddrInfo.u32VA_WriteBufferAddr = MsOS_PA2KSEG1(GpdAddrInfo.u32PA_WriteBufferAddr);
    }
    else
    {
        GpdAddrInfo.u32VA_WriteBufferAddr = MsOS_PA2KSEG0(GpdAddrInfo.u32PA_WriteBufferAddr);
    }
    GpdAddrInfo.u32WriteBufferSize = Size;


    if(GpdAddrInfo.u32VA_WriteBufferAddr == 0)
    {
        GPD_API_ERROR("Error!! MApi_GPD_OutputDecode with DecodeBuf(PA)=0x%tx, VA=0\n", (ptrdiff_t)DecodeBuf);
        bIsGPDInit = FALSE;
        return -1;
    }

    memset(&roi, 0, sizeof(roi));
    mdrv_gpd_setbuf(&GpdAddrInfo);

    drv_gpd_set_access_bound(GpdAddrInfo.u32PA_WriteBufferAddr,GpdAddrInfo.u32PA_WriteBufferAddr + GpdAddrInfo.u32WriteBufferSize - 1,
                                                    GpdAddrInfo.u32PA_ColorBufferAddr, GpdAddrInfo.u32PA_ColorBufferAddr +CBUFFER_SIZE+ZBUFFER_SIZE-1);

    GPD_API_INFO("GPDBaseAddr=0x%tx\n", (ptrdiff_t)GpdAddrInfo.RegBaseAddr);
    GPD_API_INFO("READBITBaseAddr=0x%tx\n", (ptrdiff_t)GpdAddrInfo.ReadBitBaseAddr);
    GPD_API_INFO("Read  buf PA=0x%tx,VA=0x%tx\n", (ptrdiff_t)GpdAddrInfo.u32PA_ReadBufferAddr, (ptrdiff_t)GpdAddrInfo.u32VA_ReadBufferAddr);
    GPD_API_INFO("Write buf PA=0x%tx,VA=0x%tx\n", (ptrdiff_t)GpdAddrInfo.u32PA_WriteBufferAddr, (ptrdiff_t)GpdAddrInfo.u32VA_WriteBufferAddr);
    GPD_API_INFO("Color buf PA=0x%tx,VA=0x%tx\n", (ptrdiff_t)GpdAddrInfo.u32PA_ColorBufferAddr, (ptrdiff_t)GpdAddrInfo.u32VA_ColorBufferAddr);
    GPD_API_INFO("Depth buf PA=0x%tx,VA=0x%tx\n", (ptrdiff_t)GpdAddrInfo.u32PA_DepthBufferAddr, (ptrdiff_t)GpdAddrInfo.u32VA_DepthBufferAddr);
    GPD_API_INFO("Read  buf size=0x%tx\n", (ptrdiff_t)GpdAddrInfo.u32ReadBufferSize);
    GPD_API_INFO("Write buf size=0x%tx\n", (ptrdiff_t)GpdAddrInfo.u32WriteBufferSize);

#if ENABLE_GPD_MESSAGE
//    memset((void *)GpdAddrInfo.u32VA_WriteBufferAddr, 0x37, (size_t)GpdAddrInfo.u32WriteBufferSize);
#endif

    mdrv_gpd_decode(&Param, ocolor, 0, &roi);

#if (ENABLE_GPD_MESSAGE|| LOG_DATA_TO_USB)

    MS_U8 u8BytesPerPixel;
    switch(ocolor)
    {
        case ARGB8888:
            u8BytesPerPixel = 4;
            break;
        case YUV422:
        case RGB565:
        case YVYU422:
        case GRAY_ALPHA:
            u8BytesPerPixel = 2;
            break;
        case GRAY:
        default:
            u8BytesPerPixel = 1;
            break;
    }

#if LOG_DATA_TO_USB
    GPD_WRITE_OUTPUT_2_USB((MS_U32)GpdAddrInfo.u32VA_WriteBufferAddr,
        (MS_U32)(Param.u32Height)*(Param.u32Width)*u8BytesPerPixel, Param.u32Width, Param.u32Height, u32FileIndex);
#endif

#if ENABLE_GPD_MESSAGE
    MApi_GPD_PrintMem(GpdAddrInfo.u32VA_WriteBufferAddr, 100);
    GPD_API_INFO("[%s, %td, %tdx%td], CRC result=0x%tx\n", __FUNCTION__,  (ptrdiff_t)u32FileIndex, (ptrdiff_t)Param.u32Width, (ptrdiff_t)Param.u32Height,
        (ptrdiff_t)_MApi_GPD_GetCRCResult((MS_U8 *)GpdAddrInfo.u32VA_WriteBufferAddr, (Param.u32Height)*(Param.u32Width)*u8BytesPerPixel, Param.u32Width, ocolor));
    GPD_API_INFO("[%s][addr=0x%tx, size=0x%tx] Input CRC result=0x%tx\n", __FUNCTION__,
        (ptrdiff_t)GpdAddrInfo.u32VA_ReadBufferAddr, (ptrdiff_t)GpdAddrInfo.u32ReadBufferSize,
        (ptrdiff_t)MApi_GPD_Input_GetCRCResult((MS_U8 *)GpdAddrInfo.u32VA_ReadBufferAddr,
        GpdAddrInfo.u32ReadBufferSize));

#endif
#endif

    drv_gpd_power_off();
    bIsGPDInit = FALSE;
    return Param.enGPDStatus;


}

MS_S32 _MApi_GPD_OutputDecodeMGIF(MS_PHY DecodeBuf, MS_U32 ocolor, MS_U32 Size)
{
    //DecodeBuf needs 32-byte alignment
    //gpd_pic_info info;
    //gpd_pic_info* param = &info;
    MS_U8 u8MiuSel = 0;
    MS_U32 u32Offset = 0;
    _phy_to_miu_offset(u8MiuSel, u32Offset, DecodeBuf);
    UNUSED(u32Offset);
    if(MIUAllocation != u8MiuSel)
    {
        GPD_API_INFO("Wrong Memory Allocation\n");
        bIsGPDInit = FALSE;
        return -1;
    }
    if(bIsGPDInit == FALSE)
    {
        GPD_API_ERROR("[%s]GPD is not inited\n", __FUNCTION__);
        return -1;
    }
    gpd_roi_info roi;
    GPD_API_INFO("MApi_GPD_Decode\n");
    GpdAddrInfo.u32PA_WriteBufferAddr = DecodeBuf;
    GpdAddrInfo.u32VA_WriteBufferAddr = MsOS_PA2KSEG0(GpdAddrInfo.u32PA_WriteBufferAddr);
    GpdAddrInfo.u32WriteBufferSize = Size;

    if(GpdAddrInfo.u32VA_WriteBufferAddr == 0)
    {
        GPD_API_ERROR("Error!! MApi_GPD_OutputDecodeMGIF with DecodeBuf(PA)=0x%tx, VA=0\n", (ptrdiff_t)DecodeBuf);
        bIsGPDInit = FALSE;
        return -1;
    }

    memset(&roi, 0, sizeof(roi));
    mdrv_gpd_setbuf(&GpdAddrInfo);

    drv_gpd_set_access_bound(GpdAddrInfo.u32PA_WriteBufferAddr,GpdAddrInfo.u32PA_WriteBufferAddr + GpdAddrInfo.u32WriteBufferSize - 1,
                                                    GpdAddrInfo.u32PA_ColorBufferAddr, GpdAddrInfo.u32PA_ColorBufferAddr +CBUFFER_SIZE+ZBUFFER_SIZE-1);

    GPD_API_INFO("GPDBaseAddr=0x%tx\n",(ptrdiff_t)GpdAddrInfo.RegBaseAddr);
    GPD_API_INFO("READBITBaseAddr=0x%tx\n",(ptrdiff_t)GpdAddrInfo.ReadBitBaseAddr);
    GPD_API_INFO("Read  buf PA=0x%tx,VA=0x%tx\n",(ptrdiff_t)GpdAddrInfo.u32PA_ReadBufferAddr,(ptrdiff_t)GpdAddrInfo.u32VA_ReadBufferAddr);
    GPD_API_INFO("Write buf PA=0x%tx,VA=0x%tx\n",(ptrdiff_t)GpdAddrInfo.u32PA_WriteBufferAddr,(ptrdiff_t)GpdAddrInfo.u32VA_WriteBufferAddr);
    GPD_API_INFO("Color buf PA=0x%tx,VA=0x%tx\n",(ptrdiff_t)GpdAddrInfo.u32PA_ColorBufferAddr,(ptrdiff_t)GpdAddrInfo.u32VA_ColorBufferAddr);
    GPD_API_INFO("Depth buf PA=0x%tx,VA=0x%tx\n",(ptrdiff_t)GpdAddrInfo.u32PA_DepthBufferAddr,(ptrdiff_t)GpdAddrInfo.u32VA_DepthBufferAddr);
    GPD_API_INFO("Read  buf size=0x%tx\n", (ptrdiff_t)GpdAddrInfo.u32ReadBufferSize);
    GPD_API_INFO("Write buf size=0x%tx\n", (ptrdiff_t)GpdAddrInfo.u32WriteBufferSize);

    mdrv_gpd_decode(&Param, ocolor, 3, &roi);

#if (ENABLE_GPD_MESSAGE ||LOG_DATA_TO_USB)
    MS_U8 u8BytesPerPixel;
    switch(ocolor)
    {
        case ARGB8888:
            u8BytesPerPixel = 4;
            break;
        case YUV422:
        case RGB565:
        case YVYU422:
        case GRAY_ALPHA:
            u8BytesPerPixel = 2;
            break;
        case GRAY:
        default:
            u8BytesPerPixel = 1;
            break;
    }

#if LOG_DATA_TO_USB
    GPD_WRITE_OUTPUT_2_USB((MS_U32)GpdAddrInfo.u32VA_WriteBufferAddr,
        (MS_U32)(Param.u32Height)*(Param.u32Width)*u8BytesPerPixel, Param.u32Width, Param.u32Height, u32FileIndex);
#endif

#if ENABLE_GPD_MESSAGE
    MApi_GPD_PrintMem(GpdAddrInfo.u32VA_WriteBufferAddr, 100);
    GPD_API_INFO("[%s, %td, %tdx%td], CRC result=0x%tx\n", __FUNCTION__, (ptrdiff_t)u32FileIndex, (ptrdiff_t)Param.u32Width, (ptrdiff_t)Param.u32Height,
        (ptrdiff_t)_MApi_GPD_GetCRCResult((MS_U8 *)GpdAddrInfo.u32VA_WriteBufferAddr,(Param.u32Height)*(Param.u32Width)*u8BytesPerPixel, Param.u32Width, ocolor));
    GPD_API_INFO("[%s][addr=0x%tx, size=0x%tx] Input CRC result=0x%tx\n", __FUNCTION__,
        (ptrdiff_t)GpdAddrInfo.u32VA_ReadBufferAddr, (ptrdiff_t)GpdAddrInfo.u32ReadBufferSize,
        (ptrdiff_t)MApi_GPD_Input_GetCRCResult((MS_U8 *)GpdAddrInfo.u32VA_ReadBufferAddr,
        GpdAddrInfo.u32ReadBufferSize));

#endif

#endif

    drv_gpd_power_off();
    bIsGPDInit = FALSE;
    return Param.enGPDStatus;


}

MS_S32 _MApi_GPD_GetDuration(void)
{
    return MstDGifGetDuration();
}

void _MApi_GPD_SetGIFMode(MS_U32 mode)
{
    MstDGifSetMode(mode);
}



MS_S32 _MApi_GPD_OutputDecodeROI(MS_PHY DecodeBuf, MS_U32 ocolor, MS_U32 Size, gpd_roi_info *roi)
{
    //DecodeBuf needs 32-byte alignment
    //gpd_pic_info info;
    //gpd_pic_info* param = &info;
    MS_U8 u8MiuSel = 0;
    MS_U32 u32Offset = 0;
    _phy_to_miu_offset(u8MiuSel, u32Offset, DecodeBuf);
    UNUSED(u32Offset);
    if(MIUAllocation != u8MiuSel)
    {
        GPD_API_INFO("Wrong Memory Allocation\n");
        bIsGPDInit = FALSE;
        return -1;
    }

    if(bIsGPDInit == FALSE)
    {
        GPD_API_ERROR("[%s]GPD is not inited\n", __FUNCTION__);
        return -1;
    }

    GPD_API_INFO("MApi_GPD_DecodeROI\n");
    GpdAddrInfo.u32PA_WriteBufferAddr = DecodeBuf;
    if (FALSE == drv_gpd_get_cacheable())
    {
        GpdAddrInfo.u32VA_WriteBufferAddr = MsOS_PA2KSEG1(GpdAddrInfo.u32PA_WriteBufferAddr);
    }
    else
    {
        GpdAddrInfo.u32VA_WriteBufferAddr = MsOS_PA2KSEG0(GpdAddrInfo.u32PA_WriteBufferAddr);
    }
    GpdAddrInfo.u32WriteBufferSize = Size;

    if(GpdAddrInfo.u32VA_WriteBufferAddr == 0)
    {
        GPD_API_ERROR("Error!! MApi_GPD_OutputDecodeROI with DecodeBuf(PA)=0x%tx, VA=0\n", (ptrdiff_t)DecodeBuf);
        bIsGPDInit = FALSE;
        return -1;
    }


    mdrv_gpd_setbuf(&GpdAddrInfo);

    drv_gpd_set_access_bound(GpdAddrInfo.u32PA_WriteBufferAddr,GpdAddrInfo.u32PA_WriteBufferAddr + GpdAddrInfo.u32WriteBufferSize - 1,
                                                    GpdAddrInfo.u32PA_ColorBufferAddr, GpdAddrInfo.u32PA_ColorBufferAddr +CBUFFER_SIZE+ZBUFFER_SIZE-1);

    GPD_API_INFO("GPDBaseAddr=0x%tx\n",(ptrdiff_t)GpdAddrInfo.RegBaseAddr);
    GPD_API_INFO("READBITBaseAddr=0x%tx\n",(ptrdiff_t)GpdAddrInfo.ReadBitBaseAddr);
    GPD_API_INFO("Read  buf PA=0x%tx,VA=0x%tx\n",(ptrdiff_t)GpdAddrInfo.u32PA_ReadBufferAddr,(ptrdiff_t)GpdAddrInfo.u32VA_ReadBufferAddr);
    GPD_API_INFO("Write buf PA=0x%tx,VA=0x%tx\n",(ptrdiff_t)GpdAddrInfo.u32PA_WriteBufferAddr,(ptrdiff_t)GpdAddrInfo.u32VA_WriteBufferAddr);
    GPD_API_INFO("Color buf PA=0x%tx,VA=0x%tx\n",(ptrdiff_t)GpdAddrInfo.u32PA_ColorBufferAddr,(ptrdiff_t)GpdAddrInfo.u32VA_ColorBufferAddr);
    GPD_API_INFO("Color buf PA=0x%tx,VA=0x%tx\n",(ptrdiff_t)GpdAddrInfo.u32PA_ColorBufferAddr,(ptrdiff_t)GpdAddrInfo.u32VA_ColorBufferAddr);
    GPD_API_INFO("Depth buf PA=0x%tx,VA=0x%tx\n",(ptrdiff_t)GpdAddrInfo.u32PA_DepthBufferAddr,(ptrdiff_t)GpdAddrInfo.u32VA_DepthBufferAddr);
    GPD_API_INFO("Read  buf size=0x%tx\n", (ptrdiff_t)GpdAddrInfo.u32ReadBufferSize);
    GPD_API_INFO("Write buf size=0x%tx\n", (ptrdiff_t)GpdAddrInfo.u32WriteBufferSize);

#if ENABLE_GPD_MESSAGE
//    memset((void *)GpdAddrInfo.u32VA_WriteBufferAddr, 0x37, (size_t)GpdAddrInfo.u32WriteBufferSize);
#endif
    mdrv_gpd_decode(&Param, ocolor, 2, roi);

#if (ENABLE_GPD_MESSAGE || LOG_DATA_TO_USB)

    MS_U8 u8BytesPerPixel;
    switch(ocolor)
    {
        case ARGB8888:
            u8BytesPerPixel = 4;
            break;
        case YUV422:
        case RGB565:
        case YVYU422:
        case GRAY_ALPHA:
            u8BytesPerPixel = 2;
            break;
        case GRAY:
        default:
            u8BytesPerPixel = 1;
            break;
    }

#if LOG_DATA_TO_USB
    GPD_WRITE_OUTPUT_2_USB((MS_U32)GpdAddrInfo.u32VA_WriteBufferAddr,
        (MS_U32)(roi->height)*(roi->width)*u8BytesPerPixel, roi->width, roi->height, u32FileIndex);
#endif

#if ENABLE_GPD_MESSAGE
    MApi_GPD_PrintMem(GpdAddrInfo.u32VA_WriteBufferAddr, 100);
    MS_U32 u32OutputCRC = _MApi_GPD_GetCRCResult((MS_U8 *)GpdAddrInfo.u32VA_WriteBufferAddr,(MS_U32) (roi->height)*(roi->width)*u8BytesPerPixel, roi->width, ocolor);
    GPD_API_INFO("[%s, %td, %tdx%td], CRC result=0x%tx\n", __FUNCTION__, (ptrdiff_t)u32FileIndex, (ptrdiff_t)roi->width, (ptrdiff_t)roi->height,
        (ptrdiff_t)u32OutputCRC);
    GPD_API_INFO("[%s][addr=0x%tx, size=0x%tx] Input CRC result=0x%tx\n", __FUNCTION__,
        (ptrdiff_t)GpdAddrInfo.u32VA_ReadBufferAddr, (ptrdiff_t)GpdAddrInfo.u32ReadBufferSize,
        (ptrdiff_t)MApi_GPD_Input_GetCRCResult((MS_U8 *)GpdAddrInfo.u32VA_ReadBufferAddr,
        GpdAddrInfo.u32ReadBufferSize));

#endif
#endif

    drv_gpd_power_off();
    bIsGPDInit = FALSE;
    return Param.enGPDStatus;


}

#else

void* MAdp_GPD_Init(void)
{
    MS_S32 err;
    //MS_U32 size=0;
    MS_S32 tmp;

    console_printf("MAdp_GPD_Init\n");

    GpdAddrInfo.RegBaseAddr = (MS_U32)GPD_RIU_BASE;
    GpdAddrInfo.ReadBitBaseAddr = (MS_U32)GPD_XIU_BASE;

#if 0

   *((volatile MS_U32 *)(CLK_GEN_BASE + (0x5E << 1))) = 0;   // 0x2f << 2

#else

    *((volatile MS_U32 *) (CLK_GEN_BASE + (0x50 << 1))) = 0;   // 0x28 << 2
    //*((volatile MS_U32 *) (CLK_GEN_BASE + (0x50 << 1))) = (2 << 2);   // 0x28 << 2

#endif

    //set gpd miu channel

#if 1
    tmp = *((volatile MS_U32 *)((MS_U32)MIU_CLI_BASE + (0xf4 << 1)));


    if ((MS_U32)CBUFFER & (1 << 29))  //512mb
    {
        tmp |= (0x40);  // bit 6
        console_printf("Select MIU1\n");
    }
    else
    {
        tmp &= (~0x40);
        console_printf("Select MIU0\n");

    }

    *((volatile MS_U32 *)((MS_U32)MIU_CLI_BASE +(0xf4 << 1))) = tmp;
#endif

#if 0
    *((volatile MS_U32 *)((MS_U32)MIU_CLI_BASE +(0x67 << 2))) = 0x26;

    *((volatile MS_U32 *)((MS_U32)MIU_CLI_BASE +(0x68 << 2))) = 0x8800;

    *((volatile MS_U32 *)((MS_U32)MIU_CLI_BASE +(0x69 << 2))) = 0x9800;

    *((volatile MS_U32 *)((MS_U32)MIU_CLI_BASE +(0x60 << 2))) = 0x2;

    console_printf("Enable MIU Protection\n");
#endif

    GpdAddrInfo.u32PA_ColorBufferAddr = (MS_U32)CBUFFER;
    GpdAddrInfo.u32VA_ColorBufferAddr = MsOS_PA2KSEG1(GpdAddrInfo.u32PA_ColorBufferAddr);

    console_printf("Get Color buffer\n");

    GpdAddrInfo.u32PA_DepthBufferAddr = (MS_U32)ZBUFFER;
    GpdAddrInfo.u32VA_DepthBufferAddr = MsOS_PA2KSEG1(GpdAddrInfo.u32PA_DepthBufferAddr);

    console_printf("Get Depth buffer\n");

    GpdAddrInfo.u32PA_ReadBufferAddr = (MS_U32)STREAMBUFFER;
    GpdAddrInfo.u32VA_ReadBufferAddr = MsOS_PA2KSEG1(GpdAddrInfo.u32PA_ReadBufferAddr);

    console_printf("Get Read buffer\n");

    GpdAddrInfo.u32PA_WriteBufferAddr = (MS_U32)FRAMEBUFFER;
    GpdAddrInfo.u32VA_WriteBufferAddr = MsOS_PA2KSEG1(GpdAddrInfo.u32PA_WriteBufferAddr);

    console_printf("Get Write buffer\n");

    GpdAddrInfo.u32WriteBufferSize = FRAMEBUFFER_SIZE;


    console_printf("GPDBaseAddr=0x%08x\n",GpdAddrInfo.RegBaseAddr);
    console_printf("READBITBaseAddr=0x%08x\n",GpdAddrInfo.ReadBitBaseAddr);
    console_printf("Read  buf PA=0x%08x,VA=0x%08x\n",GpdAddrInfo.u32PA_ReadBufferAddr,GpdAddrInfo.u32VA_ReadBufferAddr);
    console_printf("Write buf PA=0x%08x,VA=0x%08x\n",GpdAddrInfo.u32PA_WriteBufferAddr,GpdAddrInfo.u32VA_WriteBufferAddr);
    console_printf("Color buf PA=0x%08x,VA=0x%08x\n",GpdAddrInfo.u32PA_ColorBufferAddr,GpdAddrInfo.u32VA_ColorBufferAddr);
    console_printf("Depth buf PA=0x%08x,VA=0x%08x\n",GpdAddrInfo.u32PA_DepthBufferAddr,GpdAddrInfo.u32VA_DepthBufferAddr);
    console_printf("Read  buf size=0x%08x\n", GpdAddrInfo.u32ReadBufferSize);
    console_printf("Write buf size=0x%08x\n", GpdAddrInfo.u32WriteBufferSize);


    //Call mdrv_gpd_init, This should include Color-buffer, Z-buffer, Bitstream buffer, Frame Buffer.
    err = mdrv_gpd_init(&GpdAddrInfo);



    return &GpdAddrInfo;

}

MS_S32 MAdp_GPD_Play(gpd_pic_info* param, MS_U32 ocolor, MS_U8 *srcbuffer, MS_U32 size)
{

    extern void *memcpy(void *destination, const void *source, size_t num);
    //FILE *hFile = 0;
    //long lSize,i=0;
    //size_t result;
    //MS_U32 i = 0;
    gpd_roi_info roi;
    //MS_U8 sig[10];
    //FILE *fp;

    console_printf("MAdp_GPD_Play\n");

    Param = param;
    memset(&roi, 0, sizeof(roi));
    mdrv_gpd_setbuf(&GpdAddrInfo);


    #if 1

    console_printf("size:%d\n",size);

    memcpy((void *)GpdAddrInfo.u32VA_ReadBufferAddr, (void *)srcbuffer, size);

    GpdAddrInfo.u32ReadBufferSize = size;

    console_printf("copy image done\n");

    #endif


    #if 0
    hFile = fopen(param->filename, "rb");
    if(!hFile)
    {
        console_printf("Open file error=%s!!\n",param->filename);
        return -1;
    }
    // obtain file size:
    fseek (hFile , 0 , SEEK_END);
    lSize = ftell (hFile);
    rewind (hFile);

    //fread file into bitstream buffer
    result = fread ((char *)GpdAddrInfo.u32VA_ReadBufferAddr,1,lSize,hFile);

    #endif

#if 0
    sig[0] = MMIO8(GpdAddrInfo.u32VA_ReadBufferAddr);
    sig[1] = MMIO8(GpdAddrInfo.u32VA_ReadBufferAddr+1);
    sig[2] = MMIO8(GpdAddrInfo.u32VA_ReadBufferAddr+2);
    sig[3] = MMIO8(GpdAddrInfo.u32VA_ReadBufferAddr+3);
    console_printf("Signaute = %x,%x,%x,%x\n",sig[0],sig[1],sig[2],sig[3]);
#endif
    //Call mdrv_gpd_decode, specify output format;ARGB888/YUV422/RGB565

    #if 0
    roi.hstart = 0;
    roi.vstart = 0;
    roi.width = 64;
    roi.height = 64;
    #endif

    mdrv_gpd_decode(Param, ocolor, 0, &roi);

    return Param.enGPDStatus;
}


MS_S32 MAdp_GPD_Compare(MS_U32 ocolor_typ)
{

    extern MS_U32 memcmp(const void *ptr1, const void *ptr2, size_t num);

    MS_U32 frameptr, argbptr;

    MS_U32 hw_extra = 0, sw_extra = 0;

    MS_U32 pixelbytes = 0;

    MS_U32 width, height;

    MS_U32 result = 0;

    frameptr = MsOS_PA2KSEG1(FRAMEBUFFER);

    argbptr = MsOS_PA2KSEG1(ARGBBUFFER);

    console_printf("MAdp_GPD_Compare\n");

    #if 0

    console_printf("size:%d\n",size);

    memcpy((void *)ARGBBUFFER,(void *)srcbuffer,size);

    console_printf("copy image done\n");

    #endif

    width = Param.u32Width;

    height = Param.u32Height;

    console_printf("height:%d width:%d\n", height, width);


    switch (ocolor_typ)
    {
    case ARGB8888:
        pixelbytes = 4;
        if((width * pixelbytes) % 8)
            hw_extra = 8 - ((width * pixelbytes) % 8); break;
    case YUV422:
    case YVYU422:
        if(width & 1)
            sw_extra = 2;
    case RGB565:
    case GRAY_ALPHA:
        pixelbytes = 2;
        if((width * pixelbytes) % 8)
            hw_extra = 8 - ((width * pixelbytes) % 8); break;
    case GRAY:
        pixelbytes = 1;
        if((width * pixelbytes) % 8)
            hw_extra = 8 - ((width * pixelbytes) % 8); break;
    default:
        console_printf("Unsupported output color %d\n", ocolor_typ);
        return NULL;
    }


    while(height)
    {

        result = memcmp((void *) frameptr,(void *) argbptr, width * pixelbytes);

        if(result)
        {
            console_printf("compare failed\n");

            return result;
        }

        frameptr += width * pixelbytes + hw_extra;

        argbptr += width * pixelbytes + sw_extra;

        height--;
    }

    return 0;

}


#endif

void _MApi_GPD_ScalingEnable(MS_U32 mode)
{


    mdrv_gpd_scaling_init(1, mode);

    GPD_API_INFO("GPD Scaling Enable, mode = %td\n", (ptrdiff_t)mode);
}



void _MApi_GPD_ScalingDisable(void)
{

    mdrv_gpd_scaling_init(0, 0);

    GPD_API_INFO("GPD Scaling Disable\n");
}


MS_U32 _MApi_GPD_SetControl(gpd_user_cmd cmd_id, MS_VIRT param)
{
    switch (cmd_id)
    {
        case E_GPD_USER_CMD_SET_ACCESS_REGION:
            return _GPD_SetAccessRegion((gpd_access_region *)param);

	case E_GPD_USER_CMD_SET_CACHEABLE:
            return _GPD_SetCacheable((MS_U32)param);
	case E_GPD_USER_CMD_SET_DEBUG_LEVEL:
            eEnableDbgMsg = (gpd_debug_level)param;
            return TRUE;

        default:
            return FALSE;

    }

}


MS_U32 _MApi_GPD_GetControl(gpd_get_cmd cmd_id, MS_VIRT param)
{
    switch (cmd_id)
    {
        case E_GPD_GET_CMD_GET_PITCH:
            return _GPD_GetPitch((gpd_get_pitch_info *)param);
        default:
            return FALSE;
    }

}

static MS_U32  u32CRC;

static void verGPD_CRC32_Init(void)
{
    u32CRC = 0xFFFFFFFF;
}


static MS_U32 verGPD_CRC32_GetResult(void)
{
    return (~u32CRC);
}


static void verGPD_Init_CRC_Table(MS_U32 *u32CRCtemptable)
{

    MS_U32 i,j,tmp, result;

    for(i=0;i<=0xFF;i++)
    {
        tmp=i;
        result=0;

        for(j=1;j<9;j++)
        {
             if(tmp & 1)
             {
                        result |= (1 << (8 - j));
             }
             tmp >>= 1;
        }

        u32CRCtemptable[i]=result<<24;
        for(j=0; j<8; j++)
        {
            u32CRCtemptable[i] = (u32CRCtemptable[i] << 1) ^ ((u32CRCtemptable[i] & (1 << 31)) ? 0x04C11DB7UL : 0);
        }

        tmp=u32CRCtemptable[i];
        result=0;

        for(j=1;j<33;j++)
        {
             if(tmp & 1)
             {
                result |= (1 << (32 - j));
             }
             tmp >>= 1;
        }
        u32CRCtemptable[i]=result;
        }
}

static void verGPD_CRC32_Update(const MS_U8 *pu8Data, MS_U32 u32Size, MS_U32 u32Width, MS_U32 ocolor)
{
    MS_U32 u32CRCTable[256];
    MS_U32  u32LoopCounter;
    MS_U8 u8BytesPerPixel;
    switch(ocolor)
    {
        case ARGB8888:
            u8BytesPerPixel = 4;
            break;
        case YUV422:
        case RGB565:
        case YVYU422:
        case GRAY_ALPHA:
            u8BytesPerPixel = 2;
            break;
        case GRAY:
        default:
            u8BytesPerPixel = 1;
            break;
    }

    if (pu8Data == NULL || u32Size == 0) return;

    verGPD_Init_CRC_Table(u32CRCTable);

    if((u32Width*u8BytesPerPixel)%8 ==0)
    {
        for (u32LoopCounter=0; u32LoopCounter<u32Size; u32LoopCounter++)
        {
            u32CRC = (u32CRC >> 8) ^ u32CRCTable[ pu8Data[u32LoopCounter] ^ (MS_U8)(u32CRC & 0xFF) ];
        }
    }
    else
    {
        MS_U32 u32LoopCounter2;
        MS_U32 u32WidthAlign;
        u32WidthAlign = u32Width + ((u32Width*u8BytesPerPixel)%8)/u8BytesPerPixel;

        for (u32LoopCounter=0; u32LoopCounter<u32Size/(u32Width*u8BytesPerPixel)*(u32WidthAlign*u8BytesPerPixel); u32LoopCounter+=u32WidthAlign*u8BytesPerPixel)
            for(u32LoopCounter2=0; u32LoopCounter2<u32Width*4; u32LoopCounter2++)
            {
                u32CRC = (u32CRC >> 8) ^ u32CRCTable[ pu8Data[u32LoopCounter+u32LoopCounter2] ^ (MS_U8)(u32CRC & 0xFF) ];
            }
    }
}

MS_U32 _MApi_GPD_GetCRCResult(const MS_U8 *pu8Data, MS_U32 u32Size, MS_U32 u32Width, MS_U32 ocolor)
{
    verGPD_CRC32_Init();
    verGPD_CRC32_Update(pu8Data, u32Size, u32Width, ocolor);
    return verGPD_CRC32_GetResult();
}



static void verGPD_Input_CRC32_Init(void)
{
    u32CRC = 0xFFFFFFFFUL;
}


static MS_U32 verGPD_Input_CRC32_GetResult(void)
{
    return (~u32CRC);
}


static void verGPD_Input_Init_CRC_Table(MS_U32 *u32CRCtemptable)
{

    MS_U32 i,j,tmp, result;

    for(i=0;i<=0xFF;i++)
    {
        tmp=i;
        result=0;

        for(j=1;j<9;j++)
        {
             if(tmp & 1)
             {
                        result |= (1 << (8 - j));
             }
             tmp >>= 1;
        }

        u32CRCtemptable[i]=result<<24;
        for(j=0; j<8; j++)
        {
            u32CRCtemptable[i] = (u32CRCtemptable[i] << 1) ^ ((u32CRCtemptable[i] & (1 << 31)) ? 0x04C11DB7UL : 0);
        }

        tmp=u32CRCtemptable[i];
        result=0;

        for(j=1;j<33;j++)
        {
             if(tmp & 1)
             {
                result |= (1 << (32 - j));
             }
             tmp >>= 1;
        }
        u32CRCtemptable[i]=result;
        }
}

static void verGPD_Input_CRC32_Update(const MS_U8 *pu8Data, MS_U32 u32Size)
{
    MS_U32 u32CRCTable[256];
    MS_U32  u32LoopCounter;

    if (pu8Data == NULL || u32Size == 0) return;

    verGPD_Input_Init_CRC_Table(u32CRCTable);

    #if 0
        MS_U32 i;
        for(i = 0;i<256;i++)
        {
            printf("0x%08x,\n", u32CRCTable[i]);
        }
    #endif

    for (u32LoopCounter=0; u32LoopCounter<u32Size; u32LoopCounter++)
    {
        u32CRC = (u32CRC >> 8) ^ u32CRCTable[ pu8Data[u32LoopCounter] ^ (MS_U8)(u32CRC & 0xFF) ];
    }
}

MS_U32 MApi_GPD_Input_GetCRCResult(const MS_U8 *pu8Data, MS_U32 u32Size)
{
    verGPD_Input_CRC32_Init();
    verGPD_Input_CRC32_Update(pu8Data, u32Size);
    return verGPD_Input_CRC32_GetResult();
}


void *MApi_GPD_Init(MS_PHY SrcBuf)
{
#if (GPD_UTOPIA20)
    if (pInstantGPD == NULL)
    {
        if(UtopiaOpen(MODULE_GPD | GPD_DRIVER_BASE, &pInstantGPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            GPD_API_ERROR("Open GPD fail\n");
            return FALSE;
        }
    }

    if(pInstantGPD==NULL)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    GPD_INIT_PARAM InitPara;
    memset(&InitPara , 0 ,sizeof(GPD_INIT_PARAM));
    InitPara.SrcBuf = SrcBuf;
    GPD_API_INFO("UtopiaIoctl(): MApi_CMD_GPD_Init\n");
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_Init, (void*)(&InitPara)) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    return FALSE;
#else
    return _MApi_GPD_Init(SrcBuf);
#endif

}


MS_S32 MApi_GPD_InputSource(gpd_pic_info* param, MS_PHY InputBuf, MS_U32 Size)
{
#if (GPD_UTOPIA20)
    if(pInstantGPD==NULL)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    GPD_INPUTSOURCE_PARAM InputPara;
    memset(&InputPara , 0 ,sizeof(GPD_INPUTSOURCE_PARAM));
    InputPara.param = param;
    InputPara.InputBuf = InputBuf;
    InputPara.Size = Size;
    GPD_API_INFO("UtopiaIoctl(): MApi_CMD_GPD_InputSource\n");
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_InputSource, (void*)(&InputPara)) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    return InputPara.retVal;
#else
    return _MApi_GPD_InputSource(param, InputBuf, Size);
#endif
}

MS_S32 MApi_GPD_OutputDecode(MS_PHY DecodeBuf, MS_U32 ocolor, MS_U32 Size)
{
#if (GPD_UTOPIA20)
    if(pInstantGPD==NULL)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    GPD_OUTPUTDECODE_PARAM OutputPara;
    memset(&OutputPara , 0 ,sizeof(GPD_OUTPUTDECODE_PARAM));
    OutputPara.DecodeBuf = DecodeBuf;
    OutputPara.ocolor = ocolor;
    OutputPara.Size = Size;
    GPD_API_INFO("UtopiaIoctl(): MApi_CMD_GPD_OutputDecode\n");
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_OutputDecode, (void*)(&OutputPara)) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    else
    {
        if(UtopiaClose(pInstantGPD) != UTOPIA_STATUS_SUCCESS)
        {
            GPD_API_ERROR("Utopia Close GPD failed\n");
            return OutputPara.retVal;
        }
        pInstantGPD = NULL;
    }
    return OutputPara.retVal;
#else
    return _MApi_GPD_OutputDecode(DecodeBuf, ocolor, Size);
#endif
}

MS_S32 MApi_GPD_OutputDecodeMGIF(MS_PHY DecodeBuf, MS_U32 ocolor, MS_U32 Size)
{
#if (GPD_UTOPIA20)
    if(pInstantGPD==NULL)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    GPD_OUTPUTDECODE_PARAM OutputPara;
    memset(&OutputPara , 0 ,sizeof(GPD_OUTPUTDECODE_PARAM));
    OutputPara.DecodeBuf = DecodeBuf;
    OutputPara.ocolor = ocolor;
    OutputPara.Size = Size;
    GPD_API_INFO("UtopiaIoctl(): MApi_CMD_GPD_OutputDecodeMGIF\n");
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_OutputDecodeMGIF, (void*)(&OutputPara)) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    else
    {
        if(UtopiaClose(pInstantGPD) != UTOPIA_STATUS_SUCCESS)
        {
            GPD_API_ERROR("Utopia Close GPD failed\n");
            return OutputPara.retVal;
        }
        pInstantGPD = NULL;
    }
    return OutputPara.retVal;
#else
    return _MApi_GPD_OutputDecodeMGIF(DecodeBuf, ocolor, Size);
#endif
}

MS_S32 MApi_GPD_GetDuration(void)
{
#if (GPD_UTOPIA20)
    if(pInstantGPD==NULL)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    GPD_GETDURATION_PARAM GetDuration;
    memset(&GetDuration , 0 ,sizeof(GPD_GETDURATION_PARAM));
    GPD_API_INFO("UtopiaIoctl(): MApi_CMD_GPD_GetDuration\n");
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_GetDuration, (void*)(&GetDuration)) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    return GetDuration.retVal;
#else
    return _MApi_GPD_GetDuration();
#endif

}


void MApi_GPD_SetGIFMode(MS_U32 mode)
{
#if (GPD_UTOPIA20)
    if(pInstantGPD==NULL)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    GPD_SETGIFMODE_PARAM GIFMode;
    memset(&GIFMode , 0 ,sizeof(GPD_SETGIFMODE_PARAM));
    GIFMode.mode = mode;
    GPD_API_INFO("UtopiaIoctl(): MApi_CMD_GPD_SetGIFMode\n");
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_SetGIFMode, (void*)(&GIFMode)) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
    return;
#else
    return _MApi_GPD_SetGIFMode(mode);
#endif

}

MS_S32 MApi_GPD_OutputDecodeROI(MS_PHY DecodeBuf, MS_U32 ocolor, MS_U32 Size, gpd_roi_info *roi)
{
#if (GPD_UTOPIA20)
    if(pInstantGPD==NULL)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    GPD_OUTPUTDECODEROI_PARAM OutputParaROI;
    memset(&OutputParaROI , 0 ,sizeof(GPD_OUTPUTDECODEROI_PARAM));
    OutputParaROI.DecodeBuf = DecodeBuf;
    OutputParaROI.ocolor = ocolor;
    OutputParaROI.Size = Size;
    OutputParaROI.roi = roi;
    GPD_API_INFO("UtopiaIoctl(): MApi_CMD_GPD_OutputDecodeROI\n");
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_OutputDecodeROI, (void*)(&OutputParaROI)) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    else
    {
        if(UtopiaClose(pInstantGPD) != UTOPIA_STATUS_SUCCESS)
        {
            GPD_API_ERROR("Utopia Close GPD failed\n");
            return OutputParaROI.retVal;
        }
        pInstantGPD = NULL;
    }
    return OutputParaROI.retVal;
#else
    return _MApi_GPD_OutputDecodeROI(DecodeBuf, ocolor, Size, roi);
#endif

}

void MApi_GPD_ScalingEnable(MS_U32 mode)
{
#if (GPD_UTOPIA20)
    if(pInstantGPD==NULL)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    GPD_SCALINGENABLE_PARAM ScalingEnable;
    memset(&ScalingEnable , 0 ,sizeof(GPD_SCALINGENABLE_PARAM));
    ScalingEnable.mode = mode;
    GPD_API_INFO("UtopiaIoctl(): MApi_CMD_GPD_ScalingEnable\n");
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_ScalingEnable, (void*)(&ScalingEnable)) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
    return;
#else
    return _MApi_GPD_ScalingEnable(mode);
#endif

}

void MApi_GPD_ScalingDisable(void)
{
#if (GPD_UTOPIA20)
    if(pInstantGPD==NULL)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    GPD_API_INFO("UtopiaIoctl(): MApi_CMD_GPD_ScalingDisable\n");
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_ScalingDisable, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
    return;
#else
    return _MApi_GPD_ScalingDisable();
#endif
}

MS_U32 MApi_GPD_SetControl(gpd_user_cmd cmd_id, MS_VIRT param)
{
#if (GPD_UTOPIA20)
    if (pInstantGPD == NULL)
    {
        if(UtopiaOpen(MODULE_GPD | GPD_DRIVER_BASE, &pInstantGPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            GPD_API_ERROR("Open GPD fail\n");
            return FALSE;
        }
    }

    if(pInstantGPD==NULL)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    GPD_CONTROL_PARAM CtrlPara;
    memset(&CtrlPara , 0 ,sizeof(GPD_CONTROL_PARAM));
    CtrlPara.cmd_id = cmd_id;
    CtrlPara.param = param;
    GPD_API_INFO("UtopiaIoctl(): MApi_CMD_GPD_SetControl\n");
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_SetControl, (void*)(&CtrlPara)) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    return CtrlPara.retVal;
#else
    return _MApi_GPD_SetControl(cmd_id, param);
#endif

}

MS_U32 MApi_GPD_GetControl(gpd_get_cmd cmd_id, MS_VIRT param)
{
#if (GPD_UTOPIA20)
    if(pInstantGPD==NULL)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    GPD_GET_CONTROL_PARAM CtrlPara;
    memset(&CtrlPara , 0 ,sizeof(GPD_GET_CONTROL_PARAM));
    CtrlPara.cmd_id = cmd_id;
    CtrlPara.param = param;
    GPD_API_INFO("UtopiaIoctl(): MApi_CMD_GPD_GetControl\n");
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_GetControl, (void*)(&CtrlPara)) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    return CtrlPara.retVal;
#else
    return _MApi_GPD_GetControl(cmd_id, param);
#endif

}

MS_U32 MApi_GPD_GetCRCResult(const MS_U8 *pu8Data, MS_U32 u32Size, MS_U32 u32Width, MS_U32 ocolor)
{
#if (GPD_UTOPIA20)
    if(pInstantGPD==NULL)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    GPD_GETCRCRESULT_PARAM CRCPara;
    memset(&CRCPara , 0 ,sizeof(GPD_GETCRCRESULT_PARAM));
    CRCPara.pu8Data = pu8Data;
    CRCPara.u32Size = u32Size;
    CRCPara.u32Width = u32Width;
    CRCPara.ocolor = ocolor;
    GPD_API_INFO("UtopiaIoctl(): MApi_CMD_GPD_GetCRCResult\n");
    if(UtopiaIoctl(pInstantGPD,MApi_CMD_GPD_GetCRCResult, (void*)(&CRCPara)) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    return CRCPara.retVal;
#else
    return _MApi_GPD_GetCRCResult(pu8Data, u32Size, u32Width, ocolor);
#endif

}

