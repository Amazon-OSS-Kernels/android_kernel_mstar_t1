#include "drvLoadTbl.h"
#ifndef _BOOT_INC_
#include "boot.inc"
#endif

#ifndef _BOARD_H_
#include "Board.h"
#endif
@//# *************************************************************
@//#PAD_GPIO35	PAD_GPIO34	DRAM Brand       Speed Mode   OnebinSet
@//#     L          L          Micron        Low  Speed       0
@//#     L          L          Micron        High Speed       1
@//#     H          L          Hynix         Low  Speed       2
@//#     H          L          Hynix         High Speed       3
@//#     L          H          New Samsung   Low  Speed       4
@//#     L          H          New Samsung   High Speed       5
@//#     H          H          Old  Samsung  Low  Speed       6
@//#     H          H          Old  Samsung  High Speed       7
@//# *************************************************************
    .global Get_Packge_CFG
Get_Packge_CFG:
  push {lr}
  RREG        r2, (MS_BUS_RIU_ADDR + (0x102ba6 << 1))

@//#read GPIO34 value  reg[102ba6]#2
  and         r0, r2,#0x0004
  lsr         r0, #1
@//#read GPIO35 value reg[102ba7]#2
  and         r2, r2, #0x0400
  lsr         r2, #10
  add         r1, r2, r0
@//#High/Low speed mode
  RREG        r2, (MS_BUS_RIU_ADDR + (0x002e84 << 1))
  and         r0, r2,#0x0001
@//#r1 Multiplied by 2
  add         r1, r1, r1
  add         r1, r1, r0
  pop {pc}

@@ INPUT:
@r0: Load address (destination)
@r1: efuse (assume efuse number is the offset of board define )
@r2: type
@r3: offset for
    .global drv_LoadTbl
drv_LoadTbl:
  push {lr}

  @//cmp r2, #PADPRE
  @//beq PADPRE_HANDLE

  @//cmp r2, #PAD
  @//beq PAD_HANDLE

  cmp r2, #MIU0
  beq MIU0_HANDLE
  cmp r2, #MIU1
  beq MIU1_HANDLE
  cmp r2, #MIU2
  beq MIU2_HANDLE

  cmp r2, #MIU0_STR
  beq MIU0_STR_HANDLE
  cmp r2, #MIU1_STR
  beq MIU1_STR_HANDLE
  cmp r2, #MIU2_STR
  beq MIU2_STR_HANDLE


  @should not been here!!!
  @//DEAD_END:
  @//mov  r0, #0
  @//pop  {pc}

@//PADPRE_HANDLE:
@//ldr TBL_SIZE, =PADPRE_TBL_SIZE
@//ldr TYPE_OFF, =(OUTERSIZE+PADPRE_SEC_OFF)
@//b  MAIN_HANDLE
@//PAD_HANDLE:
@//  ldr TBL_SIZE, =PAD_TBL_SIZE
@//  ldr TYPE_OFF, =(OUTERSIZE+PAD_SECT_OFF)
@//  b  MAIN_HANDLE

MIU0_HANDLE :
  ldr TBL_SIZE, =MIU_TBL_SIZE
  ldr TYPE_OFF, =(OUTERSIZE+MIU0_SECT_OFF)
  b  MAIN_HANDLE
MIU1_HANDLE :
  ldr TBL_SIZE, =MIU_TBL_SIZE
  ldr TYPE_OFF, =(OUTERSIZE+MIU1_SECT_OFF)
  b  MAIN_HANDLE
MIU2_HANDLE :
  ldr TBL_SIZE, =MIU_TBL_SIZE
  ldr TYPE_OFF, =(OUTERSIZE+MIU2_SECT_OFF)
  b  MAIN_HANDLE

MIU0_STR_HANDLE :
  ldr TBL_SIZE, =MIU_STR_TBL_SIZE
  ldr TYPE_OFF, =(OUTERSIZE+MIU0_STR_SECT_OFF)
  b  MAIN_HANDLE
MIU1_STR_HANDLE :
  ldr TBL_SIZE, =MIU_STR_TBL_SIZE
  ldr TYPE_OFF, =(OUTERSIZE+MIU1_STR_SECT_OFF)
  b  MAIN_HANDLE
MIU2_STR_HANDLE :
  ldr TBL_SIZE, =MIU_STR_TBL_SIZE
  ldr TYPE_OFF, =(OUTERSIZE+MIU2_STR_SECT_OFF)

MAIN_HANDLE :
  mul TBL_OFF, TBL_SIZE, r1
  add TYPE_OFF, TYPE_OFF, TBL_OFF

  mov   r1, TBL_SIZE
  mov   r2, TYPE_OFF
  mov   r2, r2, lsr #9
  bl    eMMC_LoadImages
  mov  r0, #1
  pop {pc}