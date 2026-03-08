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
#include <MsIRQ.h>
#include <MsOS.h>

#include <MsUboot.h>
#include "msKeypad.h"
#include "drvSAR.h"

#include <bootlogo/MsPoolDB.h>
#include <stdlib.h>
#include "keypad_type.h"

#include <MsDebug.h>
#ifdef CONFIG_MSTAR_MAXIM
static SAR_KpdRegCfg_t m_KpdConfig[MAXKEYPADCH] = {
#include "keypad.h"
};

#if KEYPAD_USE_ISR
static MS_S32 u32Mutex_Keypad = 0;
static MS_BOOL bBlockKeypadIsr = FALSE;
static U8 g_u8KeypadKey = 0xFF;
static U8 g_u8KeypadRpt = 0;
static BOOLEAN g_u8KeypadRes=MSRET_ERROR;
static void MDrv_Keypad_Isr(MHAL_SavedRegisters *pHalReg, U32 u32Data);
#endif
#else //MASERATI

static SAR_KpdRegCfg_t m_KpdConfig_7key[MAXKEYPADCH] = {
#include "keypad.h"
};
static SAR_KpdRegCfg_t m_KpdConfig_14key[MAXKEYPADCH] = {
#include "keypad_14key.h"
};
static SAR_KpdRegCfg_t* m_KpdConfig = m_KpdConfig_7key;
#endif



static U8 Keypad_Channel[ADC_KEY_CHANNEL_NUM];

#ifndef ADC_KEY_LEVEL_TOLERANCE
#define ADC_KEY_LEVEL_TOLERANCE        0xFF
#endif

static BOOLEAN msKeypad_AllocChan(U8 u8KpdCh);
static U8 msKeypad_GetChanIndex(U8 u8KpdCh);
static BOOLEAN msKeypad_CH_GetKey(U8 Channel, U8 *pkey, U8* pflag);
#if KEYPAD_USE_ISR

static U8 tADCKeyLevel[MAXKEYPADCH][8];
static U8 tADCChannelLevels[MAXKEYPADCH];
static U8 tADCKeyFlag[MAXKEYPADCH][8];

static void msKeypad_Config_Struct(void);
static void msKeypad_Config_Struct(void)
{
   memset (tADCKeyFlag, 0x00, sizeof(char)*32);
   int i = 0;
   int j = 0;

   for (i = 0; i < MAXKEYPADCH; i++)
   {
       for (j = 0; j < 8; j++)
       {

           tADCKeyFlag[i][j] = m_KpdConfig[i].u8KeyCode[j];
       }

       tADCChannelLevels[i] = m_KpdConfig[i].u8KeyLevelNum;
   }

   memset (tADCKeyLevel, 0x00, sizeof(char)*32);
   for (i = 0; i < MAXKEYPADCH; i++)
   {
       for (j = 0; j < 8; j++)
       {

           tADCKeyLevel[i][j] = m_KpdConfig[i].u8KeyThreshold[j];
       }
   }

}
#endif
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

#if KEYPAD_USE_ISR
/******************************************************************************/
///Keypad get ADC Channel value
///@param pkey \b IN ADC Channel
///@param pflag \b IN Pointer to the ADC Channel value
/******************************************************************************/
U8 msKeypad_Get_ADC_Channel(U8 Channel, U8 *pvalue)
{
    *pvalue=MDrv_SAR_Adc_GetValue(Channel);
     return 0;
}

static U8 msKeypad_GetChanLevels(U8 u8KpdCh);
static U8 msKeypad_GetChanLevels(U8 u8KpdCh)
{
    if(u8KpdCh>MAXKEYPADCH)
        return 0;
    return tADCChannelLevels[u8KpdCh];
}

extern void MApi_PNL_SetBackLight(MS_BOOL bEnable);
static BOOLEAN msKeypad_GetKeyCodeISR(U8 *pkey, U8 *pflag)
{
    U8 u8KeypadRes;
    U8 Channel;

    for (Channel=0; Channel<KEYPAD_CHANNEL_SUPPORT; Channel++)
    {
        u8KeypadRes = msKeypad_CH_GetKey(Channel, pkey, pflag);
        //printf("MDrv_Keypad_Isr>>> msKeypad_CH_GetKey[%d] = 0x%02X\n",Channel,*pkey);
        if (u8KeypadRes != MSRET_ERROR)
        {
            return u8KeypadRes;
        }
    }
    return MSRET_ERROR;
}

