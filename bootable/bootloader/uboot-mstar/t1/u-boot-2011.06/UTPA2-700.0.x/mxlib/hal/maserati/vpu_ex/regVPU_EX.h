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
/// file    regVPU.h
/// @brief  VPU Module Register Definition
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_VPU_H_
#define _REG_VPU_H_

//#include "MsCommon.h"

#ifndef BMASK
#define BIT(_bit_)                  (1 << (_bit_))
#define BITS(_bits_, _val_)         ((BIT(((1)?_bits_)+1)-BIT(((0)?_bits_))) & (_val_<<((0)?_bits_)))
#define BMASK(_bits_)               (BIT(((1)?_bits_)+1)-BIT(((0)?_bits_)))
#endif

#ifndef READ_BYTE
#define READ_BYTE(_reg)             (*(volatile MS_U8*)(_reg))
#define READ_WORD(_reg)             (*(volatile MS_U16*)(_reg))
#define READ_LONG(_reg)             (*(volatile MS_U32*)(_reg))
#define WRITE_BYTE(_reg, _val)      { (*((volatile MS_U8*)(_reg))) = (MS_U8)(_val); }
#define WRITE_WORD(_reg, _val)      { (*((volatile MS_U16*)(_reg))) = (MS_U16)(_val); }
#define WRITE_LONG(_reg, _val)      { (*((volatile MS_U32*)(_reg))) = (MS_U32)(_val); }
#endif

//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//*****************************************************************************
// RIU macro
#define VPU_MACRO_START     do {
#define VPU_MACRO_END       } while (0)
#define VPU_RIU_BASE        u32VPURegOSBase

#define VPU_HIGHBYTE(u16)               ((MS_U8)((u16) >> 8))
#define VPU_LOWBYTE(u16)                ((MS_U8)(u16))
#define VPU_RIU_READ_BYTE(addr)   ( READ_BYTE( VPU_RIU_BASE + (addr) ) )
#define VPU_RIU_READ_WORD(addr)   ( READ_WORD( VPU_RIU_BASE + (addr) ) )
#define VPU_RIU_WRITE_BYTE(addr, val)      { WRITE_BYTE( VPU_RIU_BASE+(addr), val); }
#define VPU_RIU_WRITE_WORD(addr, val)      { WRITE_WORD( VPU_RIU_BASE+(addr), val); }


#define _VPU_ReadByte( u32Reg )   VPU_RIU_READ_BYTE(((u32Reg) << 1) - ((u32Reg) & 1))

#define _VPU_Read2Byte( u32Reg )    (VPU_RIU_READ_WORD((u32Reg)<<1))

#define _VPU_Read4Byte( u32Reg )   ( (MS_U32)VPU_RIU_READ_WORD((u32Reg)<<1) | ((MS_U32)VPU_RIU_READ_WORD(((u32Reg)+2)<<1)<<16 )  )

#define _VPU_ReadRegBit( u32Reg, u8Mask )   (VPU_RIU_READ_BYTE(((u32Reg)<<1) - ((u32Reg) & 1)) & (u8Mask))

#define _VPU_ReadWordBit( u32Reg, u16Mask )   (_VPU_Read2Byte( u32Reg ) & (u16Mask))

#define _VPU_WriteRegBit( u32Reg, bEnable, u8Mask )                                     \
    VPU_MACRO_START                                                                     \
    VPU_RIU_WRITE_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)) , (bEnable) ? (VPU_RIU_READ_BYTE(  (((u32Reg) <<1) - ((u32Reg) & 1))  ) |  (u8Mask)) :                           \
                                (VPU_RIU_READ_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)) ) & ~(u8Mask)));                            \
    VPU_MACRO_END

#define _VPU_WriteByte( u32Reg, u8Val )                                                 \
    VPU_MACRO_START                                                                     \
    VPU_RIU_WRITE_BYTE(((u32Reg) << 1) - ((u32Reg) & 1), u8Val);   \
    VPU_MACRO_END

#define _VPU_Write2Byte( u32Reg, u16Val )                                               \
    VPU_MACRO_START                                                                     \
    if ( ((u32Reg) & 0x01) )                                                        \
    {                                                                               \
        VPU_RIU_WRITE_BYTE(((u32Reg) << 1) - 1, (MS_U8)((u16Val)));                                  \
        VPU_RIU_WRITE_BYTE(((u32Reg) + 1) << 1, (MS_U8)((u16Val) >> 8));                             \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        VPU_RIU_WRITE_WORD( ((u32Reg)<<1) ,  u16Val);                                                       \
    }                                                                               \
    VPU_MACRO_END

