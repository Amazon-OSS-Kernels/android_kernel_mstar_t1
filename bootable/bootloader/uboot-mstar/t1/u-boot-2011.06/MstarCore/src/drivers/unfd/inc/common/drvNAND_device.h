/**
* Copyright (c) 2006 ¡V 2016 MStar Semiconductor, Inc.
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

#ifndef __UNFD_DEVICE_H__
#define __UNFD_DEVICE_H__

#include "drvNAND.h"
#if IF_IP_VERIFY
#if defined(H9LA25G25HAMBR) && H9LA25G25HAMBR
	#define NAND_DEVICE_ID               {0xAD,0x35}
	#define NAND_DEVICE_ID_LEN           2
	#define NAND_BLK_CNT                 0x800
	#define NAND_BLK_PAGE_CNT            0x20

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C3TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C2TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x200
	#define NAND_SPARE_BYTE_CNT          0x10

#elif defined(H8ACU0CE0BBR) && H8ACU0CE0BBR
    #define NAND_DEVICE_ID               {0xAD, 0x76}
	#define NAND_DEVICE_ID_LEN           2
	#define NAND_BLK_CNT                 4096
	#define NAND_BLK_PAGE_CNT            0x20

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C4TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x200
	#define NAND_SPARE_BYTE_CNT          0x10

#elif defined(TH58TVG7D2GBA) && TH58TVG7D2GBA
    #define NAND_DEVICE_ID               {0x98, 0xDE, 0x94, 0x82, 0x76, 0xD6}
	#define NAND_DEVICE_ID_LEN           6
	#define NAND_BLK_CNT                 4124 // original 2 /CE have 4124x2 blk, only use /CE0
	#define NAND_BLK_PAGE_CNT            0x100

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_40BIT1KB //

	#define NAND_PAGE_BYTE_CNT           0x2000
	#define NAND_SPARE_BYTE_CNT          640//0x200 // original 640

#elif defined(MT29F64G08CBAAB) && MT29F64G08CBAAB
    #define NAND_DEVICE_ID				 {0x2C, 0x88, 0x04, 0x4B, 0xA9, 0x00, 0x00, 0x00}
	#define NAND_DEVICE_ID_LEN           8
	#define NAND_BLK_CNT                 2048 // one /CE
	#define NAND_BLK_PAGE_CNT            0x100

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_24BIT1KB

	#define NAND_PAGE_BYTE_CNT           0x2000
	#define NAND_SPARE_BYTE_CNT          448
#elif defined(MT29F32G08CBADA) && MT29F32G08CBADA
    #define NAND_DEVICE_ID				 {0x2C, 0x44, 0x44, 0x4B, 0xA9}
	#define NAND_DEVICE_ID_LEN           5
	#define NAND_BLK_CNT                 2048 // one /CE
	#define NAND_BLK_PAGE_CNT            0x100

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_40BIT1KB

	#define NAND_PAGE_BYTE_CNT           0x2000
	#define NAND_SPARE_BYTE_CNT          704

#elif defined(H8ACU0CE0DAR) && H8ACU0CE0DAR
	#define NAND_DEVICE_ID               {0xAD,0x76}
	#define NAND_DEVICE_ID_LEN           2
	#define NAND_BLK_CNT                 0x1000
	#define NAND_BLK_PAGE_CNT            0x20

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C4TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x200
	#define NAND_SPARE_BYTE_CNT          0x10

#elif defined(TC58TEG5DCJTA) && TC58TEG5DCJTA
	#define NAND_DEVICE_ID               {0x98,0xD7,0x84,0x93,0x72,0x57}
	#define NAND_DEVICE_ID_LEN           6
	#define NAND_BLK_CNT                 0x424
	#define NAND_BLK_PAGE_CNT            0x100
	
    #define NAND_RW_ADR_CYCLE_CNT		 ADR_C5TFS0
    #define NAND_E_ADR_CYCLE_CNT		 ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_40BIT1KB
	
	#define NAND_PAGE_BYTE_CNT           0x4000
	#define NAND_SPARE_BYTE_CNT          0x500


#elif defined(TY890A111431KA) && TY890A111431KA
	#define NAND_DEVICE_ID               {0x98,0xD0,0x00,0x15,0x72,0x14}
	#define NAND_DEVICE_ID_LEN           1
	#define NAND_BLK_CNT                 0x200
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C4TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C2TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40

#elif defined(TY8A0A111178KC4) && TY8A0A111178KC4
	#define NAND_DEVICE_ID               {0x98,0xC1,0x90,0x55,0x76,0x14}
	#define NAND_DEVICE_ID_LEN           6
	#define NAND_BLK_CNT                 0x400
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C4TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C2TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40

#elif defined(TY890A111229KC40) && TY890A111229KC40
	#define NAND_DEVICE_ID               {0x98,0xD0,0x00,0x15,0x72,0x14}
	#define NAND_DEVICE_ID_LEN           6
	#define NAND_BLK_CNT                 0x200
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C4TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C2TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40

#elif defined(TY8A0A111162KC40) && TY8A0A111162KC40
	#define NAND_DEVICE_ID               {0x98,0xA1,0x90,0x15,0x76,0x14}
	#define NAND_DEVICE_ID_LEN           6
	#define NAND_BLK_CNT                 0x400
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C4TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C2TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40

#elif defined(K9F1G08X0C) && K9F1G08X0C /* K9F1G08U0C, K9F1G08B0C */
	#define NAND_DEVICE_ID               {0xEC,0xF1,0x00,0x95,0x40}
	#define NAND_DEVICE_ID_LEN           5
	#define NAND_BLK_CNT                 0x400
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C4TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C2TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT//ECC_TYPE_RS

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40

