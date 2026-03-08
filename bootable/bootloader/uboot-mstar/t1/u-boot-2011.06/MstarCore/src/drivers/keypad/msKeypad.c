/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
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

#define _MSKEYPAD_C_

#include <common.h>
#include <MsTypes.h>
#include <ShareType.h>

#include <MsUboot.h>
#include "msKeypad.h"
#include "drvSAR.h"

#include <bootlogo/MsPoolDB.h>
#include <stdlib.h>
#include "keypad_type.h"

#include <MsDebug.h>

static SAR_KpdRegCfg_t m_KpdConfig_7key[MAXKEYPADCH] = {
#include "keypad.h"
};
static SAR_KpdRegCfg_t m_KpdConfig_14key[MAXKEYPADCH] = {
#include "keypad_14key.h"
};
static SAR_KpdRegCfg_t* m_KpdConfig = m_KpdConfig_7key;

static U8 Keypad_Channel[ADC_KEY_CHANNEL_NUM];

#ifndef ADC_KEY_LEVEL_TOLERANCE
#define ADC_KEY_LEVEL_TOLERANCE        0xFF
#endif

static BOOLEAN msKeypad_AllocChan(U8 u8KpdCh);
static U8 msKeypad_GetChanIndex(U8 u8KpdCh);
static BOOLEAN msKeypad_CH_GetKey(U8 Channel, U8 *pkey, U8* pflag);

static BOOLEAN msKeypad_AllocChan(U8 u8KpdCh)
{
    static U8 u8KpdChIdx=0;

    if(u8KpdChIdx>=ADC_KEY_CHANNEL_NUM)
    {
        printf("[msKeypad_Init]: Allocated channel number > %d !!!\n",ADC_KEY_CHANNEL_NUM);
        return MSRET_ERROR;
    }
    Keypad_Channel[u8KpdChIdx++]=u8KpdCh;
    return MSRET_OK;
}

static U8 msKeypad_GetChanIndex(U8 u8KpdCh)
{
    U8 k;

    for(k=0;k<ADC_KEY_CHANNEL_NUM;k++)
    {
        if(Keypad_Channel[k]==u8KpdCh)
            return Keypad_Channel[k];
    }
    return 0xFF;
}

static BOOLEAN msKeypad_CH_GetKey(U8 Channel, U8 *pkey, U8* pflag)
{
    U8 i, j, Key_Value;
    U8 u8ChIdx = msKeypad_GetChanIndex(Channel);

    if(u8ChIdx == 0xFF)
        return MSRET_ERROR;

    U8 u8KeyLevelNum = m_KpdConfig[u8ChIdx].u8KeyLevelNum;
    U8 KEY_LV[u8KeyLevelNum];
    memset(KEY_LV, 0x0, u8KeyLevelNum);

    *pkey = 0xFF;
    *pflag = 0;

    for ( i = 0; i < KEYPAD_STABLE_NUM; i++ )
    {
        Key_Value = MDrv_SAR_Adc_GetValue(m_KpdConfig[u8ChIdx].u8SARChID);
        for (j=0;j<u8KeyLevelNum;j++)
        {
            if (Key_Value < m_KpdConfig[u8ChIdx].u8KeyThreshold[j])
            {
                if((m_KpdConfig[u8ChIdx].u8KeyThreshold[j] - Key_Value) <= ADC_KEY_LEVEL_TOLERANCE)
        		{
	                KEY_LV[j]++;
	                break;
        		}
            }
        }
    }

    for(i=0; i<u8KeyLevelNum; i++)
    {
        if(KEY_LV[i] > KEYPAD_STABLE_NUM_MIN)
        {
            *pkey = m_KpdConfig[u8ChIdx].u8KeyCode[i];
            return MSRET_OK;
        }
    }
    return MSRET_ERROR;

}

