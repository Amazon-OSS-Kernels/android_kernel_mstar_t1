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
#include "MsCommon.h"
#include "drvUSB.h"
#include "drvUsbcommon.h"

#define TEST_PATTERN_NUM  4
#define VER_USB_STACK_SIZE  4096

extern BOOL ChkUsbReady(void);

void verUSB_Task(MS_U32 argc, VOID *argv)
{
    // create AppMain task
    MS_U32 i,j,k=0;
    MS_U8  run_times=0;
    MS_U8 *TXMemory,*RXMemory;
    MS_U32 persent, persent_cmp;
    MS_U8 persent_num;
    MS_BOOL bRet = FALSE;

    TXMemory=((MS_U8*)Usb_AllocateNonCachedMemory(512*4));
    RXMemory=((MS_U8*)Usb_AllocateNonCachedMemory(512*4));

    diag_printf("verUSB_Task\n");

    //prepare pattern
    for(i=0;i<4;i++)
    {
        for(j=0;j<512;j++)
        {
            *(TXMemory+(i*512)+j) = (MS_U8)(i+j);
        }
    }
    //end pattern prepare

    while(1)    // Wait for device connected
    {
        if (ChkUsbReady())
            break;

        MsOS_DelayTask(100);
    }

    diag_printf("\r\n USB Burn Test Start !!!\n");
    diag_printf("Total Block Number=0x%lx\n",  MDrv_GetUsbBlockNum(0));

    //diag_printf("gSpeed: %d\n", sAttachDevice.bSpeed);

    persent = MDrv_GetUsbBlockNum(0)/100;

    while(1)
    {
        persent_num=0;
        persent_cmp=persent;
        //for(i=0x100;i<Mass_stor_us1.Mass_stor_device[0].u32BlockTotalNum-4;/*i++*/)
        diag_printf("R/W Testing : ");
        for(i=0;i<MDrv_GetUsbBlockNum(0)-4;/*i++*/)
        {
            #if 0
            for(j=0;j<4;j++)
            {
                MDrv_UsbDiskWrite(i, 1+j, TXMemory);
                for(k=0;k<(512*(j+1));k++)
                    RXMemory[k]=0;

                MDrv_UsbDiskRead(i, 1+j, RXMemory);

                if(usbUHCICompareData(TXMemory,RXMemory, 512*(j+1)) )
                {
                    while(1)
                    {
                        diag_printf("compare fail, LBA=%lx \n", i);
                        diag_printf("run times=%x\n",run_times);
                    }
                }
            }
            #else
            bRet = MDrv_UsbBlockWriteFromMIU(0, i, TEST_PATTERN_NUM, (MS_U32)TXMemory);
            ASSERT( (bRet == TRUE) );
            //diag_printf("....write\n");
            for(k=0;k<(512*TEST_PATTERN_NUM);k++)
                RXMemory[k]=0;
            MDrv_UsbBlockReadToMIU(0, i, TEST_PATTERN_NUM, (MS_U32)RXMemory);
            ASSERT( (bRet == TRUE) );
            //diag_printf("....read\n");

            for(k=0;k<4;k++)
            {
                //diag_printf("k = %d\n", k);
                for(j=0;j<0x200;j++)
                {
                    if (RXMemory[(k*512)+j]!= TXMemory[(k*512)+j])
                    {
                        diag_printf("compare fail, [%ld]=%x \n", (k*512)+j,RXMemory[(k*512)+j]);
                        diag_printf("run times=%x\n",run_times);
                        while(1);
                    }
                }
            }
            if ((i % 100) == 0)
            {
                diag_printf("....(1) persent_cmp = %ld, i = %ld\n", persent_cmp, i);
            }
            i+=TEST_PATTERN_NUM;
            //diag_printf("....(2) persent_cmp = %d, i = %d\n", persent_cmp, i);
            if (i>persent_cmp)
            {
                persent_num++;
                diag_printf("persent_num : %d\n", persent_num);
                persent_cmp += persent;
                if ((persent_num%2)==0)
                    diag_printf("_");
            }
            #endif

            MsOS_DelayTask(100);
        }

        run_times++;
        diag_printf("\nTEST LOOP (%x) SUCESS!!\n", run_times);
    }

}

#ifndef MS_NOSAPI
void VerUSBTask_Create(void)
{
    void *pointer;

    diag_printf("VerUSB_Create\n");
    pointer = Usb_AllocateCachedMemory(VER_USB_STACK_SIZE); // 8k bytes stack size
    if( pointer == NULL)
    {
        GEN_EXCEP;
    }
    if( MsOS_CreateTask(verUSB_Task, NULL, E_TASK_PRI_HIGH,true,pointer,VER_USB_STACK_SIZE,"verUSB_TSAK") < 0 )
    {
        GEN_EXCEP;
    }
}
#endif


