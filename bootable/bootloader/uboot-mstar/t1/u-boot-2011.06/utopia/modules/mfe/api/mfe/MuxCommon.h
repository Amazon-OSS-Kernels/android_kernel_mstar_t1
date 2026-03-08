/**
* Copyright (c) 2006 – 2018 MStar Semiconductor, Inc.
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
//******************************************************************************
//******************************************************************************
#ifndef _MUX_COMMON_H_
#define _MUX_COMMON_H_


//#define FILE_TEST    // If defined, will write out muxed file into D: drive.
// Allow reading all unread data
//#define CHECK_READ_WRITE_PTR

#define MAX_STREAM_COUNT 2
#define SAMPLES_IN_AUDIO_FRAME 1152

typedef enum{
    MUX_TS = 0,
    MUX_AVI,
    MUX_MP4
} MUX_TYPE;
//////////////////////////////////////////////////////////////////////////
// OutStream

typedef enum {
    INDEX_TYPE_SEQ = 0,
	INDEX_TYPE_I_PIC,
    INDEX_TYPE_SEQ_I_PIC,
    INDEX_TYPE_P_PIC,
    INDEX_TYPE_B_PIC,
    INDEX_TYPE_NONE
} INDEX_TYPE_T;

typedef struct {
    INDEX_TYPE_T indexType;
    MFE_U32 byteOffset;
} IndexType;

typedef struct {
    IndexType index_data;
    MFE_U32 used;
    MFE_U32 next;
} IndexRingEntry;

typedef struct {
#ifdef FILE_TEST
    void* handle;    // open file handle
#endif
    MFE_U8 *bbuffer;    // big buffer start
    MFE_U8 *bbufend_ptr;    // (bbufend_ptr - bbuffer) is the size of big buffer
    MFE_U8 *write_ptr, *read_ptr;
    MFE_U64 total_size;    // latest ts stream length

    // Index buffer
    IndexRingEntry* index_ring;
    MFE_U32 ring_size;
    MFE_U32 index_read, index_write;

    // Bookmark: for delayed read/write
    MFE_U32 locked;
    MFE_U8 *bookmark;
} Outstream;

#define MAX_UPDATE_COUNT 1
typedef struct OutDataInfo {
    MFE_U8 *header;
    int size1;
    MFE_U8 *data;
    int size2;
    int write_type;    // 0 or 1
    INDEX_TYPE_T type;
    int write_timecode; // 0 or 1
    MFE_U32 timecode;
    // AVI stuffing
    MFE_U32 stuffing;
    // Support multiple segments in one video/audio frame
    MFE_U8 lock_write;
    MFE_U8 update_count;
    MFE_U32 update_addr[MAX_UPDATE_COUNT];    // Offset from the bookmark position.
    MFE_U8* update_data[MAX_UPDATE_COUNT];
    MFE_U32 update_size[MAX_UPDATE_COUNT];
} OutDataInfo;

void outbuf_init(Outstream *s);
void outbuf_put(Outstream *s, OutDataInfo* pinfo);
void outbuf_flush(Outstream *s);
void outbuf_close(Outstream *s);
int outbuf_get(Outstream *s, MFE_U8 **ppbuf, MFE_U8 **ppread, MFE_U8 **ppwrite, IndexRingEntry **ppindex);
void outbuf_getdone(Outstream *s, MFE_U8 *pread, MFE_U32 index_read);

static __inline int put16_msbf(MFE_U8 **ptr, int val)
{
    MFE_U8 *q;
    q = *ptr;
    *q++ = val>>8;
    *q++ = val&0xFF;
    *ptr = q;
    return 2;
}

static __inline int put16_lsbf(MFE_U8 **ptr, int val)
{
    MFE_U8 *q;
    q = *ptr;
    *q++ = val&0xFF;
    *q++ = val>>8;
    *ptr = q;
    return 2;
}

static __inline int put32_msbf(MFE_U8 **ptr, int val)
{
    MFE_U8 *q;
    q = *ptr;
    *q++ = val>>24;
    *q++ = (val>>16)&0xFF;
    *q++ = (val>>8)&0xFF;
    *q++ = val&0xFF;
    *ptr = q;
    return 4;
}

static __inline int put32_lsbf(MFE_U8 **ptr, int val)
{
    MFE_U8 *q;
    q = *ptr;
    *q++ = val&0xFF;
    *q++ = (val>>8)&0xFF;
    *q++ = (val>>16)&0xFF;
    *q++ = val>>24;
    *ptr = q;
    return 4;
}

//////////////////////////////////////////////////////////////////////////
// Media and Access Unit
#define MAX_VALUE            (1<<30)
#ifdef _WIN32
#if (_MSC_VER < 1400) //vc 6 or older version
#define INVALID_PTS         (0x8000000000000000##i64)
#define MAXVALUE_PTS		(0x7FFFFFFFFFFFFFFF##i64)
#else
#define INVALID_PTS         ((MFE_S64)(0x8000000000000000))
#define MAXVALUE_PTS		((MFE_S64)(0x7FFFFFFFFFFFFFFF))
#endif
//#define INVALID_PTS         ((MFE_S64)(0x8000000000000000))
//#define MAXVALUE_PTS		((MFE_S64)(0x7FFFFFFFFFFFFFFF))
#else

#define INVALID_PTS         (0x8000000000000000##ULL)
#define MAXVALUE_PTS		((0x7FFFFFFFFFFFFFFF##ULL)

//#define INVALID_PTS         ((MFE_S64)(0x8000000000000000LL))
//#define MAXVALUE_PTS		((MFE_S64)(0x7FFFFFFFFFFFFFFFLL))
#endif
typedef enum _CodecType {
    CODEC_TYPE_VIDEO,
    CODEC_TYPE_AUDIO,
} CodecType;

typedef enum _CodecID {
    CODEC_ID_NONE = 999,
    // Video
    CODEC_ID_VIDEO_MPEG2 = 10,
    CODEC_ID_VIDEO_MPEG4 = 0,
    CODEC_ID_VIDEO_H264  = 2,
    // Audio
    CODEC_ID_AUDIO_MP2   = 101,
    CODEC_ID_AUDIO_MP3   = 102,
} CodecID;

typedef struct StreamContext {
    CodecType codec_type;
    CodecID codec_id;
    int bit_rate;    // Average bitrate
    MFE_S64 payload_pts;
    MFE_S64 payload_dts;
    MFE_U8 *payload;

    /* MPEG TS related */

    /*MpegTSProgram*/void *program;
    int pid; // stream pid
    int cc;        // continuity_counter

    /* AVI related */
    // Internal usage
    MFE_U32 frame_count;
    // Input: Video
    MFE_U32 width;
    MFE_U32 height;
    float framerate;
    // Input: Audio
    MFE_U32 samplerate;
} StreamContext;

