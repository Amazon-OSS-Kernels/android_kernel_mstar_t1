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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   drvDscmb.h
/// @brief  Descrambler (Dscmb) Driver Interface
/// @attention
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __REG_DSCMB_H__
#define __REG_DSCMB_H__

#define DSCMB_SHAREFLT_ENABLE         1  // share key support or not
#define DSCMB_PARAMETER_TABLE         1  // table to support n filter to 1 key slot
#define DSCMB_LAST_ENG_MULTI2_ONLY		1

#define ENG_NUM                 3UL
//
// source of cipher (CA_CTRL)
// * TSIF0_LIVE => 16~31 (Eng0)
// * TSIF0_FILE => 16~31 (Eng0)
// * TSIF1         => 32~47 (Eng1)
// * TSIF2         => 48~63 (Eng2)
//
#define GROUP0_MAP_PID_START      16
#define GROUP0_MAP_PID_LENGTH     16
#define GROUP1_MAP_PID_START      32
#define GROUP1_MAP_PID_LENGTH     16
#define GROUP2_MAP_PID_START      48
#define GROUP2_MAP_PID_LENGTH     16

#define MAX_NUM                   16UL
#define MAX_DSCMB_PIDFLT_NUM      128UL
#define DSCMB_SHARE_SLOT_REV_IDX  (MAX_NUM-1UL)  ///< the index of slot which is reserved for share key
#define REG_DSCMB_MAX_SLOT        (MAX_NUM*2UL)  ///< slot number x (odd/even)
#define REG_DSCMB_MAX_PIDFLT      128UL
#define MAX_KEY                   16UL

// Share key filter range definition
// CA0
#define DSCMB0_SHARE0_FLT_START_ID  0UL
#define DSCMB0_SHARE0_FLT_END_ID    127UL
#define DSCMB0_SHARE1_FLT_START_ID  0UL
#define DSCMB0_SHARE1_FLT_END_ID    127UL
#define DSCMB0_SHARE2_FLT_START_ID  0UL
#define DSCMB0_SHARE2_FLT_END_ID    127UL
// CA1
#define DSCMB1_SHARE0_FLT_START_ID  0UL
#define DSCMB1_SHARE0_FLT_END_ID    127UL
#define DSCMB1_SHARE1_FLT_START_ID  0UL
#define DSCMB1_SHARE1_FLT_END_ID    127UL
#define DSCMB1_SHARE2_FLT_START_ID  0UL
#define DSCMB1_SHARE2_FLT_END_ID    127UL
// CA2
#define DSCMB2_SHARE0_FLT_START_ID  0UL
#define DSCMB2_SHARE0_FLT_END_ID    127UL
#define DSCMB2_SHARE1_FLT_START_ID  0UL
#define DSCMB2_SHARE1_FLT_END_ID    127UL
#define DSCMB2_SHARE2_FLT_START_ID  0UL
#define DSCMB2_SHARE2_FLT_END_ID    127UL

/*
typedef enum
{
    HAL_DSCMB_KEY_TYPE_CLEAR = 0,
    HAL_DSCMB_KEY_TYPE_EVEN = 1,
    HAL_DSCMB_KEY_TYPE_ODD = 2,
} HAL_DscmbKeyType;
*/

// #define REG_DSCMB_KEY_TYPE_CLEAR        0UL
#define REG_DSCMB_KEY_TYPE_ODD          3UL
#define REG_DSCMB_KEY_TYPE_EVEN         2UL
#define REG_DSCMB_POS_SWITCH2           13UL

