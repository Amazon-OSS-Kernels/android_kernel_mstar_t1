#ifndef _MSTAR_STR_H_
#define _MSTAR_STR_H_

#if CONFIG_MSTAR_STR_ENABLE
extern int _is_str_resume(void);
#define is_str_resume()    _is_str_resume()
#else
#define is_str_resume()    (0)
#endif


#define PMU_WAKEUP_ADDR_REGL     0x1F001D48
#define PMU_WAKEUP_ADDR_REGH     0x1F001D48
#define PMU_WAKEUP_ADDR_LMASK    0x000000FF
#define PMU_WAKEUP_ADDR_HMASK    0x0000FF00

#define WAKEUP_ADDR_MASK          0x0000FFF0
#define WAKEUP_FLAG_MASK          0x0000000F
#define WAKEUP_ADDR_SHIFT         16
#define WAKEUP_FLAG_INVALID       0
#define WAKEUP_FLAG_SLEPT         1
#define WAKEUP_FLAG_WKUP          2

#define GPIO_NONE               0       // Not GPIO pin (default)
#define GPIO_IN                 1       // GPI
#define GPIO_OUT_LOW            2       // GPO output low
#define GPIO_OUT_HIGH           3       // GPO output high
#define PAD_PWM_PM_OEN (PAD_PWM_PM_IS_GPIO == GPIO_IN ? BIT0: 0)
#define PAD_PWM_PM_OUT (PAD_PWM_PM_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)

#define PM_SLEEP_38    0x0e38
#define PM_GPIO_28    0x0f28
#endif