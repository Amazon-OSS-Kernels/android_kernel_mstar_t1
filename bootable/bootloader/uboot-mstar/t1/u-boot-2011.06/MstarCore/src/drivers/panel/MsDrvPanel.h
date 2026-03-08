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

#ifndef _MS_DRV_PNL_
#define _MS_DRV_PNL_
#include <apiPNL.h>
#include <panel/panel.h>

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif


MS_BOOL MsDrv_PNL_Init(PanelType*  panel_data);
void MsDrv_PNL_BackLigth_On(void);
int MDrv_XC_Sys_Init_XC(void);
void setLinkExtType(APIPNL_LINK_EXT_TYPE PanelLinkType);
APIPNL_LINK_EXT_TYPE getLinkExtType(void);


#endif