//-------------------------
// KTE Parameter Table define
//-------------------------
// [15:00]
#define REG_KTE_PARA_TABLE_KL_EN            0x0001UL
#define REG_KTE_PARA_TABLE_PARA_VLD         0x0004UL
#define REG_KTE_PARA_TABLE_LOCK             0x0008UL
#define REG_KTE_PARA_TABLE_KTE0             0x0010UL
#define REG_KTE_PARA_TABLE_KTE1             0x0020UL
#define REG_KTE_PARA_TABLE_KTE2             0x0040UL
#define REG_KTE_PARA_TABLE_KTE3             0x0080UL
#define REG_KTE_PARA_TABLE_DEC              0x0100UL
#define REG_KTE_PARA_TABLE_ENC              0x0200UL
// [31:16]
#define REG_KTE_PARA_TABLE_AES              0x0100UL
#define REG_KTE_PARA_TABLE_TDES             0x0200UL
#define REG_KTE_PARA_TABLE_DES              0x0400UL
#define REG_KTE_PARA_TABLE_CSAV2            0x2000UL
#define REG_KTE_PARA_TABLE_MULTI2           0x4000UL
// [47:32]
#define REG_KTE_PARA_TABLE_64BIT            0x0001UL
#define REG_KTE_PARA_TABLE_128BIT           0x0002UL
#define REG_KTE_PARA_TABLE_192BIT           0x0004UL
#define REG_KTE_PARA_TABLE_LSB64            0x0010UL
#define REG_KTE_PARA_TABLE_MULTI_ENG        0x0020UL
#define REG_KTE_PARA_TABLE_OUTALLOW_MASK    0x3000UL
// [63:48]
#define REG_KTE_PARA_TABLE_VERSION_NO       0xF000UL

////////////////////////////////////////////////////////////////////////////////////////////////
// Descambler bank 0
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_DSCMB_BANK                          0x0600UL

#define REG_DSCMB_CTRL                          (REG_DSCMB_BANK+ 0x0000UL) // ESA + CSA control
    #define REG_DSCMB_CTRL_CSA_ENABLE           0x0001UL
    #define REG_DSCMB_CTRL_KEEP_TSSCMB_CTRL     0x0004UL
    #define REG_DSCMB_CTRL_BYPASS_SCP           0x0008UL
    #define REG_DSCMB_CTRL_BYPASS_STRID         0x0010UL
    #define REG_DSCMB_CTRL_CONFORMANCE_MECH     0x0020UL
    #define REG_DSCMB_CTRL_CORRECT_SCRMBFLAG    0x0040UL
    #define REG_DSCMB_CTRL_OLD_TSC              0x0080UL
    #define REG_DSCMB_CTRL_KEEP_PES_SCM         0x0100UL
    #define REG_DSCMB_CTRL_USE_OLD_TSC_PES      0x0200UL
    #define REG_DSCMB_CTRL_CLR_OVF_CSA          0x0400UL
//    #define REG_DSCMB_SEL_2RD_GROUP_MASK        0x1800UL
//        #define REG_DSCMB_SEL_2RD_GROUP_00      0x0000UL
//        #define REG_DSCMB_SEL_2RD_GROUP_01      0x0800UL
//        #define REG_DSCMB_SEL_2RD_GROUP_10      0x1000UL
//        #define REG_DSCMB_SEL_2RD_GROUP_11      0x1800UL
    #define REG_DSCMB_CTRL_SW_RST               0x8000UL

#define REG_DSCMB_CTRL1                         (REG_DSCMB_BANK+ 0x0001UL)
    #define REG_DSCMB_CTRL1_NEW_TSC_MASK        0x0003UL
    #define REG_DSCMB_CTRL1_NEW_TSC_EVEN        0x0002UL
    #define REG_DSCMB_CTRL1_NEW_TSC_ODD         0x0003UL
    #define REG_DSCMB_CTRL1_HDCP2_ECO           0x0004UL // ECO bit for HDCP2 mode issue(Agate)

    #define REG_DSCMB_CTRL1_CIPHER0_SW_RST      0x0010UL
    #define REG_DSCMB_CTRL1_CIPHER1_SW_RST      0x0020UL
    #define REG_DSCMB_CTRL1_CIPHER2_SW_RST      0x0040UL

#define REG_DSCMB_SCMB_TS                       (REG_DSCMB_BANK+ 0x0002UL)
#define REG_DSCMB_SCMB_PES                      (REG_DSCMB_BANK+ 0x0004UL)
#define REG_DSCMB_SCMB_CSA                      (REG_DSCMB_BANK+ 0x0006UL)

#define REG_DSCMB_CIPHER_CONNECT_L              (REG_DSCMB_BANK + 0x000AUL)
    #define REG_DSCMB_CSA_CIP                   0x1045UL
    #define REG_DSCMB_CSA_CIP_ENCRYPT           0x1044UL // HDCP2
    #define REG_DSCMB_CIP_CSA                   0x0511UL
    #define REG_DSCMB_CSA                       0x0109UL
    #define REG_DSCMB_CIP                       0x0030UL

