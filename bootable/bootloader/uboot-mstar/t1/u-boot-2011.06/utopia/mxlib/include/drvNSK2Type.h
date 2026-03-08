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

#ifndef _DRV_NSK2TYPE_H_
#define _DRV_NSK2TYPE_H_

////////////////////////////////////////////////////////////////////////////////
/// @file drvNSK2Type.h
/// @brief
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////

#define NSK2_DBGLV_EMERG     0  //Used for emergency messages, usually those that precede a crash.
#define NSK2_DBGLV_ALERT     1  //A situation requiring immediate action.
#define NSK2_DBGLV_CRIT      2  //Critical conditions, often related to serious hardware or software failures.
#define NSK2_DBGLV_ERR       3  //Used to report error conditions; device drivers often use KERN_ERR to report hardware difficulties.
#define NSK2_DBGLV_WARNING   4  //Warnings about problematic situations that do not, in themselves, create serious problems with the system.
#define NSK2_DBGLV_NOTICE    5  //Situations that are normal, but still worthy of note. A number of security-related conditions are reported at this level.
#define NSK2_DBGLV_INFO      6  //Informational messages. Many drivers print information about the hardware they find at startup time at this level.
#define NSK2_DBGLV_DEBUG     7  //Used for debugging messages.
#define NSK2_DBGLV_ARRAY     8  //Uesd for array data.


#define EMM_DBGLV_EMERG     0  //Used for emergency messages, usually those that precede a crash.
#define EMM_DBGLV_ALERT     1  //A situation requiring immediate action.
#define EMM_DBGLV_CRIT      2  //Critical conditions, often related to serious hardware or software failures.
#define EMM_DBGLV_ERR       3  //Used to report error conditions; device drivers often use KERN_ERR to report hardware difficulties.
#define EMM_DBGLV_WARNING   4  //Warnings about problematic situations that do not, in themselves, create serious problems with the system.
#define EMM_DBGLV_NOTICE    5  //Situations that are normal, but still worthy of note. A number of security-related conditions are reported at this level.
#define EMM_DBGLV_INFO      6  //Informational messages. Many drivers print information about the hardware they find at startup time at this level.
#define EMM_DBGLV_DEBUG     7  //Used for debugging messages.
#define EMM_DBGLV_ARRAY     8  //Uesd for array data.


#endif


