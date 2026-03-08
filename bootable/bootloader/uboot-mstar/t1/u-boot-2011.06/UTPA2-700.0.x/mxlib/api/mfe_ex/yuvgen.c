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

#include "mdrv_mfe_io.h"
#include "MFE_chip.h"
//#include <linux/string.h>
//#else
#ifndef __UBOOT__
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#endif
//#include "madp_ms_dprintf.h"

#define U8 MS_U8
#define U32 MS_U32

MS_U8 *YUV = 0;    // yuv buffer start point
MS_U8 *TILE = 0;    // yuv buffer start point

void copy8pixel(MS_U8 *p);
void YuvGen(MS_U8 seed, MS_S32 width, MS_S32 height, volatile MS_U8 *buff);
void YuvGen_422(MS_U8 seed, MS_S32 width, MS_S32 height, volatile MS_U8 *buff);

#ifdef _MAIN_

#define MFE_DEBUG //x

FILE *fp;
void (*do_copy)(MS_U8 *) ;
static MS_U8 *ptrInTileFormat;
MS_S32 debug = 0;

MS_S32 W = 720;
MS_S32 H = 480;


#define YUV_LEN 50UL

main(MS_S32 argc, MS_S8 *argv[])
{

#if 0
	MS_S32 i;
	YUV = (MS_U8 *) malloc( W * H * 1.5 );
	TILE = (MS_U8 *) malloc( W * H * 1.5 );

	fp = fopen(argv[1], "w");
	if( fp == NULL || YUV == NULL || TILE == NULL ) exit(-1);

	do_copy = copy8pixel;

	for( i = 0 ; i < YUV_LEN ; i ++ ) {
		ptrInTileFormat = TILE;
		YuvGen( 100 +i*5, W, H, TILE );
		ConvertYuvFromTile(W,H,0, YUV);
		ConvertYuvFromTile(W,H,1, YUV);
		fwrite(YUV, W * H * 1.5 , 1 , fp);
	}

	fclose(fp);
	free(YUV);
	free(TILE);
#else
	MS_S32 i;
	FILE *fpr= NULL;
	YUV = (MS_U8 *) malloc( W * H * 1.5 );
	TILE = (MS_U8 *) malloc( W * H * 1.5 );
	fp = fopen(argv[1], "wb");
	fpr = fopen(argv[2], "rb");
	if( fpr == NULL || fp == NULL || YUV == NULL || TILE == NULL ) exit(-1);

	do_copy = copy8pixel;

	while( fread(TILE, W * H * 1.5, 1, fpr) > 0 ) {
		ptrInTileFormat = TILE;
		ConvertYuvFromTile(W,H,0, YUV);
		ConvertYuvFromTile(W,H,1, YUV);
		fwrite(YUV, W * H * 1.5 , 1 , fp);

	}

#endif
}





void copy8pixel(MS_U8 *p)
{
	MS_S32 i ;
	for(i = 0; i < 8; i ++ ) {
		*p++ = *ptrInTileFormat++;
	}
}


void copy_row(MS_U8 *p)
{
	MS_U8 buff[512];

	MS_U8 tmpbuf[9];

	MS_S32 i,j;

	if( fgets(buff,512,fp) != NULL )
	{

		for( j = 0; j < 8 ; j ++ ) {
			MS_U8 tmp = 0;
			for( i = 0; i < 8 ; i ++ ) {
				// MS_U8 k = buff[j*8+i] - '0';
				MS_U8 k = buff[(7-j)*8+i] - '0';
				if( k != 0 && k != 1 )  {
					printf("Err inupt\n");
					exit(-3);
				}
				tmp <<= 1;
				tmp |= k;
			}
			p[j] = tmp;

			if( debug ) fprintf(stderr,"%4d",p[j]);
		}
		if( debug ) fprintf(stderr,"\r\n");
	}
	else {
		printf("read failure\n");
		exit(-4);
	}
}