#define REG_DSCMB0_HDCP2_RIV0                   (REG_DSCMB_BANK + 0x0010UL)
#define REG_DSCMB0_HDCP2_RIV1                   (REG_DSCMB_BANK + 0x0011UL)
#define REG_DSCMB0_HDCP2_RIV2                   (REG_DSCMB_BANK + 0x0012UL)
#define REG_DSCMB0_HDCP2_RIV3                   (REG_DSCMB_BANK + 0x0013UL)
#define REG_DSCMB1_HDCP2_RIV0                   (REG_DSCMB_BANK + 0x0014UL)
#define REG_DSCMB1_HDCP2_RIV1                   (REG_DSCMB_BANK + 0x0015UL)
#define REG_DSCMB1_HDCP2_RIV2                   (REG_DSCMB_BANK + 0x0016UL)
#define REG_DSCMB1_HDCP2_RIV3                   (REG_DSCMB_BANK + 0x0017UL)

#define REG_DSCMB_ACPU_START                    (REG_DSCMB_BANK+ 0x0020UL)
    #define REG_ACPU_CMD_START                  0x0001UL
    #define REG_ACPU_CMD_ENG_SHIFT              0x8UL
    #define REG_ACPU_ENG0                       0x0UL
    #define REG_ACPU_ENG1                       0x1UL
    #define REG_ACPU_ENG2                       0x2UL

#define REG_DSCMB_ACPU_CMD2                     (REG_DSCMB_BANK+ 0x0021UL)

#define REG_DSCMB_ACPU_CMD                      (REG_DSCMB_BANK+ 0x0022UL)
    #define REG_ACPU_CMD_READ                   0x0000UL
    #define REG_ACPU_CMD_WRITE                  0x0001UL
    #define REG_ACPU_CMD_CLEAR                  0x0002UL
    #define REG_ACPU_CMD_POS2                   0x0008UL  // set 1 to r/w parameter table
    #define REG_ACPU_CMD_POS_SHFT               4UL
    #define REG_ACPU_CMD_POS_MASK               0x00F0UL
    #define REG_ACPU_PIDFLTID_SHFT              8UL
    #define REG_ACPU_PIDFLTID_MASK              0x0F00UL
    #define REG_ACPU_DSCMB_TYPE_SHFT            12UL
    #define REG_ACPU_DSCMB_TYPE_MASK            0x3000UL
        #define REG_ACPU_DSCMB_TYPE_CLEAR       HAL_DSCMB_KEY_TYPE_CLEAR
        #define REG_ACPU_DSCMB_TYPE_EVEN        HAL_DSCMB_KEY_TYPE_EVEN
        #define REG_ACPU_DSCMB_TYPE_ODD         HAL_DSCMB_KEY_TYPE_ODD
    #define REG_ACPU_DSCMB_VALID                14UL

#define REG_DSCMB_WRITE                         (REG_DSCMB_BANK+ 0x0024UL)
#define REG_DSCMB_READ                          (REG_DSCMB_BANK+ 0x0026UL)

#define REG_DSCMB_KT_CTRL_EXT                   (REG_DSCMB_BANK+ 0x002fUL)
    #define REG_DSCMB_ESA_OC_MODE               0x0001UL
    #define REG_DSCMB_ESA_CLR_START_MODE        0x0002UL
    #define REG_DSCMB_ESA_TSC_PUSI_MODE         0x0004UL
    #define REG_DSCMB_ESA_UPDATE_TSC_MASK       0x0008UL
    #define REG_DSCMB_ESA_UPDATE_TSC_BY_CSA     0x0000UL
    #define REG_DSCMB_ESA_UPDATE_TSC_BY_ESA     0x0008UL
    #define REG_DSCMB_ESA_HDCP_ERRATA_EN        0x0010UL

