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
//////////////////////////////////////////////////////////////////////////

// ATV Chips
#define CHIP_PAULO      100
#define CHIP_PAULO2     101
#define CHIP_LOLA       102
#define CHIP_LOMEO      103
#define CHIP_LATTE      104
#define CHIP_LASER      105
#define CHIP_LOPEZ      106
#define CHIP_RAPHAEL    107
#define CHIP_MUSE       108
#define CHIP_METIS      109

// DTV Chips
#define CHIP_NEPTUNE    302
#define CHIP_PLUTO      303
#define CHIP_TITANIA1   304
#define CHIP_TITANIA2   305

// Monitor Chips
#define CHIP_OMEGA      502

// Others
#define CHIP_CERAMAL    702
#define CHIP_MOSES      703

#define CHIP_TYPE       CHIP_TITANIA2

//////////////////////////////////////////////////////////////////////////

#define CHIP_GROUP_ATV1     0
#define CHIP_GROUP_ATV2     1
#define CHIP_GROUP_ATV3     2
#define CHIP_GROUP_DTV1     3
#define CHIP_GROUP_DTV2     4
#define CHIP_GROUP_MONITOR1 5
#define CHIP_GROUP_CERAMAL  6
#define CHIP_GROUP_MOSES    7


#if (CHIP_TYPE == CHIP_LOPEZ || CHIP_TYPE == CHIP_RAPHAEL || CHIP_TYPE == CHIP_MUSE || CHIP_TYPE == CHIP_METIS)
    #define CHIP_GROUP      CHIP_GROUP_ATV1
#elif (CHIP_TYPE == CHIP_PAULO || CHIP_TYPE == CHIP_PAULO2 || CHIP_TYPE == CHIP_LOLA || CHIP_TYPE == CHIP_LOMEO || CHIP_TYPE == CHIP_LATTE || CHIP_TYPE == CHIP_LASER)
    #define CHIP_GROUP      CHIP_GROUP_ATV2
#elif (CHIP_TYPE == CHIP_PLUTO || CHIP_TYPE == CHIP_TITANIA1 || CHIP_TYPE == CHIP_NEPTUNE )
    #define CHIP_GROUP      CHIP_GROUP_DTV1
#elif (CHIP_TYPE == CHIP_TITANIA2)
    #define CHIP_GROUP      CHIP_GROUP_DTV2
#elif (CHIP_TYPE == CHIP_OMEGA)
    #define CHIP_GROUP      CHIP_GROUP_MONITOR1
#elif (CHIP_TYPE == CHIP_CERAMAL)
    #define CHIP_GROUP      CHIP_GROUP_CERAMAL
#elif (CHIP_TYPE == CHIP_MOSES)
    #define CHIP_GROUP      CHIP_GROUP_MOSES
#else
#endif

//////////////////////////////////////////////////////////////////////////

#if (CHIP_GROUP == CHIP_GROUP_ATV1)
    #include "MsDBC_LIB_Group_ATV1.h"
#elif (CHIP_GROUP == CHIP_GROUP_ATV2)
    #include "MsDBC_LIB_Group_ATV2.h"
#elif (CHIP_GROUP == CHIP_GROUP_ATV3)
    #include "MsDBC_LIB_Group_ATV3.h"
#elif (CHIP_GROUP == CHIP_GROUP_DTV1)
    #include "MsDBC_LIB_Group_DTV1.h"
#elif (CHIP_GROUP == CHIP_GROUP_DTV2)
    #include "MsDBC_LIB_Group_DTV2.h"
#elif (CHIP_GROUP == CHIP_GROUP_MONITOR1)
    #include "MsDBC_LIB_Group_MONITOR1.h"
#elif (CHIP_GROUP == CHIP_GROUP_CERAMAL)
    #include "MsDBC_LIB_Group_Ceramal.h"
#elif (CHIP_GROUP == CHIP_GROUP_MOSES)
    #include "MsDBC_LIB_Group_Moses.h"
#else
#endif
