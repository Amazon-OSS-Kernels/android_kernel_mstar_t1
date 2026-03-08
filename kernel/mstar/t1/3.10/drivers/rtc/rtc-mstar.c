/*
 *  A mstar dummy rtc driver
 *  ToDo : Implement the real rtc...
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/time.h>
#include <linux/platform_device.h>
#include <linux/rtc.h>
#include <linux/delay.h>

#ifdef CONFIG_MSTAR_RTC
#include "chip_int.h"
#include "mdrv_types.h"
#include "mdrv_rtc.h"


#define MSTAR_RTC_FREQ 12000000ul
static unsigned int rtc_dev_id;
static struct rtc_device *mstar_rtc;
#endif

static int mstar_rtc_get_time(struct device *dev, struct rtc_time *tm)
{
	struct timeval time;
	PM_RtcParam pmRtcParam;

	pmRtcParam.u8PmRtcIndex = 1;
	pmRtcParam.u32RtcCtrlWord = MSTAR_RTC_FREQ;
	pmRtcParam.u32RtcSetMatchCounter = 0;
	pmRtcParam.u32RtcGetMatchCounter = 0;
	pmRtcParam.u32RtcSetCounter = 0;
	pmRtcParam.u32RtcGetCounter = 0;
	time.tv_sec = MDrv_RTC_GetCount((PM_RtcParam *)&pmRtcParam);

	/*  if the time before jan 1 2007, for it to jan 1 2007 to avoid android reboot. */
	if (time.tv_sec < 1167652800)
		time.tv_sec = 1167652800;

	rtc_time_to_tm(time.tv_sec, tm);

	pr_info("%s: %04d.%02d.%02d %02d:%02d:%02d\n",
	       __func__, 1900 + tm->tm_year, tm->tm_mon + 1, tm->tm_mday,
	       tm->tm_hour, tm->tm_min, tm->tm_sec);

	return rtc_valid_tm(tm);
}

static int mstar_rtc_set_time(struct device *dev, struct rtc_time *tm)
{
	unsigned long time;
	PM_RtcParam pmRtcParam;
	rtc_tm_to_time(tm, &time);

	pr_info("%s: %04d.%02d.%02d %02d:%02d:%02d\n",
	       __func__, 1900 + tm->tm_year, tm->tm_mon + 1, tm->tm_mday,
	       tm->tm_hour, tm->tm_min, tm->tm_sec);

	pmRtcParam.u8PmRtcIndex = 1;
	pmRtcParam.u32RtcCtrlWord = MSTAR_RTC_FREQ;
	pmRtcParam.u32RtcSetMatchCounter = 0;
	pmRtcParam.u32RtcGetMatchCounter = 0;
	pmRtcParam.u32RtcSetCounter = time;
	pmRtcParam.u32RtcGetCounter = 0;
	MDrv_RTC_SetCount((PM_RtcParam *)&pmRtcParam);

	return 0;
}

static int mstar_rtc_set_alarm(struct device *dev, struct rtc_wkalrm *alarm)
{
	struct rtc_time *tm = &alarm->time;
	struct timeval tv;
	unsigned long now_time, alarm_time;
	PM_RtcParam pmRtcParam;
	pr_info("%s: %04d.%02d.%02d %02d:%02d:%02d\n",
	       __func__,
	       1900 + tm->tm_year, tm->tm_mon + 1, tm->tm_mday,
	       tm->tm_hour, tm->tm_min, tm->tm_sec);
	rtc_tm_to_time(tm, &alarm_time);

	pmRtcParam.u8PmRtcIndex = 1;
	pmRtcParam.u32RtcCtrlWord = MSTAR_RTC_FREQ;
	pmRtcParam.u32RtcSetMatchCounter = 0;
	pmRtcParam.u32RtcGetMatchCounter = 0;
	pmRtcParam.u32RtcSetCounter = 0;
	pmRtcParam.u32RtcGetCounter = 0;
	tv.tv_sec = MDrv_RTC_GetCount((PM_RtcParam *)&pmRtcParam);
	if (tv.tv_sec < 1167652800)
		tv.tv_sec = 1167652800;

	now_time = tv.tv_sec;
	pr_info("%s: next_wakeup = %d\n",
	       __func__, (alarm_time - now_time));
	pmRtcParam.u8PmRtcIndex = 1;
	pmRtcParam.u32RtcCtrlWord = MSTAR_RTC_FREQ;
	pmRtcParam.u32RtcSetMatchCounter = alarm_time;
	pmRtcParam.u32RtcGetMatchCounter = 0;
	pmRtcParam.u32RtcSetCounter = now_time;
	pmRtcParam.u32RtcGetCounter = 0;
	MDrv_RTC_SetMatchCount((PM_RtcParam *)&pmRtcParam);
	MDrv_RTC_SetCount((PM_RtcParam *)&pmRtcParam);

	return 0;
}

static int mstar_rtc_get_alarm(struct device *dev, struct rtc_wkalrm *alarm)
{
	return 0;
}