#define REG_DSCMB_KT_CTRL                       (REG_DSCMB_BANK+ 0x0030UL)
    #define REG_DSCMB_CIP_ESA_DES_MODE          0x0001UL
    #define REG_DSCMB_CIP_ESA_TDES_MODE         0x0002UL
    #define REG_DSCMB_CIP_ESA_AES_MODE          0x0008UL
    #define REG_DSCMB_CIP_ESA_ECB_MODE          0x0010UL
    #define REG_DSCMB_CIP_ESA_CBC_MODE          0x0020UL  //T8 new
    #define REG_DSCMB_CIP_ESA_CBC_CLR_MODE      0x0040UL
    #define REG_DSCMB_CIP_ESA_DECRYPT           0x0080UL
    #define REG_DSCMB_CIP_CLR_TSC_PKT_ERR       0x0100UL
    #define REG_DSCMB_CIP_CLR_OVF_ESA           0x0200UL
    #define REG_DSCMB_CIP_KTE_CEN_DISABLE       0x0400UL
    #define REG_DSCMB_CIP_ESA_PES_MODE          0x0800UL
    #define REG_DSCMB_CIP_ESA_MULTI2_MODE       0x1000UL
    #define REG_DSCMB_CIP_ESA_CTR_MODE          0x2000UL
    #define REG_DSCMB_CIP_ESA_CTR_CLR_MODE      0x4000UL
    #define REG_DSCMB_CIP_ESA_HDCP2_MODE        0x8000UL

    //
    //    EX: For CIP HD AES: 0xC8
    //    EX: For CIP SD DES: 0x91
    //    EX: MULTI2: 0x1020
    //    EX: HDCP2 : 0xA808
    //

/*
    #define REG_DSCMB_NSAD2KT_VALID             0x0001UL
    #define REG_DSCMB_ESA2KT_VALID              0x0002UL
    #define REG_DSCMB_NSAS2KT_VALID             0x0004UL
    #define REG_DSCMB_SWITCH2KT_VALID           0x0008UL
    #define REG_DSCMB_NSAD2KT_WEN               0x0010UL
    #define REG_DSCMB_ESA2KT_WEN                0x0020UL
    #define REG_DSCMB_NSAS2KT_WEN               0x0040UL
    #define REG_DSCMB_SWITCH2KT_WEN             0x0080UL
    #define REG_DSCMB_SWITCH2WNASK              0x0F00UL
*/
#define REG_DSCMB_MULTI2_ROUND                  (REG_DSCMB_BANK+ 0x0031UL)
#define REG_DSCMB_KTE_VALID_BANK                (REG_DSCMB_BANK+ 0x0032UL)
#define REG_DSCMB_IV_VALID_BANK                 (REG_DSCMB_BANK+ 0x0034UL)
//#define REG_DSCMB_CW_LEVEL0                     (REG_DSCMB_BANK+ 0x0034UL)
//#define REG_DSCMB_CW_LEVEL1                     (REG_DSCMB_BANK+ 0x003cUL)

//#define REG_DSCMB_PID_VALID0                    (REG_DSCMB_BANK+ 0x0040UL)
//#define REG_DSCMB_PID_VALID1                    (REG_DSCMB_BANK+ 0x0042UL)
//#define REG_DSCMB_PID_VALID2                    (REG_DSCMB_BANK+ 0x0043UL)

//#define REG_DSCMB_CW_LEVEL2                     (REG_DSCMB_BANK+ 0x0044UL)

#define REG_DSCMB_OUT0                          (REG_DSCMB_BANK+ 0x004EUL)
#define REG_DSCMB_ACPU2_ST                      (REG_DSCMB_BANK+ 0x0050UL)
#define REG_DSCMB_ACPU2_CMD2                    (REG_DSCMB_BANK+ 0x0051UL)
#define REG_DSCMB_ACPU2_CMD                     (REG_DSCMB_BANK+ 0x0052UL)
#define REG_DSCMB_ACPU2_WDATA                   (REG_DSCMB_BANK+ 0x0054UL)
#define REG_DSCMB_ACPU2_RDATA                   (REG_DSCMB_BANK+ 0x0056UL)

#define REG_DSCMB_KTESLOT_NONSEC_BANK           (REG_DSCMB_BANK+ 0x0058UL)
#define REG_DSCMB_KTESLOT_NONSEC_BANK1          (REG_DSCMB_BANK+ 0x005AUL)
#define REG_DSCMB_KTESLOT_NONSEC_BANK2          (REG_DSCMB_BANK+ 0x005CUL)

#define REG_DSCMB_ENG2_CTRL                     (REG_DSCMB_BANK+ 0x005FUL)
    #define REG_DSCMB_MULTI2_ROUNDS_MASK        0x00FFUL

#define REG_DSCMB_KTESLOT_SEC_BANK              (REG_DSCMB_BANK+ 0x0060UL)
#define REG_DSCMB_KTESLOT_SEC_BANK1             (REG_DSCMB_BANK+ 0x0062UL)
#define REG_DSCMB_KTESLOT_SEC_BANK2             (REG_DSCMB_BANK+ 0x0064UL)

