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


#include <string.h>

#include "MFE_chip.h"
#include "mfe_common.h"
#include "mfe_type.h"
#include "madp_ms_dprintf.h"

#include "MuxTypes.h"
#include "MuxCommon.h"
#include <pthread.h>
//#ifdef FILE_TEST
#include <stdio.h>
//#endif

//////////////////////////////////////////////////////////////////////////
// Outstream related functions
//////////////////////////////////////////////////////////////////////////
pthread_mutex_t TS_mutex;// = PTHREAD_MUTEX_INITIALIZER;

void outbuf_init(Outstream *s)
{
    MFE_U32 i;

    assert(s->bbuffer);
    assert((s->bbufend_ptr-s->bbuffer)>0);

    s->total_size = 0;
    s->read_ptr = s->write_ptr = s->bbuffer;

    s->locked = 0;
    s->bookmark = s->write_ptr;

    for (i=0; i<s->ring_size; i++) {
        s->index_ring[i].index_data.indexType = INDEX_TYPE_NONE;
        s->index_ring[i].index_data.byteOffset = 0;
        s->index_ring[i].used = 0;
        s->index_ring[i].next = i+1;
    }
    s->index_ring[s->ring_size-1].next = 0;
    s->index_read = s->index_write = 0;

#ifdef FILE_TEST
    s->handle = fopen("d:\\test.ts", "wb");
    assert(s->handle);
    s->total_size = 0;
#endif
}

#ifdef CHECK_READ_WRITE_PTR
// NOTE 1: Hazard does not necessary equal to actual overrun.
// NOTE 2: Because of the lock, this checking is intrusive.
// NOTE 3: Index buffer is not checked here!
void check_conservative(Outstream *s, int write_size)
{
	MFE_U8* tmp_ptr;
    pthread_mutex_lock(&TS_mutex);
	if (s->write_ptr >= s->read_ptr) {
		if (s->write_ptr+write_size > s->bbufend_ptr) {
			tmp_ptr = s->bbuffer + ((s->write_ptr+write_size) - s->bbufend_ptr);
			if (tmp_ptr>=s->read_ptr)
				printf("[TSMUX] Buffer overrun hazard condition 1\n");
		}
	}
	else {
		if (s->write_ptr+write_size>=s->read_ptr)
			printf("[TSMUX] Buffer overrun hazard condition 2\n");
	}
    pthread_mutex_unlock(&TS_mutex);
}
#endif

void outbuf_put(Outstream *s, OutDataInfo* pinfo)
{
    int len, size, i, allsize;
    MFE_U8 *buf, *write_ptr;
    MFE_U8 tc_buf[4];

//    printf("[TSMUX] outbuf_put\n");

    write_ptr = s->write_ptr;    // No hazard because only this function will update write_ptr
    pthread_mutex_lock(&TS_mutex);
    //write_ptr = s->write_ptr;
    if (pinfo->lock_write && !s->locked) {
        s->locked = 1;
        s->bookmark = write_ptr;
    }
    pthread_mutex_unlock(&TS_mutex);

	allsize = size = pinfo->size1+pinfo->size2;
    if (pinfo->write_timecode) {
        allsize += 4;	// plus 4-byte timecode
        buf = tc_buf;
        put32_msbf(&buf, pinfo->timecode);
    }
    s->total_size += allsize;
//    printf("[TSMUX] Before check_conservative\n");
#ifdef CHECK_READ_WRITE_PTR
    check_conservative(s, allsize);
#endif
//    printf("%d %d ", pinfo->write_type, pinfo->type);
    if (pinfo->write_type && pinfo->type!=INDEX_TYPE_NONE) {
#ifdef CHECK_READ_WRITE_PTR
 		if (s->index_ring[s->index_write].used)
 			printf("[TSMUX] Index buffer overrun\n");
        //assert(s->index_ring[s->index_write].used==0);
#endif
        s->index_ring[s->index_write].index_data.indexType = pinfo->type;
        s->index_ring[s->index_write].index_data.byteOffset = s->bookmark - s->bbuffer;
        s->index_ring[s->index_write].used = 1;
        s->index_write = s->index_ring[s->index_write].next;
    }

    // Write timecode first
    if (pinfo->write_timecode) {
        buf = tc_buf;
        len = (s->bbufend_ptr - write_ptr);
        if (len>4) {
            memcpy(write_ptr, buf, 4);
            write_ptr += 4;
        }
        else {
            memcpy(write_ptr, buf, len);
            buf += len;
            len = 4 - len;
#ifdef FILE_TEST
            fwrite(s->bbuffer, 1, s->bbufend_ptr-s->bbuffer, (FILE*)s->handle);
#endif
            write_ptr = s->bbuffer;
            if (len>0) {
                memcpy(write_ptr, buf, len);
                write_ptr += len;
            }
        }
    }

    // Write header
    if (pinfo->size1>0) {
        buf = pinfo->header;
        size = pinfo->size1;
        len = (s->bbufend_ptr - write_ptr);
        if (len>size) {
            memcpy(write_ptr, buf, size);
            write_ptr += size;
        }
        else {
            memcpy(write_ptr, buf, len);
            buf += len;
            len = size - len;
#ifdef FILE_TEST
            fwrite(s->bbuffer, 1, s->bbufend_ptr-s->bbuffer, (FILE*)s->handle);
#endif
            write_ptr = s->bbuffer;
            if (len>0) {
                memcpy(write_ptr, buf, len);
                write_ptr += len;
            }
        }
    }

    // Write data
    if (pinfo->size2>0) {
        buf = pinfo->data;
        size = pinfo->size2;
        len = (s->bbufend_ptr - write_ptr);
        if (len>size) {
            memcpy(write_ptr, buf, size);
            write_ptr += size;
        }
        else {
            memcpy(write_ptr, buf, len);
            buf += len;
            len = size - len;
#ifdef FILE_TEST
            fwrite(s->bbuffer, 1, s->bbufend_ptr-s->bbuffer, (FILE*)s->handle);
#endif
            write_ptr = s->bbuffer;
            if (len>0) {
                memcpy(write_ptr, buf, len);
                write_ptr += len;
            }
        }
    }

    // Stuffing byte is necessary for some container, say AVI.
    if (pinfo->stuffing>0) {
        size = pinfo->stuffing;
        len = (s->bbufend_ptr - write_ptr);
        if (len > size) {
            memset(write_ptr, 0, size);
            write_ptr += size;
        }
        else {
            memset(write_ptr, 0, len);
            len = size - len;
            assert(write_ptr==s->bbufend_ptr);
#ifdef FILE_TEST
            fwrite(s->bbuffer, 1, s->bbufend_ptr-s->bbuffer, (FILE*)s->handle);
#endif
            write_ptr = s->bbuffer;
            if (len>0) {
                memset(write_ptr, 0, len);
                write_ptr += len;
            }
        }
    }

    for (i=0; i<pinfo->update_count; i++) {
        len = pinfo->update_addr[i];
        size = pinfo->update_size[i];
        buf = s->bookmark+len;
        if (buf>=s->bbufend_ptr)
            buf -= (s->bbufend_ptr-s->bbuffer);
        len = s->bbufend_ptr - buf;
        if (len>size) {
            memcpy(buf, pinfo->update_data[i], size);
        }
        else {
            memcpy(buf, pinfo->update_data[i], len);
            memcpy(s->bbuffer, pinfo->update_data[i]+len, size-len);
        }
    }

    pthread_mutex_lock(&TS_mutex);
    s->write_ptr = write_ptr;
    if (!pinfo->lock_write) {
        s->locked = 0;
        s->bookmark = write_ptr;
    }
    pthread_mutex_unlock(&TS_mutex);
}