// copy_row2 is used to verify the binary format
void copy_row2(MS_U8 *p)
{
	if( fread(p, 8,1,fp) == 0 )  {
		printf("read error in fread\n");
		return ;
	}
}

#endif

void ConvertYuvFromTile(MS_S32 width, MS_S32 height, MS_S32 mode, MS_U8 *in_yuv, MS_U8 *out_yuv)
{

	MS_S32 mbx, mby;
	MS_S32 image_x, image_x2;
	MS_S32 MB_in_width, MB_in_height;
	MS_S32 k,l,lumstart,chrstart;
	MS_U8 *y_point, *u_point, *v_point;
	MS_U8 *y_point2;
	MS_U8 *blk_point;
	MS_S32 blk, row;

	image_x = width;

	MB_in_width  = width >> 4 ;
	MB_in_height = height >> 4 ;

	if( mode == 0 ) {  // Y component
		for (mby = 0; mby < MB_in_height; mby++)
		{
			for (mbx = 0; mbx < MB_in_width; mbx++)
			{
				k = mby * image_x << 4 /* * MB_SIZE*/;
				l = mbx << 4/** MB_SIZE*/;
				lumstart =k+l;

				y_point = out_yuv + lumstart;
				y_point2 = y_point + (image_x << 3) /** 8 */;
				for (blk = 0; blk < 4; blk++)
				{
					blk_point = (blk>>1)? y_point2 : y_point;
					blk_point += (blk&1)? 8/*B_SIZE*/ : 0;

					for (row = 0; row < 8; row++)
					{
						// fwrite(blk_point, 8, 1, fp);
						//do_copy(blk_point); /* 8 points  */
						memcpy(blk_point, in_yuv, 8);
//                        ms_dprintk(DRV_L3,"0x%x 0x%x %d %d %d %d %d %d %d %d\n", (U32)blk_point, (U32)in_yuv, in_yuv[0], in_yuv[1]
//                            , in_yuv[2], in_yuv[3], blk_point[0], blk_point[1], blk_point[2], blk_point[3]);
						blk_point += image_x;
                        in_yuv += 8;
					}
				}
			}
		}
	}
	else { /* U & V */
		image_x2 = image_x >> 1 ;
		for (mby = 0; mby < MB_in_height; mby++)
		{
			for (mbx = 0; mbx < MB_in_width; mbx++)
			{
				k= mby * image_x << 4 /* * MB_SIZE*/;
				l= mbx << 4   /** MB_SIZE*/;
				chrstart =(k>>2)+(l>>1);

				u_point = out_yuv + width * height + chrstart;
				v_point = out_yuv + width * height + ( width >> 1 ) * ( height >> 1 )  + chrstart;

				for (blk = 0; blk < 2; blk++) // U block then V block..
				{
					blk_point = ( blk == 0 ) ? u_point : v_point;

					for (row = 0; row < 8; row++)
					{
						//for (col = 0; col < 8; col++)
						//{
							//mem_uv_point[col] = blk_point[col];
						//}
//						do_copy(blk_point);
						memcpy(blk_point, in_yuv, 8);
						blk_point += image_x2;
                        in_yuv += 8;
					}
				}
			}
		}
	}
}


