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
/// file    jpeg_cmodel_io.h
/// @brief  JPEG decoder file system for cmodel
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _JPEG_CMODEL_IO_H_
#define _JPEG_CMODEL_IO_H_

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
//for Chakra:MediaCodec
#define JPEG_FILE_OP_TIME_OUT 2000 //millisecond


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_TGA_IMAGE_TYPE_NULL = 0,
    E_TGA_IMAGE_TYPE_BGR,
    E_TGA_IMAGE_TYPE_GREY,
} JPEG_FILE_TGAImageType_e;

//------------------------------------------------------------------------------
/// JPEG decoder file system
typedef struct JPEG_FILE_FileSystem_s
{
    FILE *Pfile;
    MS_BOOL eof_flag, error_flag;

    VOID ( *constructor )( struct JPEG_FILE_FileSystem_s *structPtr );
    VOID ( *destructor )( struct JPEG_FILE_FileSystem_s *structPtr );
    MS_BOOL (*open)( const char *Pfilename,
        struct JPEG_FILE_FileSystem_s *structPtr );
    VOID ( *close )( struct JPEG_FILE_FileSystem_s *structPtr );
    MS_S32 (*read)( MS_U8 *Pbuf, MS_U32 max_bytes_to_read, MS_BOOL *Peof_flag,
        struct JPEG_FILE_FileSystem_s *structPtr);
    MS_BOOL ( *get_error_status )( struct JPEG_FILE_FileSystem_s *structPtr );
    MS_BOOL ( *reset )( struct JPEG_FILE_FileSystem_s *structPtr );
    MS_S32 ( *get_size )( struct JPEG_FILE_FileSystem_s *structPtr );
} JPEG_FILE_FileSystem_t, *PJPEG_FILE_FileSystem_t;
//------------------------------------------------------------------------------
/// JPEG decoder file system
typedef struct JPEG_FILE_TGAFileSystem_s
{
    FILE *Pfile;

    MS_U16 width, height;
    MS_U32 bytes_per_pixel, bytes_per_line;
    JPEG_FILE_TGAImageType_e image_type;

    VOID ( *constructor )( struct JPEG_FILE_TGAFileSystem_s *structPtr );
    VOID ( *destructor )( struct JPEG_FILE_TGAFileSystem_s *structPtr );
    MS_BOOL (*open)( const char *Pfilename, MS_U16 _width, MS_U16 _height,
        JPEG_FILE_TGAImageType_e _image_type, struct JPEG_FILE_TGAFileSystem_s *structPtr );
    MS_BOOL ( *close )( struct JPEG_FILE_TGAFileSystem_s *structPtr );
    MS_U32 (*writeline)( MS_U8 *Pscan_line, struct JPEG_FILE_TGAFileSystem_s *structPtr );
} JPEG_FILE_TGAFileSystem_t, *PJPEG_FILE_TGAFileSystem_t;
//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//Real implemented functions
VOID JPEG_FILE_constructor( JPEG_FILE_FileSystem_t *thisPtr );
VOID JPEG_FILE_destructor( JPEG_FILE_FileSystem_t *thisPtr );
MS_BOOL JPEG_FILE_open( const char *Pfilename, JPEG_FILE_FileSystem_t *thisPtr );
VOID JPEG_FILE_close( JPEG_FILE_FileSystem_t *thisPtr );
MS_S32 JPEG_FILE_read( MS_U8 *Pbuf, MS_U32 max_bytes_to_read, MS_BOOL *Peof_flag, JPEG_FILE_FileSystem_t *thisPtr);
MS_BOOL JPEG_FILE_get_error_status( JPEG_FILE_FileSystem_t *thisPtr );
MS_BOOL JPEG_FILE_reset( JPEG_FILE_FileSystem_t *thisPtr );
MS_S32 JPEG_FILE_get_size( JPEG_FILE_FileSystem_t *thisPtr );
//------------------------------------------------------------------------------
VOID JPEG_FILE_tga_constructor( JPEG_FILE_TGAFileSystem_t *thisPtr );
VOID JPEG_FILE_tga_destructor( JPEG_FILE_TGAFileSystem_t *thisPtr );
MS_BOOL JPEG_FILE_tga_open( const char *Pfilename, MS_U16 _width, MS_U16 _height, JPEG_FILE_TGAImageType_e _image_type, JPEG_FILE_TGAFileSystem_t *thisPtr );
MS_BOOL JPEG_FILE_tga_close( JPEG_FILE_TGAFileSystem_t *thisPtr );
MS_U32 JPEG_FILE_tga_writeline( MS_U8 *Pscan_line, JPEG_FILE_TGAFileSystem_t *thisPtr );
//------------------------------------------------------------------------------
#endif // _JPEG_CMODEL_IO_H_