#define REG_DSCMB_KTESLOT_LOCK_BANK             (REG_DSCMB_BANK+ 0x0070UL)
#define REG_DSCMB_KTESLOT_LOCK_BANK1            (REG_DSCMB_BANK+ 0x0072UL)
#define REG_DSCMB_KTESLOT_LOCK_BANK2            (REG_DSCMB_BANK+ 0x0074UL)

#define REG_DSCMB_BISS_DISWEN_BANK              (REG_DSCMB_BANK+ 0x0078UL)
#define REG_DSCMB_BISS_DISWEN_BANK1             (REG_DSCMB_BANK+ 0x0079UL)
#define REG_DSCMB_BISS_DISWEN_BANK2             (REG_DSCMB_BANK+ 0x007AUL)

#define REG_DSCMB_PARA_STATUS                   (REG_DSCMB_BANK+ 0x007CUL)
#define REG_DSCMB_PARA_STATUS1                  (REG_DSCMB_BANK+ 0x007DUL)
#define REG_DSCMB_PARA_STATUS2                  (REG_DSCMB_BANK+ 0x007EUL)

////////////////////////////////////////////////////////////////////////////////////////////////
// Descambler bank 2
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_DSCMB1_BANK                         0x9E00UL

#define REG_ESA1_CTRL                           (REG_DSCMB1_BANK + 0x0000UL)
    #define REG_ESA1_CTRL_KEEP_TS_SCMB          0x0001UL
    #define REG_ESA1_CTRL_CORRECT_SCRMBFLAG     0x0002UL
    #define REG_ESA1_CTRL_OLD_TSC               0x0004UL
    #define REG_ESA1_CTRL_OLD_TSC_PES           0x0008UL
    #define REG_ESA1_CTRL_NEW_TSC_MASK          0x0030UL
    #define REG_ESA1_CTRL_NEW_TSC_EVEN          0x0020UL
    #define REG_ESA1_CTRL_NEW_TSC_ODD           0x0030UL
    #define REG_ESA1_CTRL_BYPASS_SCP            0x0400UL
    #define REG_ESA1_CTRL_BYPASS_STRID          0x0800UL
    #define REG_ESA1_CTRL_KEEP_PES_SCMBCTRL     0x1000UL
//    #define REG_DSCMB1_SEL_2RD_GROUP_MASK        0x00C0UL
//        #define REG_DSCMB1_SEL_2RD_GROUP_00      0x0000UL
//        #define REG_DSCMB1_SEL_2RD_GROUP_01      0x0040UL
//        #define REG_DSCMB1_SEL_2RD_GROUP_10      0x0080UL
//        #define REG_DSCMB1_SEL_2RD_GROUP_11      0x00C0UL
    #define REG_ESA1_CTRL_SWITCH_KEY_VALID      0x2000UL
    #define REG_ESA1_CTRL_USE_CSA_PARSER        0x4000UL
    #define REG_ESA1_CTRL_USE_ESA_PARSER        0x8000UL


#define REG_DSCMB1_SCMB_TS                       (REG_DSCMB1_BANK + 0x0001UL)

#define REG_DSCMB1_KT_CTRL                       (REG_DSCMB1_BANK + 0x0003UL)
    #define REG_DSCMB1_CIP_ESA_DES_MODE          0x0001UL
    #define REG_DSCMB1_CIP_ESA_TDES_MODE         0x0002UL
    #define REG_DSCMB1_CIP_ESA_TDES_ABC_KEY      0x0004UL
    #define REG_DSCMB1_CIP_ESA_AES_MODE          0x0008UL
    #define REG_DSCMB1_CIP_ESA_ECB_MODE          0x0010UL
    #define REG_DSCMB1_CIP_ESA_CBC_MODE          0x0020UL
    #define REG_DSCMB1_CIP_ESA_CBC_CLR_MODE      0x0040UL
    #define REG_DSCMB1_CIP_ESA_DECRYPT           0x0080UL
    #define REG_DSCMB1_CIP_ESA_CLR_TSC_PKT_ERR   0x0100UL
    #define REG_DSCMB1_CIP_ESA_CLR_OVF_ESA       0x0200UL
    #define REG_DSCMB1_CIP_ESA_KTE_CEN_DISABLE   0x0400UL
    #define REG_DSCMB1_CIP_ESA_PES_MODE          0x0800UL
    #define REG_DSCMB1_CIP_ESA_MULTI2_MODE       0x1000UL
    #define REG_DSCMB1_CIP_ESA_CTR_MODE          0x2000UL
    #define REG_DSCMB1_CIP_ESA_CTR_CLR_MODE      0x4000UL
    #define REG_DSCMB1_CIP_ESA_HDCP2_MODE        0x8000UL
