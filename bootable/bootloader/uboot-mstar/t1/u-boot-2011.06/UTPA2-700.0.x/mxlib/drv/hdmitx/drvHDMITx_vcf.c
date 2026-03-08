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
////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvHDMITx_VCF.c
/// @brief  HDMI Tx Driver Interface for Vancleef
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef VANCLEEF_MCP
#define  MDRV_HDMITX_VCF_C
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "MsCommon.h"
#include "MsVersion.h"
#include "halHDMIUtilTx.h"
#include "regHDMITx.h"
#include "drvHDMITx.h"
#include "drvHDMITx_vcf.h"
#include "halHDMITx.h"
#include "halHDCPTx.h"

#include "../../api/swi2c/apiSWI2C.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define HDMITX_TASK_STACK_SIZE          4096
#define MSTRX_SLAVE_ADDR                0x9C
#define MSTRX_MONITOR_DELAY               20
#define VANCLEEF_SUPPORT_REPEATER          1

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
MS_U8 rx_current_bank = 0x00;
MS_U8 gHDMIRxM0[8] = {0};
extern MS_U8 HDCP_KEY[304];
extern MDrvHDMITX_PARAMETER_LIST     gHDMITxInfo;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_S32                       _s32VancleefTaskId = -1;
static MS_S32                       _s32VancleefEventId = -1;
static MS_S32                       _s32ReadM0TaskId = -1;
static MS_S32                       _s32ReadM0EventId = -1;
static MS_U8                        _u8Vancleef_StackBuffer[HDMITX_TASK_STACK_SIZE];
static MS_U8                        _u8ReadM0_StackBuffer[HDMITX_TASK_STACK_SIZE];


MSTHDMITX_REG_TYPE Vancleef_InitTbl[]=
{
    //bank, addr,   mask,   value,
    {0x00,  0x41,   0xFF,   0x6F},    // init MPLL
    {0x00,  0x51,   0xFF,   0x89},    // input source switch to portB
    {0x00,  0x54,   0x10,   0x00},    // Tmds mode detection for vtotal & htatol
    {0x00,  0xAC,   0x80,   0x80},    // output blank when receive AV_Mute packet
    {0x00,  0xB3,   0xFF,   0xFF},    // auto TriState Data @@

    //{0x00,  0xB7,   0x02,   0x02},    // R-term power down @@
    {0x00,  0x00,   0x80,   0x80},    // test mode enable
    {0x00,  0xC4,   0xFF,   0x58},    // [6]: R-term overwrite enable, [4]: clock, [3]: data

    {0x00,  0xCE,   0x80,   0x80},    // CFh[0],CEh[7]} = 2'b01, 133% current
    {0x00,  0xCF,   0x07,   0x02},    // CFh[2:1] = 2'b01, 133% current
    {0x00,  0x00,   0x80,   0x00},

    {0x01,  0x16,   0xFF,   0x30},    // R term setting, always set when DVI input
    {0x01,  0x17,   0xFF,   0x00},    // R EQ Setting, for default
    {0x01,  0x18,   0xFF,   0x00},    // G EQ Setting, for default
    {0x01,  0x19,   0xFF,   0x00},    // B EQ Setting, for default
    {0x01,  0x24,   0xFF,   0x40},    // Force HDCP, bit 6 set is for clk always working
    {0x01,  0x2A,   0x07,   0x07},    // default:64 phase
    {0x01,  0x30,   0xFF,   0x80},    // Bcaps - HDMI/Repeater,   Rx74_0x40  @@
    {0x01,  0x31,   0xFF,   0x00},    // Bstatus - device cnt = 8, Rx74_0x41  @@
    {0x01,  0x32,   0xFF,   0x00},    // Bstatus,  Rx74_0x42  @@
    {0x02,  0x08,   0xFF,   0x03},    // Deep color enable
    {0x02,  0x09,   0x20,   0x20},    // output GCP to GCP1 and GCP2 register
};

MSTHDMITX_REG_TYPE Vancleef_RxIntEnTbl[]=
{
    //bank, addr,   mask,   value,
    {0x00,  0x65,   0xFF,   0xFF},    // INT_PW = 256 xtal cycle
    {0x00,  0x64,   0xFF,   0x00},    // [2]: edge, [1]: rising, [0]: INT enable
};


