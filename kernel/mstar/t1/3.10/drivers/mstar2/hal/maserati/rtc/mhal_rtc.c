#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/kdev_t.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/poll.h>
#include <linux/wait.h>
#include <linux/cdev.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <asm/io.h>

#include "mhal_rtc.h"
#include "mhal_rtc_reg.h"
/* ------------------------------------------------------------------------------------------------- */
/*   Local Functions */
/* ------------------------------------------------------------------------------------------------- */
#define REG16(_u32RegBase)     (*((volatile U16*)(RIU_MAP+((_u32RegBase)<<1))))


static irqreturn_t _MHAL_RTCINT_INTHandler(int irq, void *dev_id);
irq_handler_t    pm_rtc_irq_pCallback;
spinlock_t   rtc_spinlock;
void *rtc_irq_dev_id;
static E_MS_RTC  g_rtc_num = E_RTC_0;

static void MHAL_RTC_Write2Byte(U32 u32RegBase, U16 data)
{
	REG16(u32RegBase) = data;
}

static U16 MHAL_RTC_Read2Byte(U32 u32RegBase)
{
	return REG16(u32RegBase);
}

static void MHAL_RTC_WriteBit(U32 u32RegBase, U16 u16Val, bool bEnable)
{
	if (bEnable)
		REG16(u32RegBase) |= u16Val;
	else
		REG16(u32RegBase) &= (~u16Val);
}

static void MHAL_RTC_Write4Byte(U32 _u32RegBase, U32 data)
{
	REG16(_u32RegBase) = data & 0xFFFFUL;
	REG16((_u32RegBase + 0x2UL)) = (data >> 16) & 0xFFFFUL;
}

static U32 MHAL_RTC_Read4Byte(U32 _u32RegBase)
{
	U32 data = 0;
	data = REG16(_u32RegBase + 0x2UL) ;
	data = data << 16 | REG16(_u32RegBase);
	return data;
}

static U32 MHAL_RTC_GET_BASE(E_MS_RTC eRtc)
{
	U32 u32RegAddr = 0;

	switch (eRtc) {
	case E_RTC_0:
		u32RegAddr = REG_RTC_BASE_0;
		break;

	case E_RTC_2:
		u32RegAddr = REG_RTC_BASE_2;
		break;
	}

	return u32RegAddr;
}
irqreturn_t _MHAL_RTCINT_INTHandler(int irq, void *dev_id)
{
	U8 i;
	U16 u16sts;
	unsigned long flags;
	irq_handler_t  pCB;
	u16sts = (MHAL_RTC_Read2Byte(REG_HST0_IRQ_STS_15_0))&PM_SLEEP_INT_STS;
	spin_lock_irqsave(&rtc_spinlock, flags);

	if (u16sts) {
		if (MHAL_RTC_GetInterrupt_Status(g_rtc_num)) {
			pCB = pm_rtc_irq_pCallback;

			if (pCB != 0)
				pCB(0, rtc_irq_dev_id);

			MHAL_RTC_ClearInterrupt_Status(g_rtc_num);
		}
	}

	spin_unlock_irqrestore(&rtc_spinlock, flags);
	return IRQ_HANDLED;
}


int MHal_PM_RTC_Interrupt_Init(void)
{
	pm_rtc_irq_pCallback = NULL;
	MHAL_RTC_WriteBit(REG_HST0_IRQ_MASK_15_0_, PM_SLEEP_INT_MASK, 1);
	MHAL_RTC_WriteBit(MHAL_RTC_GET_BASE(g_rtc_num) + REG_RTC_CTRL_REG, RTC_INT_MASK_BIT, 1);
	MHAL_RTC_WriteBit(REG_WK_IRQ_MASK , RTC_WK_SRC, 1);
	spin_lock_init(&rtc_spinlock);

	if (request_irq(E_IRQ_PM_SLEEP, (irq_handler_t)_MHAL_RTCINT_INTHandler, SA_INTERRUPT, "RTC_PM", NULL)) {
		pr_err("request_irq fail\n");
		return -EBUSY;
	}

	return 0;
}


int MHal_RTC_Enable_Interrupt(irq_handler_t pCallback, void *dev_id)
{
	int ret = 0;
	ret = MHal_PM_RTC_Interrupt_Init();
	pm_rtc_irq_pCallback = pCallback;
	rtc_irq_dev_id = dev_id;
	MHAL_RTC_ClearInterrupt_Status(g_rtc_num);
	MHAL_RTC_WriteBit(REG_HST0_IRQ_MASK_15_0_, PM_SLEEP_INT_MASK, 0);
	MHAL_RTC_WriteBit(REG_WK_IRQ_MASK , RTC_WK_SRC, 0);
	MHAL_RTC_WriteBit(MHAL_RTC_GET_BASE(g_rtc_num) + REG_RTC_CTRL_REG, RTC_INT_MASK_BIT, 0);
	return ret;
}

