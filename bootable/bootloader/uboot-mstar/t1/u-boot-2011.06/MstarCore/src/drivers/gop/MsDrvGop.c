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

#include <common.h>
#include <config.h>
#include <MsOS.h>
#include <apiGOP.h>
#include <apiPNL.h>
#include <MsDebug.h>

#include <gop/MsDrvGop.h>
#include <uboot_mmap.h>
#include <ShareType.h>
#include <MsMmap.h>

#include <bootlogo/MsPoolDB.h>
#include <MsEnvironment.h>
#include <panel/MsDrvPanel.h>
#include "MsBoot.h"

#ifndef UNUSED
#define UNUSED( var )                   ((void)(var))
#endif
U8 u8OsdLayerFbId=0xFF, u8OsdLayerGwinId=0xFF;
U16 u16OsdW = 0,u16OsdH = 0;
U8 g_logoGopIdx = 0;
U8 g_logoMiuIdx = 0;
U8 bDelayLogo = FALSE;
U8 u8OsdLayerFbId_SC=0xFF, u8OsdLayerGwinId_SC=0xFF;
U8 g_logoGopIdx_SC = 1;

MS_BOOL _sc_is_interlace(void)
{
    return 0;
}

MS_U16 _sc_get_h_cap_start(void)
{
    return 0x60;
}
void _Sys_PQ_ReduceBW_ForOSD(MS_U8 PqWin, MS_BOOL bOSD_On)
{
    PqWin=bOSD_On=0;
    UNUSED(PqWin);
    UNUSED(bOSD_On);
}

void MsDrvGOP_SetStretch(GFX_BufferInfo *dst_info, MS_BOOL bHorStretch, MS_BOOL bVerStretch,MS_U16 u16DispX,MS_U16 u16DispY)
{
    U32 u32GWinDispX_Max;
    U32 u32GWinDispY_Max;

    u32GWinDispX_Max = g_IPanel.Width();
    u32GWinDispY_Max = g_IPanel.Height();

    MApi_GOP_GWIN_Set_HSCALE(bHorStretch, dst_info->u32Width, u32GWinDispX_Max);

    MApi_GOP_GWIN_Set_VSCALE(bVerStretch, dst_info->u32Height, u32GWinDispY_Max);

    MApi_GOP_GWIN_Set_STRETCHWIN(g_logoGopIdx, E_GOP_DST_OP0,u16DispX,u16DispY, dst_info->u32Width, dst_info->u32Height);
}


void gop_stretch(U16 graph_pitch, U16 graph_width, U16 graph_height, MS_BOOL bHorStretch, MS_BOOL bVerStretch)
{
    U32 u32GWinDispX_Max;
    U32 u32GWinDispY_Max;


    u32GWinDispX_Max = g_IPanel.Width();
    u32GWinDispY_Max = g_IPanel.Height();

    if (bHorStretch)
        MApi_GOP_GWIN_Set_HSCALE(TRUE, graph_width, u32GWinDispX_Max);
    if (bVerStretch)
        MApi_GOP_GWIN_Set_VSCALE(TRUE, graph_height, u32GWinDispY_Max);

    MApi_GOP_GWIN_Set_STRETCHWIN(g_logoGopIdx, E_GOP_DST_OP0, 0, 0, graph_pitch, graph_height);
}

