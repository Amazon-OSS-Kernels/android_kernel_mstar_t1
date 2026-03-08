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


//#include <MsCommon.h> // NUSED
//#include "drvMSC.h" // NUSED
//#include "include/drvKernel.h" // NUSED
#include "drvBulktran.h"
//#include "drvUsbd.h" // NUSED
#include "drvEHCI.h"
#include "drvUSBHwCtl.h"
/* applying drvUsbHostConfig.h (inside drvUSBHwCtl.h) */

//#define SCSI_DEBUG
#ifdef SCSI_DEBUG
#define SCSI_DbgPrint(x,arg...) diag_printf(x,##arg)
#else
#define SCSI_DbgPrint(x,...)
#endif

#define ScsiCmd_Fail_Retry      3
#define ScsiCmd_Fail_TUR_Retry      7
#define Scsi_Max_Transfer_Len   120*1024 //65536

struct ms_usdata *Mass_stor_us[NUM_OF_ROOT_HUB*MAX_USTOR] = {NULL};

static void ms_vScsi_SendCmd_Done(struct scMsc_cmd *srb);
BOOL ms_bSCSI_Initial(U8 uPort);
//BOOL ms_bInit_USB_Disk(U8 uPort);  //NUSED
BOOL ms_bSCSI_ERASE(U8 uPort, U8 uLunNum, U32 u32BlockAddr, U32 u32BlockNum,
                            U32 u32SectorSize);
BOOL ms_bSCSI_Read_10(U8 uPort, U8 uLunNum, U32 u32BlockAddr, U32 u32BlockNum,
                            U8 *u8Buffer);
BOOL ms_bSCSI_Write_10(U8 uPort, U8 uLunNum, U32 u32BlockAddr, U32 u32BlockNum,
                            U8 *u8Buffer);
// NUSED
//BOOL ms_bSCSI_Read_10_512(U8 uPort, U8 uLunNum, U32 u32BlockAddr, U32 u32BlockNum,
//                            U8 *u8Buffer);
//BOOL ms_bSCSI_Write_10_512(U8 uPort, U8 uLunNum, U32 u32BlockAddr, U32 u32BlockNum,
//                            U8 *u8Buffer);
VOID RemoveUSBDiskPort(U8 uPort, U8 uLunNum);
BOOL ConnectUSBDisk(U8 uPort, U8 uLunNum);

static U32
ms_get_value_log2(U32 val)
{
    U32 i, log2;

    if(val == 0)
        return 0;

    i    = val;
    log2 = 0;


    while (0 == (i & 1))
    {
        i >>= 1;
        log2++;
    }

    if (i != 1) return 0;
    else        return log2;
}

void ms_vSCSICmd_READ_10(struct ms_usdata *us, U8 u8LunNum,
                            U32 u32BlockAddr, U16 u16BlockNum, U8 *u8Buffer)
{
    stCmd_scsi *srb = &us->srb;

    /* set the command and the LUN */
    //memset(srb->cmnd, 0, MAX_COMMAND_SIZE);
    srb->cmnd[0] = READ_10;
    srb->cmnd[2] = (U8)(u32BlockAddr >> 24);
    srb->cmnd[3] = (U8)(u32BlockAddr >> 16);
    srb->cmnd[4] = (U8)(u32BlockAddr >> 8);
    srb->cmnd[5] = (U8)(u32BlockAddr);
    srb->cmnd[7] = (U8)(u16BlockNum >> 8);
    srb->cmnd[8] = (U8)(u16BlockNum );

    srb->cmd_len = CB_LENGTH_READ_10;

    /* set the transfer direction */
    srb->sc_data_direction = SCSI_DATA_READ;

    /* use the new buffer we have */
    srb->req_buf = u8Buffer;

    /* set the buffer length for transfer */
    srb->req_buf_len = u16BlockNum << (us->msc_device[u8LunNum].u32BlockSize_log2);

    /* change the serial number -- toggle the high bit*/
    srb->serial_number ^= 0x80000000;

    /* set Lun number*/
    srb->lun= u8LunNum;

    srb->scsi_done = ms_vScsi_SendCmd_Done;
}

void ms_vSCSICmd_WRITE_10(struct ms_usdata *us, U8 u8LunNum,
                            U32 u32BlockAddr, U16 u16BlockNum, U8 *u8Buffer)
{
    stCmd_scsi *srb = &us->srb;

    /* set the command and the LUN */
    //memset(srb->cmnd, 0, MAX_COMMAND_SIZE);
    srb->cmnd[0] = WRITE_10;
    srb->cmnd[2] = (U8)(u32BlockAddr >> 24);
    srb->cmnd[3] = (U8)(u32BlockAddr >> 16);
    srb->cmnd[4] = (U8)(u32BlockAddr >> 8);
    srb->cmnd[5] = (U8)(u32BlockAddr);
    srb->cmnd[7] = (U8)(u16BlockNum >> 8);
    srb->cmnd[8] = (U8)(u16BlockNum );

    srb->cmd_len = CB_LENGTH_WRITE_10;

    /* set the transfer direction */
    srb->sc_data_direction = SCSI_DATA_WRITE;

    /* use the new buffer we have */
    srb->req_buf = u8Buffer;

    /* set the buffer length for transfer */
    srb->req_buf_len = u16BlockNum << us->msc_device[u8LunNum].u32BlockSize_log2;

    /* change the serial number -- toggle the high bit*/
    srb->serial_number ^= 0x80000000;

    /* set Lun number*/
    srb->lun= u8LunNum;

    srb->scsi_done = ms_vScsi_SendCmd_Done;
}


void ms_vSCSICmd_INQUIRY(struct ms_usdata *us, U8 *u8Buffer,  U8 u8LunNum)
{
    stCmd_scsi *srb = &us->srb;

    /* set the command and the LUN */
    memset(srb->cmnd, 0, MAX_COMMAND_SIZE);
    srb->cmnd[0] = INQUIRY;
    srb->cmnd[4] = DATA_LENGTH_INQUIRY;

    srb->cmd_len = CB_LENGTH_INQUIRY;

    /* set the transfer direction */
    srb->sc_data_direction = SCSI_DATA_READ;

    /* use the new buffer we have */
    srb->req_buf = u8Buffer;

    /* set the buffer length for transfer */
    srb->req_buf_len = DATA_LENGTH_INQUIRY;

    /* change the serial number -- toggle the high bit*/
    srb->serial_number ^= 0x80000000;

    /* set Lun number*/
    srb->lun= u8LunNum;

    srb->scsi_done = ms_vScsi_SendCmd_Done;
}

void ms_vSCSICmd_READ_CAPACITY(struct ms_usdata *us, U8 *u8Buffer,  U8 u8LunNum)
{
    stCmd_scsi *srb = &us->srb;

    /* set the command and the LUN */
    memset(srb->cmnd, 0, MAX_COMMAND_SIZE);
    srb->cmnd[0] = READ_CAPACITY;

    srb->cmd_len = CB_LENGTH_READ_CAPACITY;

    /* set the transfer direction */
    srb->sc_data_direction = SCSI_DATA_READ;

    /* use the new buffer we have */
    srb->req_buf = u8Buffer;

    /* set the buffer length for transfer */
    srb->req_buf_len = DATA_LENGTH_READ_CAPACITY;

    /* change the serial number -- toggle the high bit*/
    srb->serial_number ^= 0x80000000;

    /* set Lun number*/
    srb->lun= u8LunNum;

    srb->scsi_done = ms_vScsi_SendCmd_Done;
}