// return 0 if success (buffer not empty)
int outbuf_get(Outstream *s, MFE_U8 **ppbuf, MFE_U8 **ppread, MFE_U8 **ppwrite, IndexRingEntry **ppindex)
{
    int rtn;
//    int i;
    pthread_mutex_lock(&TS_mutex);

    *ppbuf = s->bbuffer;
    *ppread = s->read_ptr;
    *ppwrite = s->bookmark;//s->write_ptr;
    *ppindex = &s->index_ring[s->index_read];
    rtn = (s->read_ptr==s->write_ptr) ? 1 : 0;
    /*
    for (i=0; i<4; i++)
        printf("outbuf_get: %d %d %d %d\n", s->index_ring[i].used, (*ppindex)[i].used, s->index_read
                , s->index_write);
                */
    pthread_mutex_unlock(&TS_mutex);
    return rtn;
}

void outbuf_getdone(Outstream *s, MFE_U8 *pread, MFE_U32 index_read)
{
    pthread_mutex_lock(&TS_mutex);
//    printf("in outbuf_getdone %d\n", index_read);
    s->read_ptr = pread;
    while (index_read>0) {
        s->index_ring[s->index_read].used = 0;
        s->index_read = s->index_ring[s->index_read].next;
        index_read--;
    }

    pthread_mutex_unlock(&TS_mutex);
}

void outbuf_flush(Outstream *s)
{
#ifdef FILE_TEST
    if (s->handle) {
        fwrite(s->bbuffer, 1, s->write_ptr-s->bbuffer, (FILE*)s->handle);
    }
#endif
}

void outbuf_close(Outstream *s)
{
#ifdef FILE_TEST
    if (s->handle) {
        fclose((FILE*)s->handle);
        s->handle=0;
    }
#endif
}

//////////////////////////////////////////////////////////////////////////
// MUX interface common routines

/* return 0 if success, 1 if fail (no data) */
int mux_read(struct avmux_s *inst, MFE_U8 **ppStreamBuffer, MFE_U8 **ppBufReadPtr, MFE_U8 **ppBufWritePtr, IndexRingEntry **ppIndexPtr)
{
    MediaContext *s = inst->mctx;
    int rtn;
    rtn = outbuf_get(s->outstream, ppStreamBuffer, ppBufReadPtr, ppBufWritePtr, ppIndexPtr);
    return rtn;
}

void mux_readdone(struct avmux_s *inst, MFE_U8 *pBufReadPtr, MFE_U32 IndexRead)
{
    MediaContext *s = inst->mctx;
    outbuf_getdone(s->outstream, pBufReadPtr, IndexRead);
}