#ifdef MS_DEBUG
static MS_BOOL _bDbgHDMITXSwitch = TRUE;
#else
static MS_BOOL _bDbgHDMITXSwitch = FALSE;
#endif

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#define DBG_VCF(_f)                   do{ if(_bDbgHDMITXSwitch) (_f); } while(0);

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// @brief This routine is to HDCP check repeater function
/// @return authentication status
//------------------------------------------------------------------------------
MsHDMITX_HDCP_AUTH_STATUS _MDrv_HDMITx_Vancleef_HdcpCheckKSVList(void)
{
    MS_U16 i, j;
    MS_U8 count;
    MS_U8 testbyte[635], bstatus[2], ksvlist[80];
    SHA1_DATA   test_dig;
    MS_U8 check_error = 0;

    //Vancleef
    MS_U8 u8Depth = 0;

    MHal_HDMITx_HdcpSha1Init(&test_dig);

    DBG_VCF(printf("Repeater is ready!!!\r\n"));

    // read ksv list
    if(MHal_HDMITx_Rx74ReadBytes(0x41, 2, bstatus) == FALSE)
    {
        DBG_VCF(printf("HDCP check repeater failed because I2C\r\n"));
        return E_HDMITX_HDCP_REPEATER_SHA1_FAIL;
    }

    //Vancleef 5.4, write Bstatus list to MST3000C
    MS_U8 u8VCbstatus[2] = {0x00, 0x00};

    count = 1;

    //Vancleef 5.4, write Bstatus to MST3000C
    u8Depth = 1;

    u8VCbstatus[0] = ( (count) & 0xFF);
    u8VCbstatus[1] = ( (u8Depth) & 0xF);

    if ( MHal_HDMITx_Read(HDCP_REG_BASE, 0x01) & BIT0 )
    {
        u8VCbstatus[1] = ( (u8Depth) & 0xF) | BIT4;
    }
    else
    {
        u8VCbstatus[1] = ( (u8Depth) & 0xF);
    }

    MDrv_HDMITx_Vancleef_RxLoadBStatus(u8VCbstatus);

    //Vancleef 5.5, write KSV list to MST3000C
    memcpy(ksvlist, gHDMITxInfo.HDCP_BKSV, 5);


    DBG_VCF(printf("KSV List:(=%d) ", count));
    if(count)
    {
        for (j = 0; j < 5; j++)
            DBG_VCF(printf("0x%x, ", ksvlist[j]));
        DBG_VCF(printf("\r\n"));
        MHal_HDMITx_HdcpSha1AddData(&test_dig, ksvlist, 5);
    }

    DBG_VCF(printf("Bstatus: 0x%x , 0x%x \n", u8VCbstatus[0], u8VCbstatus[1]));

    // read Bstatus (2 bytes)
    memcpy(testbyte, u8VCbstatus, 2);

    MHal_HDMITx_HdcpSha1AddData(&test_dig, testbyte, 2);

    #if 0
    // read M0 (8 byte)
    for (i = 0; i < 4; i++)
    {
        reg_value = MHal_HDMITX_GetM02Bytes(i);
        testbyte[2*i] = (MS_U8)(reg_value & 0x00FF);
        testbyte[2*i+1] = (MS_U8)((reg_value & 0xFF00) >> 8);
       // DBG_HDCP(printf(DBG_HDCP, "0x%x, ", testbyte[i]);
    }
    #else
    // read M0 (8 byte)

    MDrv_HDMITx_Vancleef_GetHDMIRxM0(testbyte);

    #endif

    MHal_HDMITx_HdcpSha1AddData(&test_dig, testbyte, 8);
    MHal_HDMITx_HdcpSha1FinalDigest(&test_dig, testbyte);

    //print out SHA1 encode result
    DBG_VCF(printf("SHA1 encode result(V)=: "));
    for (i = 0; i < 20; i++)
        DBG_VCF(printf("0x%x, ", testbyte[i]));
    DBG_VCF(printf("\r\n"));

    DBG_VCF(printf("V'=: "));

    DBG_VCF(printf("\r\n"));

    //Vancleef
    if (check_error == 0)
    {
        //Vancleef 5.5, write KSV list to MST3000C
        MDrv_HDMITx_Vancleef_RxWriteKSVFIFO(ksvlist, 5*count);

        //Vancleef 5.6, write V' list to MST3000C
        MDrv_HDMITx_Vancleef_RxWriteV(testbyte);

        //Vancleef 5.7, write "Ready Bit" list to MST3000C
        MDrv_HDMITx_Vancleef_RxWriteRepeaterReady(TRUE);
    }

    return ((check_error == 0) ? E_HDMITX_HDCP_REPEATER_SHA1_PASS: E_HDMITX_HDCP_REPEATER_SHA1_FAIL);
}



