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
#ifndef __MS_SECURE_TEE_H__
#define __MS_SECURE_TEE_H__


#if defined (CONFIG_R2_BRINGUP) || defined (CONFIG_AMP_TRUSTZONE_ENABLE)
#define HEADER_SIZE 128
#define MAGIC_ID_LEN 16
#define BOOT_CONFIG_FLAG_SIZE 1
#define ONE_G_MIU_INTERVAL_FLAG_SIZE 1

#define DEBUG_INFO_SIZE 0x200

extern char TeeMagicID[MAGIC_ID_LEN];

typedef struct
{
    unsigned int uitee_header_len;
    unsigned int uitee_start_paddr;
    unsigned int uitee_start_vaddr;
    unsigned int uitee_image_size;
    unsigned long long ulltime;
    unsigned long long ullTeeBinVersion;
    unsigned int uitee_mem_len;
    unsigned int uihw_aes_paddr;
    unsigned int uihw_aes_buffer_len;
    unsigned int uiHeaderVersion;
    unsigned int uicrc_value;
}_ST_TEE_HEADER;


typedef struct
{
    _ST_TEE_HEADER _sthd;
    unsigned char reserved[HEADER_SIZE-sizeof(_ST_TEE_HEADER)-ONE_G_MIU_INTERVAL_FLAG_SIZE-BOOT_CONFIG_FLAG_SIZE-MAGIC_ID_LEN];
    unsigned char one_g_miu_interval[ONE_G_MIU_INTERVAL_FLAG_SIZE];
    unsigned char bootConfigFlag[BOOT_CONFIG_FLAG_SIZE];
    unsigned char ucMagicID[MAGIC_ID_LEN];
}ST_TEE_HEADER;
#endif


int do_readNuttx (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_bootNuttx(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_save_nconfig(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_load_nconfig(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_set_tee_debug_level (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_wait_tee_ready(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#ifdef CONFIG_MSTAR_R2_STR
int do_r2_resume(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
#endif
