/**
* Copyright (c) 2006 ¡V 2016 MStar Semiconductor, Inc.
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

#ifndef _MS_API_PANEL_
#define _MS_API_PANEL_
#include <MsTypes.h>
#include <apiPNL.h>
#include <panel/panel.h>

/// Max order Size  for CH order
#define MAX_ORDER_SIZE      8

typedef enum
{
    //this valuse is offset in chunk header.
    VB1_CH_ORDER_16V=0x00,
    VB1_CH_ORDER_8V,
    VB1_CH_ORDER_4V,
    VB1_CH_ORDER_2V,
    VB1_CH_ORDER_1V,
    VB1_CH_ORDER_4O,
    VB1_CH_ORDER_2O,
    VB1_CH_ORDER_ORTHER=0xFF
} EN_VB1_CH_ORTHER_ITEM;

/// VB1ChannelOrder
typedef struct
{
    // 16V VB1 Channel order, Max Number @ref MAX_ORDER_SIZE
    MS_U8              u8Vb116vOrder[MAX_ORDER_SIZE];
    // 8V VB1 Channel order, Max Number @ref MAX_ORDER_SIZE
    MS_U8              u8Vb18vOrder[MAX_ORDER_SIZE];
    // 4V VB1 Channel order, Max Number @ref MAX_ORDER_SIZE
    MS_U8              u8Vb14vOrder[MAX_ORDER_SIZE];
    // 2V VB1 Channel order, Max Number @ref MAX_ORDER_SIZE
    MS_U8              u8Vb12vOrder[MAX_ORDER_SIZE];
    // 1V VB1 Channel order, Max Number @ref MAX_ORDER_SIZE
    MS_U8              u8Vb11vOrder[MAX_ORDER_SIZE];
    // 4O VB1 Channel order, Max Number @ref MAX_ORDER_SIZE
    MS_U8              u8Vb14OOrder[MAX_ORDER_SIZE];
    // 2O VB1 Channel order, Max Number @ref MAX_ORDER_SIZE
    MS_U8              u8Vb12OOrder[MAX_ORDER_SIZE];
}pnl_VB1ChannelOrder;

typedef struct
{
    MS_U8 URSA_6M30_LVDS_CHANNEL_SWAP_MODE;
    MS_U8 URSA_6M30_IN_LVDS_TIMODE;
    MS_U8 URSA_6M30_IN_LVDS_TI_BITNUM;
    MS_U8 URSA_6M30_OUT_LVDS_TIMODE;
    MS_U8 URSA_6M30_OUT_LVDS_TI_BITNUM;
    MS_U8 URSA_6M30_SSC_SEL;
    MS_U8 URSA_6M30_SSC_ENABLE;
    MS_U16 URSA_6M30_SSC_FREQ;
    MS_U16 URSA_6M30_SSC_PERCENT;
}ursa_6m30_cmd_table;

typedef struct
{
    MS_U8 URSA_EXTERNAL;
    MS_U8 URSA_MEMC_ENABLE;
    MS_U8 URSA_LVDS_CHANNEL_SWAP_MODE;
    MS_U8 URSA_LVDS_SWAP_POL;
    MS_U8 URSA_IN_LVDS_TIMODE;
    MS_U8 URSA_IN_LVDS_TI_BITNUM;
    MS_U8 URSA_OUT_LVDS_TIMODE;
    MS_U8 URSA_OUT_LVDS_TI_BITNUM;
    MS_U8 URSA_SSC_SEL;
    MS_U8 URSA_SSC_ENABLE;
    MS_U16 URSA_SSC_FREQ;
    MS_U16 URSA_SSC_PERCENT;
	MS_U16 URSA_OSD_PROTECT_MODE;
}ursa_cmd_table;

typedef struct
{
    MS_U8   m_u8BOARD_PWM_PORT;
    MS_U16  m_u16BOARD_LVDS_CONNECT_TYPE;
    MS_U8   m_bPANEL_PDP_10BIT;
    MS_U8   m_bPANEL_SWAP_LVDS_POL;
    MS_U8   m_bPANEL_SWAP_LVDS_CH;
    MS_U8   m_bPANEL_CONNECTOR_SWAP_PORT;
    MS_U16  m_u16LVDS_PN_SWAP_L;
    MS_U16  m_u16LVDS_PN_SWAP_H;
    char    m_sGPIO1_PAD_NAME[32];
    MS_U16  m_u16GPIO1_INDEX;
    MS_U8   m_u8GPIO1_VALUE;
    char    m_sGPIO2_PAD_NAME[32];
    MS_U16  m_u16GPIO2_INDEX;
    MS_U8   m_u8GPIO2_VALUE;
}st_board_para;

int do_panel_init( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_panel_pre_init( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

int do_backLigth_on( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_local_dimming( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

//-------------------------------------------------------------------------------------------------
/// Query panle status
/// @return                                   \b OUT: 0: panel is ready, -1: panel is not ready
//-------------------------------------------------------------------------------------------------
int IsPanelReady(void);
APIPNL_LINK_EXT_TYPE GetPanelLinkExtType(void);

typedef PANEL_RESOLUTION_TYPE (*GetPnlTypeSet_cb)(void);
void RegisterCBGetPnlTypeSetting(GetPnlTypeSet_cb cb);


#endif