MS_U8 _MDrv_HDMITx_I2CReadByte(MS_U8 ucSlaveAdr, MS_U8 ucSubAdr)
{
    MS_U16 u16BusNumSlaveID = ( (0x00 <<8) | (MSTRX_SLAVE_ADDR <<0) );

    MS_U8 u8Buf = 0xFF; // MS_U8 buffer

    if( TRUE != MApi_SWI2C_ReadByte(u16BusNumSlaveID, ucSubAdr, &u8Buf) )
    {
        DBG_VCF(printf("MApi_SWI2C_ReadMS_U8 failed, ID = 0x%x , ADDR= 0x%x, Data = 0x%x \n", u16BusNumSlaveID, ucSubAdr, u8Buf));

        return u8Buf;
    }
    return u8Buf;
}

void _MDrv_HDMITx_I2CWriteByte(MS_U8 bank, MS_U8 addr, MS_U8 value)
{
    MS_U16 u16BusNumSlaveID = ( (0x00 <<8) | (MSTRX_SLAVE_ADDR <<0) );

    if( TRUE != MApi_SWI2C_WriteByte(u16BusNumSlaveID, addr, value) )
    {
        DBG_VCF(printf("MApi_SWI2C_WriteMS_U8 failed, ID = 0x%x , ADDR= 0x%x, Data = 0x%x \n", u16BusNumSlaveID, addr, value));
    }
}

MS_U8 _MDrv_HDMITx_Vancleef_RxReadByte(MS_U8 bank, MS_U8 addr)
{
    MS_U8 u8Value;

    if (rx_current_bank == bank)
    {
        u8Value = _MDrv_HDMITx_I2CReadByte(MSTRX_SLAVE_ADDR, addr);                 //the same bank, from the last
    }
    else
    {
        u8Value = (_MDrv_HDMITx_I2CReadByte(MSTRX_SLAVE_ADDR, 0x00) & 0xFC);
        bank&=0x03;

        _MDrv_HDMITx_I2CWriteByte(MSTRX_SLAVE_ADDR, 0x00, bank | u8Value);       //bank setting
        u8Value = _MDrv_HDMITx_I2CReadByte(MSTRX_SLAVE_ADDR, addr);                 //index data
        rx_current_bank = bank;
    }
    return u8Value;
}


void _MDrv_HDMITx_Vancleef_RxWriteByte(MS_U8 bank, MS_U8 addr, MS_U8 value)
{
    MS_U8 temp;
    if (rx_current_bank == bank) //the same bank from the last
    {
        _MDrv_HDMITx_I2CWriteByte(MSTRX_SLAVE_ADDR, addr, value);
        if (addr == 0x00)
            rx_current_bank = value;
    }
    else
    {
        //lowest 2 bit as BANK selection
        temp = (_MDrv_HDMITx_I2CReadByte(MSTRX_SLAVE_ADDR, 0x00) & 0xFC);
        bank&=0x03;

        _MDrv_HDMITx_I2CWriteByte(MSTRX_SLAVE_ADDR, 0x00, bank | temp);  //bank setting
        _MDrv_HDMITx_I2CWriteByte(MSTRX_SLAVE_ADDR, addr, value);
        rx_current_bank = bank;
    }
}

void _MDrv_HDMITx_Vancleef_RxWriteRegsTbl(MSTHDMITX_REG_TYPE *pTable, MS_U8 num)
{
    MS_U8 i, temp, temp1;

    for (i = 0; i < num; i++)
    {
        if (pTable->mask != 0xFF)
        {
            temp1 = _MDrv_HDMITx_Vancleef_RxReadByte(pTable->bank, pTable->address);
            temp = (temp1 & (~pTable->mask)) | pTable->value;
        }
        else
            temp = pTable->value;
        _MDrv_HDMITx_Vancleef_RxWriteByte(pTable->bank, pTable->address, temp);
        pTable++;
    }
}