int MHal_RTC_Disable_Interrupt(void)
{
	int ret = 0;
	MHAL_RTC_WriteBit(REG_HST0_IRQ_MASK_15_0_, PM_SLEEP_INT_MASK, 1);
	MHAL_RTC_WriteBit(MHAL_RTC_GET_BASE(g_rtc_num) + REG_RTC_CTRL_REG, RTC_INT_MASK_BIT, 1);
	MHAL_RTC_WriteBit(REG_WK_IRQ_MASK , RTC_WK_SRC, 1);
	pm_rtc_irq_pCallback = NULL;
	rtc_irq_dev_id = NULL;
	return 0;
}


/* ------------------------------------------------------------------------------------------------- */
/*   Global Functions */
/* ------------------------------------------------------------------------------------------------- */

void MHAL_RTC_Reading(E_MS_RTC eRtc, bool bEnable)
{
	MHAL_RTC_WriteBit(MHAL_RTC_GET_BASE(eRtc) + REG_RTC_CTRL_REG, RTC_READ_EN_BIT, bEnable);
}

void MHAL_RTC_Loading (E_MS_RTC eRtc, bool bEnable)
{
	MHAL_RTC_WriteBit(MHAL_RTC_GET_BASE(eRtc) + REG_RTC_CTRL_REG, RTC_LOAD_EN_BIT, bEnable);
}

void MHAL_RTC_RESET(E_MS_RTC eRtc, bool bEnable)
{
	MHAL_RTC_WriteBit(MHAL_RTC_GET_BASE(eRtc) + REG_RTC_CTRL_REG, RTC_SOFT_RSTZ_BIT, bEnable);
}

void MHAL_RTC_Counter(E_MS_RTC eRtc, bool bEnable)
{
	MHAL_RTC_WriteBit(MHAL_RTC_GET_BASE(eRtc) + REG_RTC_CTRL_REG, RTC_CNT_EN_BIT, bEnable);
}

void MHAL_RTC_Init(E_MS_RTC eRtc, U32 u32Xtal)
{
	MHAL_RTC_WriteBit(REG_PM_CKG_RTC, BIT2, ENABLE); /* RTC clock switch to 12MHz */
	MHAL_RTC_RESET(eRtc, ENABLE);
	MHAL_RTC_Write4Byte(MHAL_RTC_GET_BASE(eRtc) + REG_RTC_FREQ_CW, u32Xtal);
	MHAL_RTC_Counter(eRtc, ENABLE);
}

void MHAL_RTC_SetCounter(E_MS_RTC eRtc, U32 u32RtcSetCounter)
{
	MHAL_RTC_Write4Byte(MHAL_RTC_GET_BASE(eRtc) + REG_RTC_LOAD_VAL , u32RtcSetCounter);
	MHAL_RTC_Write4Byte(MHAL_RTC_GET_BASE(eRtc) + REG_RTC_LOAD_VAL + 0x4, 0);
	MHAL_RTC_Loading(eRtc, ENABLE);
	MHAL_RTC_ClearInterrupt_Status(eRtc);
}

U32 MHAL_RTC_GetCounter(E_MS_RTC eRtc)
{
	U32 u32Reg;
	MHAL_RTC_Reading(eRtc, ENABLE);

	/* wait for HW latch bits okay, otherwise sometimes it read wrong value */
	udelay(500);

	u32Reg = MHAL_RTC_Read4Byte(MHAL_RTC_GET_BASE(eRtc) + REG_RTC_CNT);
	/* printk(KERN_EMERG "==RTC== %s, %d, %x\n" , __FUNCTION__, __LINE__, u32Reg); */
	return u32Reg;
}

void MHAL_RTC_SetMatchCounter(E_MS_RTC eRtc, U32 u32RtcSetCounter)
{
	g_rtc_num = eRtc;
	MHAL_RTC_Write4Byte(MHAL_RTC_GET_BASE(eRtc) + REG_RTC_MATCH_VAL , u32RtcSetCounter);
	MHAL_RTC_Write4Byte(MHAL_RTC_GET_BASE(eRtc) + REG_RTC_MATCH_VAL + 0x4, 0);
	MHAL_RTC_ClearInterrupt_Status(eRtc);
}

U32 MHAL_RTC_GetMatchCounter(E_MS_RTC eRtc)
{
	U32 u32Reg;
	u32Reg = MHAL_RTC_Read4Byte(MHAL_RTC_GET_BASE(eRtc) + REG_RTC_MATCH_VAL);
	/* printk(KERN_EMERG "==RTC== %s, %d, %x\n" , __FUNCTION__, __LINE__, u32Reg); */
	return u32Reg;
}

U16 MHAL_RTC_GetInterrupt_Status(E_MS_RTC eRtc)
{
	U16 u16Reg;
	u16Reg = MHAL_RTC_Read2Byte(MHAL_RTC_GET_BASE(eRtc) + REG_RTC_CTRL_REG);
	return u16Reg & RTC_INT_STATUS_BIT;
}

void MHAL_RTC_ClearInterrupt_Status(E_MS_RTC eRtc)
{
	MHAL_RTC_WriteBit(MHAL_RTC_GET_BASE(eRtc) + REG_RTC_CTRL_REG, RTC_INT_CLEAR_BIT, 1);
}