#elif defined(K9GAG08U0E) && K9GAG08U0E /* K9GAG08U0E, K9GAG08U0E */
	#define NAND_DEVICE_ID               {0xEC, 0xD5, 0x84, 0x72, 0x50, 0x42}
	#define NAND_DEVICE_ID_LEN           6
	#define NAND_BLK_CNT                 0x800
	#define NAND_BLK_PAGE_CNT            0x80

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_24BIT1KB//ECC_TYPE_RS//ECC_TYPE_4BIT//
	#define NAND_PAGE_BYTE_CNT           0x2000
	#define NAND_SPARE_BYTE_CNT          0x1B0

#elif defined(K522H1GACE) && K522H1GACE /* K522H1GACE, K522H1GACE */
	#define NAND_DEVICE_ID               {0xEC,0xBA,0x00,0x55,0x44}
	#define NAND_DEVICE_ID_LEN           5

	#define NAND_BLK_CNT                 0x800
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40

#elif defined(H27UAG8T2M) && H27UAG8T2M
	#define NAND_DEVICE_ID               {0xAD,0xD5,0x14,0xB6,0x44}
	#define NAND_DEVICE_ID_LEN           5

	#define NAND_BLK_CNT                 0x1000
	#define NAND_BLK_PAGE_CNT            0x80

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x1000
	#define NAND_SPARE_BYTE_CNT          0x80

#elif defined(H27UAG8T2M_FAKE2K) && H27UAG8T2M_FAKE2K
	#define NAND_DEVICE_ID               {0xAD,0xD5,0x14,0xB6,0x44}
	#define NAND_DEVICE_ID_LEN           5

	#define NAND_BLK_CNT                 0x1000
	#define NAND_BLK_PAGE_CNT            0x80

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_24BIT1KB

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x80

#elif defined(NAND256W3A) && NAND256W3A
	#define NAND_DEVICE_ID               {0x20,0x75}
	#define NAND_DEVICE_ID_LEN           2

	#define NAND_BLK_CNT                 0x800
	#define NAND_BLK_PAGE_CNT            0x20

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C3TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C2TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x200
	#define NAND_SPARE_BYTE_CNT          0x10

#elif defined(NAND512W3A2C) && NAND512W3A2C
	#define NAND_DEVICE_ID               {0x20,0x76}
	#define NAND_DEVICE_ID_LEN           2

	#define NAND_BLK_CNT                 0x1000
	#define NAND_BLK_PAGE_CNT            0x20

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C4TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x200
	#define NAND_SPARE_BYTE_CNT          0x10

#elif defined(H27UF081G2A) && H27UF081G2A
	#define NAND_DEVICE_ID               {0xAD,0xF1,0x80,0x1D}
	#define NAND_DEVICE_ID_LEN           4

	#define NAND_BLK_CNT                 0x400
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C4TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C2TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40

#elif defined(H27UBG8T2A) && H27UBG8T2A
	#define NAND_DEVICE_ID               {0xAD,0xD7,0x94,0x9A,0x74,0x42}
	#define NAND_DEVICE_ID_LEN           6

	#define NAND_BLK_CNT                 0x1000
	#define NAND_BLK_PAGE_CNT            0x100

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_24BIT1KB

	#define NAND_PAGE_BYTE_CNT           0x2000
	#define NAND_SPARE_BYTE_CNT          448

#elif defined(H27U1G8F2B) && H27U1G8F2B
	#define NAND_DEVICE_ID               {0xAD,0xF1,0x00,0x15}
	#define NAND_DEVICE_ID_LEN           4

	#define NAND_BLK_CNT                 0x400
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C4TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C2TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40

#elif defined(HY27UF084G2B) && HY27UF084G2B
	#define NAND_DEVICE_ID               {0xAD,0xDC,0x10,0x95,0x54}
	#define NAND_DEVICE_ID_LEN           5

	#define NAND_BLK_CNT                 0x1000
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40