// ******************************* //
// Bcaps[7:0]:
// Bit 7: HDMI_RESERVED
// Bit 6: REPEATER, HDCP Repeater capability
// Bit 5: READY, KSV FIFO ready.
// Bit 4: FAST. When set to one, this device supports 400 KHz transfers.
// Bits 3-2: Reserved (must be zero).
// Bit 1: 1.1_FEATURES. When set to one, this HDCP Receiver supports EESS, Advance Cipher, and Enhanced Link Verification options.
// Bit 0: FAST_REAUTHENTICATION. When set to 1, the receiver is capable of receiving (unencrypted) video signal during the session re-authentication.
// ******************************* //
void MDrv_HDMITx_Vancleef_RxLoadBCaps(MS_U8 u8BCapsData)
{
    // Load HDCP Bcaps to register
    _MDrv_HDMITx_Vancleef_RxWriteByte(0x01, 0x30, u8BCapsData);
}

void _MDrv_HDMITx_Vancleef_RxReadAnAksv(MS_U8 *An_data, MS_U8 *Aksv_data)
{
    MS_U8 idx;

    // Read An
    _MDrv_HDMITx_Vancleef_RxWriteByte(0x01, 0x25, 0x08);      // Set to read, 0x74 address,
    _MDrv_HDMITx_Vancleef_RxWriteByte(0x01, 0x26, 0x18);      // Address set to 0x18
    DBG_VCF(printf(" An = "));
    for (idx = 0; idx < 8; idx++)
    {
        An_data[idx] = _MDrv_HDMITx_Vancleef_RxReadByte(0x01, 0x27);
        DBG_VCF(printf("0x%x ", An_data[idx]));
    }
    DBG_VCF(printf(" \n "));

    // Read Aksv
    _MDrv_HDMITx_Vancleef_RxWriteByte(0x01, 0x25, 0x08);      // Set to read, 0x74 address,
    _MDrv_HDMITx_Vancleef_RxWriteByte(0x01, 0x26, 0x10);      // Address set to 0x10
    DBG_VCF(printf(" Aksv = "));
    for (idx = 0; idx < 5; idx++)
    {
        Aksv_data[idx] = _MDrv_HDMITx_Vancleef_RxReadByte(0x01, 0x27);
        DBG_VCF(printf("0x%x ", Aksv_data[idx]));
    }
    DBG_VCF(printf(" \n "));
}

// ************************************ //
// 6. Write KSV FIFO, V', Bstatus, Ready bit
// ************************************ //
void MDrv_HDMITx_Vancleef_RxWriteKSVFIFO(MS_U8 *u8KsvFifo, MS_U8 u8Size)
{
    MS_U8 idx;

    _MDrv_HDMITx_Vancleef_RxWriteByte(0x01, 0x3A, 0x00); // KSV FIFO write
    _MDrv_HDMITx_Vancleef_RxWriteByte(0x01, 0x3B, 0x00); // KSV FIFO address
    for(idx = 0; idx<u8Size; idx++)
    {
        _MDrv_HDMITx_Vancleef_RxWriteByte(0x01, 0x3C, *(u8KsvFifo+idx));
    }
    _MDrv_HDMITx_Vancleef_RxWriteByte(0x01, 0x3A, 0x01); // KSV FIFO read
}

void MDrv_HDMITx_Vancleef_RxWriteV(MS_U8 *u8VData)
{
    MS_U8 idx;

    _MDrv_HDMITx_Vancleef_RxWriteByte(0x01, 0x25, 0x00);      // Set to Write, 0x74 address,
    _MDrv_HDMITx_Vancleef_RxWriteByte(0x01, 0x26, 0x20);      // Address set to 0x20
    for (idx = 0; idx < 20; idx++)
    {
        _MDrv_HDMITx_Vancleef_RxWriteByte(0x01, 0x27, *(u8VData+idx));
    }
}

