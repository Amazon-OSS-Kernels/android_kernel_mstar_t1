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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jpeg_cmodel_def.h"
#include "apiJPEG.h"
#include "jpeg_cmodel.h"
#include "jpeg_cmodel_io.h"

#define INTERNAL_MEM_POOL_SIZE (13*1024*1024)
#define READ_BUFF_SIZE (384*1024 + 128)

// TGA_WRITER enable -> output file as RGB TGA file, else output to frame buffer directly
//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
static    JPEG_FILE_FileSystem_t input_stream;
static    PJPEG_FILE_FileSystem_t Pinput_stream = &input_stream;

static    JPEG_FILE_TGAFileSystem_t tga_stream;
static    PJPEG_FILE_TGAFileSystem_t Ptga_stream = &tga_stream;

static const char *Psrc_filename;
static const char *Pdst_filename;

static MS_S32 JPEG_FillReadBuff(MS_U32 BufAddr, MS_U32 BufLength)
{
    MS_S32 bytes_read = 0;
    MS_U32 buf_left = 0;
    MS_BOOL bIsEOF = FALSE;

    printf("verJPD_FillHdrFunc, bytes_read = %lu\n", BufAddr);
    do
    {
        bytes_read = Pinput_stream->read((MS_U8 *)(BufAddr + buf_left), BufLength - buf_left, &bIsEOF, Pinput_stream);

        if (bytes_read < 0)
        {
            return bytes_read;
        }

        buf_left += bytes_read;
    } while(( buf_left < BufLength ) && ( !bIsEOF ));
    MApi_JPEG_UpdateReadInfo(buf_left, bIsEOF);
    return buf_left;
}


BOOL JPEGInit( void )
{
    printf("Main::JPEGInit\n");
    Psrc_filename = NULL;
    Pdst_filename = NULL;

    Pinput_stream->constructor = JPEG_FILE_constructor;
    Pinput_stream->destructor = JPEG_FILE_destructor;
    Pinput_stream->open = JPEG_FILE_open;
    Pinput_stream->close = JPEG_FILE_close;
    Pinput_stream->read = JPEG_FILE_read;
    Pinput_stream->get_error_status = JPEG_FILE_get_error_status;
    Pinput_stream->reset = JPEG_FILE_reset;
    Pinput_stream->get_size = JPEG_FILE_get_size;

    Ptga_stream->constructor = JPEG_FILE_tga_constructor;
    Ptga_stream->destructor = JPEG_FILE_tga_destructor;
    Ptga_stream->open = JPEG_FILE_tga_open;
    Ptga_stream->close = JPEG_FILE_tga_close;
    Ptga_stream->writeline = JPEG_FILE_tga_writeline;

    MApi_JPEG_SetDbgLevel(E_JPEG_DEBUG_ALL);

    return TRUE;
}