#define _VPU_Write3Byte( u32Reg, u32Val )   \
    if ((u32Reg) & 0x01)                                                                \
    {                                                                                               \
        VPU_RIU_WRITE_BYTE((u32Reg << 1) - 1, u32Val);                                    \
        VPU_RIU_WRITE_WORD( (u32Reg + 1)<<1 , ((u32Val) >> 8));                                      \
    }                                                                                           \
    else                                                                                        \
    {                                                                                               \
        VPU_RIU_WRITE_WORD( (u32Reg) << 1,  u32Val);                                                         \
        VPU_RIU_WRITE_BYTE( (u32Reg + 2) << 1 ,  ((u32Val) >> 16));                             \
    }

#define _VPU_Write4Byte( u32Reg, u32Val )                                               \
    VPU_MACRO_START                                                                     \
    if ((u32Reg) & 0x01)                                                      \
    {                                                                                               \
        VPU_RIU_WRITE_BYTE( ((u32Reg) << 1) - 1 ,  u32Val);                                         \
        VPU_RIU_WRITE_WORD( ((u32Reg) + 1)<<1 , ( (u32Val) >> 8));                                      \
        VPU_RIU_WRITE_BYTE( (((u32Reg) + 3) << 1) ,  ((u32Val) >> 24));                           \
    }                                                                                               \
    else                                                                                                \
    {                                                                                                   \
        VPU_RIU_WRITE_WORD( (u32Reg) <<1 ,  u32Val);                                                             \
        VPU_RIU_WRITE_WORD(  ((u32Reg) + 2)<<1 ,  ((u32Val) >> 16));                                             \
    }                                                                     \
    VPU_MACRO_END

#define _VPU_WriteByteMask( u32Reg, u8Val, u8Msk )                                      \
    VPU_MACRO_START                                                                     \
    VPU_RIU_WRITE_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)), (VPU_RIU_READ_BYTE((((u32Reg) <<1) - ((u32Reg) & 1))) & ~(u8Msk)) | ((u8Val) & (u8Msk)));                   \
    VPU_MACRO_END

#define _VPU_WriteWordMask( u32Reg, u16Val , u16Msk)                                               \
    VPU_MACRO_START                                                                     \
    if ( ((u32Reg) & 0x01) )                                                        \
    {                                                                                           \
        if ((u16Msk)&0xff00) _VPU_WriteByteMask( ((u32Reg)+1) , (((u16Val) & 0xff00)>>8) , (((u16Msk)&0xff00)>>8) );    \
        _VPU_WriteByteMask( (u32Reg) , ((u16Val) & 0x00ff) , ((u16Msk)&0x00ff) );                                                                          \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        VPU_RIU_WRITE_WORD( ((u32Reg)<<1) ,  (((u16Val) & (u16Msk))  | (_VPU_Read2Byte( u32Reg  ) & (~( u16Msk ))))  );                                                       \
    }                                                                               \
    VPU_MACRO_END



//------------------------------------------------------------------------------
// VPU Reg
//------------------------------------------------------------------------------
#define REG_VPU_BASE            (0x0300UL)
#define REG_MBX_BASE            (0x0400UL)
#define REG_MAU1_BASE           (0x0400UL)

//write back stage PC
#define VPU_REG_EXPC_L          (REG_VPU_BASE+(0x000a<<1))
#define VPU_REG_EXPC_H          (REG_VPU_BASE+(0x000b<<1))

#define VPU_REG_CPU_SETTING     (REG_VPU_BASE+( 0x0040<<1))
    #define VPU_REG_CPU_R2_EN               BIT(0)
    #define VPU_REG_CPU_SW_RSTZ             BIT(1)
    #define VPU_REG_CPU_MIU_SW_RSTZ         BIT(2)
    #define VPU_REG_CPU_RIU_SW_RSTZ         BIT(3)
    #define VPU_REG_CPU_SPI_BOOT            BIT(4)
    #define VPU_REG_CPU_SDRAM_BOOT          BIT(5)
    #define VPU_REG_CPU_R2_INTO             BIT(6)
    #define VPU_REG_CPU_DBG_ON_DCU          BIT(8)
    #define VPU_REG_CPU_DBG_ON_ICU          BIT(9)
    #define VPU_REG_CPU_DBG_CLK_SEL         BIT(10)
    #define VPU_REG_CPU_DBG_CLK_TOGGLE      BIT(11)

