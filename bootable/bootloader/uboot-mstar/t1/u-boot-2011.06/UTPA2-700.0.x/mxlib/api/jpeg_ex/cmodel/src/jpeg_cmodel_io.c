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
/// file    jpeg_file.c
/// @brief  JPEG decoder file system
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "jpegmain.h"
#include "jpeg_cmodel_io.h"


//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
VOID JPEG_FILE_constructor( JPEG_FILE_FileSystem_t *thisPtr )
{
    thisPtr->Pfile = NULL;
    thisPtr->eof_flag = FALSE;
    thisPtr->error_flag = FALSE;
}

VOID JPEG_FILE_destructor( JPEG_FILE_FileSystem_t *thisPtr )
{
    thisPtr->close( thisPtr );
}

MS_BOOL JPEG_FILE_open( const char *Pfilename, JPEG_FILE_FileSystem_t *thisPtr )
{
    thisPtr->close( thisPtr );

    thisPtr->eof_flag = FALSE;
    thisPtr->error_flag = FALSE;
    thisPtr->Pfile = fopen( Pfilename, "rb" );
    if ( !thisPtr->Pfile )
    {
        return ( TRUE );
    }

    return ( FALSE );
}

VOID JPEG_FILE_close( JPEG_FILE_FileSystem_t *thisPtr )
{
    if ( thisPtr->Pfile )
    {
        fclose( thisPtr->Pfile );
        thisPtr->Pfile = NULL;
    }

    thisPtr->eof_flag = FALSE;
    thisPtr->error_flag = FALSE;
}

MS_S32 JPEG_FILE_read( MS_U8 *Pbuf, MS_U32 max_bytes_to_read, MS_BOOL *Peof_flag, JPEG_FILE_FileSystem_t *thisPtr)
{
    MS_U32 bytes_read;

    if ( !thisPtr->Pfile )
    {
        return ( -1 );
    }

    if ( thisPtr->eof_flag )
    {
        *Peof_flag = TRUE;
        return ( 0 );
    }

    if ( thisPtr->error_flag )
    {
        return ( -1 );
    }


    bytes_read = fread( Pbuf, 1, max_bytes_to_read, thisPtr->Pfile );

    if ( bytes_read < max_bytes_to_read )
    {
        if ( ferror( thisPtr->Pfile ) )
        {
            thisPtr->error_flag = TRUE;
            return ( -1 );
        }

        thisPtr->eof_flag = TRUE;
        *Peof_flag = TRUE;
    }

    return ( bytes_read );
}

MS_BOOL JPEG_FILE_get_error_status( JPEG_FILE_FileSystem_t *thisPtr ) //not used
{
    return ( thisPtr->error_flag );
}

MS_BOOL JPEG_FILE_reset( JPEG_FILE_FileSystem_t *thisPtr ) //not used
{
    if ( thisPtr->error_flag )
    {
        return ( TRUE );
    }

    fseek( thisPtr->Pfile, 0, SEEK_SET );

    thisPtr->eof_flag = FALSE;

    return ( FALSE );
}

MS_S32 JPEG_FILE_get_size( JPEG_FILE_FileSystem_t *thisPtr ) //not necessary
{
    MS_U32 size;
    MS_U32 loc;

    if ( !thisPtr->Pfile )
    {
        return ( -1 );
    }

    loc = ftell( thisPtr->Pfile );

    fseek( thisPtr->Pfile, 0, SEEK_END );

    size = ftell( thisPtr->Pfile );

    fseek( thisPtr->Pfile, loc, SEEK_SET );

    return ( size );
}

VOID JPEG_FILE_tga_constructor( JPEG_FILE_TGAFileSystem_t *thisPtr )
{
    thisPtr->Pfile = NULL;
    thisPtr->width = thisPtr->height = thisPtr->bytes_per_pixel = thisPtr->bytes_per_line = 0;
    thisPtr->image_type = E_TGA_IMAGE_TYPE_NULL;
}

VOID JPEG_FILE_tga_destructor( JPEG_FILE_TGAFileSystem_t *thisPtr )
{
    thisPtr->close( thisPtr );
}

MS_BOOL JPEG_FILE_tga_open( const char *Pfilename, MS_U16 _width, MS_U16 _height, JPEG_FILE_TGAImageType_e _image_type, JPEG_FILE_TGAFileSystem_t *thisPtr )
{
    MS_U8 tga_header[18];
    MS_BOOL backwards_flag = FALSE;

    MS_ASSERT( Pfilename );

    thisPtr->close( thisPtr );

    thisPtr->Pfile = fopen( Pfilename, "wb" );
    if ( !thisPtr->Pfile )
    {
        return ( TRUE );
    }

    thisPtr->width = _width;
    thisPtr->height = _height;
    thisPtr->image_type = _image_type;

    memset( tga_header, 0, sizeof( tga_header ) );

    tga_header[12] = ( MS_U8 )( _width & 0xFF );
    tga_header[13] = ( MS_U8 )( ( _width >> 8 ) & 0xFF );
    tga_header[14] = ( MS_U8 )( _height & 0xFF );
    tga_header[15] = ( MS_U8 )( ( _height >> 8 ) & 0xFF );
    tga_header[17] = backwards_flag ? 0x00 : 0x20;

    switch ( _image_type )
    {
        case E_TGA_IMAGE_TYPE_BGR:
            {
                tga_header[2] = 2;
                tga_header[16] = 24;
                thisPtr->bytes_per_pixel = 3;
                break;
            }
        case E_TGA_IMAGE_TYPE_GREY:
            {
                tga_header[2] = 3;
                tga_header[16] = 8;
                thisPtr->bytes_per_pixel = 1;
                break;
            }
        default:
            MS_ASSERT( FALSE );
    }

    thisPtr->bytes_per_line = _width * thisPtr->bytes_per_pixel;

    if ( fwrite( tga_header, sizeof( tga_header ), 1, thisPtr->Pfile ) != 1 )
    {
        return ( TRUE );
    }

    return ( FALSE );
}

MS_BOOL JPEG_FILE_tga_close( JPEG_FILE_TGAFileSystem_t *thisPtr )
{
    thisPtr->width = thisPtr->height = thisPtr->bytes_per_pixel = thisPtr->bytes_per_line = 0;
    thisPtr->image_type = E_TGA_IMAGE_TYPE_NULL;

    if ( thisPtr->Pfile )
    {
        MS_BOOL failed = ( fclose( thisPtr->Pfile ) == EOF );

        thisPtr->Pfile = NULL;

        return ( failed );
    }

    return ( FALSE );
}

MS_U32 JPEG_FILE_tga_writeline( MS_U8 *Pscan_line, JPEG_FILE_TGAFileSystem_t *thisPtr )
{
    if ( !thisPtr->Pfile )
    {
        return ( TRUE );
    }

    if ( fwrite( Pscan_line, thisPtr->bytes_per_line, 1, thisPtr->Pfile ) != 1 )
    {
        return ( TRUE );
    }

    return ( FALSE );
}