int main( int arg_c, char *arg_v[] )
{
    U8 *Pbuf = NULL;
    int lines_decoded = 0;

    BOOL status;
    BOOL bIsEOF;

    U8 *memory_pool;
    U8 *read_buffer;
    JPEG_InitParam init_param;

    // set memory for JPEG buffer using
    memory_pool = malloc(INTERNAL_MEM_POOL_SIZE*sizeof(U8));

    if ( !memory_pool )
    {
        printf( "Error: Out of memory!\n" );
    }


    read_buffer = malloc((READ_BUFF_SIZE + 128)*sizeof(U8));

    if ( !read_buffer )
    {
        printf( "Error: Out of memory!\n" );
    }


    init_param.u32MRCBufAddr = (U32)read_buffer;
    init_param.u32MRCBufSize = READ_BUFF_SIZE;
    init_param.u32MWCBufAddr = 0;
    init_param.u32MWCBufSize = 0;
    init_param.u32InternalBufAddr = (U32)memory_pool;
    init_param.u32InternalBufSize = INTERNAL_MEM_POOL_SIZE;
    init_param.bInitMem = TRUE;
//    MApi_JPEG_SetInitParameter(&init_param);

    printf( "JPEG to TGA file conversion example. Compiled %s %s\n", __DATE__, __TIME__ );
    printf( "Copyright (C) 1994-2000 Rich Geldreich\n" );

    if ( arg_c != 4 )
    {
        printf( "Usage: jpg2tga <source_file> <dest_file> <type>\n" );
        printf( "Outputs greyscale and truecolor 24-bit TGA files.\n" );
        return ( EXIT_FAILURE );
    }

    JPEGInit();

    Psrc_filename = arg_v[1];
    Pdst_filename = arg_v[2];

    printf( "Source file:      \"%s\"\n", Psrc_filename );
    printf( "Destination file: \"%s\"\n", Pdst_filename );


    //Pjpeg_decoder_file_stream Pinput_stream = new jpeg_decoder_file_stream();
    Pinput_stream->constructor( Pinput_stream );

    if ( Pinput_stream->open( Psrc_filename, Pinput_stream ) )
    {
        //delete Pinput_stream;
        Pinput_stream->destructor( Pinput_stream );
        printf( "Error: Unable to open file \"%s\" for reading!\n", Psrc_filename );
        return ( EXIT_FAILURE );
    }

    init_param.u32DecByteRead = Pinput_stream->read(read_buffer, READ_BUFF_SIZE - 128, &bIsEOF, Pinput_stream);
    init_param.bEOF = bIsEOF;
    init_param.pFillHdrFunc = (JPEG_FillHdrFunc)JPEG_FillReadBuff;

    //Pjpeg_decoder Pd = new jpeg_decoder(Pinput_stream, use_mmx);
    if(strcmp(arg_v[3], "1")==0)
    {
        //MApi_JPEG_Constructor( Pinput_stream, E_JPEG_TYPE_MAIN);
        init_param.u8DecodeType = E_JPEG_TYPE_MAIN;
        MApi_JPEG_Init(&init_param);
    }
    else
    {
        //MApi_JPEG_Constructor( Pinput_stream, E_JPEG_TYPE_THUMBNAIL);
        init_param.u8DecodeType = E_JPEG_TYPE_THUMBNAIL;
        MApi_JPEG_Init(&init_param);
    }

    if ( MApi_JPEG_GetErrorCode() != E_JPEG_NO_ERROR )
    {
        printf( "Error: Decoder failed! Error status: %i\n", MApi_JPEG_GetErrorCode() );

        // Always be sure to delete the input stream object _after_
        // the decoder is deleted. Reason: the decoder object calls the input
        // stream's detach() method.
        //delete Pd;
        MApi_JPEG_Exit();
        //delete Pinput_stream;
        Pinput_stream->destructor( Pinput_stream );

        return ( EXIT_FAILURE );
    }

    if(MApi_JPEG_DecodeHdr()==E_JPEG_FAILED)
    {
        printf("Decode Header error\n");
        // Always be sure to delete the input stream object _after_
        // the decoder is deleted. Reason: the decoder object calls the input
        // stream's detach() method.
        //delete Pd;
        MApi_JPEG_Exit();
        //delete Pinput_stream;
        Pinput_stream->destructor( Pinput_stream );

        return ( EXIT_FAILURE );
    }

    #ifdef NO_OUTPUT
    Ptga_stream = NULL;
    #else
    //Pdst = new tga_writer();

    Ptga_stream->constructor( Ptga_stream );

    status = Ptga_stream->open( Pdst_filename, MApi_JPEG_GetWidth(), MApi_JPEG_GetHeight(),
        ( JPEG_CMODEL_get_num_components() == 1 ) ? E_TGA_IMAGE_TYPE_GREY : E_TGA_IMAGE_TYPE_BGR, Ptga_stream );

    if ( status )
    {
        //delete Pd;
        MApi_JPEG_Exit();
        //delete Pinput_stream;
        Pinput_stream->destructor( Pinput_stream );
        //delete Pdst;
        Ptga_stream->destructor( Ptga_stream );

        printf( "Error: Unable to open file \"%s\" for writing!\n", Pdst_filename );

        return ( EXIT_FAILURE );
    }
    #endif

    printf( "Width: %i\n", MApi_JPEG_GetWidth() );
    printf( "Height: %i\n", MApi_JPEG_GetHeight() );
    printf( "Components: %i\n", JPEG_CMODEL_get_num_components() );

#if 0
    if ( msAPI_JPEG_begin() )
    {
        printf( "Error: Decoder failed! Error status: %i\n", msAPI_JPEG_get_error_code() );

        //delete Pd;
        msAPI_JPEG_Finalize();
        //delete Pinput_stream;
        Pinput_stream->destructor( Pinput_stream );
        //delete Pdst;
        Ptga_stream->destructor( Ptga_stream );
        remove( Pdst_filename );

        return ( EXIT_FAILURE );
    }
#endif

    JPEG_CMODEL_DumpTables();

    if ( JPEG_CMODEL_get_num_components() == 3 )
    {
        Pbuf = ( U8 * )malloc( MApi_JPEG_GetWidth() * 3 );
        if ( !Pbuf )
        {
            printf( "Error: Out of memory!\n" );

            //delete Pd;
            MApi_JPEG_Exit();
            //delete Pinput_stream;
            Pinput_stream->destructor( Pinput_stream );
            //delete Pdst;
            Ptga_stream->destructor( Ptga_stream );
            remove( Pdst_filename );

            return ( EXIT_FAILURE );
        }
    }

    if(MApi_JPEG_IsProgressive())
    {
        printf("Do RLE\n");
        printf("==========\n");
    }

    for ( ; ; )
    {
        void *Pscan_line_ofs;
        U32 scan_line_len;
        BOOL status;

        if (E_JPEG_OKAY == JPEG_CMODEL_decode( &Pscan_line_ofs, &scan_line_len ))
        {
            break;
        }

        lines_decoded++;

        #ifndef NO_OUTPUT
        if ( JPEG_CMODEL_get_num_components() == 3 )
        {
            U8 *Psb = ( U8 * )Pscan_line_ofs;
            U8 *Pdb = Pbuf;
            int src_bpp = JPEG_CMODEL_get_bytes_per_pixel();
            int x;

            for ( x = MApi_JPEG_GetWidth(); x > 0; x--, Psb += src_bpp, Pdb += 3 )
            {
                Pdb[0] = Psb[2];
                Pdb[1] = Psb[1];
                Pdb[2] = Psb[0];
            }

            status = Ptga_stream->writeline( Pbuf, Ptga_stream );
        }
        else
        {
            status = Ptga_stream->writeline( Pscan_line_ofs, Ptga_stream );
        }

        if ( status )
        {
            printf( "Error: Unable to write to file \"%s\"!\n", Pdst_filename );

            free( Pbuf );
            //delete Pd;
            MApi_JPEG_Exit();
            //delete Pinput_stream;
            Pinput_stream->destructor( Pinput_stream );
            //delete Pdst;
            Ptga_stream->destructor( Ptga_stream );
            remove( Pdst_filename );

            return ( EXIT_FAILURE );
        }
        #endif
    }

    free( Pbuf );

    if(MApi_JPEG_IsProgressive())
    {
        printf("==========\n");
        printf("RLE end\n");
    }

    #ifndef NO_OUTPUT
    if ( Ptga_stream->close( Ptga_stream ) )
    {
        printf( "Error: Unable to write to file \"%s\"!\n", Pdst_filename );

        //delete Pd;
        MApi_JPEG_Exit();
        //delete Pinput_stream;
        Pinput_stream->destructor( Pinput_stream );
        //delete Pdst;
        Ptga_stream->destructor( Ptga_stream );
        remove( Pdst_filename );

        return ( EXIT_FAILURE );
    }

    //delete Pdst;
    Ptga_stream->destructor( Ptga_stream );
    #endif

    if (E_JPEG_NO_ERROR != MApi_JPEG_GetErrorCode())
    {
        printf( "Error: Decoder failed! Error status: %i\n", MApi_JPEG_GetErrorCode() );

        //delete Pd;
        MApi_JPEG_Exit();
        //delete Pinput_stream;
        Pinput_stream->destructor( Pinput_stream );
        //delete Pdst;
        Ptga_stream->destructor( Ptga_stream );

        return ( EXIT_FAILURE );
    }

    //msAPI_JPEG_Dump_Tbl();

    printf( "Lines decoded: %i\n", lines_decoded );
    printf( "Input file size:  %ld\n", Pinput_stream->get_size( Pinput_stream ) );
    printf( "Input bytes actually read: %ld\n", JPEG_CMODEL_get_total_bytes_read() );

    //delete Pd;
    MApi_JPEG_Exit();
    //delete Pinput_stream;
    Pinput_stream->destructor( Pinput_stream );

    return ( EXIT_SUCCESS );
}
//------------------------------------------------------------------------------