void msKeypad_Init(void)
{
    if (is_14key_keypad())
    {
        m_KpdConfig = m_KpdConfig_14key;
    }

    MS_U8 u8ChEnb[MAXKEYPADCH]={m_KpdConfig[KEYPADCH1].bEnable,
                                m_KpdConfig[KEYPADCH2].bEnable,
                                m_KpdConfig[KEYPADCH3].bEnable,
                                m_KpdConfig[KEYPADCH4].bEnable};

    MS_U8 u8UpBnd[MAXKEYPADCH]={m_KpdConfig[KEYPADCH1].tSARChBnd.u8UpBnd,
                                m_KpdConfig[KEYPADCH2].tSARChBnd.u8UpBnd,
                                m_KpdConfig[KEYPADCH3].tSARChBnd.u8UpBnd,
                                m_KpdConfig[KEYPADCH4].tSARChBnd.u8UpBnd};

    MS_U8 u8LoBnd[MAXKEYPADCH]={m_KpdConfig[KEYPADCH1].tSARChBnd.u8LoBnd,
                                m_KpdConfig[KEYPADCH2].tSARChBnd.u8LoBnd,
                                m_KpdConfig[KEYPADCH3].tSARChBnd.u8LoBnd,
                                m_KpdConfig[KEYPADCH4].tSARChBnd.u8LoBnd};

    MS_U8 u8Idx;

	//for utopia driver for sar mmio base init
	MDrv_SAR_Kpd_MMIO_Init();

    memset(Keypad_Channel, 0xFF, ADC_KEY_CHANNEL_NUM);

    for(u8Idx=0; u8Idx < MAXKEYPADCH; u8Idx++)
    {
        if(u8ChEnb[u8Idx]==ENABLE)
        {
			MDrv_SAR_Kpd_CfgChannelBound(m_KpdConfig[u8Idx].u8SARChID, u8UpBnd[u8Idx], u8LoBnd[u8Idx]);
            msKeypad_AllocChan(u8Idx);
        }
    }

    MDrv_WriteByte(REG_SAR_CTRL0, SAR_MODE_FREERUN);
    MDrv_WriteByte(REG_SAR_CTRL0, MDrv_ReadByte(REG_SAR_CTRL0)&(~SAR_DIGITAL_PWRDN));
    MDrv_WriteByte(REG_SAR_CTRL1, MDrv_ReadByte(REG_SAR_CTRL1)&(~SAR_ADC_PWRDN));
    MDrv_WriteByte(REG_SAR_CTRL1, MDrv_ReadByte(REG_SAR_CTRL1)|(SAR_ADC_FREERUN));
    MDrv_WriteByte(REG_SAR_SELCH, MDrv_ReadByte(REG_SAR_SELCH)&(~SAR_NCH_EN));
    MDrv_WriteByte(REG_SAR_CKSAMP_PRD, 0x05);   //sample period
#if defined(CONFIG_MSTAR_BD_MST176A_D01A_S_NIKE)
    MDrv_WriteByte(REG_SAR_TEST0, MDrv_ReadByte(REG_SAR_TEST0)|(BIT6));   //Full scale=3.3V
#elif defined(CONFIG_MSTAR_BD_MST049B_10AQV_NIKE) || defined(CONFIG_MSTAR_BD_MST049B_10AQV_NIKE_U)
    MDrv_WriteByte(REG_SAR_TEST0, MDrv_ReadByte(REG_SAR_TEST0)|(BIT6));   //Full scale=3.3V
#else
    //do nothing
#endif

    for(u8Idx=0; u8Idx < MAXKEYPADCH; u8Idx++)
    {
        if(u8ChEnb[u8Idx]==ENABLE)
        {
            switch (m_KpdConfig[u8Idx].u8SARChID)
            {
                case 0:
                    //select pad as analog input
                    MDrv_WriteByte(REG_SAR_AISEL, MDrv_ReadByte(REG_SAR_AISEL)|(SAR_AISEL_CH0_MSK));
                    //select pad direction as input mode
                    MDrv_WriteByte(REG_SAR_GPIOOEN, MDrv_ReadByte(REG_SAR_GPIOOEN)|(SAR_GPIOOEN_CH0_MSK));
                    break;

                case 1:
                    //select pad as analog input
                    MDrv_WriteByte(REG_SAR_AISEL, MDrv_ReadByte(REG_SAR_AISEL)|(SAR_AISEL_CH1_MSK));
                    //select pad direction as input mode
                    MDrv_WriteByte(REG_SAR_GPIOOEN, MDrv_ReadByte(REG_SAR_GPIOOEN)|(SAR_GPIOOEN_CH1_MSK));
                    break;

                case 2:
                    //select pad as analog input
                    MDrv_WriteByte(REG_SAR_AISEL, MDrv_ReadByte(REG_SAR_AISEL)|(SAR_AISEL_CH2_MSK));
                    //select pad direction as input mode
                    MDrv_WriteByte(REG_SAR_GPIOOEN, MDrv_ReadByte(REG_SAR_GPIOOEN)|(SAR_GPIOOEN_CH2_MSK));
                    break;

                case 3:
                    //select pad as analog input
                    MDrv_WriteByte(REG_SAR_AISEL, MDrv_ReadByte(REG_SAR_AISEL)|(SAR_AISEL_CH3_MSK));
                    //select pad direction as input mode
                    MDrv_WriteByte(REG_SAR_GPIOOEN, MDrv_ReadByte(REG_SAR_GPIOOEN)|(SAR_GPIOOEN_CH3_MSK));
                    break;
            }
        }
    }

    //disable sar mask
    MDrv_WriteByte(REG_SAR_INT_MASK, MDrv_ReadByte(REG_SAR_INT_MASK)|(SAR_INT_MASK));
}

/******************************************************************************/
///Keypad get key value and repeat flag
///@param pkey \b IN return the key value(The same as Irda key value)
///@param pflag \b IN return the repeat flag(1:Repeat)
/******************************************************************************/
BOOLEAN msKeypad_GetKey(U8 *pkey, U8 *pflag)
{
    U8 Channel;

    for (Channel=0; Channel<MAXKEYPADCH; Channel++)
    {
        if (msKeypad_CH_GetKey(Channel, pkey, pflag))
        {
            return MSRET_OK;
        }
    }
    return MSRET_ERROR;
}

#undef _MSKEYPAD_C_