static int mstar_rtc_proc(struct device *dev, struct seq_file *seq)
{
	struct platform_device *plat_dev = to_platform_device(dev);
	return 0;
}

static int mstar_rtc_alarm_irq_enable(struct device *dev, unsigned int enable)
{
	return 0;
}

int mstar_rtc_irq(irq_handler_t handler, void *dev_id)
{
	unsigned long    events = 0;
	events |= RTC_IRQF | RTC_AF;
	rtc_update_irq(mstar_rtc, 1, events);
}

static const struct rtc_class_ops mstar_rtc_ops = {
	.proc = mstar_rtc_proc,
	.read_time = mstar_rtc_get_time,
	.set_time = mstar_rtc_set_time,
	.read_alarm = mstar_rtc_get_alarm,
	.set_alarm = mstar_rtc_set_alarm,
	.alarm_irq_enable = mstar_rtc_alarm_irq_enable,
};

#if defined(CONFIG_ARM64)
static const struct of_device_id mstar_rtc_dt_match[] = {
	{ .compatible = "mstar-rtc", },
	{}
};
MODULE_DEVICE_TABLE(of, mstar_rtc_dt_match);
#endif

struct mstar_rtc {
	struct rtc_device *rtc;
	int rtc_alarm_irq;
	int rtc_base;
	int rtc_read_base;
	int rtc_write_base;
	int alarm_rw_base;
	u8  ctrl_reg;
	struct device *rtc_dev;
	spinlock_t ctrl_reg_lock;
};



static int __init mstar_rtc_probe(struct platform_device *dev)
{
	struct rtc_device *rtc;
#ifdef CONFIG_MSTAR_RTC
	PM_RtcParam pmRtcParam;

	if (!device_can_wakeup(&dev->dev))
		device_init_wakeup(&dev->dev, 1);

#endif
	rtc = rtc_device_register("rtc-mstar", &dev->dev, &mstar_rtc_ops, THIS_MODULE);

	if (IS_ERR(rtc))
		return PTR_ERR(rtc);

	platform_set_drvdata(dev, rtc);
#ifdef CONFIG_MSTAR_RTC
	mstar_rtc = rtc;
	pmRtcParam.u8PmRtcIndex = 1;
	pmRtcParam.u32RtcCtrlWord = MSTAR_RTC_FREQ;
	pmRtcParam.u32RtcSetMatchCounter = 0;
	pmRtcParam.u32RtcGetMatchCounter = 0;
	pmRtcParam.u32RtcSetCounter = 1167652800;
	pmRtcParam.u32RtcGetCounter = 0;
	MDrv_RTC_SetMatchCount((PM_RtcParam *)&pmRtcParam);
	Request_RTC_IRQ(mstar_rtc_irq, &rtc_dev_id);
	MDrv_RTC_Init((PM_RtcParam *)&pmRtcParam);
	pmRtcParam.u32RtcGetCounter = MDrv_RTC_GetCount((PM_RtcParam *)&pmRtcParam);
	if (pmRtcParam.u32RtcGetCounter < 1167652800)
		MDrv_RTC_SetCount((PM_RtcParam *)&pmRtcParam);
	pmRtcParam.u32RtcGetCounter = MDrv_RTC_GetCount((PM_RtcParam *)&pmRtcParam);
#endif
	return 0;
}

static int __exit mstar_rtc_remove(struct platform_device *dev)
{
	struct rtc_device *rtc = platform_get_drvdata(dev);
	rtc_device_unregister(rtc);
	return 0;
}

static int mstar_rtc_suspend(struct device *dev)
{
	if (device_may_wakeup(dev))
		enable_irq_wake(E_IRQ_PM_SLEEP);
	return 0;
}

static int mstar_rtc_resume(struct device *dev)
{
	if (device_may_wakeup(dev)) {
		PM_RtcParam pmRtcParam;
		disable_irq_wake(E_IRQ_PM_SLEEP);
		MDrv_RTC_GetCount((PM_RtcParam *)&pmRtcParam);
		MDrv_RTC_ClearInterrupt_Status(E_RTC_2);
	}
	return 0;
}

static SIMPLE_DEV_PM_OPS(mstar_rtc_pm_ops, mstar_rtc_suspend, mstar_rtc_resume);

static struct platform_driver mstar_rtc_driver = {
	.driver = {
		.name = "rtc-mstar",
		.owner = THIS_MODULE,
#if defined(CONFIG_ARM64)
		.of_match_table = mstar_rtc_dt_match,
#endif
		.pm	= &mstar_rtc_pm_ops,
	},
	.remove = __exit_p(mstar_rtc_remove),
};

static int __init mstar_rtc_init(void)
{
	int ret;
	ret = platform_driver_probe(&mstar_rtc_driver, mstar_rtc_probe);
	return ret;
}

static void __exit mstar_rtc_fini(void)
{
	platform_driver_unregister(&mstar_rtc_driver);
}

module_init(mstar_rtc_init);
module_exit(mstar_rtc_fini);

MODULE_AUTHOR("Mstarsemi");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("mstar RTC driver");
MODULE_ALIAS("platform:rtc-mstar");
