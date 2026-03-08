/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
/**
 *
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////
// file:   regCMDQ.h
// Description: CMDQ Register Definition
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CMDQ_REG_MCU_H_
#define _CMDQ_REG_MCU_H_


//--------------------------------------------------------------------------------------------------
//  Abbreviation
//--------------------------------------------------------------------------------------------------
// Addr                             Address
// Buf                              Buffer
// Clr                              Clear
// CmdQ                             Command queue
// Cnt                              Count
// Ctrl                             Control
// Flt                              Filter
// Hw                               Hardware
// Int                              Interrupt
// Len                              Length
// Ovfw                             Overflow
// Pkt                              Packet
// Rec                              Record
// Recv                             Receive
// Rmn                              Remain
// Reg                              Register
// Req                              Request
// Rst                              Reset
// Scmb                             Scramble
// Sec                              Section
// Stat                             Status
// Sw                               Software
// Ts                               Transport Stream


//--------------------------------------------------------------------------------------------------
//  Global Definition
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Compliation Option
//--------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Harware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
#define NUMBER_OF_CMDQ_HW         0x2
#define REG_CMDQCTRL_BASE         0x46A00UL //0x123500 * 2  -> 0x246A00 (0xBF246A00)
#define REG_CMDQCTRL_BASE2        0x46800UL //0x123400 * 2  -> 0x246800 (0x)

typedef struct _REG32
{
    volatile MS_U16                L;
    volatile MS_U16                empty_L;
    volatile MS_U16                H;
    volatile MS_U16                empty_H;
} REG32;


typedef struct _REG_CMDQCtrl
{
    REG32            CMDQ_Enable;                            //0x01 & 0x00
    #define CMDQ_CMDQ_EN                  0x00010000UL       //0x01 bit:0 1:enable
    #define CMDQ_CMDQ_DISEN               0x00000000UL       //0x01 bit:0 1:enable
    REG32            CMDQ_Trig_Mode;                         //0x03 & 0x02
    #define CMDQ_DMA_TRIG_EN              0x00000000UL       //0x02 bit:0 cmdq enable
    #define CMDQ_CMD_BUF_DIRECT_MODE      0x00000000UL       //0x02 bit:1 0:direct
    #define CMDQ_CMD_INCREAMENT_MODE      0x00000002UL       //0x02 bit:1 1:increament
    #define CMDQ_RING_BUFFER_MODE         0x00000004UL       //0x02 bit:4 ring buffer mode
    #define CMDQ_DMA_TRIG                 0x00010000UL       //0x03 bit:0
    #define CMDQ_MOV_CMD_PTR              0x00020000UL       //0x03 bit:1  cmdq start run
    #define CMDQ_READ_TRIG                0x01000000UL       //0x03 bit:8  read trig bit
    #define CMDQ_RST_CMD_ST_PTR_TRIG      0x00040000UL       //0x03 bit:2increament reset start
    REG32            CMDQ_Cmd_St_Ptr;                        //0x05 & 0x04 start pointer
    REG32            CMDQ_Cmd_End_Ptr;                       //0x07 & 0x06 End pointer
    REG32            CMDQ_Sw_Wr_Mi_Wadr;                     //0x09 & 0x08 write pointer
    REG32            CMDQ_Rd_Mi_Radr;                        //0x0B & 0x0A read pointer
    REG32            CMDQ_Buf_Radr_Wadr;                     //0x0D & 0x0C write & read buffer
    REG32            CMDQ_0F_0E;                             //0x0F & 0x0E  non-use
    REG32            CMDQ_Length_ReadMode;                   //0x11 & 0x10
    #define CMDQ_MIU_SELECT_MIU1          0x00000002UL       //0x10 bit:2 miu_select
    #define CMDQ_REQ_LEN_MASK             0x000d0000UL       //0x11 bit:0~4
    #define CMDQ_REQ_TH_MASK              0x09000000UL       //0x11 bit:8~11 -> not used
    #define REQ_DMA_BURST_MODE            0x10000000UL       //0x11 bit:12
    REG32            CMDQ_1f_12[7];                          //0x1f ~ 0x12
    REG32            CMDQ_Mask_Setting;                      //0x21 & 0x20
    #define CMDQ_MASK_BIT                 0x00000080UL       //0x20 bit:7=1
    #define CMDQ_WAIT_TRIG_FORCE_MASK     0x0000FFFFUL       //0x21 bit:0~15 force trig bit mask
    REG32            CMDQ_Wait_Trig;                         //0x22 & 0x23
    #define CMDQ_WAIT_TRIG_FORCE          0x0000FFFFUL       //0x22 bit:0~15 force trig bus bit
    #define CMDQ_SKIP_WAIT_TRIG_MASK      0x00010000UL       //0x23 bit:0 skip wait trigger mask bit
    #define CMDQ_CAP_WAIT_TRIG            0x00020000UL       //0x23 bit:1 capture wait trigger
    REG32            CMDQ_Force_Skip;                        //0x24 & 0x25
    #define CMDQ_SKIP_WR                  0x00000001UL
    #define CMDQ_SKIP_WAIT                0x00000002UL
    #define CMDQ_SKIP_POLLEQ              0x00000004UL
    #define CMDQ_SKIP_POLLNEQ             0x00000008UL
    #define CMDQ_SKIP_WR_MASK             0x00000010UL
    #define CMDQ_SKIP_WAIT_MASK           0x00000020UL
    #define CMDQ_SKIP_POLLEQ_MASK         0x00000040UL
    #define CMDQ_SKIP_POLLNEQ_MASK        0x00000080UL
    #define CMDQ_FORCE_WR_MASK            0x00000100UL
    #define CMDQ_FORCE_WAIT_MASK          0x00000200UL
    #define CMDQ_FORCE_POLLEQ_MASK        0x00000400UL
    #define CMDQ_FORCE_POLLNEQ_MASK       0x00000800UL
    #define CMDQ_FORCE_POLL_MISS          0x00004000UL
    #define CMDQ_FORCE_POLL_HIT           0x00008000UL
    #define CMDQ_FORCE_STOP_WRITE         0x00000001UL       //0x25 bit:0 stop write op
    #define CMDQ_FORCE_STOP_WATCH         0x00000002UL       //0x25 bit:1 stop watch (poll,eq,wait..)
    REG32            CMDQ_27_26;                             //0x26 & 0x27
    REG32            CMDQ_Tout_Base_Amount;                  //0x29 ~ 0x28
    #define TOUT_DO_NOT_JUMP              0xFF7F0000UL       //0x29 bit7: reg_tout_jump_cmd_en
    REG32            CMDQ_Cmd_Mode_Enable;                   //0x2b & 0x2a
    #define CMDQ_DEBUG_MODE_ENABLE        0x00000001UL       //0x2a bit:0
    #define CMDQ_DEBUG_ONESTEP_ENABLE     0x00000002UL       //0x2a bit:1-2 1 debug mode step one
    #define CMDQ_CMD_SEARCH_ENABLE        0x00000008UL       //0x2a bit: 3
    #define CMDQ_CMD_REPLACE_ENABLE       0x00000010UL       //0x2a bit: 4
    REG32            CMDQ_One_Step_Trig;                     //0x2d & 0x2c
    #define CMDQ_DEBUG_ONESTEP            0x00000001UL       //0x2c bit:0 trigger one step
    REG32            CMDQ_Cmd_Replace_Dat2_3;                //0x2f & 0x2e
    REG32            CMDQ_En_Clk_Miu;                        //0x31 & 0x30
    #define CMDQ_SOFT_RSTZ                0x00000000UL       //0x31 bit:0
    #define CMDQ_SOFT_RSTZ_BIT            0x00010000UL       //0x31 bit:0
    REG32            CMDQ_Debug_Sel;                         //0x33 & 0x32
    REG32            CMDQ_3f_34[6];                          //0x3f ~ 0x34
    REG32            CMDQ_Dma_State_Rb_Cmd;                  //0x41 & 0x40
    REG32            CMDQ_Cmd_Sel_Decode_State;              //0x43 & 0x42
    #define CMDQ_CRASH_15_0_BIT           0x00000000UL       //0x43 bit:3 command[15:0]
    #define CMDQ_CRASH_31_16_BIT          0x00010000UL       //0x43 bit:3 command[31:16]
    #define CMDQ_CRASH_55_32_BIT          0x00020000UL       //0x43 bit:3 command[55:32]
    #define CMDQ_CRASH_63_56_BIT          0x00030000UL       //0x43 bit:3 command[63:56]
    REG32            CMDQ_Raw_Irq_Final_Irq;                 //0x45 & 0x44
    #define CMDQ_READ_DEC_DONE            0x00000004UL       //0x44 bit:3 read cmdq done
    #define CMDQ_READ_SOFT_INTER          0x000000f0UL       //0x44 bit:4~7 read interrupt
    REG32            CMDQ_Irq_Mask_Irq_Force;                //0x47 & 0x46
    REG32            CMDQ_Sw_Trig_Cap_Sel_Irq_Clr;           //0x49 & 0x48
    #define CMDQ_DEC_DONE_CLR             0x00000004UL       //0x48 bit:3 clear cmdq done
    #define CMDQ_SOFT_INTER_CLR           0x000000f0UL       //0x48 bit:4~7 clear interrupt
    REG32            CMDQ_Poll_Ratio_Wait_Time;              //0x4b & 0x4a
    #define CMDQ_POLL_WAIT_TIME_MASK      0x0000ffffUL       //0x4a bit:0~15
    #define CMDQ_POLL_TIMER_RATIO_MASK    0x00ff0000UL       //0x4b bit:0~7
    REG32            CMDQ_Cmd_Ptr_Vld;                       //0x4d & 0x4c
    #define CMDQ_CMD_PTR_VLD              0x00000001UL       //0x4c bit:0 cmd ptr is valid
    #define CMDQ_RD_SRAM_EMPTY            0x00000002UL       //0x4c bit:1 sram is empty
    #define CMDQ_SRAM_BIST_FAIL           0x00000080UL       //0x4c bit:15 sram bist
    REG32            CMDQ_75_4e[20];                         //0x75 ~ 0x4e
    REG32            CMDQ_Hw_dummy;                          //0x77 & 0x76
    REG32            CMDQ_dummy;                             //0x79 & 0x78
    #define CMDQ__DUMMY_WRITE_ZERO        0x00000000UL       //0x78 bit:0~7

}REG_CMDQCtrl;

#endif // #ifndef _CMDQ_REG_MCU_H_
