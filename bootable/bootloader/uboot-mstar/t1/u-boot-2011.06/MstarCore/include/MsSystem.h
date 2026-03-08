/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
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

#ifndef __CMD_MS_SYSTEM__
#define __CMD_MS_SYSTEM__
#include <MsOS.h>

#if defined(__ARM__)
#define SPI_FLAS_BASE_ADDRESS 0x14000000
#else
#define SPI_FLAS_BASE_ADDRESS 0xBFC00000
#endif

typedef enum
{
    //this valuse is offset in chunk header.
    CH_ITEM_FIRST=0x00,
    CH_UBOOT_ROM_START=CH_ITEM_FIRST,
    CH_UBOOT_RAM_START,//0x04
    CH_UBOOT_RAM_END,//0x08
    CH_UBOOT_ROM_END,//0x0c
    CH_UBOOT_RAM_ENTRY,//0x10
    RESERVED1,//0x14,Reserved1
    RESERVED2,//0x18,Reserved2
    CH_BINARY_ID,//0x1c
    CH_LOGO_ROM_OFFSET,//0x20
    CH_LOGO_ROM_SIZE,//0x24
    CH_SBOOT_ROM_OFFSET,//0x28
    CH_SBOOT_SIZE,//0x2c
    CH_SBOOT_RAM_OFFSET,//0x30
    CH_PM_ROM_OFFSET,//0x34
    CH_PM_SIZE,//0x38
    CH_PM_RAM_OFFSET,//0x3c
    CH_SECURITY_INFO_LOADER_ROM_OFFSET,//0x40
    CH_SECURITY_INFO_LOADER_SIZE,//0x44
    CH_SECURITY_INFO_LOADER_RAM_OFFSET,//0x48
    CH_CUSTOMER_KEY_BANK_ROM_OFFSET,//0x4c
    CH_CUSTOMER_KEY_BANK_SIZE,//0x50
    CH_CUSTOMER_KEY_BANK_RAM_OFFSET,//0x54
    CH_SECURITY_INFO_AP_ROM_OFFSET,//0x58
    CH_SECURITY_INFO_AP_SIZE,//0x5C
    CH_UBOOT_ENVIRONMENT_ROM_OFFSET,//0x60
    CH_UBOOT_ENVIRONMENT_SIZE,//0x64
    CH_DDR_BACKUP_TABLE_ROM_OFFSET,//0x68
    CH_POWER_SEQUENCE_TABLE_ROM_OFFSET,//0x6c
    CH_UBOOT_POOL_ROM_OFFSET,//0x70
    CH_UBOOT_POOL_SIZE,//0x74
    CH_NUTTX_CONFIG_OFFSET,//0x78
    CH_NUTTX_CONFIG_SIZE,//0x7c
    CH_RESCURE_ENVIRONMENT_ROM_OFFSET,//0x80
    CH_RESCURE_ENVIRONMENT_SIZE,//0x84
    CH_RESCURE_STATUS_ROM_OFFSET,//0x88
    CH_RESCURE_STATUS_SIZE,//0x8c
    CH_ITEM_LAST=CH_RESCURE_STATUS_SIZE
} EN_CHUNK_HEADER_ITEM;

#define CMD_BUF 128

typedef struct
{
    MS_U32  m_u32MusicVol;                  //MsDrvAudio.c
    MS_U8   m_u8LogoGopIdx;                 //MsDrvGop.c
    MS_U8   m_u8MirrorMode;                 //MsApiGEGOP.c
    MS_U16  m_u16Panel_SwingLevel;          //MsApiPanel.c
    MS_U16  m_u16Panel_ext_type;            //MsApiPanel.c
    char    m_Panel_LVDS_Connect_Type[10];  //MsApiPanel.c
    MS_U16  m_u16PanelDCLK;                 //MsApiPanel.c MsDrvPanel.c
    MS_U32  m_u32ursa_type;                 //Ursa_common.c
    char    m_Ursa_Bin_Name[CMD_BUF];       //CusUrsa8Init.c
    MS_U8   m_u8PixelShiftEnable;           //MsDrvPanel.c
}st_sys_misc_setting;


#define VA2PA(addr) MS_VA2PA(addr)

#define PA2NVA(noncache_addr) MS_PA2KSEG1(noncache_addr)

#define PA2VA(cache_addr) MS_PA2KSEG0(cache_addr)


typedef struct
{
    MS_U32  ldr_run_base;
    MS_U32  header_size;
    MS_U32  ldr_stack_end;
    MS_U32  hash_size;
    MS_U32  reserve[28];         // Reserve space for OTP Code, align to 0x80
    MS_U32  rsa_pub_dram_addr;   // The addr of EmbeddedMBootRsaPub
}stb_hash2_header;


int MsApiChunkHeader_Init(void);
int MsApiChunkHeader_GetValue(EN_CHUNK_HEADER_ITEM item, unsigned int *puValue);

int do_wdt_enable (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_disable_uart ( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_BrickTerminator_Main (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

void console_init(void);
void console_disable(void);

//-------------------------------------------------------------------------------------------------
/// Query whether or not current booting mode is house keeping booting mode
/// @return  int                              \b OUT: TURE: Housekeeping booting , FALSE: Not Housekeeping booting
//-------------------------------------------------------------------------------------------------
MS_BOOL IsHouseKeepingBootingMode(void);

//-------------------------------------------------------------------------------------------------
/// Query if system is configures as booting from mask rom
/// @return  int                              \b OUT: TURE: booting from mask rom , FALSE: Doesn't boot from mask rom
//-------------------------------------------------------------------------------------------------
MS_BOOL IsBootingFromMaskRom(void);

//-------------------------------------------------------------------------------------------------
/// Do mailbox initializtion
/// @return  int                              \b OUT: TURE: successfully , FALSE: fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MsApi_MBX_Init(void);

//-------------------------------------------------------------------------------------------------
/// Do system reset
//-------------------------------------------------------------------------------------------------
void MsSystemReset(void);

//-------------------------------------------------------------------------------------------------
/// Get boot time in ms
/// @return  unsigned long                         \b OUT: Tick
//-------------------------------------------------------------------------------------------------
unsigned long MsSystemGetBootTime(void);
unsigned long MsSystemGetUSTime(void);

signed int get_intr_rec_percpu(unsigned int cpuid);

#if CONFIG_RESCUE_ENV  && CONFIG_RESCUE_ENV_IR_TRIGGER
#define BRICK_TERMINATOR_IR_TRIGGERED_STRING "BRICK_TERMINATOR_IR_TRIGGERED"
#endif

#if CONFIG_RESCUE_ENV
MS_BOOL MSys_IsStatusBricking(void);
#endif
#endif