//
//    EX: For CIP HD AES: 0xC8
//    EX: For CIP SD DES: 0x91
//    EX: MULTI2: 0x1020
//    EX: HDCP2 : 0xA808
//

#define REG_DSCMB1_PID_VALID0                    (REG_DSCMB1_BANK + 0x0006UL)
#define REG_DSCMB1_MULTI2_ROUND                  (REG_DSCMB1_BANK + 0x0007UL)

#define REG_CSA1_CTRL                            (REG_DSCMB1_BANK + 0x0008UL)
    #define REG_DSCMB1_CTRL_CSA_MODE             0x0001UL
    #define REG_DSCMB1_CTRL_CONFORMANCE_MECH     0x0002UL
    #define REG_DSCMB1_CTRL_CLR_OVERFLOW_CSA     0x0004UL

#define REG_DSCMB1_PID_VALID1                    (REG_DSCMB1_BANK + 0x000AUL)
#define REG_DSCMB1_PID_VALID2                    (REG_DSCMB1_BANK + 0x000BUL)

#define REG_DSCMB2_CIPHERENG_CTRL                (REG_DSCMB1_BANK + 0x000CUL)
    #define REG_SRAM_SD_EN_KTE_ENG0              0x0001UL
    #define REG_SRAM_SD_EN_KTE_ENG1              0x0002UL
    #define REG_SRAM_SD_EN_KTE_ENG2              0x0001UL
    #define REG_BLK_AF_ENG0                      0x0100UL                    // DSCMB status don't care packet which full of AF data
    #define REG_BLK_AF_ENG1                      0x0200UL
    #define REG_BLK_AF_ENG2                      0x0400UL

#define REG_DSCMB1_KT_CTRL_EXT                   (REG_DSCMB1_BANK + 0x000dUL)
    #define REG_DSCMB1_ESA_OC_MODE               0x0001UL
    #define REG_DSCMB1_ESA_CLR_START_MODE        0x0002UL
    #define REG_DSCMB1_ESA_TSC_PUSI_MODE         0x0004UL
    #define REG_DSCMB1_ESA_UPDATE_TSC            0x0008UL
    #define REG_DSCMB1_ESA_HDCP_ERRATA_EN        0x0010UL

#define REG_ESA2_CTRL                            (REG_DSCMB1_BANK + 0x0010UL)
    #define REG_ESA2_CTRL_KEEP_TS_SCMB           0x0001UL
    #define REG_ESA2_CTRL_CORRECT_SCRMBFLAG      0x0002UL
    #define REG_ESA2_CTRL_OLD_TSC                0x0004UL
    #define REG_ESA2_CTRL_OLD_TSC_PES            0x0008UL
    #define REG_ESA2_CTRL_NEW_TSC_MASK           0x0030UL
    #define REG_ESA2_CTRL_NEW_TSC_EVEN           0x0020UL
    #define REG_ESA2_CTRL_NEW_TSC_ODD            0x0030UL
    //    #define REG_DSCMB1_SEL_2RD_GROUP_MASK        0x00C0UL
    //        #define REG_DSCMB1_SEL_2RD_GROUP_00      0x0000UL
    //        #define REG_DSCMB1_SEL_2RD_GROUP_01      0x0040UL
    //        #define REG_DSCMB1_SEL_2RD_GROUP_10      0x0080UL
    //        #define REG_DSCMB1_SEL_2RD_GROUP_11      0x00C0UL
    #define REG_ESA2_CTRL_ESA_IN_SRC2_MASK       0x0300UL
    #define REG_ESA2_CTRL_BYPASS_SCP             0x0400UL
    #define REG_ESA2_CTRL_BYPASS_STRID           0x0800UL
    #define REG_ESA2_CTRL_KEEP_PES_SCMB          0x1000UL
    #define REG_ESA2_CTRL_SWITCH_KEY_VALID       0x2000UL

#define REG_DSCMB2_SCMB_TS                       (REG_DSCMB1_BANK + 0x0011UL)