#define VPU_REG_ICU_SDR_BASE_L  (REG_VPU_BASE+(0x0041<<1))  //byte address
#define VPU_REG_ICU_SDR_BASE_H    (REG_VPU_BASE+(0x0042<<1))
#define VPU_REG_DCU_SDR_BASE_L  (REG_VPU_BASE+(0x0043<<1))  //byte address
#define VPU_REG_DCU_SDR_BASE_H  (REG_VPU_BASE+(0x0044<<1))

#define VPU_REG_SPI_BASE     (REG_VPU_BASE+(0x0048<<1))    //REG ACCESS BASE32


#define VPU_REG_IQMEM_BASE_L    (REG_VPU_BASE+(0x0049<<1))
#define VPU_REG_IQMEM_BASE_H    (REG_VPU_BASE+(0x004a<<1))
#define VPU_REG_IQMEM_MASK_L    (REG_VPU_BASE+(0x004b<<1))
#define VPU_REG_IQMEM_MASK_H    (REG_VPU_BASE+(0x004c<<1))

#define VPU_REG_DQMEM_BASE_L    (REG_VPU_BASE+(0x004d<<1))
#define VPU_REG_DQMEM_BASE_H    (REG_VPU_BASE+(0x004e<<1))
#define VPU_REG_DQMEM_MASK_L    (REG_VPU_BASE+(0x004f<<1))
#define VPU_REG_DQMEM_MASK_H    (REG_VPU_BASE+(0x0050<<1))

#define VPU_REG_PQMEM_BASE_L    (REG_VPU_BASE+(0x0051<<1))
#define VPU_REG_PQMEM_BASE_H    (REG_VPU_BASE+(0x0052<<1))
#define VPU_REG_PQMEM_MASK_L    (REG_VPU_BASE+(0x0053<<1))
#define VPU_REG_PQMEM_MASK_H    (REG_VPU_BASE+(0x0054<<1))

#define VPU_REG_VQMEM_BASE_L    (REG_VPU_BASE+(0x0067<<1))
#define VPU_REG_VQMEM_BASE_H    (REG_VPU_BASE+(0x0068<<1))
#define VPU_REG_VQMEM_MASK_L    (REG_VPU_BASE+(0x0069<<1))
#define VPU_REG_VQMEM_MASK_H    (REG_VPU_BASE+(0x006a<<1))

#define VPU_REG_IO0_BASE     (REG_VPU_BASE+(0x0045<<1))   //RIU
#define VPU_REG_IO1_BASE     (REG_VPU_BASE+(0x0055<<1))   //R2 internal UART
#define VPU_REG_IO2_BASE     (REG_VPU_BASE+(0x0056<<1))   //R2 read SPI
#define VPU_REG_IO3_BASE     (REG_VPU_BASE+(0x0057<<1))   //IP use
#define VPU_REG_CONTROL_SET  (REG_VPU_BASE+(0x0058<<1))
    #define VPU_REG_IO0_EN           BIT(0)  //default Enable
    #define VPU_REG_IO1_EN           BIT(1)  //default Enable
    #define VPU_REG_IO2_EN           BIT(2)
    #define VPU_REG_IO3_EN           BIT(3)
    #define VPU_REG_QMEM_SPACE_EN    BIT(4)
    #define VPU_REG_MMU_IO_EN        BIT(5)
    #define VPU_REG_WMB_FORCE_OFF    BIT(6)
    #define VPU_REG_WMB_AUTO_OFF     BIT(7)
    #define VPU_REG_PQMEM_SPACE_EN   BIT(13)
    #define VPU_REG_VQMEM_SPACE_EN   BIT(14)


#define VPU_REG_VERSION                        (REG_MBX_BASE+(0x0055<<1))

#define VPU_REG_HI_MBOX0_L                     (REG_MBX_BASE+(0x005b<<1))
#define VPU_REG_HI_MBOX0_H                     (REG_MBX_BASE+(0x005c<<1))
#define VPU_REG_HI_MBOX1_L                     (REG_MBX_BASE+(0x005d<<1))
#define VPU_REG_HI_MBOX1_H                     (REG_MBX_BASE+(0x005e<<1))

#define VPU_REG_HI_MBOX_SET                    (REG_MBX_BASE+(0x005f<<1))
    #define VPU_REG_HI_MBOX0_SET   BIT(0)
    #define VPU_REG_HI_MBOX1_SET   BIT(1)

