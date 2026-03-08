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
#ifndef __SPINAND_CONFIG_H__
#define __SPINAND_CONFIG_H__

//=====================================================
// select a HW platform:
//   - 1: enable, 0: disable.
//   - only one platform can be 1, others have to be 0.
//=====================================================
#define SPINAND_DRV_TV_MBOOT           1


//=====================================================
// do NOT edit the following content.
//=====================================================
#include <config.h>

#if defined(SPINAND_DRV_TV_MBOOT) && SPINAND_DRV_TV_MBOOT
  #include <config.h>

  #if (ENABLE_NUGGET)
  #include "drvSPINAND_nugget_uboot.h"
  #elif (ENABLE_NIKON)
  #include "drvSPINAND_nikon_uboot.h"
  #elif (ENABLE_MILAN)
  #include "drvSPINAND_milan_uboot.h"
  #elif (ENABLE_MARLON)
  #include "drvSPINAND_marlon_uboot.h"
  #elif (ENABLE_KENYA)
  #include "drvSPINAND_kenya_uboot.h"
  #elif (ENABLE_KRITI)
  #include "drvSPINAND_kriti_uboot.h"
  #elif (ENABLE_KRATOS)
  #include "drvSPINAND_kratos_uboot.h"
  #else
  #error "Wrong chip setting!"
  #endif
#else
  #error "Error! no platform selected."
#endif
extern void flush_cache(U32 start_addr, U32 size);

#endif /* __UNFD_CONFIG_H__ */