void MsDrvGFX_Init(void)
{
    GFX_Config tGFXcfg;

    tGFXcfg.bIsCompt = TRUE;
    tGFXcfg.bIsHK = TRUE;
    MApi_GFX_Init(&tGFXcfg);

}
int MsDrvGFX_Blt(GFX_DrawRect *rect,GFX_BufferInfo *src_info, GFX_BufferInfo *dst_info )
{
    GFX_Point gfxPt0 = { rect->dstblk.x, rect->dstblk.y };
    GFX_Point gfxPt1 = { rect->dstblk.x+rect->dstblk.width,rect->dstblk.y+rect->dstblk.height };

    MApi_GFX_SetSrcBufferInfo(src_info, 0);

    MApi_GFX_SetDstBufferInfo(dst_info, 0);

    MApi_GFX_SetClip(&gfxPt0, &gfxPt1);

    MApi_GFX_SetDC_CSC_FMT(GFX_YUV_RGB2YUV_PC, GFX_YUV_OUT_PC, GFX_YUV_IN_255, GFX_YUV_YVYU,  GFX_YUV_YUYV);

    if( (rect->srcblk.width!= rect->dstblk.width)||(rect->srcblk.height!= rect->dstblk.height) )
    {
        MApi_GFX_BitBlt(rect, GFXDRAW_FLAG_SCALE);
    }
    else
    {
        MApi_GFX_BitBlt(rect, 0);
    }
    MApi_GFX_FlushQueue();

    return 1;
    //BitBlt to GOP SD Path

}
void MsDrvGOP_Init(void)
{
    // MS_U8 u8Idx;
    GOP_InitInfo pGopInit;
    //MApi_GOP_RegisterFBFmtCB(( U32(*)(MS_U16 pitch,U32 addr , MS_U16 fmt ))OSD_RESOURCE_SetFBFmt);
    MApi_GOP_RegisterXCIsInterlaceCB(_sc_is_interlace);
    MApi_GOP_RegisterXCGetCapHStartCB(_sc_get_h_cap_start);
    MApi_GOP_RegisterXCReduceBWForOSDCB(_Sys_PQ_ReduceBW_ForOSD);
    pGopInit.u16PanelWidth = g_IPanel.Width();
    pGopInit.u16PanelHeight = g_IPanel.Height();
    pGopInit.u16PanelHStr = g_IPanel.HStart();
    pGopInit.u32GOPRBAdr = 0x0;//((GOP_GWIN_RB_MEMORY_TYPE & MIU1) ? (GOP_GWIN_RB_ADR | MIU_INTERVAL) : (GOP_GWIN_RB_ADR));
    pGopInit.u32GOPRBLen = 0x0;

    //there is a GOP_REGDMABASE_MIU1_ADR for MIU1
    pGopInit.u32GOPRegdmaAdr = 0;//((GOP_REGDMABASE_MEMORY_TYPE & MIU1) ? (GOP_REGDMABASE_ADR | MIU_INTERVAL) : (GOP_REGDMABASE_ADR));
    pGopInit.u32GOPRegdmaLen = 0;
    pGopInit.bEnableVsyncIntFlip = FALSE;

#if ENABLE_STB_ECOS_BOOT
    g_logoGopIdx = 3;
#else
    if (isBootToRecovery() == 0)
    {
        // normal mode bootlogo use bootlogo_gopidx
        // initilize the GOP of Subtitle instead of UI;
        // otherwise, then bootlogo will be closed unexpectedly,
        // because UI is initilalized during booting up
        char *p_str = getenv("bootlogo_gopidx");
        if(p_str!=NULL)
        {
            g_logoGopIdx = simple_strtoul(p_str, NULL, 10);
        }
        else
        {
            st_sys_misc_setting misc_data;
            Read_MiscSetting_ToFlash(&misc_data);
            g_logoGopIdx = misc_data.m_u8LogoGopIdx;
        }
    } else {
        // recovery mode bootlogo use gop 0
        g_logoGopIdx = 0;
    }
#endif


    MApi_GOP_GWIN_SetForceWrite(TRUE);
    MApi_GOP_InitByGOP(&pGopInit, g_logoGopIdx);
    MApi_GOP_GWIN_SwitchGOP(g_logoGopIdx);

#if (ENABLE_HDMITX_MSTAR_ROCKET==1)
    MApi_GOP_GWIN_EnableT3DMode(FALSE);
#endif

    if (TRUE == bDelayLogo)
    {
        // read the MIU in which the bootlogo stored
        UBOOT_DEBUG("BOOTLOGO DELAY  SET GOP INDEX[%d]\n",g_logoGopIdx);
        MApi_GOP_MIUSel(g_logoGopIdx, g_logoMiuIdx);
    }
    else
   {
        UBOOT_DEBUG("BOOTLOGO DELAY FAIL !!!! SET GOP INDEX[%d]\n",g_logoGopIdx);
   }
    char * p_str = getenv("GOP_SET_MUX");
    if(p_str != NULL)
    {
        int i = 0;
        int j = 0;
        U8 au8GopMux[4] = {0};
        GOP_MuxConfig stMuxCfg;

        memset(&stMuxCfg, 0, sizeof(GOP_MuxConfig));

        while(p_str[i] != '\0')
        {
            if((p_str[i] != ':') && (p_str[i] >= 0x30) &&(p_str[i] <= 0x39))
            {
                au8GopMux[j] = p_str[i] - 0x30;
                stMuxCfg.GopMux[j].u8GopIndex = au8GopMux[j];
                stMuxCfg.GopMux[j].u8MuxIndex = j;
                stMuxCfg.u8MuxCounts = ++j;
            }

            i++;
        }

        MApi_GOP_GWIN_SetMux(&stMuxCfg, sizeof(GOP_MuxConfig));
    }

#if(CONFIG_TV_CHIP == 0)
    MApi_GOP_GWIN_OutputColor(GOPOUT_YUV);	//for BOX
#else
    MApi_GOP_GWIN_OutputColor(GOPOUT_RGB);  //TV
#endif

#if defined(CONFIG_MSTAR_MASERATI) || defined(CONFIG_MSTAR_KASTOR) || defined(CONFIG_MSTAR_MACAN) || defined(CONFIG_MSTAR_CURRY) || defined(CONFIG_MSTAR_MAXIM)
    st_gopmux_para gopSetting_para;
	Read_Gop_Mux_ParaFromFlash(&gopSetting_para);
	if (gopSetting_para.m_u8SettingType == 1)
	{
    	GOP_LayerConfig layerConfig;
    	layerConfig.u32LayerCounts = gopSetting_para.m_u16SettingCounts;
    	layerConfig.stGopLayer[0].u32LayerIndex = 0;
    	layerConfig.stGopLayer[0].u32GopIndex = gopSetting_para.m_u16Setting0GopIndex;
    	layerConfig.stGopLayer[1].u32LayerIndex = 1;
    	layerConfig.stGopLayer[1].u32GopIndex = gopSetting_para.m_u16Setting1GopIndex;
    	layerConfig.stGopLayer[2].u32LayerIndex = 2;
    	layerConfig.stGopLayer[2].u32GopIndex = gopSetting_para.m_u16Setting2GopIndex;
    	layerConfig.stGopLayer[3].u32LayerIndex = 3;
    	layerConfig.stGopLayer[3].u32GopIndex = gopSetting_para.m_u16Setting3GopIndex;
    	layerConfig.stGopLayer[4].u32LayerIndex = 4;
    	layerConfig.stGopLayer[4].u32GopIndex = gopSetting_para.m_u16Setting4GopIndex;
    	MApi_GOP_GWIN_SetLayer(&layerConfig, sizeof(GOP_LayerConfig));
	}
	else
	{
    	GOP_MuxConfig muxConfig;
    	muxConfig.u8MuxCounts = gopSetting_para.m_u16SettingCounts;
    	muxConfig.GopMux[0].u8MuxIndex = EN_GOP_MUX0;
    	muxConfig.GopMux[0].u8GopIndex = gopSetting_para.m_u16Setting0GopIndex;
    	muxConfig.GopMux[1].u8MuxIndex = EN_GOP_MUX1;
    	muxConfig.GopMux[1].u8GopIndex = gopSetting_para.m_u16Setting1GopIndex;
    	muxConfig.GopMux[2].u8MuxIndex = EN_GOP_MUX2;
    	muxConfig.GopMux[2].u8GopIndex = gopSetting_para.m_u16Setting2GopIndex;
    	muxConfig.GopMux[3].u8MuxIndex = EN_GOP_MUX3;
    	muxConfig.GopMux[3].u8GopIndex = gopSetting_para.m_u16Setting3GopIndex;
    	muxConfig.GopMux[4].u8MuxIndex = EN_GOP_MUX4;
    	muxConfig.GopMux[4].u8GopIndex = gopSetting_para.m_u16Setting4GopIndex;
    	MApi_GOP_GWIN_SetMux(&muxConfig,sizeof(muxConfig));
	}
#endif

    /* avoiding the fail log in getwinfb func while in init stage
    for(u8Idx=0;u8Idx< MApi_GOP_GWIN_GetGwinNum(g_logoGopIdx);u8Idx++)
    {
        MApi_GOP_GWIN_DeleteWin(u8Idx);
    }
    */
    MApi_GOP_GWIN_SetForceWrite(FALSE);

}

