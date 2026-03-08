
#ifndef _DRV_RTC_H
#define _DRV_RTC_H


#include "mdrv_types.h"

typedef struct {
	U8 u8PmRtcIndex;
	U32 u32RtcCtrlWord;
	U32 u32RtcSetMatchCounter;
	U32 u32RtcGetMatchCounter;
	U32 u32RtcSetCounter;
	U32 u32RtcGetCounter;

} PM_RtcParam;

typedef enum {
	E_RTC_0,
	E_RTC_2,
} E_MS_RTC;

void MDrv_RTC_Init(PM_RtcParam *pPmRtcParam);
void MDrv_RTC_SetCount(PM_RtcParam *pPmRtcParam);
U32 MDrv_RTC_GetCount(PM_RtcParam *pPmRtcParam);
void MDrv_RTC_SetMatchCount(PM_RtcParam *pPmRtcParam);
U32 MDrv_RTC_GetMatchCount(PM_RtcParam *pPmRtcParam);
int Request_RTC_IRQ(irq_handler_t handler, void *dev_id);
int Free_RTC_IRQ(void *dev_id);
void MDrv_RTC_ClearInterrupt_Status(E_MS_RTC eRtc);
#endif