#define VPU_REG_RISC_MBOX_CLR                  (REG_MBX_BASE+(0x0067<<1))
    #define VPU_REG_RISC_MBOX0_CLR    BIT(0)
    #define VPU_REG_RISC_MBOX1_CLR    BIT(1)
    #define VPU_REG_RISC_ISR_CLR      BIT(2)
    #define VPU_REG_RISC_ISR_MSK      BIT(6)
    #define VPU_REG_RISC_ISR_FORCE    BIT(10)

#define VPU_REG_RISC_MBOX_RDY                  (REG_MBX_BASE+( 0x0068<<1))
    #define VPU_REG_RISC_MBOX0_RDY     BIT(0)
    #define VPU_REG_RISC_MBOX1_RDY     BIT(1)
    #define VPU_REG_RISC_ISR_VALID     BIT(2)

#define VPU_REG_HI_MBOX_RDY                    (REG_MBX_BASE+(0x0069<<1))
    #define VPU_REG_HI_MBOX0_RDY   BIT(0)
    #define VPU_REG_HI_MBOX1_RDY   BIT(1)

#define VPU_REG_RISC_MBOX0_L                   (REG_MBX_BASE+(0x006b<<1))
#define VPU_REG_RISC_MBOX0_H                   (REG_MBX_BASE+(0x006c<<1))
#define VPU_REG_RISC_MBOX1_L                   (REG_MBX_BASE+(0x006d<<1))
#define VPU_REG_RISC_MBOX1_H                   (REG_MBX_BASE+(0x006e<<1))

#define MAU1_CPU_RST                           (REG_MAU1_BASE+(0x0002<<1))
    #define MAU1_REG_SW_RESET           BIT(0)

#define MAU1_ARB0_DBG0                         (REG_MAU1_BASE+(0x0008<<1))
#define MAU1_ARB1_DBG0                         (REG_MAU1_BASE+(0x000a<<1))
    #define MAU1_FSM_CS_MASK            BMASK(13:9)
    #define MAU1_FSM_CS_IDLE            BITS(13:9, 1)

//------------------------------------------------------------------------------
// ChipTop Reg
//------------------------------------------------------------------------------

#define CHIPTOP_REG_BASE               (0x1E00UL )
#define CLKGEN0_REG_BASE               (0x0B00UL )

#define REG_TOP_VPU             (CLKGEN0_REG_BASE+(0x0030<<1))
    #define TOP_CKG_VPU_MASK                BMASK(4:0)
    #define TOP_CKG_VPU_DIS                 BIT(0)
    #define TOP_CKG_VPU_INV                 BIT(1)
    #define TOP_CKG_VPU_CLK_MASK            BMASK(4:2)

//------------------------------------------------------------------------------
// MIU Reg
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// MIU Reg
//------------------------------------------------------------------------------
#define MIU0_REG_HVD_BASE             	(0x1200UL)
#define MIU1_REG_HVD_BASE             	(0x0600UL)

#define MIU0_REG_RQ0_MASK                 (MIU0_REG_HVD_BASE+(( 0x0023)<<1))
#define MIU0_REG_RQ1_MASK                 (MIU0_REG_HVD_BASE+(( 0x0033)<<1))
#define MIU0_REG_RQ2_MASK                 (MIU0_REG_HVD_BASE+(( 0x0043)<<1))
#define MIU0_REG_RQ3_MASK                 (MIU0_REG_HVD_BASE+(( 0x0053)<<1))
#define MIU1_REG_RQ0_MASK                 (MIU1_REG_HVD_BASE+(( 0x0023)<<1))
#define MIU1_REG_RQ1_MASK                 (MIU1_REG_HVD_BASE+(( 0x0033)<<1))
#define MIU1_REG_RQ2_MASK                 (MIU1_REG_HVD_BASE+(( 0x0043)<<1))
#define MIU1_REG_RQ3_MASK                 (MIU1_REG_HVD_BASE+(( 0x0053)<<1))

#define MIU0_REG_SEL0                 (MIU0_REG_HVD_BASE+(( 0x0078)<<1))
#define MIU0_REG_SEL1                 (MIU0_REG_HVD_BASE+(( 0x0079)<<1))
#define MIU0_REG_SEL2                 (MIU0_REG_HVD_BASE+(( 0x007A)<<1))
#define MIU0_REG_SEL3                 (MIU0_REG_HVD_BASE+(( 0x007B)<<1))


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------




#endif // _REG_VPU_H_

