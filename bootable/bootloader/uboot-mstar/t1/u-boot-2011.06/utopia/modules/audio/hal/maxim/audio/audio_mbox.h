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
#ifndef _AUDIO_MBOX_H_
#define _AUDIO_MBOX_H_

/********************************************************************
* MIU unit type
********************************************************************/

    #define SE_DMA_16                 0


/* MIU Line Size */
    #define MIU_128                     1

    #if (MIU_128 == 1)
        #define BYTES_IN_MIU_LINE       16
        #define BYTES_IN_MIU_LINE_LOG2  4
        #define MAD_MEMOFFSET_LOG2      12                
    #else
        #define BYTES_IN_MIU_LINE       8
        #define BYTES_IN_MIU_LINE_LOG2  3
        #define MAD_MEMOFFSET_LOG2      11        
    #endif

/************************************************
*   mailbox type
************************************************/
    #define MBOX_BITS_OFFSET   0
    #define MBOX_BITS_SHIFT    0
    #define MBOX_STEP_ONE      0x1
    #define MBOX_MASK          0xFFFF
    #define MBOX_HIBYTE_MASK   0x00FF00
    #define MBOX_LOBYTE_MASK   0x0000FF

    #define MBOX_BIT0       MBOX_BITS_OFFSET+0
    #define MBOX_BIT1       MBOX_BITS_OFFSET+1
    #define MBOX_BIT2       MBOX_BITS_OFFSET+2
    #define MBOX_BIT3       MBOX_BITS_OFFSET+3
    #define MBOX_BIT4       MBOX_BITS_OFFSET+4
    #define MBOX_BIT5       MBOX_BITS_OFFSET+5
    #define MBOX_BIT6       MBOX_BITS_OFFSET+6
    #define MBOX_BIT7       MBOX_BITS_OFFSET+7
    #define MBOX_BIT8       MBOX_BITS_OFFSET+8
    #define MBOX_BIT9       MBOX_BITS_OFFSET+9
    #define MBOX_BIT10      MBOX_BITS_OFFSET+10
    #define MBOX_BIT11      MBOX_BITS_OFFSET+11
    #define MBOX_BIT12      MBOX_BITS_OFFSET+12
    #define MBOX_BIT13      MBOX_BITS_OFFSET+13
    #define MBOX_BIT14      MBOX_BITS_OFFSET+14
    #define MBOX_BIT15      MBOX_BITS_OFFSET+15

