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
#if defined (MSOS_TYPE_LINUX)
#include "frhsl_ringbuffer.h"
#include "frhsl_cfg.h"
#include "frhsl_RAWLOB.h"

#include "MsCommon.h"
#include "drvWDT.h"

#include <pthread.h>

static MS_BOOL _bFrHslReset = FALSE;

#define COUNTERS_PER_US 12 /* 1 counter = (1/12M) sec  */

/*******************************************************************************
* Function    : frHslReset
* Group       : PC Simulation, Framework, HSL, Internals
* Parameter   :
* Returns     : nothing
* Description :
*******************************************************************************/
void FrHslResetConnection(stHslController* const _hsl)
{
  _bFrHslReset = TRUE;

    CmdInfo cmdinfo;
    cmdinfo.u16Cmd = HSL_CMD_RESET;

    bufferProcessCommand(_hsl, &cmdinfo);
}

/*******************************************************************************
* Function    : FrHslInitialiseBegin
* Group       : PC Simulation, Framework, HSL, Internals
* Parameter   : none
* Returns     : nothing
* Description : This function is called to perform any initialisation prior
*               to other generic initialisation configuration.
*******************************************************************************/
MS_BOOL FrHslInitialiseBegin(void)
{
    /* Initialise the counter for geting timestamp info. */

    MS_BOOL bRet = FALSE;

    if ( E_WDT_OK == MDrv_WDT_Init(E_WDT_DBGLV_ERR_ONLY) )    
    {       
        MDrv_TIMER_Count(E_TIMER_1, ENABLE);        
        bRet = TRUE;    
    }

    return bRet;
}

/*******************************************************************************
* Function    : FrHslInitialiseEnd
* Group       : Linux uart, Framework, HSL, Internals
* Parameter   : none
* Returns     : nothing
* Description : This function is called to indicate when initialisation is
*               completed and we are ready to do some logging.
*******************************************************************************/
MS_BOOL FrHslInitialiseEnd(void)
{
    /* Nothing to do as stdout doesn't need closing here.
    However we could possible output some extra information to let the
    PC know that the logging can start.
    Typical info could be outputting a single message log point.
    This is useful to help the PC detect that the target system has been reset.*/

    return TRUE;
}

MS_BOOL FrHslStop(void)
{
    return TRUE;
}

/*******************************************************************************
* Function    : FrHslWritePayload
* Group       : Linux uart, Framework, HSL, Internals
* Parameter   : MS_BOOL: is the last payload?
* Parameter   : MS_U16: payload to write to the given file descriptor.
* Parameter   : MS_U8*: buffer to be written to
* Returns     : nothing
* Description : This function is used to write payload
*******************************************************************************/
void FrHslWritePayload (const MS_BOOL bLast, const MS_U16 u16Payload, MS_U8 *pu8Buff)
{
    if (bLast)
    {
        pu8Buff[HSL_RAWLOB_OFFSET_OF_PAYLOAD_MARKER_IN_PACKET] = HSL_RAWLOB_LAST_PAYLOAD_MARKER;
    }
    else
    {
        pu8Buff[HSL_RAWLOB_OFFSET_OF_PAYLOAD_MARKER_IN_PACKET] = HSL_RAWLOB_MORE_PAYLOAD_MARKER;
    }

    memcpy(&pu8Buff[HSL_RAWLOB_OFFSET_OF_PAYLOAD_IN_PACKET], &u16Payload, 2);
}

/*******************************************************************************
* Function    : FrHslWriteHeader
* Group       : Linux uart, Framework, HSL, Internals
* Parameter   : MS_U8: Message type
* Parameter   : MS_U16: Header id write to the given file descriptor.
* Parameter   : MS_U8*: buffer to be written to
* Returns     : nothing
* Description : This function is used to write header
*******************************************************************************/
void FrHslWriteHeader (const MS_U8 u8MsgType, const MS_U16 u16ID, MS_U8 *pu8Buff, MS_U32 u32Time)
{
    MS_U16 u16TimeStamp = ~((MS_U16)u32Time);

    pu8Buff[HSL_RAWLOB_OFFSET_OF_MSG_TYPE] = u8MsgType;

    memcpy(&pu8Buff[HSL_RAWLOB_OFFSET_OF_MSG_ID], &u16ID, 2);

    memcpy(&pu8Buff[HSL_RAWLOB_OFFSET_OF_TIME_STAMP], &u16TimeStamp, 2);
}

#endif