#define REG_DSCMB2_CW_STATUS2                    (REG_DSCMB1_BANK + 0x0012UL)

#define REG_DSCMB2_KT_CTRL                       (REG_DSCMB1_BANK + 0x0013UL)
    #define REG_DSCMB2_CIP_ESA_DES_MODE          0x0001UL
    #define REG_DSCMB2_CIP_ESA_TDES_MODE         0x0002UL
    #define REG_DSCMB2_CIP_ESA_AES_MODE          0x0008UL
    #define REG_DSCMB2_CIP_ESA_ECB_MODE          0x0010UL
    #define REG_DSCMB2_CIP_ESA_CBC_MODE          0x0020UL
    #define REG_DSCMB2_CIP_ESA_CBC_CLR_MODE      0x0040UL
    #define REG_DSCMB2_CIP_ESA_DECRYPT           0x0080UL
    #define REG_DSCMB2_CIP_ESA_CLR_TSC_PKT_ERR   0x0100UL
    #define REG_DSCMB2_CIP_ESA_CLR_OVF_ESA       0x0200UL
    #define REG_DSCMB2_CIP_ESA_KTE_CEN_DISABLE   0x0400UL
    #define REG_DSCMB2_CIP_ESA_PES_MODE          0x0800UL
    #define REG_DSCMB2_CIP_ESA_MULTI2_MODE       0x1000UL
    #define REG_DSCMB2_CIP_ESA_CTR_MODE          0x2000UL
    #define REG_DSCMB2_CIP_ESA_CTR_CLR_MODE      0x4000UL
    #define REG_DSCMB2_CIP_ESA_HDCP2_MODE        0x8000UL

#define REG_DSCMB2_KT_VALID_BANK                (REG_DSCMB1_BANK + 0x0014UL)
#define REG_DSCMB2_PID_VALID0                    (REG_DSCMB1_BANK+ 0x0016UL)
#define REG_DSCMB2_MULTI2_ROUND                 (REG_DSCMB1_BANK+ 0x0017UL)

#define REG_CSA2_CTRL                           (REG_DSCMB1_BANK+ 0x0018UL)
    #define REG_DSCMB2_CTRL_CSA_MODE             0x0001UL
    #define REG_DSCMB2_CTRL_CONFORMANCE_MECH     0x0002UL
    #define REG_DSCMB2_CTRL_CLR_OVERFLOW_CSA     0x0004UL

#define REG_DSCMB2_PID_VALID1                   (REG_DSCMB1_BANK+ 0x001AUL)
#define REG_DSCMB2_PID_VALID2                   (REG_DSCMB1_BANK+ 0x001BUL)

#define REG_DSCMB2_KT_CTRL_EXT                  (REG_DSCMB1_BANK+ 0x001DUL)
    #define REG_DSCMB2_ESA_OC_MODE               0x0001UL
    #define REG_DSCMB2_ESA_CLR_START__MODE       0x0002UL
    #define REG_DSCMB2_TSC_PUSI_MODE             0x0004UL
    #define REG_DSCMB2_ESA_UPDATE_TSC_CAS        0x0000UL
    #define REG_DSCMB2_ESA_UPDATE_TSC_ESA        0x0008UL
    #define REG_DSCMB2_ESA_HDCP_ERRATA_EN        0x0010UL

#define REG_DSCMB0_MULTI2_SYSKEY_L0             (REG_DSCMB1_BANK+ 0x0020UL) // systemkey 0
#define REG_DSCMB0_MULTI2_SYSKEY_H0             (REG_DSCMB1_BANK+ 0x0022UL)
#define REG_DSCMB0_MULTI2_SYSKEY_L1             (REG_DSCMB1_BANK+ 0x0024UL) // systemkey 1
#define REG_DSCMB0_MULTI2_SYSKEY_H1             (REG_DSCMB1_BANK+ 0x0026UL)
#define REG_DSCMB0_MULTI2_SYSKEY_L2             (REG_DSCMB1_BANK+ 0x0028UL) // systemkey 2
#define REG_DSCMB0_MULTI2_SYSKEY_H2             (REG_DSCMB1_BANK+ 0x002AUL)
#define REG_DSCMB0_MULTI2_SYSKEY_L3             (REG_DSCMB1_BANK+ 0x002CUL) // systemkey 3
#define REG_DSCMB0_MULTI2_SYSKEY_H3             (REG_DSCMB1_BANK+ 0x002EUL)