#if 0
main(MS_S32 argc, MS_S8 *argv[])
{
	FILE *fpout = NULL;
	MS_S32 width;
	MS_S32 height;

	MS_S8 buff[512];
	MS_U32 num[8];

	MS_S32 i = 0;
	MS_S32 j = 0;
	MS_S32 m = 0;

	if( argc < 3 )  {
		printf("%s width height input_y input_c out.yuv\n",argv[0]);
		exit(-1);
	}
	width  = atoi(argv[1]);
	height = atoi(argv[2]);

	fp = fopen(argv[3], "r");
	if( fp == NULL) exit(-1);

	fpout = fopen(argv[5], "w");
	if( fpout == NULL) exit(-1);

	yuv = ( MS_U8 *) malloc( width * height * 1.5 ) ;
	if( yuv == NULL ) {
		printf("malloc fail");
		exit(-5);
	}

	// do_copy = copy_row2;
	do_copy = copy_row;

    gogo(width,height,0);
	fclose(fp);

	debug = 1;
	fopen( argv[4] , "rb");
	if( fp == NULL) {ms_dprintk(DRV_L1,"open %s failure\n",argv[4]); exit(-1); }
    gogo(width,height,1);
	fclose(fp);

	fwrite(yuv, width * height * 1.5 , 1 , fpout);

	fclose(fpout);
	free(yuv);
}
#endif

void YuvGen(MS_U8 seed, MS_S32 width, MS_S32 height, volatile MS_U8 *buff)
{
	MS_S32 i, j,k,m,n;

	MS_S32 inc = 5 ;
	MS_S32 MB_width = width >> 4;
	MS_S32 MB_height = height >> 4 ;

	//ms_dprintf(ADP_L1,"YuvGen begin: MB_width: %d MB_height: %d seed:%d\n",MB_width, MB_height,seed);

    // Y component

	for( i = 0; i < MB_height ; i ++ ) {
		for( j = 0; j < MB_width ; j ++ ) {
			for( m = 0; m < 4 ; m ++ ) {  // 4 block
                for( k = 0; k < 8 ; k ++ ) {
                    for( n = 0; n < 8; n ++ )
                        *buff++ = seed;
                }
            }
            if( inc ) {
                seed += inc ;
                if( seed >= 255 ) seed = 0;
			}
//			printf("buff addr: 0x%x block[%2d][%2d][%d]\n",(U32)buff,i,j,m);
		}
	}
    //MFE_DEBUG(printf("UV component begin\n"));
    // U V component
    height <<= 1 ;
    width <<= 1 ;

	for( i = 0; i < MB_height ; i ++ ) {
		for( j = 0; j < MB_width ; j ++ ) {
			for( k = 0; k < 8 ; k ++ ) {    // U
                for( n = 0; n < 8; n ++ ) {
                    //*buff++ = seed;
                    *buff = seed;
                    buff++;
                }
			}
			for( k = 0; k < 8 ; k ++ ) {    // V
                for( n = 0; n < 8; n ++ ) {
                    //*buff++ = seed;
                    *buff = seed;
                    buff++;
                }
			}
			if( inc ) {
				seed += inc ;
				if( seed >= 255 ) seed = 0;
			}
		}
	}
    //ms_dprintf(ADP_L1,"YuvGen Done. seed:%d\n",seed);
}

void YuvGen_422(MS_U8 seed, MS_S32 width, MS_S32 height, volatile MS_U8 *buff)
{
	MS_S32 i, j,k,m,n;

	MS_S32 inc = 5 ;
	MS_S32 MB_width = width >> 4 ;
	MS_S32 MB_height = height >> 4 ;

	//ms_dprintf(ADP_L1,"YuvGen begin: MB_width: %d MB_height: %d seed:%d\n",MB_width, MB_height,seed);

	// Y component

	for( i = 0; i < MB_height ; i ++ ) {
		for( j = 0; j < MB_width ; j ++ ) {
			for( m = 0; m < 4 ; m ++ ) {  // 4 block
				for( k = 0; k < 8 ; k ++ ) {
					for( n = 0; n < 8; n ++ )
                        *buff++ = seed;
				}
			}
			if( inc ) {
				seed += inc ;
				if( seed >= 255 ) seed = 0;
			}
//			printf("buff addr: 0x%x block[%2d][%2d][%d]\n",(U32)buff,i,j,m);
		}
	}
	//ms_dprintf(ADP_L1,"UV component begin\n");
	// U V component
	width <<= 1 ;

	for( i = 0; i < MB_height ; i ++ ) {
		for( j = 0; j < MB_width ; j ++ ) {
			for( k = 0; k < 16 ; k ++ ) {    // U
				for( n = 0; n < 8; n ++ )
                    *buff++ = seed;
			}

			for( k = 0; k < 16 ; k ++ ) {    // V
				for( n = 0; n < 8; n ++ )
                    *buff++ = seed;
			}
			if( inc ) {
				seed += inc ;
				if( seed >= 255 ) seed = 0;
			}
		}
	}
	//ms_dprintf(ADP_L1,"YuvGen Done. seed:%d\n",seed);
}