typedef struct MediaContext {    // Media Context
    void* muxinfo;            // Hold mux-format specific internal data
    Outstream* outstream;    // Output context
    MFE_U32 stream_count;
    StreamContext* streams[MAX_STREAM_COUNT];
    MFE_S64 timestamp;
} MediaContext;

typedef struct AUContext {    // Access Unit Context
    MFE_U8 *access_unit;
    MFE_U32 unit_size;
    MFE_S64 pts;
    MFE_S64 dts;
    int   stream_index;    // index into MediaContext.streams
    INDEX_TYPE_T index_type;
    // (For multiple segments for one video/audio frame)
    MFE_U8 first_segment;
    MFE_U8 last_segment;
} AUContext;

//////////////////////////////////////////////////////////////////////////
// muxer structures

typedef struct avmux_s
{
    MFE_U32 state;

    // internal function
    int (*write_open)(struct avmux_s *inst);
    int (*write_pause)(struct avmux_s *inst);
    int (*write_packet)(struct avmux_s *inst, AUContext *packet);
    int (*write_resume)(struct avmux_s *inst);
    int (*write_close)(struct avmux_s *inst);
    // read functions
    int (*read)(struct avmux_s *inst, MFE_U8 **ppStreamBuffer, MFE_U8 **ppBufReadPtr, MFE_U8 **ppBufWritePtr, IndexRingEntry **ppIndexPtr);
    void (*read_done)(struct avmux_s *inst, MFE_U8 *pBufReadPtr, MFE_U32 IndexRead);

    MediaContext* mctx;
    void*          endinfo;
}avmux_st, *pavmux_st;

// Return 0 if success (buffer not empty).
int mux_read(struct avmux_s *inst, MFE_U8 **ppStreamBuffer, MFE_U8 **ppBufReadPtr, MFE_U8 **ppBufWritePtr, IndexRingEntry **ppIndexPtr);
// pBufReadPtr: Updated read pointer of bitstream buffer
// IndexRead: Number of index read.
void mux_readdone(struct avmux_s *inst, MFE_U8 *pBufReadPtr, MFE_U32 IndexRead);

#endif // _MUX_COMMON_H_