void MsDrvGOP_Show(GFX_BufferInfo *dst_info,MS_BOOL bHorStretch, MS_BOOL bVerStretch,MS_U16 u16DispX,MS_U16 u16DispY)
{
    MApi_GOP_GWIN_SetForceWrite(TRUE);
    EN_GOP_CONSALPHA_BITS enGOPConsAlphaBit;
    MApi_GOP_GWIN_SwitchGOP(g_logoGopIdx);

#if  (CONFIG_TV_CHIP==1)
    if ((LINK_VBY1_10BIT_16LANE == getLinkExtType()) ||
        (LINK_VBY1_8BIT_16LANE == getLinkExtType())
        )
    {
        MApi_GOP_GWIN_SetGOPDst(g_logoGopIdx, E_GOP_DST_OP_DUAL_RATE);
    }
    else
    {
        MApi_GOP_GWIN_SetGOPDst(g_logoGopIdx, E_GOP_DST_OP0);
    }
#else

    MApi_GOP_GWIN_SetGOPDst(g_logoGopIdx, E_GOP_DST_OP0);

#endif

    MsDrvGOP_SetStretch(dst_info, bHorStretch, bVerStretch,u16DispX,u16DispY);

    u8OsdLayerFbId = MApi_GOP_GWIN_GetFreeFBID();
    u8OsdLayerGwinId = MApi_GOP_GWIN_GetFreeWinID();

    MApi_GOP_GWIN_CreateFBbyStaticAddr(u8OsdLayerFbId, 0, 0, dst_info->u32Width, dst_info->u32Height, E_MS_FMT_YUV422, dst_info->u32Addr);

    MApi_GOP_GWIN_MapFB2Win(u8OsdLayerFbId, u8OsdLayerGwinId);
    //jh patch for k3 need fix after sn has right dac panel ini
    MApi_GOP_GetChipCaps(E_GOP_CAP_CONSALPHA_VALIDBITS, &enGOPConsAlphaBit,sizeof(enGOPConsAlphaBit));
    MApi_GOP_SetConfig(E_GOP_CONSALPHA_BITS, &enGOPConsAlphaBit);
    MApi_GOP_GWIN_SetBlending(u8OsdLayerGwinId, FALSE, 0xFF);

    MApi_GOP_GWIN_SetForceWrite(FALSE);
    MApi_GOP_GWIN_Enable(u8OsdLayerGwinId, TRUE);

}