#define CEILING_ALIGN(value, align) (((MS_U32)(value)+((align)-1UL)) & ~((align)-1UL))

static void PutTileY(MS_U8 *pY, MS_U8 *pDst, MS_S32 nStride, MS_S32 nMaxX, MS_S32 nMaxY)
{
    MS_U8 *s = pY;
    MS_U8 *t = pDst;
    MS_S32 y;
    for (y=0; y<nMaxY; y++) {
        memcpy(t, s, nMaxX);
        s += nStride;
        t += 16;
    }
}

static void PutTileUV(MS_U8 *pU, MS_U8 *pV, MS_U8 *pDst, MS_S32 nStride, MS_S32 nMaxX, MS_S32 nMaxY)
{
    MS_U8 *s1 = pU;
    MS_U8 *s2 = pV;
    MS_U8 *t = pDst;
    MS_S32 x, y;
    for (y=0; y<nMaxY; y++) {
        for (x=0; x<nMaxX; x++) {
            t[x<<1] = s1[x];
            t[(x<<1)+1] = s2[x];
        }
        s1 += nStride;
        s2 += nStride;
        t += 16;
    }
}

MS_BOOL ConvertYV12ToHVDTile(MS_U8 *pSrc1, MS_U8 *pSrc2, MS_S32 nSrcWidth, MS_S32 nSrcHeight, MS_S32 nSrcStride, MS_U8 *pDst, MS_S32 nDstSize, MS_S32 YPlane)
{
    MS_S32 nHvdWidth, nHvdHeight, nSize;
    MS_S32 x, y, nTileWidth, nTileHeight;
    MS_U8 *pSrcPtr1, *pSrcPtr2, *pDstPtr;

    // Calculate dst width and height
    if (YPlane) {
        nHvdWidth  = CEILING_ALIGN(nSrcWidth, 16);
        nHvdHeight = CEILING_ALIGN(nSrcHeight, 32);
    }
    else {
        nHvdWidth  = CEILING_ALIGN(nSrcWidth<<1, 16); // U,V interleave
        nHvdHeight = CEILING_ALIGN(nSrcHeight, 32);
    }
    // Check dst size
    nSize = nHvdWidth * nHvdHeight;
    if (nDstSize < nSize) {
        //MS_ASSERT(0);
        return FALSE;
    }

    // tile by tile conversion
    nTileWidth  = nHvdWidth /16;
    nTileHeight = nHvdHeight/32;
    if (!YPlane)
        goto TILE_LOOP_UV;

    // Y Plane
    pDstPtr = pDst;
    for (y=0; y<nTileHeight; y++) {
        for (x=0; x<nTileWidth; x++) {
            pSrcPtr1 = pSrc1 + (y*32*nSrcStride) + (x*16);
            PutTileY(pSrcPtr1, pDstPtr, nSrcStride,
                (x==nTileWidth-1) ? nSrcWidth -nHvdWidth +16 : 16,
                (y==nTileHeight-1)? nSrcHeight-nHvdHeight+32 : 32);
            pDstPtr += 16*32;
        }
    }
    return TRUE;

TILE_LOOP_UV:
    // U,V planes
    pDstPtr = pDst;
    for (y=0; y<nTileHeight; y++) {
        for (x=0; x<nTileWidth; x++) {
            pSrcPtr1 = pSrc1 + (y*32*nSrcStride) + (x*8);
            pSrcPtr2 = pSrc2 + (y*32*nSrcStride) + (x*8);
            PutTileUV(pSrcPtr1, pSrcPtr2, pDstPtr, nSrcStride,
                (x==nTileWidth-1) ? nSrcWidth -(nHvdWidth>>1) +8  : 8,
                (y==nTileHeight-1)? nSrcHeight-nHvdHeight     +32 : 32);
            pDstPtr += 16*32;
        }
    }
    return TRUE;
}