void ms_vSCSICmd_MODE_SENSE(
    struct ms_usdata *us,
    U8 u8PageCode,
    U8 *u8Buffer,
    U8 u8LunNum,
    U16 uBufLen)
{
    stCmd_scsi *srb = &us->srb;

    /* set the command and the LUN */
    memset(srb->cmnd, 0, MAX_COMMAND_SIZE);
    srb->cmnd[0] = MODE_SENSE6;
    srb->cmnd[2] = u8PageCode;
    //srb->cmnd[7] = (U8)((uBufLen >> 8) & 0x00FF);
    //srb->cmnd[8] = (U8)uBufLen;
        srb->cmnd[4] = (U8)uBufLen;

    srb->cmd_len = CB_LENGTH_MODE_SENSE;

    /* set the transfer direction */
    srb->sc_data_direction = SCSI_DATA_READ;

    /* use the new buffer we have */
    srb->req_buf = u8Buffer;

    /* set the buffer length for transfer */
    srb->req_buf_len = uBufLen;

    /* change the serial number -- toggle the high bit*/
    srb->serial_number ^= 0x80000000;

    /* set Lun number*/
    srb->lun= u8LunNum;

    srb->scsi_done = ms_vScsi_SendCmd_Done;
}

void ms_vSCSICmd_REQUEST_SENSE(struct ms_usdata *us, U8 u8LunNum)
{
    stCmd_scsi *srb = &us->srb;

    /* set the command and the LUN */
    memset(srb->cmnd, 0, MAX_COMMAND_SIZE);
    srb->cmnd[0] = REQUEST_SENSE;
    srb->cmnd[4] = DATA_LENGTH_REQUEST_SENSE;

    srb->cmd_len = CB_LENGTH_REQUEST_SENSE;

    /* set the transfer direction */
    srb->sc_data_direction = SCSI_DATA_READ;

    /* use the new buffer we have */
    srb->req_buf = srb->sense_buffer;

    /* set the buffer length for transfer */
    srb->req_buf_len = DATA_LENGTH_REQUEST_SENSE;

    /* change the serial number -- toggle the high bit*/
    srb->serial_number ^= 0x80000000;

    /* set Lun number*/
    srb->lun= u8LunNum;

    srb->scsi_done = ms_vScsi_SendCmd_Done;
}

void ms_vSCSICmd_TEST_UNIT_READY(struct ms_usdata *us, U8 u8LunNum)
{
    stCmd_scsi *srb = &us->srb;

    /* set the command and the LUN */
    memset(srb->cmnd, 0, MAX_COMMAND_SIZE);
    srb->cmnd[0] = TEST_UNIT_READY;

    srb->cmd_len = CB_LENGTH_TEST_UNIT_READY;

    /* set the transfer direction */
    srb->sc_data_direction = SCSI_DATA_READ;

    /* set the buffer length for transfer */
    srb->req_buf_len = 0;

    /* change the serial number -- toggle the high bit*/
    srb->serial_number ^= 0x80000000;

    /* set Lun number*/
    srb->lun= u8LunNum;

    srb->scsi_done = ms_vScsi_SendCmd_Done;
}


BOOL ms_bSCSI_REQ_SENSE(U8 uPort, U8 uLunNum, U8 *pSenseBuf)
{
    //BOOL	err;
    BOOL      bRet=FALSE;
    struct ms_usdata *pMass_stor;
    U8      host_id;


    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        return FALSE;
    //err = MsOS_ObtainSemaphore(g_SCSISem,MSOS_WAIT_FOREVER);
    //USB_ASSERT(err, "Wait SCSI sem fail\n");
    //err=MsOS_ObtainMutex(_s32MutexUSB, MSOS_WAIT_FOREVER);
    // if (err==FALSE) return FALSE;
    //U32 u32OldIntr;

    // MsOS_DisableAllInterrupts(u32OldIntr);

    //lock_usb_core();
    host_id = pMass_stor->host_id;
    ms_USBCriticalSectionIn(host_id);

    memset(&pMass_stor->srb, 0, sizeof(struct scMsc_cmd ));

    ms_vSCSICmd_REQUEST_SENSE(pMass_stor, uLunNum);

    ms_usb_msc_control_thread(pMass_stor);

    if (pMass_stor->srb.result == MS_STAT_GOOD)
    {
        memcpy(pSenseBuf, pMass_stor->srb.sense_buffer, DATA_LENGTH_REQUEST_SENSE);
        bRet=TRUE;
    }
    else
        bRet=FALSE;

    //unlock_usb_core();
    ms_USBCriticalSectionOut(host_id);

   //MsOS_ReleaseMutex(_s32MutexUSB);
  // MsOS_RestoreAllInterrupts(u32OldIntr);
   return bRet;
}


BOOL ms_bSCSI_INQUIRY(U8 uPort, U8 uLunNum, U8 *pIngBuf)
{
    BOOL	bRet = FALSE;
    int		i;
    struct ms_usdata *pMass_stor;
    U8      host_id;

    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        return FALSE;
    
    host_id = pMass_stor->host_id;
    ms_USBCriticalSectionIn(host_id);
    for(i =0; i< ScsiCmd_Fail_Retry ;i++)
    {
        if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
            break;

        memset(&pMass_stor->srb, 0, sizeof(struct scMsc_cmd ));
        ms_vSCSICmd_INQUIRY(pMass_stor, pIngBuf, uLunNum);

        ms_usb_msc_control_thread(pMass_stor);

        if(pMass_stor->srb.result == MS_STAT_GOOD)
        {
            bRet = TRUE;
            break;
        }
        else if (pMass_stor->srb.result == (DID_NO_CONNECT << 16))
        {
            diag_printf("INQUIRY break by NO_CONNECT\n");
            break;
        }
        else if (pMass_stor->srb.result == (DID_HCD_SHUTDOWN << 16))
        {
            diag_printf("INQUIRY break by HCD shutdown\n");
            break;
        }
    }
    //unlock_usb_core();
    ms_USBCriticalSectionOut(host_id);
    return bRet;
}

// save all Mode Sense(page code=0x3F) data
// Now we only use u8ModeSenseData[2], this byte save device
// write protection information
BOOL ms_bSCSI_MODE_SENSE(U8 uPort, U8 uLunNum, U8 *pModSenBuf, U16 uBufLen)
{
    int     i;
    BOOL    bRet = FALSE;
    struct ms_usdata *pMass_stor;
    U8      host_id;

    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        return FALSE;
    
    host_id = pMass_stor->host_id;
    ms_USBCriticalSectionIn(host_id);
    for(i =0; i< ScsiCmd_Fail_Retry ; i++)
    {
        if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
            break;

        memset(&pMass_stor->srb, 0, sizeof(struct scMsc_cmd ));
        ms_vSCSICmd_MODE_SENSE(pMass_stor, 0x3F, pModSenBuf, uLunNum, uBufLen);
        ms_usb_msc_control_thread(pMass_stor);

        if(pMass_stor->srb.result == MS_STAT_GOOD)
        {
            bRet = TRUE;
            break;
        }
        else if (pMass_stor->srb.result == (DID_NO_CONNECT << 16))
        {
            diag_printf("MODE_SENSE break by NO_CONNECT\n");
            break;
        }
        else if (pMass_stor->srb.result == (DID_HCD_SHUTDOWN << 16))
        {
            diag_printf("MODE_SENSE break by HCD shutdown\n");
            break;
        }
    }
    //unlock_usb_core();
    ms_USBCriticalSectionOut(host_id);
    return bRet;
}