#define REG_DSCMB1_MULTI2_SYSKEY_L0             (REG_DSCMB1_BANK+ 0x0030UL) // systemkey 0
#define REG_DSCMB1_MULTI2_SYSKEY_H0             (REG_DSCMB1_BANK+ 0x0032UL)
#define REG_DSCMB1_MULTI2_SYSKEY_L1             (REG_DSCMB1_BANK+ 0x0034UL) // systemkey 1
#define REG_DSCMB1_MULTI2_SYSKEY_H1             (REG_DSCMB1_BANK+ 0x0036UL)
#define REG_DSCMB1_MULTI2_SYSKEY_L2             (REG_DSCMB1_BANK+ 0x0038UL) // systemkey 2
#define REG_DSCMB1_MULTI2_SYSKEY_H2             (REG_DSCMB1_BANK+ 0x003AUL)
#define REG_DSCMB1_MULTI2_SYSKEY_L3             (REG_DSCMB1_BANK+ 0x003CUL) // systemkey 3
#define REG_DSCMB1_MULTI2_SYSKEY_H3             (REG_DSCMB1_BANK+ 0x003EUL)

#define REG_DSCMB2_MULTI2_SYSKEY_L0             (REG_DSCMB1_BANK+ 0x0040UL) // systemkey 0
#define REG_DSCMB2_MULTI2_SYSKEY_H0             (REG_DSCMB1_BANK+ 0x0042UL)
#define REG_DSCMB2_MULTI2_SYSKEY_L1             (REG_DSCMB1_BANK+ 0x0044UL) // systemkey 1
#define REG_DSCMB2_MULTI2_SYSKEY_H1             (REG_DSCMB1_BANK+ 0x0046UL)
#define REG_DSCMB2_MULTI2_SYSKEY_L2             (REG_DSCMB1_BANK+ 0x0048UL) // systemkey 2
#define REG_DSCMB2_MULTI2_SYSKEY_H2             (REG_DSCMB1_BANK+ 0x004AUL)
#define REG_DSCMB2_MULTI2_SYSKEY_L3             (REG_DSCMB1_BANK+ 0x004CUL) // systemkey 3
#define REG_DSCMB2_MULTI2_SYSKEY_H3             (REG_DSCMB1_BANK+ 0x004EUL)

#define REG_DSCMB2_HDCP2_RIV0                   (REG_DSCMB1_BANK + 0x0050UL)
#define REG_DSCMB2_HDCP2_RIV1                   (REG_DSCMB1_BANK + 0x0051UL)
#define REG_DSCMB2_HDCP2_RIV2                   (REG_DSCMB1_BANK + 0x0052UL)
#define REG_DSCMB2_HDCP2_RIV3                   (REG_DSCMB1_BANK + 0x0053UL)

#define REG_DSCMB2_ARRAY_WDATA                  (REG_DSCMB1_BANK+ 0x0054UL)
#define REG_DSCMB2_ARRAY_RDATA                  (REG_DSCMB1_BANK+ 0x0055UL)
    #define REG_DSCMB2_ARRAY_ENG0_VLD           0x0100UL
    #define REG_DSCMB2_ARRAY_ENG1_VLD           0x0200UL
    #define REG_DSCMB2_ARRAY_ENG2_VLD           0x0400UL
    #define REG_DSCMB2_KEY_IDX_MASK             0x000FUL

#define REG_DSCMB2_ARRAY_ADR                    (REG_DSCMB1_BANK+ 0x0056UL) // equal to PID index
    #define REG_DSCMB2_ARRAY_ADR_MASK           0x00FFUL

#define REG_DSCMB2_ARRAY_RW_TRIG                (REG_DSCMB1_BANK+ 0x0057UL)
    #define REG_DSCMB2_TRIG_W                   0x0001UL
    #define REG_DSCMB2_TRIG_R                   0x0002UL

#define REG_DSCMB2_ARRAY_RDATA1                 (REG_DSCMB1_BANK+ 0x0058UL)
    #define REG_DSCMB2_ENG0_SHFT                0UL
    #define REG_DSCMB2_ENG1_SHFT                2UL
    #define REG_DSCMB2_ENG2_SHFT                4UL
    #define REG_DSCMB2_ENG_TS                   0x0001UL
    #define REG_DSCMB2_ENG_PES                  0x0002UL

#endif // #ifndef __REG_DSCMB_H__
