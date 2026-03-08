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
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


#ifndef _TS_MUXER_H_
#define _TS_MUXER_H_


//#define _EN_TSCRC_
//#define TS_192_BYTE   //must be defined for LG
#define DEFAULT_START_PTS 1000

typedef struct MpegTSSection {
    int pid;
    int cc;
} MpegTSSection;

typedef struct MpegTSProgram {
    MpegTSSection pmt;
    int prog_num;
    int pcr_pid;
    int pcr_packet_count;
    int pcr_packet_freq;
} MpegTSProgram;

// TS stream
typedef struct MpegTSWrite {
    MpegTSSection pat;
    MpegTSProgram program;	// One and only 1 program
    int pat_packet_count;
    int pat_packet_freq;
    int tsid;
    MFE_S64 cur_pcr;
	// Time-code related info
    int mux_rate;
	MFE_S64 min_time;	// min PTS of all streams
	int waited_datasize;
    // Support multiple segments in one video/audio frame
    MFE_U32 temp_size;
    MFE_U8 temp188[188];
} MpegTSWrite;

int tsmux_open(struct avmux_s *inst);
int tsmux_write_packet(struct avmux_s *inst, AUContext *pkt);
int tsmux_close(struct avmux_s *inst);

#endif