BOOL ms_bSCSI_READ_CAPACITY(U8 uPort, U8 uLunNum, U32 *pTotalBlks, U32 *pBlkSize)
{
    int     i;
    BOOL    bRet = FALSE;
    U8      CapBuf[DATA_LENGTH_READ_CAPACITY];
    struct ms_usdata *pMass_stor;
    U8      host_id;

    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        return bRet;
    host_id = pMass_stor->host_id;
    ms_USBCriticalSectionIn(host_id);
    for(i =0; i< ScsiCmd_Fail_Retry ; i++)
    {
        if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
            break;

        memset(&pMass_stor->srb, 0, sizeof(struct scMsc_cmd ));

        ms_vSCSICmd_READ_CAPACITY(pMass_stor, CapBuf, uLunNum);

        ms_usb_msc_control_thread(pMass_stor);

        if(pMass_stor->srb.result == MS_STAT_GOOD)
        {
            *pTotalBlks = (((U32)CapBuf[0] << 24) |
            ((U32)CapBuf[1] << 16) |
            ((U32)CapBuf[2] << 8) |
            ((U32)CapBuf[3] )) + 1 ;

            *pBlkSize = ((U32)CapBuf[4] << 24) |
            ((U32)CapBuf[5] << 16) |
            ((U32)CapBuf[6] << 8) |
            ((U32)CapBuf[7] ) ;

            SCSI_DbgPrint("SCSI CAPACITY : SCSI Device total block <0x%x%x>\n",
            (U16)(*pTotalBlks >> 16),
            (U16)*pTotalBlks);
            SCSI_DbgPrint("SCSI CAPACITY : SCSI Product block size <0x%x bytes>\n",
            (U16)*pBlkSize);

            bRet = TRUE;
            break;
        }
        else if (pMass_stor->srb.result == (DID_NO_CONNECT << 16))
        {
            diag_printf("READ_CAPACITY break by NO_CONNECT\n");
            break;
        }
        else if (pMass_stor->srb.result == (DID_HCD_SHUTDOWN << 16))
        {
            diag_printf("READ_CAPACITY break by HCD shutdown\n");
            break;
        }
    }

    ms_USBCriticalSectionOut(host_id);
    return bRet;
}

// Some cameras with CBI subclass can't accept the TUR, but others need.
// Our solution is don't send real TUR to camera. If the command Get Capacity is failed,
// we send the real TUR.
BOOL ms_bSCSI_TEST_UNIT_READY(U8 uPort, U8 uLunNum, BOOL CheckCBI)
{
    int		i;
    BOOL	bRet = FALSE;
    struct ms_usdata *pMass_stor;
    struct LUN_Device* LunDevice;    
    U8      host_id;

    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        return FALSE;
    host_id = pMass_stor->host_id;
    ms_USBCriticalSectionIn(host_id);

    if (CheckCBI)
    {
        if ( ( Mass_stor_us[uPort] != NULL) &&
            (Mass_stor_us[uPort]->subclass == MS_SFF_SC) &&
            (Mass_stor_us[uPort]->protocol == MS_CBI_PR) )
        {
            bRet = TRUE;
            goto Fun_Exit;
        }
    }

    // fixed device test
    LunDevice = pMass_stor->msc_device;
    SCSI_DbgPrint("TUR:: [%d]u8DevType %x\n", uLunNum, LunDevice[uLunNum].u8DevType);
    if ((LunDevice[uLunNum].u8DevType & 0x80) == 0)
    {
        bRet = TRUE;
        goto Fun_Exit;
    }

    for(i =0; i< ScsiCmd_Fail_TUR_Retry ;i++)
    {
        if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
            break;

        memset(&pMass_stor->srb, 0, sizeof(struct scMsc_cmd ));

        ms_vSCSICmd_TEST_UNIT_READY(pMass_stor, uLunNum);

        ms_usb_msc_control_thread(pMass_stor);

        if(pMass_stor->srb.result == MS_STAT_GOOD)
        {
            //diag_printf(" Succeed for lun %d\n",uLunNum);

            bRet = TRUE;
            break;
        }
        else if (pMass_stor->srb.result == (DID_NO_CONNECT << 16))
        {
            diag_printf("TUR break by NO_CONNECT\n");
            break;
        }
        else if (pMass_stor->srb.result == (DID_HCD_SHUTDOWN << 16))
        {
            diag_printf("TUR break by HCD shutdown\n");
            break;
        }
        else
        {
            if (i > ScsiCmd_Fail_Retry)
                SCSI_DbgPrint("TUR failed retry %d\n",i);
        }
    }

Fun_Exit:
    ms_USBCriticalSectionOut(host_id);
    return bRet;
}