/************************************************
*   mailbox define
************************************************/
    #define MB_2D00         0x2D00
    #define MB_2D02         0x2D02
    #define MB_2D04         0x2D04
    #define MB_2D06         0x2D06
    #define MB_2D08         0x2D08
    #define MB_2D0A         0x2D0A
    #define MB_2D0C         0x2D0C
    #define MB_2D0E         0x2D0E
    #define MB_2D10         0x2D10
    #define MB_2D12         0x2D12
    #define MB_2D14         0x2D14
    #define MB_2D16         0x2D16
    #define MB_2D18         0x2D18
    #define MB_2D1A         0x2D1A
    #define MB_2D1C         0x2D1C
    #define MB_2D1E         0x2D1E
    #define MB_2D20         0x2D20
    #define MB_2D22         0x2D22
    #define MB_2D24         0x2D24
    #define MB_2D26         0x2D26
    #define MB_2D28         0x2D28
    #define MB_2D2A         0x2D2A
    #define MB_2D2C         0x2D2C
    #define MB_2D2E         0x2D2E
    #define MB_2D30         0x2D30
    #define MB_2D32         0x2D32
    #define MB_2D34         0x2D34
    #define MB_2D36         0x2D36
    #define MB_2D38         0x2D38
    #define MB_2D3A         0x2D3A
    #define MB_2D3C         0x2D3C
    #define MB_2D3E         0x2D3E
    #define MB_2D40         0x2D40
    #define MB_2D42         0x2D42
    #define MB_2D44         0x2D44
    #define MB_2D46         0x2D46
    #define MB_2D48         0x2D48
    #define MB_2D4A         0x2D4A
    #define MB_2D4C         0x2D4C
    #define MB_2D4E         0x2D4E
    #define MB_2D50         0x2D50
    #define MB_2D52         0x2D52
    #define MB_2D54         0x2D54
    #define MB_2D56         0x2D56
    #define MB_2D58         0x2D58
    #define MB_2D5A         0x2D5A
    #define MB_2D5C         0x2D5C
    #define MB_2D5E         0x2D5E
    #define MB_2D60         0x2D60
    #define MB_2D62         0x2D62
    #define MB_2D64         0x2D64
    #define MB_2D66         0x2D66
    #define MB_2D68         0x2D68
    #define MB_2D6A         0x2D6A
    #define MB_2D6C         0x2D6C
    #define MB_2D6E         0x2D6E
    #define MB_2D70         0x2D70
    #define MB_2D72         0x2D72
    #define MB_2D74         0x2D74
    #define MB_2D76         0x2D76
    #define MB_2D78         0x2D78
    #define MB_2D7A         0x2D7A
    #define MB_2D7C         0x2D7C
    #define MB_2D7E         0x2D7E
    #define MB_2D80         0x2D80
    #define MB_2D82         0x2D82
    #define MB_2D84         0x2D84
    #define MB_2D86         0x2D86
    #define MB_2D88         0x2D88
    #define MB_2D8A         0x2D8A
    #define MB_2D8C         0x2D8C
    #define MB_2D8E         0x2D8E
    #define MB_2D90         0x2D90
    #define MB_2D92         0x2D92
    #define MB_2D94         0x2D94
    #define MB_2D96         0x2D96
    #define MB_2D98         0x2D98
    #define MB_2D9A         0x2D9A
    #define MB_2D9C         0x2D9C
    #define MB_2D9E         0x2D9E
    #define MB_2DA0         0x2DA0
    #define MB_2DA2         0x2DA2
    #define MB_2DA4         0x2DA4
    #define MB_2DA6         0x2DA6
    #define MB_2DA8         0x2DA8
    #define MB_2DAA         0x2DAA
    #define MB_2DAC         0x2DAC
    #define MB_2DAE         0x2DAE
    #define MB_2DB0         0x2DB0
    #define MB_2DB2         0x2DB2
    #define MB_2DB4         0x2DB4
    #define MB_2DB6         0x2DB6
    #define MB_2DB8         0x2DB8
    #define MB_2DBA         0x2DBA
    #define MB_2DBC         0x2DBC
    #define MB_2DBE         0x2DBE
    #define MB_2DC0         0x2DC0
    #define MB_2DC2         0x2DC2
    #define MB_2DC4         0x2DC4
    #define MB_2DC6         0x2DC6
    #define MB_2DC8         0x2DC8
    #define MB_2DCA         0x2DCA
    #define MB_2DCC         0x2DCC
    #define MB_2DCE         0x2DCE
    #define MB_2DD0         0x2DD0
    #define MB_2DD2         0x2DD2
    #define MB_2DD4         0x2DD4
    #define MB_2DD6         0x2DD6
    #define MB_2DD8         0x2DD8
    #define MB_2DDA         0x2DDA
    #define MB_2DDC         0x2DDC
    #define MB_2DDE         0x2DDE
    #define MB_2DE0         0x2DE0
    #define MB_2DE2         0x2DE2
    #define MB_2DE4         0x2DE4
    #define MB_2DE6         0x2DE6
    #define MB_2DE8         0x2DE8
    #define MB_2DEA         0x2DEA
    #define MB_2DEC         0x2DEC
    #define MB_2DEE         0x2DEE
    #define MB_2DF0         0x2DF0
    #define MB_2DF2         0x2DF2
    #define MB_2DF4         0x2DF4
    #define MB_2DF6         0x2DF6
    #define MB_2DF8         0x2DF8
    #define MB_2DFA         0x2DFA
    #define MB_2DFC         0x2DFC
    #define MB_2DFE         0x2DFE

#endif //_AUDIO_MBOX_H_
