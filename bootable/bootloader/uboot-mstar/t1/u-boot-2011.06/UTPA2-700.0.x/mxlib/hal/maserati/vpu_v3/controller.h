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

#define CTL_VERSION         0x16040120
#define CTL_INFO_ADDR         0x0

// _ctl_info statue
#define CTL_STU_NONE         0
#define CTL_STU_INIT         1
#define CTL_STU_TASK         2

// _ctl_info task_statue[x]
#define CTL_TASK_NONE       0
#define CTL_TASK_CREATE     1  // task has already created by controller
#define CTL_TASK_CMDRDY     2  // task has already inited and ready to get command
#define CTL_TASK_TO_BE_DELETED      3  // task is going to be deteled

// _ctl_info task_mode
#define CTL_MODE_NORMAL                0
#define CTL_MODE_3DWMV                 1  // 3d wmv
#define CTL_MODE_3DTV                  2  // mpeg2+h.264
#define CTL_MODE_3DTV_PROG             3  // Korea 3DTV forced progressive mode
#define CTL_MODE_ONE_STC               4  // only one STC, sub view sync main stc
#define CTL_MODE_SWITCH_STC            5  // switch target STC , main view sync sub stc and  sub view sync main stc
#define CTL_MODE_3DTV_TWO_PITCH        6  //Korea 3DTV, 2nd pitch enabled for 3DLR
#define CTL_MODE_3DTV_PROG_TWO_PITCH   7  // Korea 3DTV PROG, 2nd pitch enabled for 3DLR
#define CTL_MODE_SEC_MCU               8

#define MAX_TASKS 16 // max tasks number
#define VSYNC_BRIDGE_TASK_NUM 4

#define VDEC_FW31

#ifdef LIGHTWEIGHT //FW31_1.8M
#define COMMON_AREA_START 0xB0000
#define HEAP_START        0xE0000
#else
#if defined(SUPPORT_EVD) && (SUPPORT_EVD==1)
#define COMMON_AREA_START 0xF0000
#define HEAP_START       0x130000
#else
#define COMMON_AREA_START 0x90000
#define HEAP_START        0xC0000
#endif
#endif

/*
    == Common Area Layout ==

    +-----------------------------------+   0x0
    | HVD_BBU_TBL_SIZE * 4              |
    +-----------------------------------+   0x4000
    | EVD_BBU_TBL_SIZE * 4              |
    +-----------------------------------+   0x5000
    | VP8_BBU_TBL_SIZE * 2              |
    +-----------------------------------+   0x5800
    | MVD_SLQ_TBL_SIZE * 4              |
    +-----------------------------------+   0x6000
    | VSyncBridge structure * 2         |
    | ds_xc_data_structure (old usage)  |
    +-----------------------------------+   0x6800
    | VSyncBridgeExt structure * 2      |
    +-----------------------------------+   0x7000
    | VSyncBridge structure * 2         |
    +-----------------------------------+   0x7800
    | VSyncBridgeExt structure * 2      |
    +-----------------------------------+   0x8000
    | DS IP OP Page                     |
    +-----------------------------------+   0x9F00
    | DS Scaler Info                    |
    +-----------------------------------+   0xA000

*/
#if 1
#define HVD_BBU_TBL_SIZE       0x1000
#define EVD_BBU_TBL_SIZE       0x1000
#define VP8_BBU_TBL_SIZE       0x1000
#define MVD_SLQ_TBL_SIZE        0x800
#define VSYNC_BRIDGE_INFO_SIZE  0x400

#define HVD_BBU_TBL_OFFSET               0x0
#define EVD_BBU_TBL_OFFSET            0x4000
#define VP8_BBU_TBL_OFFSET            0x8000
#define MVD_SLQ_TBL_OFFSET            0xA000
#define VSYNC_BRIDGE_OFFSET           0xC000  // 2 * sizeof(MCU_DISPQ_INFO) + sizeof(ds_xc_data_structure)
#define VSYNC_BRIDGE_EXT_OFFSET       0xC800  // 2 * sizeof(MCU_DISPQ_INFO_EXT)
#define VSYNC_BRIDGE_NWAY_OFFSET      0xD000  // 2 MCU_DISPQ_INFO, each occupy 0x400
#define VSYNC_BRIDGE_EXT_NWAY_OFFSET  0xD800  // 2 MCU_DISPQ_INFO_EXT, each occupy 0x400
#define DS_IPOP_PAGE_OFFSET           0xE000
#define DS_SCALER_INFO_OFFSET         0xFF00  //0x10F00
#else
#define HVD_BBU_TBL_SIZE       0x1000
#define EVD_BBU_TBL_SIZE        0x400
#define VP8_BBU_TBL_SIZE        0x400
#define MVD_SLQ_TBL_SIZE        0x200
#define VSYNC_BRIDGE_INFO_SIZE  0x400