BOOL ms_bInit_USB_LUN(U8 uPort, U8 uLunIdx)
{
    struct ms_usdata *pMass_stor;
    struct LUN_Device* LunDevice;
    U8      pIngBuf[DATA_LENGTH_INQUIRY];
    U8      pModSenBuf[8];
    U8 u8DevType;
    
    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        return FALSE;

    LunDevice = pMass_stor->msc_device;

    if(ms_bSCSI_TEST_UNIT_READY(uPort, uLunIdx, TRUE))
    {
        SCSI_DbgPrint("Scsi Device is ready (Lun=%d).\n", uLunIdx);
    }
    else
        return FALSE;

    if (ms_bSCSI_INQUIRY(uPort, uLunIdx, pIngBuf))
    {
        LunDevice[uLunIdx].u8DevType = (pIngBuf[1] & 0x80) | (pIngBuf[0] & 0x1F);
        SCSI_DbgPrint("SCSI INQUIRY : SCSI Device ID <%s>\n",&pMass_stor->device.u8VendorID);
        SCSI_DbgPrint("SCSI INQUIRY : SCSI Product ID <%s>\n",&pMass_stor->device.u8ProductID);
        SCSI_DbgPrint("SCSI INQUIRY : SCSI Product ver <%s>\n",&pMass_stor->device.u8ProductVer);
    }
    else
        return FALSE;

    u8DevType = LunDevice[uLunIdx].u8DevType & 0x1F;
    if( (u8DevType == TYPE_ROM) ||  // FS does't support CD-ROM well
        (u8DevType == TYPE_SES) )
    {
        return FALSE;
    }

    if(ms_bSCSI_READ_CAPACITY(uPort, uLunIdx, &LunDevice[uLunIdx].u32BlockTotalNum, &LunDevice[uLunIdx].u32BlockSize))
    {
        SCSI_DbgPrint("Read CAPACITY: TotalBlks %d, BlkSize %d\n",
                                LunDevice[uLunIdx].u32BlockTotalNum, LunDevice[uLunIdx].u32BlockSize);
        LunDevice[uLunIdx].u32BlockSize_log2 = ms_get_value_log2(LunDevice[uLunIdx].u32BlockSize);
    }
    else
    {
        // Some cameras need real TUR command
        // to start work. We send real TUR here for these special devices.
        ms_bSCSI_TEST_UNIT_READY(uPort, uLunIdx, FALSE);
        return FALSE;
    }

    LunDevice[uLunIdx].bWriteProtect = FALSE;
    if (ms_bSCSI_MODE_SENSE(uPort, uLunIdx, pModSenBuf, 8))     // Just read the header of mode sense
    {
        LunDevice[uLunIdx].bWriteProtect = (pModSenBuf[2]==0x80) ? TRUE:FALSE;
        if (LunDevice[uLunIdx].bWriteProtect )
        {
            diag_printf("Usb Device WriteProtected! \n");
        }
    }

    return TRUE;
}
extern MS_BOOL UsbReady;
extern MS_BOOL UsbReady_Port2;
BOOL ms_bSCSI_Initial(U8 uPort)
{
    BOOL        bFoundValidDev = FALSE;
    struct ms_usdata *pMass_stor;
    struct LUN_Device* LunDevice;
    U8      pIngBuf[DATA_LENGTH_INQUIRY];
    int     LunIdx;

    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        return FALSE;

    LunDevice = pMass_stor->msc_device;
    // Firstly, we send a Inquiry to every LUN
    // It is simulated from PC
    for (LunIdx=0; LunIdx<=pMass_stor->max_lun; LunIdx++)
    {
        LunDevice[LunIdx].bDeviceReady = FALSE;
        if (LunIdx == 0)
        {
            memset(pIngBuf, 0, DATA_LENGTH_INQUIRY);
            if (ms_bSCSI_INQUIRY(uPort, 0, pIngBuf))
            {
                // save device type
                pMass_stor->device.u8DeviceType = (pIngBuf[0] & 0x1F);
                // save vendor string, total 8 bytes
                memcpy(pMass_stor->device.u8VendorID, &pIngBuf[8], 8);
                // save product string, total 16 bytes
                memcpy(pMass_stor->device.u8ProductID, &pIngBuf[16], 16);
                // save revision information, total 4 bytes
                memcpy(pMass_stor->device.u8ProductVer, &pIngBuf[32], 4);
                SCSI_DbgPrint("SCSI INQUIRY : SCSI Device ID <%s>\n",&pMass_stor->device.u8VendorID);
                SCSI_DbgPrint("SCSI INQUIRY : SCSI Product ID <%s>\n",&pMass_stor->device.u8ProductID);
                SCSI_DbgPrint("SCSI INQUIRY : SCSI Product ver <%s>\n",&pMass_stor->device.u8ProductVer);
            }
            else
                goto Init_Done;
        }
        else
        {
            if (!ms_bSCSI_INQUIRY(uPort, LunIdx, pIngBuf))
                goto Init_Done;
        }
        LunDevice[LunIdx].u8DevType = (pIngBuf[0] & 0x1F) | (pIngBuf[1] & 0x80);
        if (LunDevice[LunIdx].u8DevType & 0x80)
            diag_printf("Removable MSC Lun %d\n", LunIdx);
    }

    // Start to init every LUN
    for (LunIdx=0; LunIdx<=pMass_stor->max_lun; LunIdx++)
    {
        if (!ms_bInit_USB_LUN(uPort, LunIdx))
            continue;

        LunDevice[LunIdx].bDeviceReady = TRUE;

        if (ConnectUSBDisk(uPort, LunIdx))
            LunDevice[LunIdx].bFSInit = TRUE;

        bFoundValidDev = TRUE;
    }
Init_Done:
    SCSI_DbgPrint("Exit ms_bSCSI_Initial %d\n", bFoundValidDev);

    if (bFoundValidDev)
    {
        if (uPort < MAX_USTOR)
            UsbReady=TRUE;
        else if (uPort < MAX_USTOR*2)
            UsbReady_Port2=TRUE;
    }
    return bFoundValidDev;
}

/* eCos USB host driver API, do not alter the function name!!!
 * the function body not maintained, just complaint with the API interface
*/
BOOL ChkUsbReady(void)
{
    return UsbReady;
}

/* eCos USB host driver API, do not alter the function name!!!
 * the function body not maintained, just complaint with the API interface
*/
MS_BOOL ChkUsbReadyEx(MS_U8 uHostPort)
{
    if (uHostPort == 0)
            return UsbReady;
    else if (uHostPort == 1)
            return UsbReady_Port2;
    else
    {
        diag_printf("Invalid port number for ms_ChkUsbReadyEx\n");
        return FALSE;
    }
}

BOOL ms_bInit_USB_Disk(U8 uPort)
{
    struct ms_usdata *pMass_stor;
    int     i;

    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        return FALSE;

    for (i=0; i<=pMass_stor->max_lun; i++)
    {
        //RemoveUSBDiskPort(uPort, i); // no need
        pMass_stor->msc_device[i].u8LunNum = i;
    }
    return ms_bSCSI_Initial(uPort);
}

BOOL ms_vChk_USB_LUNs(U8 uPort)
{
    int         LunIdx;
    struct ms_usdata *pMass_stor;
    struct LUN_Device* LunDevice;
    BOOL        bLUNSts;
    BOOL        retv = TRUE;
    U8 u8DevType;

    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        return FALSE;

    LunDevice = pMass_stor->msc_device;

    for (LunIdx=0; LunIdx<=pMass_stor->max_lun; LunIdx++)
    { 
        if ((bLUNSts = ms_bSCSI_TEST_UNIT_READY(uPort, LunIdx, TRUE)) != LunDevice[LunIdx].bDeviceReady) // Status of LUN changed
        {
            if (bLUNSts)	// No media card -> has media card
            {
                if (ms_bInit_USB_LUN(uPort, LunIdx))
                {
                    LunDevice[LunIdx].bDeviceReady = TRUE;
                    RemoveUSBDiskPort(uPort, LunIdx);
                    if (ConnectUSBDisk(uPort, LunIdx))
                        LunDevice[LunIdx].bFSInit = TRUE;
                }
                u8DevType = LunDevice[LunIdx].u8DevType & 0x1F;                
                if( (u8DevType == TYPE_ROM) ||
                    (u8DevType == TYPE_SES) )
                {
                    LunDevice[LunIdx].bDeviceReady = TRUE;
                }
            }
            else	// has media card -> no media card
            {
                LunDevice[LunIdx].bDeviceReady = FALSE;
                LunDevice[LunIdx].bFSInit = FALSE;
                Set_Stor_Dev_Init(uPort, FALSE);
                diag_printf("TUR fail -> remove disk, us_id: %d\n", uPort);
                RemoveUSBDiskPort(uPort, LunIdx);
                Clr_Stor_Dev_Info(uPort);
                retv = FALSE;
            }
        }
    }
    return retv;
}

void ms_vRemove_DISK_LUNs(U8 uPort)
{
    int         LunIdx;
    struct ms_usdata *pMass_stor;
    struct LUN_Device* LunDevice;

    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        return;

    LunDevice = pMass_stor->msc_device;

    for (LunIdx=0; LunIdx<=pMass_stor->max_lun; LunIdx++)
    {
        LunDevice[LunIdx].bDeviceReady = FALSE;
        LunDevice[LunIdx].bFSInit = FALSE;
        RemoveUSBDiskPort(uPort, LunIdx);
    }
}