// ******************************* //
// Bstatus[15:0]
// Bit 15-14: Reserved. Read as zero.
// Bit 13: Reserved for future possible HDMI use.
// Bit 12: HDMI Mode. When set to one, the HDCP Receiverhas transitioned from DVI Mode to HDMI Mode.
// Bit 11: MAX_CASCADE_EXCEEDED, Topology error indicator.
// Bit 10-8: DEPTH, Three-bit repeater cascade depth
// Bit 7: MAX_DEVS_EXCEEDED, Topology error indicator
// Bit 6-0: DEVICE_COUNT, Total number of attached downstream devices(not include the HDCP Repeater itself)
// ******************************* //
void MDrv_HDMITx_Vancleef_RxLoadBStatus(MS_U8 *u8BStatusData)
{
    // Load HDCP Bstatus to register
    _MDrv_HDMITx_Vancleef_RxWriteByte(0x01, 0x31, u8BStatusData[0]);
    _MDrv_HDMITx_Vancleef_RxWriteByte(0x01, 0x32, u8BStatusData[1]);
}

void MDrv_HDMITx_Vancleef_RxWriteRepeaterReady(MS_BOOL bFlag)
{
    MS_U8 reg_val;

    reg_val = _MDrv_HDMITx_Vancleef_RxReadByte(0x01, 0x30) & (~BIT5);
    _MDrv_HDMITx_Vancleef_RxWriteByte(0x01, 0x30, (bFlag == TRUE) ? (reg_val | BIT5) : reg_val);
}

void MDrv_HDMITx_Vancleef_GetHDMIRxM0(MS_U8 *u8Ptr)
{
    memcpy(u8Ptr, gHDMIRxM0, 8);
}

static void _MDrv_HDMITx_Vancleef_ReadHDMIRxM0(void)
{
    int i = 0;
    MS_U8 gHDMIRxR0[2] = {0};
    MS_U8 gHDMIRxKm[2] = {0};

    DBG_VCF(printf("R0: "));

    gHDMIRxR0[2*i]   = MHal_HDMITx_Read(HDCP_REG_BASE, (0x12 + i))&0x00FF;
    gHDMIRxR0[2*i+1] = (MHal_HDMITx_Read(HDCP_REG_BASE, (0x12 + i))&0xFF00) >> 8;

    DBG_VCF(printf("0x%x, 0x%x ", gHDMIRxR0[2*i], gHDMIRxR0[2*i+1]));

    DBG_VCF(printf("\n"));

    DBG_VCF(printf("Km: "));


    for (i = 0; i < 4; i++)
    {
        gHDMIRxKm[2*i]   = MHal_HDMITx_Read(HDCP_REG_BASE, (0x0A + i))&0x00FF;
        gHDMIRxKm[2*i+1] = (MHal_HDMITx_Read(HDCP_REG_BASE, (0x0A + i))&0xFF00)>>8;
        DBG_VCF(printf("0x%x, 0x%x ", gHDMIRxKm[2*i], gHDMIRxKm[2*i+1]));
    }

    DBG_VCF(printf("\n"));

    DBG_VCF(printf("M0: "));

    for (i = 0; i < 4; i++)
    {
        gHDMIRxM0[2*i]   = MHal_HDMITx_Read(HDCP_REG_BASE, (0x0E + i))&0x00FF;
        gHDMIRxM0[2*i+1] = (MHal_HDMITx_Read(HDCP_REG_BASE, (0x0E + i))&0xFF00)>>8;
        DBG_VCF(printf("0x%x, 0x%x ", gHDMIRxM0[2*i], gHDMIRxM0[2*i+1]));
    }

    DBG_VCF(printf("\n"));

}

static void _MDrv_HDMITx_Vancleef_RxLoadHDCPKey(MS_U8 *keydata)
{
    MS_U16 idx;

    // Load HDCP BKSV to register
    _MDrv_HDMITx_Vancleef_RxWriteByte(0x01, 0x25, 0x00);      // Set to Write, 0x74 address,
    _MDrv_HDMITx_Vancleef_RxWriteByte(0x01, 0x26, 0x00);      // Address set to 0
    for (idx = 0; idx < 5; idx++)
        _MDrv_HDMITx_Vancleef_RxWriteByte(0x01, 0x27, *(keydata+idx));

    // Load HDCP key to register
    _MDrv_HDMITx_Vancleef_RxWriteByte(0x01, 0x25, 0x04);      // Set to Write, Sram register,
    _MDrv_HDMITx_Vancleef_RxWriteByte(0x01, 0x26, 0x00);      // Address set to 0
    for (idx = 5; idx < 289; idx++)
        _MDrv_HDMITx_Vancleef_RxWriteByte(0x01, 0x27, *(keydata+idx));

    _MDrv_HDMITx_Vancleef_RxWriteByte(0x01, 0x25, 0x08);      // Set to read, 0x74 register,
}