#define HVD_BBU_TBL_OFFSET               0x0
#define EVD_BBU_TBL_OFFSET            0x4000
#define VP8_BBU_TBL_OFFSET            0x5000
#define MVD_SLQ_TBL_OFFSET            0x5800
#define VSYNC_BRIDGE_OFFSET           0x6000
#define VSYNC_BRIDGE_EXT_OFFSET       0x6800
#define VSYNC_BRIDGE_NWAY_OFFSET      0x7000
#define VSYNC_BRIDGE_EXT_NWAY_OFFSET  0x7800
#define DS_IPOP_PAGE_OFFSET           0x8000
#define DS_SCALER_INFO_OFFSET         0x9F00
#endif

#define COMMON_AREA_SIZE 0x10000
#define FW_TASK_SIZE    0x100000

#if defined(SUPPORT_VDEC_STR)
/*
    | STR_FLAG : 16byte | CTL_CMD : 15 set | MAIN_CMD : 120 set | SUB_CMD : 120 set |

    1 set = 16 byte
    total str buffer ~ 4k
*/

#define VDEC_STR_ALIGN  16
#define VDEC_STR_CTL_CMD_RESERVERD  8
#define VDEC_STR_CMD_RESERVERD 120

#if 0
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
#else

#define VDEC_STR_BUFFER_DUAL_OFFSET     0x2B0000
#define VDEC_STR_BUFFER_SINGLE_OFFSET     0x1D0000

#define VDEC_STR_MAIN_CTL_CMD_OFFSET  (VDEC_STR_ALIGN)
#define VDEC_STR_SUB_CTL_CMD_OFFSET   (VDEC_STR_MAIN_CTL_CMD_OFFSET + (VDEC_STR_ALIGN * VDEC_STR_CTL_CMD_RESERVERD))
#define VDEC_STR_MAIN_CMD_OFFSET      (VDEC_STR_SUB_CTL_CMD_OFFSET  + (VDEC_STR_ALIGN * VDEC_STR_CTL_CMD_RESERVERD))
#define VDEC_STR_SUB_CMD_OFFSET      (VDEC_STR_MAIN_CMD_OFFSET + (VDEC_STR_ALIGN * VDEC_STR_CMD_RESERVERD))

#define VDEC_STR_MAIN_WORK_OFFSET         0x0
#define VDEC_STR_SUB_WORK_OFFSET          0x1
#define VDEC_STR_MAIN_RESUME_OFFSET       0x2
#define VDEC_STR_SUB_RESUME_OFFSET        0x3
#define VDEC_STR_MAIN_CTL_CMD_COUNT_OFFSET    0x4
#define VDEC_STR_SUB_CTL_CMD_COUNT_OFFSET     0x5
#define VDEC_STR_MAIN_CMD_COUNT_OFFSET        0x6
#define VDEC_STR_SUB_CMD_COUNT_OFFSET         0x8  //0x7 for VDEC_UNMUTE_BYTE

#endif



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
/* Structure definition */
struct _ctl_info {
    const unsigned int readonly[4];        // CTL_INFO_ADDR + 0x00 read only for tag.
    unsigned int vpu_clk;                  // CTL_INFO_ADDR + 0x10 reserved for driver to fw message.(VDEC CPU clock)
    unsigned int ctl_interface;            // CTL_INFO_ADDR + 0x14 driver interface(read only)
    unsigned int verion;                   // CTL_INFO_ADDR + 0x18
    unsigned int statue;                   // CTL_INFO_ADDR + 0x1C
    unsigned int last_ctl_cmd;             // CTL_INFO_ADDR + 0x20
    unsigned int last_ctl_arg;             // CTL_INFO_ADDR + 0x24
    unsigned short task_single;            // CTL_INFO_ADDR + 0x28
    unsigned short burst_mode;             // CTL_INFO_ADDR + 0x2A 0:normal 1:burst cmd
    unsigned char task_hvd;                // CTL_INFO_ADDR + 0x2C
    unsigned char task_mvd;                // CTL_INFO_ADDR + 0x2D
    unsigned short u16TaskFeature;         // CTL_INFO_ADDR + 0x2E

    unsigned char task_statue[MAX_TASKS];  // CTL_INFO_ADDR + 0x30 fixed to 4 elements for alignment
    unsigned char task_mode[MAX_TASKS];    // CTL_INFO_ADDR + 0x40 0:normal 1:3d WMV 2:korea 3d TV
    unsigned int u32TaskShareInfoAddr[MAX_TASKS]; // CTL_INFO_ADDR + 0x50 offset from FW beginning

    unsigned int u32CommonAreaAddr;        // CTL_INFO_ADDR + 0x90