BOOL ms_bSCSI_ERASE(U8 uPort, U8 uLunNum, U32 u32BlockAddr, U32 u32BlockNum,
                            U32 u32SectorSize)
{
    if (uLunNum > Mass_stor_us[uPort]->max_lun)
        return FALSE;
    else
        return TRUE;
}

BOOL ms_bSCSI_Read_10_512(U8 uPort, U8 uLunNum, U32 u32BlockAddr, U32 u32BlockNum,
                            U8 *u8Buffer)
{
    struct ms_usdata *pMass_stor;
    //U32 u32TransBlockTmp, u32TransSizeTmp,u32BlockOfSet = 0, u32TransOfSet = 0;
    BOOL    bRet = TRUE;;
    struct LUN_Device* LunDevice;
    U32 u32BlkSize_log2;
    U32 u32DataLeft;
    U32 u32BlockSize;
    U8  *lpu8TempBuf = NULL;

    U32 u32RealBlkAddr;
    U32 u32RealBlkOffset;
    U32 u32RealBlkLast;
    U32 u32RealBlkLeft;
    U32 u32CopyBytes;
    U32 u32BlockNumAdj=0;

    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
    {
        bRet = FALSE;
        goto Read10_done;
    }

    LunDevice = pMass_stor->msc_device;
    u32BlkSize_log2 = LunDevice[uLunNum].u32BlockSize_log2;
    u32DataLeft = u32BlockNum << 9;
    u32BlockSize = LunDevice[uLunNum].u32BlockSize;

    if(u32BlockSize != 512)
    {
        u32RealBlkAddr = (u32BlockAddr * 512) >> u32BlkSize_log2;
        u32RealBlkOffset = u32BlockAddr * 512 - u32RealBlkAddr * u32BlockSize;

        u32RealBlkLast = ((u32BlockAddr + u32BlockNum) * 512) >> u32BlkSize_log2;
        u32RealBlkLeft = ((u32BlockAddr + u32BlockNum) * 512) - u32RealBlkLast * u32BlockSize;

        u32BlockNumAdj = u32RealBlkLast - u32RealBlkAddr + 1;


        if(u32RealBlkOffset > 0)
        {
            lpu8TempBuf = kmalloc(u32BlockSize, GFP_KERNEL);
            bRet = ms_bSCSI_Read_10(uPort, uLunNum, u32RealBlkAddr, 1, lpu8TempBuf);
            if(!bRet)
                goto Read10_done;
            if(u32DataLeft > (u32BlockSize - u32RealBlkOffset))
            {
                u32CopyBytes = u32BlockSize - u32RealBlkOffset;
                u32DataLeft -= u32CopyBytes;
                //u32ByteOffset = u32RealBlkOffset;
            }
            else
            {
                u32DataLeft = 0;
                u32CopyBytes = u32DataLeft;
            }

            memcpy(u8Buffer,
                    lpu8TempBuf + u32RealBlkOffset ,
                    (size_t)u32CopyBytes);
            u32BlockNumAdj--;
            u32RealBlkAddr++;
            u8Buffer += u32CopyBytes;
        }//Deal with beginning block

        if(u32BlockNumAdj > 1)  //Transfer mid blocks
        {
            if(u32RealBlkLeft != 0)
                u32BlockNumAdj--;

            bRet = ms_bSCSI_Read_10(uPort, uLunNum, u32RealBlkAddr, u32BlockNumAdj, u8Buffer);
            if(!bRet)
                goto Read10_done;
            u8Buffer += u32BlockNumAdj * u32BlockSize;
            u32DataLeft -= u32BlockNumAdj * u32BlockSize;
            if(u32DataLeft > 0)
                u32BlockNumAdj = 1;
            else
                u32BlockNumAdj = 0;
        }

        if(u32BlockNumAdj > 0) // Have last block
        {
            if(u32RealBlkLeft > 0)
            {
                if(lpu8TempBuf == NULL)
                    lpu8TempBuf = kmalloc(u32BlockSize, GFP_KERNEL);
                bRet = ms_bSCSI_Read_10(uPort, uLunNum, u32RealBlkLast, 1, lpu8TempBuf);
                if(!bRet)
                    goto Read10_done;

                memcpy(u8Buffer,
                        lpu8TempBuf,
                        (size_t)u32DataLeft);
            }//Deal with ending block
        }
    }
    else
    {
        bRet = ms_bSCSI_Read_10(uPort, uLunNum, u32BlockAddr, u32BlockNum, u8Buffer);
    }

Read10_done:
    if(lpu8TempBuf != NULL)
        kfree(lpu8TempBuf);
    return bRet;
}