MS_BOOL MDrv_HDMITx_Vancleef_RxLoadHDCPKey(void)
{

    _MDrv_HDMITx_Vancleef_RxLoadHDCPKey(HDCP_KEY);

    return TRUE;
}
static void MDrv_HDMITx_Vancleef_Task(void)
{
    MS_U32  u32Events = 0;
    MS_U8   Vancleef_HDCP_AKSV[5] = {0, 0, 0, 0, 0};
    MS_U8   Vancleef_HDCP_AN[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    while (1)
    {
        MsOS_WaitEvent(_s32VancleefEventId, E_VANCLEEF_EVENT_IRQ, &u32Events, E_OR_CLEAR, 5000);

        if (1)
        {
            if(u32Events & E_VANCLEEF_EVENT_IRQ)
            {
                DBG_VCF(printf("[%s][%d] E_VANCLEEF_EVENT_IRQ! \n", __FUNCTION__, __LINE__));
                DBG_VCF(printf("[%s][%d] System Time %ld !! \n", __FUNCTION__, __LINE__, MsOS_GetSystemTime()));

                /*******************************************/
                // 1. IQR
                // 2. KaiserS Read An, AKSV from Vancleef
                // 3. KaiserS Write An, AKSV to K3S HDMIRx
                // 4. V' (TBD)
                /*******************************************/

                int i = 0;

                // 2.
                _MDrv_HDMITx_Vancleef_RxReadAnAksv(Vancleef_HDCP_AN, Vancleef_HDCP_AKSV);

                // 3.
                // 3.1 Write An
                #define AN_PORT 0x18
                #define AN_SIZE 8

                 for (i=0; i < AN_SIZE ; i++)
                {
                    if( TRUE != MApi_SWI2C_WriteByte(0x0074, (AN_PORT + i ), Vancleef_HDCP_AN[i]) )
                    {
                        printf("[%s][%d] Write AN Failed!, i = %d \n", __FUNCTION__, __LINE__, i);
                    }
                }

                // 3.2 Write Aksv
                #define AKSV_PORT 0x10
                #define AKSV_SIZE 5

                for (i=0; i < AKSV_SIZE ; i++)
                {
                    if( TRUE != MApi_SWI2C_WriteByte(0x0074, (AKSV_PORT + i ), Vancleef_HDCP_AKSV[i]) )
                    {
                        printf("[%s][%d] Write AKSV Failed!, i = %d \n", __FUNCTION__, __LINE__, i);
                    }
                }

#if (VANCLEEF_SUPPORT_REPEATER)

                // 4. After writing An and Aksv, trig ReadM0 Event
                MsOS_SetEvent(_s32ReadM0EventId, E_READM0_EVENT_RUN);
#endif
                DBG_VCF(printf("[%s][%d] System Time %ld !! \n", __FUNCTION__, __LINE__, MsOS_GetSystemTime()));

                MsOS_EnableInterrupt(E_INT_IRQ_PM);

            }
            else
            {
                //do nothing
                //db_print("ERROR WAS HAPPENED, Wrong Event Status!! \n");
            }
        }

        MsOS_DelayTask(MSTRX_MONITOR_DELAY);
    } // Task loop
}


static void MDrv_HDMITx_ReadM0_Task(void)
{
    MS_U32  u32Events = 0;

    while (1)
    {
        MsOS_WaitEvent(_s32ReadM0EventId, E_READM0_EVENT_RUN, &u32Events, E_OR_CLEAR, 5000);

        if (1)
        {
            if(u32Events & E_READM0_EVENT_RUN)
            {
                DBG_VCF(printf("[%s][%d] E_READM0_EVENT_RUN! \n", __FUNCTION__, __LINE__));
                DBG_VCF(printf("[%s][%d] System Time %ld !! \n", __FUNCTION__, __LINE__, MsOS_GetSystemTime()));

                // wait until R0 Ready
                while( !( MHal_HDMITx_Read(HDCP_REG_BASE, 0x01) & BIT7 ))
                {
                    MsOS_DelayTask(5);
                }

                DBG_VCF(printf("[%s][%d] R0 Ready, System Time %ld !! \n", __FUNCTION__, __LINE__, MsOS_GetSystemTime()));
                _MDrv_HDMITx_Vancleef_ReadHDMIRxM0();

                _MDrv_HDMITx_Vancleef_HdcpCheckKSVList();

                DBG_VCF(printf("[%s][%d] System Time %ld !! \n", __FUNCTION__, __LINE__, MsOS_GetSystemTime()));
            }
            else
            {
                //do nothing
                //db_print("ERROR WAS HAPPENED, Wrong Event Status!! \n");
                DBG_VCF(printf("[%s][%d] invoked! \n", __FUNCTION__, __LINE__));
            }
        }

        MsOS_DelayTask(MSTRX_MONITOR_DELAY);
    } // Task loop
}

MS_BOOL MDrv_HDMITx_Vancleef_Init(void)
{
    //SWI2C Init, PM_GPIO5(GPIO144) and PM_PGIO7(GPIO146)
    //SWI2C_BusCfg I2CConfig = {PAD_GPIO_PM7, PAD_GPIO_PM5, 70};
    SWI2C_BusCfg I2CConfig = {VCF_SWI2C_PAD_SCL, VCF_SWI2C_PAD_SDA, VCF_SWI2C_DEF_DELAY};

    //Init 1 SWI2C
    //MApi_SWI2C_SetDbgLevel(E_SWI2C_DBGLVL_ALL);
    MApi_SWI2C_Init(&I2CConfig,1);
    //--------------------------------------------//

    MS_U8 u8Size;

    u8Size = sizeof(Vancleef_InitTbl) / sizeof(MSTHDMITX_REG_TYPE);
    _MDrv_HDMITx_Vancleef_RxWriteRegsTbl(Vancleef_InitTbl, u8Size);

    // Initialize Task and Event Handler
    _s32VancleefEventId = MsOS_CreateEventGroup("Vancleef_Event");

#if (VANCLEEF_SUPPORT_REPEATER)
    _s32ReadM0EventId = MsOS_CreateEventGroup("ReadM0_Event");
#endif

    _s32VancleefTaskId = MsOS_CreateTask((TaskEntry)MDrv_HDMITx_Vancleef_Task,
                                        (MS_U32)NULL,
                                        E_TASK_PRI_HIGH,
                                        TRUE,
                                        _u8Vancleef_StackBuffer,
                                        HDMITX_TASK_STACK_SIZE,
                                        "Vancleef_Task");

    if (_s32VancleefTaskId < 0)
    {

        MsOS_DeleteEventGroup(_s32VancleefEventId);
        ///MsOS_DeleteMutex(_s32HDMITxMutexId);

        printf("Create _Vancleef_Task ERROR!! \n");

        return FALSE;
    }

#if (VANCLEEF_SUPPORT_REPEATER)
    _s32ReadM0TaskId = MsOS_CreateTask((TaskEntry)MDrv_HDMITx_ReadM0_Task,
                                       (MS_U32)NULL,
                                       E_TASK_PRI_HIGH,
                                       TRUE,
                                       _u8ReadM0_StackBuffer,
                                       HDMITX_TASK_STACK_SIZE,
                                       "ReadM0_Task");

    if (_s32ReadM0TaskId < 0)
    {

        MsOS_DeleteEventGroup(_s32ReadM0EventId);
        ///MsOS_DeleteMutex(_s32HDMITxMutexId);

        printf("Create _ReadM0_Task ERROR!! \n");

        return FALSE;
    }
#endif

    return TRUE;
}

MS_BOOL MDrv_HDMITx_Vancleef_GetIntStatus(void)
{
    MS_U8 u8Data1 = 0x00;
    MS_U8 u8Data2 = 0x00;

    //fiq final status for edge wake-up source
    u8Data1 = (MS_U8)(MHal_HDMITxPM_Read(PMBK_PMSLEEP_REG_BASE, 0x0A) >> 8);

    //fiq raw status for edge wake-up source
    u8Data2 = (MS_U8)(MHal_HDMITxPM_Read(PMBK_PMSLEEP_REG_BASE, 0x0B) >> 8);

    DBG_VCF(printf("[%s][%d] Data1 = 0x%x, Data2 = 0x%x!\n", __FUNCTION__, __LINE__, u8Data1, u8Data2));

    //fiq final status for edge wake-up source
    if (u8Data1 & 0x80 )
    {
        return TRUE;
    }

    //fiq raw status for edge wake-up source
    if (u8Data2 & 0x80 )
    {
        return TRUE;
    }

    return FALSE;
}

void MDrv_HDMITx_Vancleef_SetIntClear(void)
{
    //fiq clear for edge wake-up source
    //0x04 BIT[15], PM_GPIO12, WRITE 1 to clear
    //MApi_XC_WriteByteMask(0x000E09, 0x80, 0x80);
    MHal_HDMITxPM_Mask_Write(PMBK_PMSLEEP_REG_BASE, 0x04, 0x8000, 0x8000);

    DBG_VCF(printf("[%s][%d] clear PM_GPIO12 Int! \n", __FUNCTION__, __LINE__));
}

void MDrv_HDMITx_Vancleef_Isr(InterruptNum eIntNum, void* dev_id)
{
    //MS_U32 irq_status;

    DBG_VCF(printf("[%s][%d] invoked!! \n", __FUNCTION__, __LINE__));
    DBG_VCF(printf("[%s][%d] System Time %ld !! \n", __FUNCTION__, __LINE__, MsOS_GetSystemTime()));

    if(TRUE == MDrv_HDMITx_Vancleef_GetIntStatus()) // PM_GPIO12
    {
        //Clear Repeater Ready Status, NO NEED to do this, because Kaisers Tx has its own state machine to handle this
        //_MDrv_HDMITx_Vancleef_RxWriteRepeaterReady(FALSE);
        MDrv_HDMITx_Vancleef_RxWriteRepeaterReady(FALSE);

        //Clear Kaiser HDMIRx M0 Status, TBD
        //MApi_XC_WriteByte(0x110AC3, 0xFF);

        //Clear interrutp event first (Vancleef)
        MDrv_HDMITx_Vancleef_RxClearINT();

        //Clear Interrupt of PM_GPIO12 (PM_SLEEP)
        MDrv_HDMITx_Vancleef_SetIntClear();
        MsOS_SetEvent(_s32VancleefEventId, E_VANCLEEF_EVENT_IRQ);

        //clear HDMIRx Km(M0) Status
        //MApi_XC_WriteByte(0x110AC3, BIT2);
        MHal_HDMITx_Mask_Write(HDCP_REG_BASE, 0x01, BIT10, BIT10);
    }
    else
    {
        printf("[%s][%d] other events! \n", __FUNCTION__, __LINE__);
    }
}


void _MDrv_HDMITx_Vancleef_RxEnableINT(void)
{
    MS_U8 num;

    num = sizeof(Vancleef_RxIntEnTbl) / sizeof(MSTHDMITX_REG_TYPE);
    _MDrv_HDMITx_Vancleef_RxWriteRegsTbl(Vancleef_RxIntEnTbl, num);
}

void MDrv_HDMITx_Vancleef_RxClearINT(void)
{
    _MDrv_HDMITx_Vancleef_RxWriteByte(0x01, 0x35, 0x07);      // write 1 clear
}

MS_BOOL MDrv_HDMITx_Vancleef_RegisterISR(void)
{

    _MDrv_HDMITx_Vancleef_RxEnableINT();

    //fiq unmask for edge wake-up source
    MHal_HDMITxPM_Mask_Write(PMBK_PMSLEEP_REG_BASE, 0x00, 0x8000, 0x0000);

    //MsOS_AttachInterrupt(E_INT_IRQ_PMSLEEP, (InterruptCb)_Vancleef_Isr);

    //E_INT_IRQ_PM
    //MsOS_DetachInterrupt(E_INT_IRQ_PM);

    if (FALSE == MsOS_AttachInterrupt(E_INT_IRQ_PM, (InterruptCb)MDrv_HDMITx_Vancleef_Isr) )
    {
        printf("[%s][%d] register ISR Failed!! \n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    MsOS_EnableInterrupt(E_INT_IRQ_PM);

    return TRUE;
}

void MDrv_HDMITx_Vancleef_DebugEnable(MS_BOOL bEnable)
{
    _bDbgHDMITXSwitch = bEnable;
}


#endif // VANCLEEF_MCP

