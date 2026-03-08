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

#ifndef _INTERN_DVBC_H_
#define _INTERN_DVBC_H_

#ifdef _INTERN_DVBC_C_
#define EXTSEL
#else
#define EXTSEL extern
#endif

typedef enum
{
    QAM_FEC_LOCK,
    QAM_PSYNC_LOCK,
    QAM_TPS_LOCK,
    QAM_DCR_LOCK,
    QAM_AGC_LOCK,
    QAM_MODE_DET,

} QAM_LOCK_STATUS;


//--------------------------------------------------------------------
//EXTSEL BOOLEAN INTERN_DVBC_Reset ( void );
EXTSEL BOOLEAN INTERN_DVBC_Active(BOOLEAN bEnable);
//EXTSEL BOOLEAN INTERN_DVBC_Cmd_Packet_Send(S_CMDPKTREG* pCmdPacket, U8 param_cnt);
//EXTSEL BOOLEAN INTERN_DVBC_Get_TPS_Parameter_Const( WORD * TSP_parameter);
EXTSEL BOOLEAN INTERN_DVBC_Exit ( void );
//--------------------------------------------------------------------


#undef EXTSEL
#endif