int USB_RW_lock = 0; // [15:8] read:1, write:2; [7:0] host_id 
BOOL ms_bSCSI_Read_10(U8 uPort, U8 uLunNum, U32 u32BlockAddr, U32 u32BlockNum,
                            U8 *u8Buffer)
{
    struct ms_usdata *pMass_stor;
    U32 u32TransBlockTmp, u32TransSizeTmp,u32BlockOfSet = 0, u32TransOfSet = 0;
    BOOL    bRet = TRUE, bConnSts;
    int     i;
    U8      host_id;

    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        return FALSE;
#ifdef ENABLE_RW_DISCONNECTING    
    if (ms_test_bit(MS_DISCONNECTING, &pMass_stor->flags))
    {
            SCSI_DbgPrint("[ms_bSCSI_Read_10] OUT\n");
            return FALSE;
    }
#endif    
    host_id = pMass_stor->host_id;
    USB_RW_lock = 0x100 | host_id;
    ms_USBCriticalSectionIn(host_id);
    USB_RW_lock = 0;
    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
    {
        bRet=FALSE;
        goto Func_Done;
    }
    
    struct LUN_Device* LunDevice = pMass_stor->msc_device;
    U32 u32BlkSize_log2 = LunDevice[uLunNum].u32BlockSize_log2;
    U32 u32DataLeft = u32BlockNum << u32BlkSize_log2;
    if (uLunNum > pMass_stor->max_lun)
    {
        SCSI_DbgPrint("Invalid LUN Index %d\n",uLunNum);
        bRet = FALSE;
        goto Func_Done;
    }
    if (LunDevice[uLunNum].u32BlockTotalNum < u32BlockNum)
    {
        bRet=FALSE;
        goto Func_Done;
    }
    if (LunDevice[uLunNum].bDeviceReady == FALSE)
    {
        bRet = FALSE;
        goto Func_Done;
    }

    while(u32DataLeft > 0)
    {
        if(u32DataLeft > Scsi_Max_Transfer_Len)
        {
            u32TransBlockTmp = Scsi_Max_Transfer_Len >> u32BlkSize_log2;
            u32TransSizeTmp = u32TransBlockTmp << u32BlkSize_log2;
            u32DataLeft -= u32TransSizeTmp;
        }
        else
        {
            u32TransBlockTmp = u32DataLeft >> u32BlkSize_log2;
            u32TransSizeTmp = u32DataLeft;
            u32DataLeft = 0;
        }

        for(i =0; i< ScsiCmd_Fail_Retry+2 ;i++) // 20130221, WD HDD My Book 1140 should take 4 times to read the data at the begining
        {
            struct usb_hcd *hcd = pMass_stor->pusb_dev->bus->hcpriv;

            bConnSts = ms_RoothubPortConnected(hcd) & !(hcd->isRootHubPortReset);

            if (!bConnSts)
            {
                struct ehci_hcd *ehci = hcd_to_ehci(hcd);
                diag_printf("Device is disconnect @ read 10 (retry %d)\n", i);
#ifdef ENABLE_RW_DISCONNECTING                    
                ms_set_bit(MS_DISCONNECTING, &pMass_stor->flags, unsigned long);
#endif
                
                if (hcd->isRootHubPortReset) // debug purpose
                diag_printf("bSCSI_Read10:: ehci reset done = %x, isRootHubPortReset = %x\n", (unsigned int)ehci->u32ResetEnd[0], hcd->isRootHubPortReset);            
                            
                bRet = FALSE;
                goto Func_Done;
            }

            //if (pMass_stor==NULL)
            if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
            {
                diag_printf("Mass_stor_us==NULL!\n");
                bRet = FALSE;
                goto Func_Done;
            }

            memset(&pMass_stor->srb, 0, sizeof(struct scMsc_cmd ));

            // Build SCSI command.
            ms_vSCSICmd_READ_10(pMass_stor, uLunNum,
                    u32BlockAddr + u32BlockOfSet, (U16)u32TransBlockTmp, u8Buffer + u32TransOfSet);
            // call mass storage function to send scsi command
            ms_usb_msc_control_thread(pMass_stor);
            if (pMass_stor->srb.result != MS_STAT_GOOD)
            {
                if (pMass_stor->srb.result == MS_STAT_CHECK_CONDITION)
                {
                    if(((pMass_stor->srb.sense_buffer[2] & 0xf) == 0x02) &&
                        (pMass_stor->srb.sense_buffer[12]  == 0x3A))
                    {
                        LunDevice[uLunNum].bDeviceReady = FALSE;
                    }
                    else if(((pMass_stor->srb.sense_buffer[2] & 0xf) == 0x06) &&
                        (pMass_stor->srb.sense_buffer[12]  == 0x28))
                    {
                        LunDevice[uLunNum].bDeviceReady = FALSE;
                    }
                }

                SCSI_DbgPrint("Scsi READ_10 command failed.\n");
                bRet = FALSE;

                if (pMass_stor->srb.result == (DID_NO_CONNECT << 16))
                    break;
            }
            else
            {
                bRet = TRUE;
                break;
            }
        }

        u32BlockOfSet += u32TransBlockTmp;
        u32TransOfSet += u32TransSizeTmp;

        if (bRet == FALSE)
            break;
    }

Func_Done:
    //unlock_usb_core();
    //MsOS_ReleaseMutex(_s32MutexUSB);
    ms_USBCriticalSectionOut(host_id);
    //   MsOS_RestoreAllInterrupts(u32OldIntr);

    return bRet;
}

BOOL ms_bSCSI_Write_10_512(U8 uPort, U8 uLunNum, U32 u32BlockAddr, U32 u32BlockNum,
                            U8 *u8Buffer)
{
    struct ms_usdata *pMass_stor;
    BOOL    bRet = TRUE;;
    struct LUN_Device* LunDevice;
    U32 u32BlkSize_log2;
    U32 u32DataLeft;
    U32 u32BlockSize;
    U8  *lpu8TempBuf = NULL;

    U32 u32RealBlkAddr;
    U32 u32RealBlkOffset;
    U32 u32RealBlkLast;
    U32 u32RealBlkLeft;
    U32 u32CopyBytes;
    U32 u32BlockNumAdj=0;

    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
    {
        bRet = FALSE;
        goto Write10_done;
    }

    LunDevice = pMass_stor->msc_device;
    u32BlkSize_log2 = LunDevice[uLunNum].u32BlockSize_log2;
    u32DataLeft = u32BlockNum << 9;
    u32BlockSize = LunDevice[uLunNum].u32BlockSize;
    
    if(u32BlockSize != 512)
    {
        u32RealBlkAddr = (u32BlockAddr * 512) >> u32BlkSize_log2;
        u32RealBlkOffset = u32BlockAddr * 512 - u32RealBlkAddr * u32BlockSize;

        u32RealBlkLast = ((u32BlockAddr + u32BlockNum) * 512) >> u32BlkSize_log2;
        u32RealBlkLeft = ((u32BlockAddr + u32BlockNum) * 512) - u32RealBlkLast * u32BlockSize;

        u32BlockNumAdj = u32RealBlkLast - u32RealBlkAddr + 1;


        if(u32RealBlkOffset > 0)
        {
            lpu8TempBuf = kmalloc(u32BlockSize, GFP_KERNEL);
            bRet = ms_bSCSI_Read_10(uPort, uLunNum, u32RealBlkAddr, 1, lpu8TempBuf);
            if(!bRet)
                goto Write10_done;
            if(u32DataLeft > (u32BlockSize - u32RealBlkOffset))
            {
                u32CopyBytes = u32BlockSize - u32RealBlkOffset;
                u32DataLeft -= u32CopyBytes;
                //u32ByteOffset = u32RealBlkOffset;
            }
            else
            {
                u32DataLeft = 0;
                u32CopyBytes = u32DataLeft;
            }

            memcpy(lpu8TempBuf + u32RealBlkOffset , u8Buffer, (size_t)u32CopyBytes);
            bRet = ms_bSCSI_Write_10(uPort, uLunNum, u32RealBlkAddr, 1, lpu8TempBuf);
            if(!bRet)
                goto Write10_done;
            u32BlockNumAdj--;
            u32RealBlkAddr++;
            u8Buffer += u32CopyBytes;
        }//Deal with beginning block

        if(u32BlockNumAdj > 0)  //Transfer mid blocks
        {
            if(u32RealBlkLeft != 0)
                u32BlockNumAdj--;

            bRet = ms_bSCSI_Write_10(uPort, uLunNum, u32RealBlkAddr, u32BlockNumAdj, u8Buffer);
            if(!bRet)
                goto Write10_done;
            u8Buffer += u32BlockNumAdj * u32BlockSize;
            u32DataLeft -= u32BlockNumAdj * u32BlockSize;
            if(u32DataLeft > 0)
                u32BlockNumAdj = 1;
            else
                u32BlockNumAdj = 0;
        }

        if(u32BlockNumAdj > 0) // Have last block
        {
            if(u32RealBlkLeft > 0)
            {
                if(lpu8TempBuf == NULL)
                    lpu8TempBuf = kmalloc(u32BlockSize, GFP_KERNEL);
                bRet = ms_bSCSI_Read_10(uPort, uLunNum, u32RealBlkLast, 1, lpu8TempBuf);
                if(!bRet)
                    goto Write10_done;

                memcpy(lpu8TempBuf, u8Buffer, (size_t)u32DataLeft);
                bRet = ms_bSCSI_Write_10(uPort, uLunNum, u32RealBlkLast, 1, lpu8TempBuf);
                if(!bRet)
                    goto Write10_done;
            }//Deal with ending block
        }
    }
    else
    {
        bRet = ms_bSCSI_Write_10(uPort, uLunNum, u32BlockAddr, u32BlockNum, u8Buffer);
    }

Write10_done:
    if(lpu8TempBuf != NULL)
        kfree(lpu8TempBuf);
    return bRet;
}



