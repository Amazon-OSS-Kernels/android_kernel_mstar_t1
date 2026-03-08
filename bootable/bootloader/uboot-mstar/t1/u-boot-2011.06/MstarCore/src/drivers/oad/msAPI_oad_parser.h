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
#ifndef _MSAPI_OAD_PARSER_H_
#define _MSAPI_OAD_PARSER_H_

/*------------------------------------------------------------------------------
*   include files
*-----------------------------------------------------------------------------*/
#include "msAPI_demux.h"

/*------------------------------------------------------------------------------
*   Function prototypes.
*-----------------------------------------------------------------------------*/
MS_U8 *msAPI_OAD_PollingSectionFilter(MS_U8  u08OadFID,MS_U8  *sectionBuffer);
MS_BOOL msAPI_OAD_CreateSectionFilter( MS_U32 u32Type,MS_U8 u08TableId,MS_U16 u16MsgId,MS_U16 u16Pid,MS_U8  *u08OadFID);
void msAPI_OAD_DeleteSectionFilter(MS_U8  *u08OadFID);

#endif // _MSAPI_OAD_PARSER_H_