#elif defined(HY27UF082G2B) && HY27UF082G2B
	//#define NAND_DEVICE_ID               "\xAD\xDA\x10\x95\x44"
	#define NAND_DEVICE_ID               {0xAD,0xDA,0x10,0x95,0x44}
	#define NAND_DEVICE_ID_LEN           5

	#define NAND_BLK_CNT                 0x800
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40//0x100

#elif defined(H27U2G8F2CTR) && H27U2G8F2CTR
    //#define NAND_DEVICE_ID               "\xAD\xDA\x10\x95\x44"
	#define NAND_DEVICE_ID                {0xAD,0xDA,0x90,0x95,0x44}
	#define NAND_DEVICE_ID_LEN           5

	#define NAND_BLK_CNT                 0x800
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40//0x100

#elif defined(H27U4G8F2DTR) && H27U4G8F2DTR

	#define NAND_DEVICE_ID               {0xAD,0xDC,0x90,0x95,0x54}
	#define NAND_DEVICE_ID_LEN           5

	#define NAND_BLK_CNT                 0x1000
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40//0x100

#elif defined(H27U4G8F2ETR) && H27U4G8F2ETR
	
	#define NAND_DEVICE_ID               {0xAD,0xDC,0x90,0x95,0x56}
	#define NAND_DEVICE_ID_LEN           5
	
	#define NAND_BLK_CNT                 0x1000
	#define NAND_BLK_PAGE_CNT            0x40
	
	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT
	
	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40//0x100


#elif defined(K9HAG08U1M) && K9HAG08U1M
	#define NAND_DEVICE_ID               {0xEC,0xD3,0x55,0x25,0x58}
	#define NAND_DEVICE_ID_LEN           5

	#define NAND_BLK_CNT                 0x1000
	#define NAND_BLK_PAGE_CNT            0x80

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40

#elif defined(K511F12ACA) && K511F12ACA
	#define NAND_DEVICE_ID               {0xEC,0xA1,0x00,0x15,0x40}
	#define NAND_DEVICE_ID_LEN           5

	#define NAND_BLK_CNT                 0x400
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C4TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C2TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40

#elif defined(K9F4G08U0D) && K9F4G08U0D
	#define NAND_DEVICE_ID               {0xEC,0xDC,0x10,0x95,0x54}
	#define NAND_DEVICE_ID_LEN           5

	#define NAND_BLK_CNT                 0x1000
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40

#elif defined(P1UAGA30AT) && P1UAGA30AT
	#define NAND_DEVICE_ID               {0xC8,0xD5,0x14,0x29,0x34,0x01}
	#define NAND_DEVICE_ID_LEN           0x6

	#define NAND_BLK_CNT                 0x1000
	#define NAND_BLK_PAGE_CNT            0x80

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_12BIT // only 8-bits specified

	#define NAND_PAGE_BYTE_CNT           0x1000
	#define NAND_SPARE_BYTE_CNT          218
	
#elif defined(MT29F32G08CBACA) && MT29F32G08CBACA
    #define NAND_DEVICE_ID				 {0x2C, 0x68 ,0x04 ,0x4A ,0xA9}
    #define NAND_DEVICE_ID_LEN			 0x5
   
    #define NAND_BLK_CNT				 0x1000
    #define NAND_BLK_PAGE_CNT			 0x100
   
    #define NAND_RW_ADR_CYCLE_CNT		 ADR_C5TFS0
    #define NAND_E_ADR_CYCLE_CNT		 ADR_C3TRS0
    #define NAND_ECC_TYPE				 ECC_TYPE_24BIT1KB // only 8-bits specified
   
    #define NAND_PAGE_BYTE_CNT			 0x1000
    #define NAND_SPARE_BYTE_CNT 		 224

#elif defined(MT29F32G08CBADA) && MT29F32G08CBADA
    #define NAND_DEVICE_ID				 {0x2C, 0x44 ,0x44 ,0x4B ,0xA9}
    #define NAND_DEVICE_ID_LEN			 0x5
       
    #define NAND_BLK_CNT				 0x800//2128
    #define NAND_BLK_PAGE_CNT			 0x100
       
    #define NAND_RW_ADR_CYCLE_CNT		 ADR_C5TFS0
    #define NAND_E_ADR_CYCLE_CNT		 ADR_C3TRS0
    #define NAND_ECC_TYPE				 ECC_TYPE_40BIT1KB // only 8-bits specified
       
    #define NAND_PAGE_BYTE_CNT			 0x2000
    #define NAND_SPARE_BYTE_CNT 		 640

#else
	#error "No NAND device specified\n"
#endif
#endif // IF_IP_VERIFY
#endif /* __UNFD_DEVICE_H__ */