BOOL ms_bSCSI_Write_10(U8 uPort, U8 uLunNum, U32 u32BlockAddr, U32 u32BlockNum, U8 *u8Buffer)
{
    struct ms_usdata *pMass_stor;
    U32 u32TransBlockTmp, u32TransSizeTmp,u32BlockOfSet = 0, u32TransOfSet = 0;
    BOOL    bRet = TRUE, bConnSts;
    int     i;
    int     retrycount=0;
    U8      host_id;

    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
        return FALSE;
#ifdef ENABLE_RW_DISCONNECTING   
    if (ms_test_bit(MS_DISCONNECTING, &pMass_stor->flags))
    {
            SCSI_DbgPrint("[ms_bSCSI_Write_10] OUT\n");
            return FALSE;
    }
#endif 
    host_id = pMass_stor->host_id;
    USB_RW_lock = 0x200 | host_id;
    ms_USBCriticalSectionIn(host_id);
    USB_RW_lock = 0;
    if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
    {
        bRet=FALSE;
        goto Func_Done;
    }

    struct LUN_Device* LunDevice = pMass_stor->msc_device;
    U32 u32BlkSize_log2 = LunDevice[uLunNum].u32BlockSize_log2;
    U32 u32DataLeft = u32BlockNum << u32BlkSize_log2;

    if (uLunNum > pMass_stor->max_lun)
    {
        SCSI_DbgPrint("Invalid LUN Index %d\n",uLunNum);
        bRet = FALSE;
        goto Func_Done;
    }
    if (LunDevice[uLunNum].u32BlockTotalNum < u32BlockNum)
    {
        bRet=FALSE;
        goto Func_Done;
    }
    if (LunDevice[uLunNum].bDeviceReady == FALSE)
    {
        bRet = FALSE;
        goto Func_Done;
    }
    if(pMass_stor->msc_device[uLunNum].bWriteProtect)
    {
        SCSI_DbgPrint("Fail to write because write protection\n");
        bRet = FALSE;
        goto Func_Done;
    }

    while(u32DataLeft > 0)
    {
        if(u32DataLeft > Scsi_Max_Transfer_Len)
        {
            u32TransBlockTmp = Scsi_Max_Transfer_Len >> u32BlkSize_log2;
            u32TransSizeTmp = u32TransBlockTmp << u32BlkSize_log2;
            u32DataLeft -= u32TransSizeTmp;
        }
        else
        {
            u32TransBlockTmp = u32DataLeft >> u32BlkSize_log2;
            u32TransSizeTmp = u32DataLeft;
            u32DataLeft = 0;
        }

        for(i =0; i< ScsiCmd_Fail_Retry ;i++)
        {
            struct usb_hcd *hcd = pMass_stor->pusb_dev->bus->hcpriv;
        
            retrycount = 0;
            //When USB disk is disconnect or under reseting, FileSystem still writing data
            //and this may caused file system crush. So we block the writing operation for
            //20 secs, waiting for USB disk reseting or reconnect. 
retry:
            {
                //struct usb_hcd *hcd = pMass_stor->current_urb->dev->bus->hcpriv;
                struct usb_hcd *hcd = pMass_stor->pusb_dev->bus->hcpriv;
                bConnSts = ms_RoothubPortConnected(hcd) & !(hcd->isRootHubPortReset);
            }

            if (!bConnSts)
            {
                struct ehci_hcd *ehci = hcd_to_ehci(hcd);            
                diag_printf("Device is disconnect @ write 10 (retry %d)\n", i);
#ifdef ENABLE_RW_DISCONNECTING                    
                ms_set_bit(MS_DISCONNECTING, &pMass_stor->flags, unsigned long);
#endif
                
                if (hcd->isRootHubPortReset) // debug purpose
                    diag_printf("bSCSI_Write10:: ehci reset done = %x, isRootHubPortReset = %x\n", (unsigned int)ehci->u32ResetEnd[0], hcd->isRootHubPortReset);            
                
                retrycount++;

                if (0)  //Jonas
                {
                    ms_USBCriticalSectionOut(host_id);
                    MsOS_DelayTask(2000);
                    ms_USBCriticalSectionIn(host_id);
                    goto retry;
                }
                else
                {
                    bRet = FALSE;
                    goto Func_Done;
                }
            }
            if ( (pMass_stor = Mass_stor_us[uPort]) == NULL)
            {
                diag_printf("Mass_stor_us==NULL!\n");
                retrycount++;
                //if(retrycount <= 10)
                if(0)  //Jonas
                {
                    ms_USBCriticalSectionOut(host_id);
                    MsOS_DelayTask(2000);
                    ms_USBCriticalSectionIn(host_id);
                    goto retry;
                }
                else
                {
                    bRet = FALSE;
                    goto Func_Done;
                }
            }

            memset(&pMass_stor->srb, 0, sizeof(struct scMsc_cmd ));

            // Build SCSI command.
            ms_vSCSICmd_WRITE_10(pMass_stor, uLunNum,
                                u32BlockAddr + u32BlockOfSet, (U16)u32TransBlockTmp, u8Buffer + u32TransOfSet);

            // call mass storage function to send scsi command
            ms_usb_msc_control_thread(pMass_stor);

            if(pMass_stor->srb.result != MS_STAT_GOOD)
            {
                if (pMass_stor->srb.result == MS_STAT_CHECK_CONDITION)
                {
                    if(((pMass_stor->srb.sense_buffer[2] & 0xf) == 0x02) &&
                        (pMass_stor->srb.sense_buffer[12]  == 0x3A))
                    {
                        LunDevice[uLunNum].bDeviceReady = FALSE;
                    }
                    else if(((pMass_stor->srb.sense_buffer[2] & 0xf) == 0x06) &&
                        (pMass_stor->srb.sense_buffer[12]  == 0x28))
                    {
                        LunDevice[uLunNum].bDeviceReady = FALSE;
                    }
                }

                SCSI_DbgPrint("Scsi WRITE_10 command failed.\n");
                bRet = FALSE;
                
                if (pMass_stor->srb.result == (DID_NO_CONNECT << 16))
                    break;                
            }
            else
            {
                bRet = TRUE;
                break;
            }
        }

        //err = MsOS_ReleaseSemaphore(g_SCSISem);
        //USB_ASSERT(err, "Signal SCSI sem fail\n");

        u32BlockOfSet += u32TransBlockTmp;
        u32TransOfSet += u32TransSizeTmp;

        if (bRet == FALSE)
            break;
    }

Func_Done:
    //unlock_usb_core();
    //MsOS_ReleaseMutex(_s32MutexUSB);
    ms_USBCriticalSectionOut(host_id);
    //MsOS_RestoreAllInterrupts(u32OldIntr);

    return bRet;
}

static void ms_vScsi_SendCmd_Done(struct scMsc_cmd *srb)
{
    SCSI_DbgPrint("SCSI command (0x%x) Done, result = 0x%x\n", srb->cmnd[0], srb->result);
}

