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
#if defined (MSOS_TYPE_LINUX)
#ifndef _FRHSL_DATALINK_
#define _FRHSL_DATALINK_

#include "MsTypes.h"

//---------------------------------------------------
// Message's Body
//---------------------------------------------------
// MS_U8 | MS_U8 | MS_U32 | MS_U32 |
// ---------------------------------
// GetCardStatusReq
// 0x01  | SN    |
// GetCardStatusResp
// 0x02  | SN    |used    |total   |
// GetDataReq
// 0x03  | SN    |req.size|
// GetDataResp
// 0x04  | SN    |size    | DATA (size) bytes ...
// SendCommandReq
// 0x05  | SN    |snd.size| DATA (size) bytes ...
// SendCommandResp
// 0x06  | SN    |err.code|
// SendFilterArrayReq
// 0x07  | SN    |snd.size| Byte array from Genie
// SendFilterArrayResp
// 0x08  | SN    |(MS_U8)[0|1]|

#define LINK_PROTOCOL_VERSION           0x02

#define LINK_BUFFER_SIZE        (32)
#define LINK_MAX_COMMAND        (sizeof(ST_HSL_CMD_REQ))
// connection timeout [s].
#define LINK_CONNECTION_TIMEOUT (15)

#define LINK_HEADER_SIZE                2
#define LINK_DATA_GET_CARD_STATUS       0
#define LINK_DATA_RESPONSE_CARD_STATUS  8
#define LINK_DATA_GET_HSL_DATA          4
#define LINK_DATA_RESPONSE_HSL_DATA     4
#define LINK_DATA_SEND_HSL_CMD          4
#define LINK_DATA_RESPONSE_HSL_CMD      4

typedef enum EnLinkCommandTag
{
    EN_LINK_CMD_NONE                 = 0x00,
    EN_LINK_CMD_GET_CARD_STATUS      = 0x01,
    EN_LINK_CMD_RESP_CARD_STATUS     = 0x02,
    EN_LINK_CMD_GET_HSL_DATA         = 0x03,
    EN_LINK_CMD_RESP_HSL_DATA        = 0x04,
    EN_LINK_CMD_SEND_HSL_CMD         = 0x05,
    EN_LINK_CMD_RESP_HSL_CMD         = 0x06,
    EN_LINK_CMD_SND_FILTER_MX        = 0x07,
    EN_LINK_CMD_RSP_FILTER_MX        = 0x08
} EN_LINK_CMD;

/* ===================================== */
/* COMMAND LAYER */

typedef enum EnCmdErrorCodeTag
{
    EN_CMD_EC_OK                      = 0,
    EN_CMD_EC_VERSION,
    EN_CMD_EC_DATA_SIZE,
    EN_CMD_EC_AT_FAILURE,
    EN_CMD_EC_THREAD_FAIL,
    EN_CMD_EC_CMD_UNKNOWN,
    EN_CMD_EC_LAST
} EN_CMD_ERROR_CODE;

typedef enum EnCommandIdTag
{
    EN_CMD_TEST_NONE                  = 0x00,
    EN_CMD_TEST_AT                    = 0x01,
    EN_CMD_TEST_LOAD                  = 0x02,
    EN_CMD_TEST_CMD_LAST
} EN_COMMAND_ID;

typedef enum EnResponseIdTag
{
    EN_RESP_TEST_AT = 0,
    EN_RESP_TEST_AT_ECHO = 1,
    EN_RESP_TEST_CMD_LAST
} EN_RESPONSE_ID;

#define CMD_MAX_AT              (128)
#define RSP_MAX_AT              (1024)
#define CMD_MAX_FILTER          (10000)  //maximum single use local variable
#define HSL_MAX_LOG_POINT_SIZE (20*1000)   // Maximum size of GKI signal that can be handled by Genie.
#define HSL_MAX_SILO_ARRAY_SIZE (HSL_MAX_LOG_POINT_SIZE - 4)  // The -4 accounts for length field of HslLogPointRawData (with 2 bytes padding)

typedef struct StHslSelftestCmd
{
    MS_U32  mRunTime;
    MS_U32  mThreadDelay;
    MS_U16  mThreadNo;
    MS_U16  mCntSimpleLP;
    MS_U16  mCntPrintf1p;
    MS_U16  mParamPrintf1p;
    MS_U16  mCntArray16;
    MS_U16  mParamArray16Size;
    MS_U16  mCntArray32;
    MS_U16  mParamArray32Size;
    MS_U16  mCntTypedef;
} ST_HSL_SELFTEST_CMD;

typedef struct StTestStruct
{
    MS_U32 a;
    MS_U32 b;
    MS_U32 c;
} ST_TEST_STRUCT;

typedef union U_HslCmdReqTag
{
  char    atCmd[CMD_MAX_AT];  /**< \assoc STRING */
  ST_HSL_SELFTEST_CMD loadTestCmd;
} U_HslCmdReq;

typedef struct ST_HSL_CMD_REQTag
{
    MS_U8                 ver;
    /** \assoc UNION \ref U_Cmd */
    EN_COMMAND_ID         cmdId;
    U_HslCmdReq           U_Cmd;
} ST_HSL_CMD_REQ;


#define HSL_CMD_RESP_LOG_POINT_ID (0x000A)  /* Hard coded log point id to containt the ST_HSL_CMD_RESP structure */


typedef char AtCommandEchoString[CMD_MAX_AT];
typedef struct AtCmdEchoTag
{
  MS_BOOL result;
  AtCommandEchoString atCmd; /**< \assoc STRING */
} AtCmdEcho;

typedef char AtCommandRespString[RSP_MAX_AT];
typedef union U_HslCmdRespTag
{
  AtCommandRespString atResponse;  /**< \assoc STRING */
  AtCmdEcho atCmdEcho;
} U_HslCmdResp;

/** Please update the function PackIntoHslCmdResp() when adding new members before U_Resp union.*/
typedef struct ST_HSL_CMD_RESPTag
{
    /** \assoc UNION \ref U_Resp */
    EN_RESPONSE_ID         respId;
    U_HslCmdResp            U_Resp;
} ST_HSL_CMD_RESP;


/** This type is used to wrap around every array HSL log point.
    This is not used for logged typedef arrays which use a different HSL macro. */
typedef struct HslArrayLogPointTag
{
  MS_U8  rawData[HSL_MAX_SILO_ARRAY_SIZE];
}HslArrayLogPoint;

/** This type is used to wrap around every printf HSL log point to present
    them as serialised SILO GKI signals. */
typedef struct HslLogPointRawDataTag
{
    /** \assoc VAR_ARRAY \ref rawData */
    MS_U16 length;
    MS_U8  rawData[HSL_MAX_SILO_ARRAY_SIZE];
}HslLogPointRawData;

/** This type is used on PC to interpret those Log points coming via log
    point ID 0x9 which is effectively a wrapper for string info. */
typedef struct HslMessageLogPointTag
{
  char message[RSP_MAX_AT];  /**< \assoc STRING */
}HslMessageLogPoint;


void PackAtRespIntoHslCmdResp(const char* a_atResp);
void PackIntoHslCmdResp(EN_RESPONSE_ID a_respId, const void* a_body);

#endif
#endif
