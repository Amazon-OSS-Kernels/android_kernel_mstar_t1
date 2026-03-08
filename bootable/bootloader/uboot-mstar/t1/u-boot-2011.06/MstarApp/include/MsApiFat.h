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

#ifndef __CMD_MS_API_FAT__
#define __CMD_MS_API_FAT__

#define ENV_PARTNO                  "partno"
#define MAX_FAT_PARTNO      10 // support 10 partitions
#define CMD_BUF_SIZE           20

#define MAX_FILE_NAME_LENGTH 256


typedef struct{
    char name[MAX_FILE_NAME_LENGTH];
    unsigned int idx;
}ST_FILE_INFO;


int do_fat_fswrite (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_fat_fspartload (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_fat_fsfilesize (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_fat_fileList_test(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_dump_fileList(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

//-------------------------------------------------------------------------------------------------
/// Creat a file list by the input path. (Not include folder)
/// @param  path                           \b IN: path
/// @param  cur                             \b IN/OUT: return the first file information
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int createfileList(char *path,ST_FILE_INFO *cur);

//-------------------------------------------------------------------------------------------------
/// Return the next file information by the current file information
/// @param  cur                             \b IN/OUT: input the current file information/return the next file information
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int getNextFile(ST_FILE_INFO *cur);

//-------------------------------------------------------------------------------------------------
/// Return the prev file information by the current file information
/// @param  cur                             \b IN/OUT: input the current file information/return the prev file information
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int getPrevFile(ST_FILE_INFO *cur);

//-------------------------------------------------------------------------------------------------
/// Return the the first file information in file list
/// @param  cur                             \b IN/OUT: return the first file information
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int getFirstFile(ST_FILE_INFO *cur);

//-------------------------------------------------------------------------------------------------
/// Return the the last file information in file list
/// @param  cur                             \b IN/OUT: return the last file information
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int getLastFile(ST_FILE_INFO *cur);

//-------------------------------------------------------------------------------------------------
/// Delete the internal file list
/// @return  int                              \b OUT: 0: successful. -1: fail
//-------------------------------------------------------------------------------------------------
int DeleteFileList(void);



#endif