BOOL ms_bIsDevValid(U8 uPort, U8 LunNum)
{
    struct LUN_Device* LunDevice = Mass_stor_us[uPort]->msc_device;

    if (LunNum <= Mass_stor_us[uPort]->max_lun)
        return (LunDevice[LunNum].bFSInit);
    else
        return FALSE;
}

U8 ms_u8GetDevType(U8 uPort, U8 LunNum)
{
    struct LUN_Device* LunDevice = Mass_stor_us[uPort]->msc_device;

    if (LunNum <= Mass_stor_us[uPort]->max_lun)
        return LunDevice[LunNum].u8DevType;
    else
        return  0;
}

/*-------------------------------------------------------------------------*/
/* Mass storage class API */
MS_U32 MDrv_GetUsbBlockSize(MS_U8 lun)
{
    MS_U32 uBlkSize = 0;

    if ( (Mass_stor_us[0] != NULL) && (lun <= Mass_stor_us[0]->max_lun) )
    {
        uBlkSize = Mass_stor_us[0]->msc_device[lun].u32BlockSize;
    }

    return uBlkSize;
}

MS_U32 MDrv_GetUsbBlockSizeEx(MS_U8 uPort, MS_U8 lun)
{
    MS_U32 uBlkSize = 0;

    if ( (Mass_stor_us[uPort] != NULL) && (lun <= Mass_stor_us[uPort]->max_lun) )
    {
        uBlkSize = Mass_stor_us[uPort]->msc_device[lun].u32BlockSize;
    }

    return uBlkSize;
}

MS_U32 MDrv_GetUsbBlockNum(MS_U8 lun)
{
    MS_U32 uTotalBlks = 0;

    if ( (Mass_stor_us[0] != NULL) && (lun <= Mass_stor_us[0]->max_lun) )
    {
        uTotalBlks = Mass_stor_us[0]->msc_device[lun].u32BlockTotalNum;
    }

    return uTotalBlks;
}

MS_U32 MDrv_GetUsbBlockNumEx(MS_U8 uPort, MS_U8 lun)
{
    MS_U32 uTotalBlks = 0;

    if ( (Mass_stor_us[uPort] != NULL) && (lun <= Mass_stor_us[uPort]->max_lun) )
    {
        uTotalBlks = Mass_stor_us[uPort]->msc_device[lun].u32BlockTotalNum;
    }

    return uTotalBlks;
}

MS_U8 MDrv_UsbGetMaxLUNCount(void)
{
    if (Mass_stor_us[0] != NULL)
        return Mass_stor_us[0]->max_lun + 1;
    else
        return 0;
}

MS_U8 MDrv_UsbGetMaxLUNCountEx(MS_U8 uPort)
{
    if (Mass_stor_us[uPort] != NULL)
        return Mass_stor_us[uPort]->max_lun + 1;
    else
        return 0;
}

MS_BOOL MDrv_UsbBlockReadToMIU(
    MS_U8 lun,
    MS_U32 u32BlockAddr,
    MS_U32 u32BlockNum,
    MS_U32 u32MIUAddr)
{
    return ms_bSCSI_Read_10(0, lun, u32BlockAddr, u32BlockNum, (U8*) u32MIUAddr);
}

MS_BOOL MDrv_UsbBlockReadToMIUEx(
    MS_U8 uPort,
    MS_U8 lun,
    MS_U32 u32BlockAddr,
    MS_U32 u32BlockNum,
    MS_U32 u32MIUAddr)
{
    return ms_bSCSI_Read_10(uPort, lun, u32BlockAddr, u32BlockNum, (U8*) u32MIUAddr);
}

MS_BOOL MDrv_UsbBlockWriteFromMIU(
    MS_U8 lun,
    MS_U32 u32BlockAddr,
    MS_U32 u32BlockNum,
    MS_U32 u32MIUAddr)
{
    return ms_bSCSI_Write_10(0, lun, u32BlockAddr, u32BlockNum, (U8*) u32MIUAddr);
}

MS_BOOL MDrv_UsbBlockWriteFromMIUEx(
    MS_U8 uPort,
    MS_U8 lun,
    MS_U32 u32BlockAddr,
    MS_U32 u32BlockNum,
    MS_U32 u32MIUAddr)
{
    return ms_bSCSI_Write_10(uPort, lun, u32BlockAddr, u32BlockNum, (U8*) u32MIUAddr);
}

MS_BOOL MDrv_UsbIsLunConnected(MS_U8 uPort, MS_U8 lun)
{
    struct LUN_Device* LunDevice;

    if (Mass_stor_us[uPort] == NULL)
        return FALSE;

    LunDevice = Mass_stor_us[uPort]->msc_device;

    if (lun <= Mass_stor_us[uPort]->max_lun)
        return (LunDevice[lun].bFSInit);
    else
        return FALSE;

}

void ms_MSC_fast_device_disconnect(MS_U8 uDevS, MS_U8 uDevE)
{
    MS_U8       us_id; // for MSC
    
    for (us_id=uDevS; us_id<uDevE; us_id++)
    {
        if (Is_Stor_Dev_Info_Valid(us_id))   // Check if we found a Mass Stoarge device.
        {
            struct ms_usdata *pMass_stor = Mass_stor_us[us_id];
            ms_set_bit(MS_DISCONNECTING, &pMass_stor->flags, unsigned long);
        }
    }
}

MS_BOOL ms_readyChk_USB_LUNs(void)
{
    static MS_U32 usbTURStartTime = 0;

    if (MsOS_GetSystemTime()-usbTURStartTime > 1000 )
    {
        usbTURStartTime = MsOS_GetSystemTime();
        return TRUE;
    }
    return FALSE;
}

void ms_MSC_device_inquiry(MS_U8 uDevS, MS_U8 uDevE, MS_BOOL isMSCPlugIn)
{
    MS_U8       ii, us_id; // for MSC
    
    for (us_id=uDevS; us_id<uDevE; us_id++)
    {
        if (Is_Stor_Dev_Info_Valid(us_id))   // Check if we found a Mass Stoarge device.
        {
            isMSCPlugIn = TRUE;
            if (!Is_Stor_Dev_Init(us_id))    // First time after connected
            {
                diag_printf("found a Mass Storage device @ port %d, try to init it\n", Mass_stor_us[us_id]->host_id);
    
                if (ms_bInit_USB_Disk(us_id))
                {
                    for (ii=0; ii<=Get_Stor_Max_Lun(us_id); ii++)
                    {
                        if (ms_bIsDevValid(us_id, ii))
                        {
                            diag_printf("LUN %d is init ok\n", ii);
                        }
                    }
                    Set_Stor_Dev_Init(us_id, TRUE);
                    diag_printf("MSC plug in\n");
                }
            }
            else if (ms_readyChk_USB_LUNs())
            {
                if (ms_vChk_USB_LUNs(us_id)) // all LUN ready
                {
                    for (ii=0; ii<=Get_Stor_Max_Lun(us_id); ii++)
                    {
                        if (ms_bIsDevValid(us_id, ii))
                        {
                            SCSI_DbgPrint("Chk LUN %d is init ok\n", ii);
                        }
                    }
                }
            }
        }
    }
}