static void MDrv_Keypad_Isr(MHAL_SavedRegisters *pHalReg, U32 u32Data)
{
    MS_U8 u8KeypadKey=0xFF;
    MS_U8 u8KeypadRpt=0;
    BOOLEAN u8KeypadRes=MSRET_ERROR;

    UNUSED(pHalReg);
    UNUSED(u32Data);

    //printf("MDrv_Keypad_Isr>>> IN\n");
    //##########################
    //
    // Processing current interrupt
    // Get keypad key code + repeat status
    //
    if(bBlockKeypadIsr == FALSE)
    {
        u8KeypadRes = msKeypad_GetKeyCodeISR(&u8KeypadKey,&u8KeypadRpt);
        //msKeypad_Lock();    //No effect in non-os, replace by bBlockKeypadIsr flag
        //if(u8KeypadRes==MSRET_OK)
        if(u8KeypadRes!=MSRET_ERROR)
        {
            g_u8KeypadKey = (u8KeypadRes)? u8KeypadKey : 0xFF;
            g_u8KeypadRpt = (u8KeypadRes)? u8KeypadRpt : 0;
            g_u8KeypadRes = u8KeypadRes;
            if(g_u8KeypadKey==IRKEY_POWER)
            {
                printf("\n[Keypad] Key Code = 0x%02X\n",g_u8KeypadKey);
                MApi_PNL_SetBackLight(1);
                MDrv_PM_ActiveStandbyMode(FALSE);
                run_command ("setenv force_onetime_standby", 0);
                saveenv();
            }
        }
        else
        {
            g_u8KeypadRes = u8KeypadRes;
        }
        //msKeypad_UnLock();    //No effect in non-os, replace by bBlockKeypadIsr flag
    }
    //else
    //{
    //    printf("\n\rBlocked!");
    //}

    //##########################
    //
    // Processing next interrupt
    //
    if(u8KeypadRes!=MSRET_ERROR)
    {
        //enable interrupt after get key routine if  result<>MSRET_ERROR
        //Not to clear interrupt to let SAR generate continuous interrupts
        #if 0
        if(u8KeypadRes==MSRET_OK)
            printf("MDrv_Keypad_Isr>>> OK\n");
        else
            printf("MDrv_Keypad_Isr>>> OTHER\n");
        #endif
    }
    else
    {
        //printf("MDrv_Keypad_Isr>>> ERROR\n");
    }

    //enable interrupt
    if(g_u8KeypadKey!=IRKEY_POWER)
    {
       MsOS_EnableInterrupt(E_INT_IRQ_PMSLEEP);
    }
    //clear interrupt
    MDrv_WriteRegBit(REG_SAR_INT_CLR, ENABLE, SAR_INT_CLR_ALL);
    MDrv_WriteRegBit(REG_SAR_INT_CLR, DISABLE, SAR_INT_CLR_ALL);

}

static BOOLEAN msKeypad_CH_GetKey(U8 Channel, U8 *pkey, U8* pflag)
{
    U8 i, j, KEY_LV[MAXKEYPADLVLS], Key_Value, *Keymapping=NULL;
    U8 u8ChIdx=0;

    *pkey = 0xFF;
    *pflag = 0;

    udelay(100*1000); //IMPORTANT!!!

    for(i=0; i<MAXKEYPADLVLS; i++)
        KEY_LV[i] = 0;

    u8ChIdx = msKeypad_GetChanIndex(Channel);

    if(u8ChIdx == 0xFF)
        return MSRET_ERROR;

    Keymapping = (U8*)tADCKeyFlag[u8ChIdx];

    for ( i = 0; i < KEYPAD_STABLE_NUM; i++ )
    {
        msKeypad_Get_ADC_Channel(Channel,&Key_Value);
        for (j=0;j<msKeypad_GetChanLevels(u8ChIdx);j++)
        {
            if (Key_Value < tADCKeyLevel[u8ChIdx][j])
            {
                KEY_LV[j]++;
                break;
            }
        }
    }

    for(i=0; i<msKeypad_GetChanLevels(u8ChIdx); i++)
    {
        if(KEY_LV[i] >= KEYPAD_STABLE_NUM_MIN)
        {
            *pkey = *(Keymapping+i);
            return MSRET_OK;
        }
    }

    return MSRET_ERROR;

}

#else
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
#endif

void msKeypad_Init(void)
{
#ifdef CONFIG_MSTAR_MASERATI
    if (is_14key_keypad())
    {
        m_KpdConfig = m_KpdConfig_14key;
    }
#endif

#if KEYPAD_USE_ISR
    msKeypad_Config_Struct();
#endif

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

    #if KEYPAD_USE_ISR
    //irq : hook isr for pm_sleep
    MsOS_AttachInterrupt(E_INT_IRQ_PMSLEEP, (InterruptCb)MDrv_Keypad_Isr);
    //irq : enable interrupt
    MsOS_EnableInterrupt(E_INT_IRQ_PMSLEEP);
    //pm_sleep : un-mask sar wakeup event
    MDrv_WriteRegBit(REG_PM_WK_IRQ_MSK, DISABLE, PM_WK_IRQ_SAR);

    //sar : un-mask
    for(u8Idx=0; u8Idx < MAXKEYPADCH; u8Idx++)
    {
        if(u8ChEnb[u8Idx]==ENABLE)
        {
            switch (u8Idx)
            {
                case 0: MDrv_WriteRegBit(REG_SAR_INT_MASK, 0, BIT0); break;
                case 1: MDrv_WriteRegBit(REG_SAR_INT_MASK, 0, BIT1); break;
                case 2: MDrv_WriteRegBit(REG_SAR_INT_MASK, 0, BIT2); break;
                case 3: MDrv_WriteRegBit(REG_SAR_INT_MASK, 0, BIT3); break;
                default: break;
            }
        }
    }
    //sar : clear interrupt
    MDrv_WriteRegBit(REG_SAR_INT_CLR, ENABLE, SAR_INT_CLR_ALL);
    MDrv_WriteRegBit(REG_SAR_INT_CLR, DISABLE, SAR_INT_CLR_ALL);
    //sw pack for new irq structure //IMPORTANT!!!
    //MDrv_WriteRegBit(REG_SAR_SW_PACK, ENABLE, SAR_IRQ_CTRL);
    //create mutex
    if(u32Mutex_Keypad<=0)
    {
        //msKeypad_InitLock();
    }
    #else

    //disable sar mask
    MDrv_WriteByte(REG_SAR_INT_MASK, MDrv_ReadByte(REG_SAR_INT_MASK)|(SAR_INT_MASK));

    #endif
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

