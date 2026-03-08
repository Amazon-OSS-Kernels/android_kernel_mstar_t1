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

#ifndef _CONTROL_H_
#define _CONTROL_H_

extern void CTL_main( void *pvParameters );
extern void CTL_Init(void);
extern void CTL_Deinit(void);

#define CTL_VERSION         0x14063017
#define CTL_INFO_ADDR         0x0

// _ctl_info statue
#define CTL_STU_NONE         0
#define CTL_STU_INIT         1
#define CTL_STU_TASK         2

// _ctl_info task_statue[x]
#define CTL_TASK_NONE       0
#define CTL_TASK_CREATE     1  // task has already created by controller
#define CTL_TASK_CMDRDY     2  // task has already inited and ready to get command

// _ctl_info task_mode
#define CTL_MODE_NORMAL                  0
#define CTL_MODE_3DWMV                 1  // 3d wmv
#define CTL_MODE_3DTV                  2  // mpeg2+h.264
#define CTL_MODE_3DTV_PROG             3  // Korea 3DTV forced progressive mode
#define CTL_MODE_ONE_STC               4  // only one STC, sub view sync main stc
#define CTL_MODE_SWITCH_STC            5  // switch target STC , main view sync sub stc and  sub view sync main stc
#define CTL_MODE_3DTV_TWO_PITCH        6  //Korea 3DTV, 2nd pitch enabled for 3DLR
#define CTL_MODE_3DTV_PROG_TWO_PITCH   7  // Korea 3DTV PROG, 2nd pitch enabled for 3DLR
#define CTL_MODE_SEC_MCU               8

//fw allocate dram
#if defined(SUPPORT_NEW_MEM_LAYOUT)
#define DRAM_OFFSET 0x20000     // starts from 0xB0000+0x20000
#elif (SUPPORT_EVD) // Local FPGA verification
#define DRAM_START 0x100000
#else  //For HVD and MVD, or real chip verification
#define DRAM_START 0xA0000
#endif // #if defined(SUPPORT_NEW_MEM_LAYOUT)

#if defined(SUPPORT_NEW_MEM_LAYOUT)
  #if defined(SUPPORT_EVD) && (SUPPORT_EVD==1)
  #define HEAP_START 0xD0000
  #else
  #define HEAP_START 0xC0000
  #endif
#else
  #define HEAP_START 0xA0000
#endif

#define VSYNC_BRIDGE_OFFSET 0x1FA00


#if defined(SUPPORT_VDEC_STR)
/*
    | STR_FLAG : 16byte | CTL_CMD : 15 set | MAIN_CMD : 120 set | SUB_CMD : 120 set |

    1 set = 16 byte
    total str buffer ~ 4k
*/

#define VDEC_STR_ALIGN  16
#define VDEC_STR_CTL_CMD_RESERVERD  8
#define VDEC_STR_CMD_RESERVERD 120

#define VDEC_STR_BUFFER_START      0x2B0000
#define VDEC_STR_MAIN_CTL_CMD_BUF  (VDEC_STR_BUFFER_START + VDEC_STR_ALIGN)
#define VDEC_STR_SUB_CTL_CMD_BUF   (VDEC_STR_MAIN_CTL_CMD_BUF + (VDEC_STR_ALIGN * VDEC_STR_CTL_CMD_RESERVERD))
#define VDEC_STR_MAIN_CMD_BUF      (VDEC_STR_SUB_CTL_CMD_BUF  + (VDEC_STR_ALIGN * VDEC_STR_CTL_CMD_RESERVERD))
#define VDEC_STR_SUB_CMD_BUF      (VDEC_STR_MAIN_CMD_BUF + (VDEC_STR_ALIGN * VDEC_STR_CMD_RESERVERD))

#define VDEC_STR_MAIN_WORK         VDEC_STR_BUFFER_START
#define VDEC_STR_SUB_WORK          VDEC_STR_BUFFER_START+0x1
#define VDEC_STR_MAIN_RESUME       VDEC_STR_BUFFER_START+0x2
#define VDEC_STR_SUB_RESUME        VDEC_STR_BUFFER_START+0x3
#define VDEC_STR_MAIN_CTL_CMD_COUNT    VDEC_STR_BUFFER_START+0x4
#define VDEC_STR_SUB_CTL_CMD_COUNT     VDEC_STR_BUFFER_START+0x5
#define VDEC_STR_MAIN_CMD_COUNT        VDEC_STR_BUFFER_START+0x6
#define VDEC_STR_SUB_CMD_COUNT         VDEC_STR_BUFFER_START+0x8  //0x7 for VDEC_UNMUTE_BYTE

#define VDEC_STR_CMD     4
#define VDEC_STR_ARG0    8
#define VDEC_STR_ARG1    9
#define VDEC_STR_ARG2    10
#define VDEC_STR_ARG3    11
#define VDEC_STR_ARG4    12
#define VDEC_STR_ARG5    13

#define VDEC_STR_MVD 1
#define VDEC_STR_HVD 2

#define VDEC_UNMUTE_BYTE  7

#endif
/* Structure defination */
struct _ctl_info {
    const unsigned int readonly[4];       // CTL_INFO_ADDR + 0x00 read only for tag.
    unsigned int vpu_clk;                 // CTL_INFO_ADDR + 0x10 reserved for driver to fw message.(VDEC CPU clock)
    unsigned int ctl_interface;           // CTL_INFO_ADDR + 0x14 driver interface(read only)
    unsigned int heap_size[2];            // CTL_INFO_ADDR + 0x18 heap size available for each task
    unsigned int verion;                  // CTL_INFO_ADDR + 0x20
    unsigned int statue;                  // CTL_INFO_ADDR + 0x24
    unsigned int last_ctl_cmd;            // CTL_INFO_ADDR + 0x28
    unsigned int last_ctl_arg;            // CTL_INFO_ADDR + 0x2C
    unsigned int task_statue[4];          // CTL_INFO_ADDR + 0x30 fixed to 4 elements for alignment
    unsigned int task_single;             // CTL_INFO_ADDR + 0x40
    unsigned short task_mode[2];          // CTL_INFO_ADDR + 0x44 0:normal 1:3d WMV 2:korea 3d TV
    unsigned int burst_mode;              // CTL_INFO_ADDR + 0x48 0:normal 1:burst cmd
    unsigned char task_hvd;               // CTL_INFO_ADDR + 0x4c
    unsigned char task_mvd;               // CTL_INFO_ADDR + 0x4d
    unsigned short u16TaskFeature;        // CTL_INFO_ADDR + 0x4e
    unsigned int u32Reserved;             // CTL_INFO_ADDR + 0x50 reserved
    unsigned int u32TaskShareInfoAddr[4]; // CTL_INFO_ADDR + 0x54 offset from FW beginning
    unsigned int u32VsyncBridgeAddr;      // CTL_INFO_ADDR + 0x64
    unsigned int FB_ADDRESS;              // CTL_INFO_ADDR + 0x68 , this value is offset of miu, unit is byte
    unsigned int FB_Total_SIZE;           // CTL_INFO_ADDR + 0x6C , unit is byte
    unsigned int FB_Used_SIZE;            // CTL_INFO_ADDR + 0x70 , unit is byte
} ;


extern struct _ctl_info *g_ctl_ptr;
extern unsigned char Wakeup_Controller(unsigned char ISR);
extern unsigned char CTL_burst_cmd(unsigned int cmd, unsigned int arg);



#endif // _CONTROL_H_

