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
/// @file   apiFS.h
/// @brief  FileSystem wrapper
/// @note   Most of the APIs follows POSIX Specification including: \n
///                  - Files and Directories [POSIX Section 5] \n
///                  - Input and Output [POSIX Section 6]
/// @note   With some limitations: \n
///                  - Doesn't support symolic link, user/group/other ID, ...
/// @note   Plz reference Open Group's Single Unix Specification V3 for detailed POSIX API descriptions: \n
///                  - http://www.opengroup.org/onlinepubs/009695399/
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MSFS_H__
#define __MSFS_H__

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "stdlib.h"
#include "stdio.h"
#include "dirent.h"

//-------------------------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Extern Functions
//-------------------------------------------------------------------------------------------------
// int MsFS_Init( int bUnicode);
int MsFS_Init(int bUnitcode);

//
// Mount/Umount operations
//
int MsFS_Mount( const char *devname, const char *dir, const char *fsname, unsigned long mountflags, const void* data );
int MsFS_Umount( const char *dir);

//
// File/Directory operations
//
int MsFS_Open( const char *path, int oflag, ... );
int MsFS_Unlink( const char *path );
int MsFS_MkDir( const char *path, mode_t mode );
int MsFS_RmDir( const char *path );
int MsFS_ChDir(const char *path);
int MsFS_Rename( const char *path1, const char *path2 );
int MsFS_Link( const char *path1, const char *path2 );
int MsFS_Stat( const char *path, struct stat *buf );
int MsFS_FStat( int fd, struct stat *buf );
long MsFS_PathConf( const char *path, int name );
long MsFS_FPathConf( int fd, int name );
int MsFS_Access( const char *path, int amode );
char* MsFS_GetCwd( char *buf, size_t size );
DIR* MsFS_OpenDir( const char *dirname );
struct dirent* MsFS_ReadDir( DIR *dirp );
void MsFS_RewindDir( DIR *dirp );
void MsFS_CloseDir( DIR *dirp );


//
// File IO operations
//
ssize_t MsFS_Read( int fd, void *buf, size_t len );
ssize_t MsFS_Write( int fd, const void *buf, size_t len );
int MsFS_Close( int fd );
MS_U64 MsFS_Lseek( int fd, MS_U64 pos, int whence );
int MsFS_FCntl( int fd, int cmd, ... );
int MsFS_FSync( int fd );
void MsFS_Sync(void);


//
// Standard C Library's counterparts
//
FILE * MsFS_Fopen( const char *filename, const char *mode );
int MsFS_Fclose( FILE *stream );
size_t MsFS_Fread( void *data, size_t itemsize, size_t nitems, FILE *stream );
size_t MsFS_Fwrite( const void *data, size_t itemsize, size_t nitems, FILE *stream );
int MsFS_Fseek( FILE *stream, MS_U64 pos, int whence );
void MsFS_Rewind( FILE *stream );
MS_U64 MsFS_Ftell( FILE *stream );
int MsFS_Fflush(FILE *stream);

//
// File system information query
//
typedef struct
{
    MS_U32      u32ClusTotal;
    MS_U32      u32ClusFree;
    MS_U32      u32ClusSize;
} MApi_FsInfo;

MS_BOOL MApi_FS_Info(const char *path, MS_BOOL bUnicode, MApi_FsInfo* pFsInfo);

#ifdef __cplusplus
}
#endif


#endif // #ifndef __MSFS_H__
