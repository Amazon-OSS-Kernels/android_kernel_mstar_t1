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
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define PQTABLE_NAME    MAIN                // table config parameter
#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <pthread.h>
#endif

#include "MsTypes.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"

#include "color_reg.h"
#include "drvPQ_Define.h"
#include "drvPQ_Declare.h"
#include "drvPQ.h"
#include "hwreg_utility2.h"

#define PQTBL_REGTYPE       PQTBL_NORMAL    // table config parameter

#include "Maxim_Main.c"                  // table config parameter
#include "Maxim_Main_1920.c"             // table config parameter
#include "Maxim_Main_1920_PIP.c"         // table config parameter
#include "Maxim_Main_1920_POP.c"         // table config parameter
#include "Maxim_Main_1366.c"             // table config parameter
#include "Maxim_Main_1366_PIP.c"         // table config parameter
#include "Maxim_Main_1366_POP.c"         // table config parameter
#include "Maxim_Main_4K2K.c"             // table config parameter
#include "Maxim_Main_4K2K_PIP.c"         // table config parameter
#include "Maxim_Main_4K2K_POP.c"         // table config parameter
#include "Maxim_Main_HSDRule.c"          // table config parameter
#include "Maxim_Main_VSDRule.c"          // table config parameter
#include "Maxim_Main_HSPRule.c"          // table config parameter
#include "Maxim_Main_VSPRule.c"          // table config parameter
#include "Maxim_Main_CSCRule.c"          // table config parameter
#include "Maxim_Main_GRule.c"            // table config parameter
#include "Maxim_Main_1920_GRule.c"       // table config parameter
#include "Maxim_Main_1920_PIP_GRule.c"   // table config parameter
#include "Maxim_Main_1920_POP_GRule.c"   // table config parameter
#include "Maxim_Main_1366_GRule.c"       // table config parameter
#include "Maxim_Main_1366_PIP_GRule.c"   // table config parameter
#include "Maxim_Main_1366_POP_GRule.c"   // table config parameter
#include "Maxim_Main_4K2K_GRule.c"       // table config parameter
#include "Maxim_Main_4K2K_PIP_GRule.c"   // table config parameter
#include "Maxim_Main_4K2K_POP_GRule.c"   // table config parameter

#include "QualityMode.h"
#include "drvPQ_Datatypes.h"
#include "mhal_pq.h"
#include "drvPQ_Template.h"

