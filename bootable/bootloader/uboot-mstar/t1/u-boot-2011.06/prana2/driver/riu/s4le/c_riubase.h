/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//******************************************************************************
//******************************************************************************
#define RIUBASE_MENULOAD                        0x0200  // 0x0200 - 0x021F
#define RIUBASE_NFIE                            0x0300  // 0x0300 - 0x03FF
#define RIUBASE_DDC                             0x0400  // 0x0400 - 0x04FF
#define RIUBASE_POR_STATUS                      0x0500  // 0x0500 - 0x053F
#define RIUBASE_IRQ_AEON1                       0x0540  // 0x0540 - 0x057F
#define RIUBASE_NORPF                           0x0580  // 0x0580 - 0x05FF
#define RIUBASE_MIU2                            0x0600  // 0x0600 - 0x06FF
#define RIUBASE_USB                             0x0700  // 0x0700 - 0x077F
#define RIUBASE_USB1                            0x0780  // 0x0780 - 0x07FF
#define RIUBASE_ISP                             0x0800  // 0x0800 - 0x08FF
#define RIUBASE_BDMA                            0x0900  // 0x0900 - 0x097F
#define RIUBASE_UART                            0x0980  // 0x0980 - 0x09FF
#define RIUBASE_RVD                             0x0A00  // 0x0A00 - 0x0AFF
#define RIUBASE_DSCRMB                          0x0C00  // 0x0C00 - 0x0CFF
#define RIUBASE_UHC1                            0x0D00  // 0x0D00 - 0x0DFF
#define RIUBASE_PM_SLEEP                        0x0E00  // 0x0E00 - 0x0E7F
#define RIUBASE_MAU_DBG                         0x0E80  // 0x0E80 - 0x0EBF
#define RIUBASE_RIU_DBG                         0x0EC0  // 0x0EC0 - 0x0EFF
#define RIUBASE_AEON                            0x0F00  // 0x0F00 - 0x0FFF
#define RIUBASE_MCU_PERI                        0x1000  // 0x1000 - 0x10FF
#define RIUBASE_MVD                             0x1100  // 0x1100 - 0x11FF
#define RIUBASE_MIU                             0x1200  // 0x1200 - 0x12FF
#define RIUBASE_VD_MCU                          0x1300  // 0x1300 - 0x13FF
#define RIUBASE_MVOP                            0x1400  // 0x1400 - 0x14FF
#define RIUBASE_TSP0                            0x1500  // 0x1500 - 0x15FF
#define RIUBASE_TSP1                            0x1600  // 0x1600 - 0x16FF
#define RIUBASE_JPD                             0x1700  // 0x1700 - 0x17FF
#define RIUBASE_SEM                             0x1800  // 0x1800 - 0x183F
#define RIUBASE_MAU0                            0x1840  // 0x1840 - 0x185F
#define RIUBASE_MAU1                            0x1860  // 0x1860 - 0x187F
#define RIUBASE_WDT                             0x1880  // 0x1880 - 0x18BF
#define RIUBASE_ECBRIDGE                        0x18C0  // 0x18C0 - 0x18FF
#define RIUBASE_IRQ_EXP                         0x1900  // 0x1900 - 0x19FF
#define RIUBASE_VIVALDI                         0x1A00  // 0x1A00 - 0x1AFF
#define RIUBASE_SVD                             0x1B00  // 0x1B00 - 0x1BFF
#define RIUBASE_CHIP                            0x1E00  // 0x1E00 - 0x1EFF
#define RIUBASE_GOP                             0x1F00  // 0x1F00 - 0x1FFF
#define RIUBASE_EMAC0                           0x2000  // 0x2000 - 0x20FF
#define RIUBASE_EMAC1                           0x2100  // 0x2100 - 0x21FF
#define RIUBASE_EMAC2                           0x2200  // 0x2200 - 0x22FF
#define RIUBASE_EMAC3                           0x2300  // 0x2300 - 0x23FF
#define RIUBASE_UHC0                            0x2400  // 0x2400 - 0x24FF
#define RIUBASE_ADC_ATOP                        0x2500  // 0x2500 - 0x25FF
#define RIUBASE_ADC_DTOP                        0x2600  // 0x2600 - 0x26EF
#define RIUBASE_HDCP                            0x26F0  // 0x26F0 - 0x26FF
#define RIUBASE_HDMI                            0x2700  // 0x2700 - 0x27FF
#define RIUBASE_GE                              0x2800  // 0x2800 - 0x28FF
#define RIUBASE_STRLD                           0x2900  // 0x2900 - 0x29FF
#define RIUBASE_CI                              0x2A00  // 0x2A00 - 0x2AFF
#define RIUBASE_IRQ                             0x2B00  // 0x2B00 - 0x2B7F
#define RIUBASE_CACHE                           0x2B80  // 0x2B80 - 0x2BBF
#define RIUBASE_XDMIU                           0x2BC0  // 0x2BC0 - 0x2BFF
#define RIUBASE_VIVALDI0                        0x2C00  // 0x2C00 - 0x2CFF
#define RIUBASE_VIVALDI1                        0x2D00  // 0x2D00 - 0x2DFF
#define RIUBASE_SC0                             0x2E00  // 0x2E00 - 0x2EFF
#define RIUBASE_SC1                             0x2F00  // 0x2F00 - 0x2FFF
#define RIUBASE_SC2                             0x3000  // 0x3000 - 0x30FF
#define RIUBASE_SC3                             0x3100  // 0x3100 - 0x31FF
#define RIUBASE_SC4                             0x3200  // 0x3200 - 0x32FF
#define RIUBASE_MAILBOX                         0x3380  // 0x3380 - 0x33FF
#define RIUBASE_IIC                             0x3420  // 0x3420 - 0x343F
#define RIUBASE_PCM                             0x3440  // 0x3440 - 0x345F
#define RIUBASE_RTC                             0x3480  // 0x3480 - 0x349F
#define RIUBASE_PM                              0x34A0  // 0x34A0 - 0x34BF
#define RIUBASE_AFEC                            0x3500  // 0x3500 - 0x35FF
#define RIUBASE_COMB                            0x3600  // 0x3600 - 0x36FF
#define RIUBASE_VBI                             0x3700  // 0x3700 - 0x37FF
#define RIUBASE_SCM                             0x3800  // 0x3800 - 0x38FF
#define RIUBASE_PATGEN                          0x3980  // 0x3980 - 0x39FF
#define RIUBASE_SAR                             0x3A00  // 0x3A00 - 0x3A7F
#define RIUBASE_UTMI                            0x3A80  // 0x3A80 - 0x3AFF
#define RIUBASE_VE0                             0x3B00  // 0x3B00 - 0x3BFF
#define RIUBASE_PIU_MISC                        0x3C00  // 0x3C00 - 0x3CFF
#define RIUBASE_PWM                             0x3D00  // 0x3D00 - 0x3D3F
#define RIUBASE_IR_REG                          0x3D80  // 0x3D80 - 0x3DFF
#define RIUBASE_VE1                             0x3E00  // 0x3E00 - 0x3EFF
#define RIUBASE_VE2                             0x3F00  // 0x3F00 - 0x3FFF