#if (CONFIG_BOOTVIDEO == 1)
static MS_BOOL _MsDrvGOP_EnableDwinCap(MS_BOOL bCfgOnly)
{
    MMapInfo_s stMmapInfo;
    GOP_DwinProperty stDwinProperty;
    E_GOP_API_Result eDwinRet;

    memset(&stMmapInfo, 0, sizeof(MMapInfo_s));
    if(get_mmap("E_DFB_FRAMEBUFFER", &stMmapInfo)!=0)
    {
        UBOOT_ERROR("get E_DFB_FRAMEBUFFER fail\n");
        return FALSE;
    }

    // GOP DWIN init
    eDwinRet = MApi_GOP_DWIN_Init();
    if(eDwinRet != GOP_API_SUCCESS)
    {
        UBOOT_ERROR("Error 0x%x: failed to MApi_GOP_DWIN_Init()\n", eDwinRet);
        return FALSE;
    }

    //Source can only from Scaler OP
    eDwinRet = MApi_GOP_DWIN_SetSourceSel(DWIN_SRC_OP);
    if(eDwinRet != GOP_API_SUCCESS)
    {
        UBOOT_ERROR("Error 0x%x: failed to MApi_GOP_DWIN_SetSourceSel(DWIN_SRC_OP)\n", eDwinRet);
        return FALSE;
    }

    //OP scan type is progressive
    eDwinRet = MApi_GOP_DWIN_SelectSourceScanType(DWIN_SCAN_MODE_PROGRESSIVE);
    if(eDwinRet != GOP_API_SUCCESS)
    {
        UBOOT_ERROR("Error 0x%x: failed to MApi_GOP_DWIN_SelectSourceScanType(DWIN_SCAN_MODE_PROGRESSIVE)\n", eDwinRet);
        return FALSE;
    }

    //Data format only YUV; Set data format to YUV422
    eDwinRet = MApi_GOP_DWIN_SetDataFmt(DWIN_DATA_FMT_UV8Y8);
    if(eDwinRet != GOP_API_SUCCESS)
    {
        UBOOT_ERROR("Error 0x%x: failed to MApi_GOP_DWIN_SetDataFmt(DWIN_DATA_FMT_UV8Y8)\n", eDwinRet);
        return FALSE;
    }

    memset(&stDwinProperty, 0, sizeof(GOP_DwinProperty));
    // Set DWin property
    stDwinProperty.u16x = 0;
    stDwinProperty.u16y = 0;
    stDwinProperty.u16fbw = g_IPanel.Width();
    stDwinProperty.u16w = g_IPanel.Width();
    stDwinProperty.u16h = g_IPanel.Height();
    stDwinProperty.u32fbaddr0 = stMmapInfo.u32Addr;
    stDwinProperty.u32fbaddr1 = stMmapInfo.u32Addr + stMmapInfo.u32Size;
    eDwinRet = MApi_GOP_DWIN_SetWinProperty(&stDwinProperty);
    if(eDwinRet != GOP_API_SUCCESS)
    {
        UBOOT_ERROR("Error 0x%x: failed to MApi_GOP_DWIN_SetWinProperty()!\n", eDwinRet);
        return FALSE;
    }

    if(bCfgOnly)
        return TRUE;

    // start DWIN capture
    eDwinRet = MApi_GOP_DWIN_Enable(TRUE);
    if(eDwinRet != GOP_API_SUCCESS)
    {
        UBOOT_ERROR("Error 0x%x: failed to MApi_GOP_DWIN_Enable(TRUE)!\n", eDwinRet);
        return FALSE;
    }

    return TRUE;
}

