#ifndef __ASM_IRQ_H
#define __ASM_IRQ_H

/* mach/irqs.h defines specific NR_IRQS for our mstar_chips @ hal layer.
 * For replacing the NR_IRQS defined @asm-generic/irq.h, we include mach/irqs.h here.
 */
#include <mach/irqs.h>
#include <asm-generic/irq.h>

extern void (*handle_arch_irq)(struct pt_regs *);
extern void migrate_irqs(void);
extern void set_handle_irq(void (*handle_irq)(struct pt_regs *));

#endif
