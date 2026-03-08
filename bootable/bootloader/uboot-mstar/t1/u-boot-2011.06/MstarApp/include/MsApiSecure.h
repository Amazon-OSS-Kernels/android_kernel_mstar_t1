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

#ifndef __MS_API_SECURE_H__
#define __MS_API_SECURE_H__

int Kernel_Authentication(unsigned int u32kernelAddr);
int FileSystem_Authentication(void);

int do_verify_sha256(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_sha256(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_rsa(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_DoSecureEngUnitTestCase1(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_DoSecureEngUnitTestCase2(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_DoSecureEngUnitTestCase3(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_DoSecureEngUnitTestCase4(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_DoSecureEngUnitTestCase5(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_save_secure_info(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

int do_secure_init(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

int do_file_part_load_with_segment_aes_decrypted(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_file_segment_rsa_authendication(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