void yuv2tile(/*FILE *fp*/MS_U8 *output, MS_S32 width, MS_S32 height, MS_U8 *buff , MS_S32 Ypart)
{
    MS_S32 mbx, mby;
    MS_S32 image_x, image_x2;
    MS_S32 MB_in_width, MB_in_height;
    MS_S32 k,l,lumstart,chrstart;
    U8 *y_point, *u_point, *v_point;
    U8 *y_point2;
    U8 *blk_point;
    volatile MS_U8 *tmp_out=output;
    MS_S32 blk, row;


    if (buff != 0)
    {
        image_x = width;
        MB_in_width = image_x >>4/*/ MB_SIZE*/;
        MB_in_height = height >> 4;

        if (Ypart == 1)
        {
            // dump Y plane first
            for (mby = 0; mby < MB_in_height; mby++)
            {
                for (mbx = 0; mbx < MB_in_width; mbx++)
                {
                    k = mby * image_x << 4 /* * MB_SIZE*/;
                    l = mbx << 4/** MB_SIZE*/;
                    lumstart =k+l;

                    y_point = buff + lumstart;
                    y_point2 = y_point + (image_x << 3) /** 8 */;

                    for (blk = 0; blk < 4; blk++)
                    {
                        blk_point = (blk>>1)? y_point2 : y_point;
                        blk_point += (blk&1)? 8/*B_SIZE*/ : 0;

                        for (row = 0; row < 8; row++)
                        {
                            //for (col = 0; col < 8; col++)
                            //{
                            //	mem_y_point[col] = blk_point[col];
                            //}
//                           ms_dprintk(DRV_L3,"in 0x%2x 0x%2x 0x%2x\n", blk_point[0], blk_point[1], blk_point[2]);
                            memcpy((void *)tmp_out, (const void *)blk_point, 8);
//                            ms_dprintk(DRV_L3,"out 0x%2x 0x%2x 0x%2x\n", tmp_out[0], tmp_out[1], tmp_out[2]);
                            tmp_out+=8;
                            //fwrite(blk_point, 8, 1, fp);
                            blk_point += image_x;
                            //mem_y_point += 8;
                        }
                    }
                }
            }
        }
        else
        {
            // dump UV plane
            image_x2 = image_x >> 1;
            for (mby = 0; mby < MB_in_height; mby++)
            {
                for (mbx = 0; mbx < MB_in_width; mbx++)
                {
                    k= mby * image_x <<4 /* * MB_SIZE*/;
                    l= mbx <<4/** MB_SIZE*/;
                    chrstart =(k>>2)+(l>>1);

                    u_point = buff + /*width * height +*/ chrstart;
                    v_point = buff + /*width * height +*/ ( (width*height) >> 2 ) + chrstart;

                    for (blk = 0; blk < 2; blk++) // U block then V block..
                    {
                        blk_point = (blk==0)? u_point : v_point;

                        for (row = 0; row < 8; row++)
                        {
                            memcpy((void *)tmp_out, (const void *)blk_point, 8);
                            tmp_out+=8;
                            //fwrite(blk_point, 8, 1, fp);
                            blk_point += image_x2;
                        }
                    }
                }
            }
        }
    } else {
    //    ms_dprintf(ADP_L1,"input buffer in yuv2tile() is zero\n");

	}
}
