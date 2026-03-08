
#ifndef _HAL_RTC_H_
#define _HAL_RTC_H_


#include "mdrv_types.h"
#include "mdrv_rtc.h"
#include "chip_int.h"



void MHAL_RTC_Reading(E_MS_RTC eRtc, bool bEnable); /*  Value to load into RTC counter
 */
void MHAL_RTC_Loading (E_MS_RTC eRtc, bool bEnable); /*  Load enable for loading value into RTC counter
 */
void MHAL_RTC_RESET(E_MS_RTC eRtc, bool bEnable); /*  RTC software reset (low active)
 */
void MHAL_RTC_Counter(E_MS_RTC eRtc, bool bEnable); /* Enable RTC Counter
 */

void MHAL_RTC_Init(E_MS_RTC eRtc, U32 u32Xtal);
void MHAL_RTC_SetCounter(E_MS_RTC eRtc, U32 u32RtcSetCounter);
U32 MHAL_RTC_GetCounter(E_MS_RTC eRtc);
void MHAL_RTC_SetMatchCounter(E_MS_RTC eRtc, U32 u32RtcSetCounter);
U32 MHAL_RTC_GetMatchCounter(E_MS_RTC eRtc);
U16 MHAL_RTC_GetInterrupt_Status(E_MS_RTC eRtc);
void MHAL_RTC_ClearInterrupt_Status(E_MS_RTC eRtc);
int MHal_PM_RTC_Interrupt_Init(void);
int MHal_RTC_Enable_Interrupt(irq_handler_t pCallback, void *dev_id);
int MHal_RTC_Disable_Interrupt(void);
#endif