    unsigned int FB_ADDRESS;               // CTL_INFO_ADDR + 0x94 , this value is offset of miu, unit is byte
    unsigned int FB_Total_SIZE;            // CTL_INFO_ADDR + 0x98 , unit is byte
    unsigned int FB_Used_SIZE;             // CTL_INFO_ADDR + 0x9C , unit is byte
#ifdef VDEC3_FB
    unsigned int u32FrameBufAddr;          // CTL_INFO_ADDR + 0xA0 frame buffer base address
    unsigned int u32FrameBufSize;          // CTL_INFO_ADDR + 0xA4 frame buffer size for all tasks
    unsigned char u8FrameBufSegment;       // CTL_INFO_ADDR + 0xA8 select one enumeration from Split_FB
    unsigned char bFrameBufUsed[4];        // CTL_INFO_ADDR + 0xA9 record if each segment is used.
#endif
    unsigned char u8UseIMITaskId;          // CTL_INFO_ADDR + 0xAD indicate which task is using IMI
    unsigned char u8HicodecType;           // CTL_INFO_ADDR + 0xAE Kano, 0:Hicodec 1:Hicodec_Lite
    unsigned char u8MultiHVDTaskSameBBU;   // CTL_INFO_ADDR + 0xAF
    unsigned int  u32DolbyVisionXCShmAddr; // CTL_INFO_ADDR + 0xB0 record the dolby vision XC share memory address for transfer DM/composer
    unsigned int  u32DVDumpMetaAddrWithXC; // CTL_INFO_ADDR + 0xB4 record dolby meta XC test pattern
    unsigned char STCindex[2];             // CTL_INFO_ADDR + 0xB8
    unsigned char STCMode[2];              // CTL_INFO_ADDR + 0xBA
#if defined(SUPPORT_VDEC_STR)
    unsigned int  u32StrAddrOffset;        // CTL_INFO_ADDR + 0xBC
#endif
} ;

#define INVALID_ADDR_U32 0xFFFFFFFF

#ifdef VDEC3
#define VDEC_SHARE_MEM_MASK  0x0FFFFFFF
#define VDEC_BBU_ID_MASK     0xF0000000
#define VDEC_BBU_ID_SHIFT            28

#define MAX_VDEC_VBBU_ENTRY_COUNT 254

typedef struct
{
    unsigned int u32Offset;             ///< Packet offset from bitstream buffer base address. unit: byte.
    unsigned int u32Length;             ///< Packet size. unit: byte. ==> Move _VDEC_EX_ReparseVP8Packet to FW
    unsigned long long u64TimeStamp;    ///< Packet time stamp.
    unsigned int u32ID_L;               ///< Packet ID low part.
    unsigned int u32ID_H;               ///< Packet ID high part.
    unsigned char u8Reserved[8];        ///< Revserved space and for 16-byte alignment
} VDEC_VBBU_Entry;

typedef struct
{
    unsigned int u32WrPtr;
    unsigned int u32RdPtr;
    unsigned char u8Reserved[8];
    VDEC_VBBU_Entry stEntry[MAX_VDEC_VBBU_ENTRY_COUNT + 1];
} VDEC_VBBU;

#endif

typedef struct
{
    unsigned int u32HVD_PENDING_RELEASE_ST_ADDR;//[0][0]:0x0D98 [0][1]:0x0DA4 [1][0]:0x0DB0 [1][1]:0x0DBC
    unsigned int u32HVD_PENDING_RELEASE_SIZE;   //[0][0]:0x0D9C [0][1]:0x0DA8 [1][0]:0x0DB4 [1][1]:0x0DC0
    unsigned int u32HVD_COLLISION_NUM;          //[0][0]:0x0DA0 [0][1]:0x0DAC [1][0]:0x0DB8 [1][1]:0x0DC4
} PENDING_RELEASE_QUEUE;

typedef struct
{
    unsigned int u32HVD_DISPCMDQ_DRAM_ST_ADDR;//0x0FA8 // for VDEC3 display command queue usage
    unsigned int u32HVD_STREAM_DISPCMDQ_RD;   //0x0FAC // stream display command queue read ptr for VDEC3 display command queue usage
    unsigned int u32HVD_STREAM_DISPCMDQ_WD;   //0x0FB0 // stream display command queue write ptr for VDEC3 display command queue usage
    unsigned int u32HVD_CMDQ_DRAM_ST_ADDR;    //0x0FB4 // for VDEC3 dram command queue usage
    unsigned int u32HVD_STREAM_CMDQ_RD;       //0x0FB8 // stream command queue read ptr for VDEC3 dram command queue usage
    unsigned int u32HVD_STREAM_CMDQ_WD;       //0x0FBC // stream command queue write ptr for VDEC3 dram command queue usage
} CMD_QUEUE;

extern struct _ctl_info *g_ctl_ptr;
extern unsigned char Wakeup_Controller(unsigned char ISR);
extern unsigned char CTL_burst_cmd(unsigned int cmd, unsigned int arg);
extern volatile char g_ctl_Version[] __attribute__ ((section(".tail_version"), aligned(16)));


#endif // _CONTROL_H_