void MsDrvGOP_Hide(void)
{
    if(u8OsdLayerGwinId == 0xFF)
        return;

    // Prepare DWIN capture
    //_MsDrvGOP_EnableDwinCap(TRUE);

    // hide bootlogo
    MApi_GOP_GWIN_Enable(u8OsdLayerGwinId, FALSE);

}
#endif  // #if (CONFIG_BOOTVIDEO == 1)

#if CONFIG_MINIUBOOT
#else
#if (ENABLE_DISPLAY_LOGO) // GEGOP
int do_destroy_logo (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    if (u8OsdLayerGwinId != 0xFF)
    {
        MApi_GOP_GWIN_Set_HSCALE(FALSE, 0, 0);
        MApi_GOP_GWIN_Set_VSCALE(FALSE, 0, 0);
        MApi_GOP_GWIN_Enable(u8OsdLayerGwinId, FALSE);
        MApi_GOP_GWIN_DestroyWin(u8OsdLayerGwinId);
        u8OsdLayerGwinId = 0xFF;
    }

    if (u8OsdLayerFbId != 0xFF)
    {
        MApi_GOP_GWIN_DestroyFB(u8OsdLayerFbId);
        u8OsdLayerFbId = 0xFF;
    }
    return 0;
}

U_BOOT_CMD(
    destroy_logo,    1,   1,  do_destroy_logo,
    "destroy_logo - Stop logo display \n",
    "Stop logo display\n"
);
#endif
#endif
