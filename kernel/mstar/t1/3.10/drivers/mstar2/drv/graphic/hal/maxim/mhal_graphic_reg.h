/**
* Copyright (c) 2016 – 2017 MStar Semiconductor, Inc.
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
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// @file   mhal_graphic_reg.h
// @brief  Graphic Driver Interface
// @author MStar Semiconductor Inc.
////////////////////////////////////////////////////////////////////////////////////////////////*/


/*=============================================================================
// Defines & Macros
//=============================================================================*/
#define BMASK(bits)                             (BIT(((1) ? bits) + 1) - BIT(((0) ? bits)))
#define GOP_REG_VAL(x)                          (1 << x)

#if defined(CONFIG_ARM64)
extern ptrdiff_t mstar_pm_base;
#define RIU_MAP                                 (mstar_pm_base+0x200000UL)
#else
#define RIU_MAP                                 0xFD200000UL
#endif

#define RIU                                     ((unsigned short volatile*) RIU_MAP)

#define GOP_WRITE2BYTE(addr, val)    { RIU[addr] = val; }
#define GOP_READ2BYTE(addr)            RIU[addr]


/*----------------------------------------------------------------------------
// MIU Reg
//----------------------------------------------------------------------------*/
#define GOP_MIU_CLIENT_GOP0     0x5
#define GOP_MIU_CLIENT_GOP1     0x6
#define GOP_MIU_CLIENT_GOP2     0x7
#define GOP_MIU_CLIENT_GOP3     0x8
#define GOP_MIU_CLIENT_GOP4     0xff

/*----------------------------------------------------------------------------
// GOP related GOP Reg Defines
//----------------------------------------------------------------------------*/
#define GOP_REG_DIRECT_BASE                            (0x20200)
#define GOP_REG(bk, reg)                    (GOP_REG_DIRECT_BASE + ((MS_U32)(bk) << 8) + (reg) * 2)

#define GOP_OFFSET_WR                       8
#define GOP_VAL_WR                          GOP_REG_VAL(GOP_OFFSET_WR)
#define GOP_OFFSET_FWR                      9
#define GOP_VAL_FWR                         GOP_REG_VAL(GOP_OFFSET_FWR)


#define GOP_4G_OFST                           0x0
#define GOP_2G_OFST                           0x3
#define GOP_1G_OFST                           0x6
#define GOP_1GX_OFST                          0x9
#define GOP_DW_OFST                           0xC

#define GOP_4G_CTRL0                        GOP_REG(0, 0x00)
#define GOP_4G_CTRL1                        GOP_REG(0, 0x01)
#define GOP_4G_RATE                         GOP_REG(0, 0x02)
#define GOP_4G_RDMA_HT                      GOP_REG(GOP_4G_OFST, 0x0e)
#define GOP_4G_BW                           GOP_REG(GOP_4G_OFST, 0x19)
#define GOP_4G_SRAM_BORROW                  GOP_REG(GOP_4G_OFST, 0x1D)
#define GOP_4G_MIU_SEL                      GOP_REG(GOP_4G_OFST, 0x1F)
#define GOP_4G_STRCH_HSZ                    GOP_REG(GOP_4G_OFST, 0x30)
#define GOP_4G_STRCH_VSZ                    GOP_REG(GOP_4G_OFST, 0x31)
#define GOP_4G_STRCH_HSTR                   GOP_REG(GOP_4G_OFST, 0x32)
#define GOP_4G_STRCH_VSTR                   GOP_REG(GOP_4G_OFST, 0x34)
#define GOP_4G_HSTRCH                       GOP_REG(0, 0x35)
#define GOP_4G_VSTRCH                       GOP_REG(0, 0x36)
#define GOP_4G_HSTRCH_INI                   GOP_REG(GOP_4G_OFST, 0x38)
#define GOP_4G_VSTRCH_INI                   GOP_REG(GOP_4G_OFST, 0x39)
#define GOP_4G_OLDADDR                      GOP_REG(GOP_4G_OFST, 0x3b)
#define GOP_BAK_SEL                         GOP_REG(GOP_4G_OFST, 0x7f)

#define GOP_4G_GWIN0_CTRL(id)               GOP_REG(GOP_4G_OFST+1, 0x00 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_DRAM_RBLK_L(id)              GOP_REG(GOP_4G_OFST+1, 0x01 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_DRAM_RBLK_H(id)              GOP_REG(GOP_4G_OFST+1, 0x02 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_HSTR(id)                     GOP_REG(GOP_4G_OFST+1, 0x04 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_HEND(id)                     GOP_REG(GOP_4G_OFST+1, 0x05 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_VSTR(id)                     GOP_REG(GOP_4G_OFST+1, 0x06 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_VEND(id)                     GOP_REG(GOP_4G_OFST+1, 0x08 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_DRAM_RBLK_HSIZE(id)          GOP_REG(GOP_4G_OFST+1, 0x09 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_DRAM_RBLK_SIZE_L(id)         GOP_REG(GOP_4G_OFST+1, 0x10 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_DRAM_RBLK_SIZE_H(id)         GOP_REG(GOP_4G_OFST+1, 0x11 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_GWIN_ALPHA01(id)             GOP_REG(GOP_4G_OFST+1, 0x0A + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_DRAM_VSTR_L(id)              GOP_REG(GOP_4G_OFST+1, 0x0C + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_DRAM_VSTR_H(id)              GOP_REG(GOP_4G_OFST+1, 0x0D + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_DRAM_HSTR(id)                GOP_REG(GOP_4G_OFST+1, 0x0E + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_DRAM_HVSTOP_L(id)            GOP_REG(GOP_4G_OFST+1, 0x14 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_DRAM_HVSTOP_H(id)            GOP_REG(GOP_4G_OFST+1, 0x15 + (0x20*((id)%MAX_GOP0_GWIN)))

