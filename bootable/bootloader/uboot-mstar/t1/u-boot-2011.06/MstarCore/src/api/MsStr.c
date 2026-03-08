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

#include <ShareType.h>
#include <MsUtility.h>
#include <MsApiMiu.h>
#include <MsDebug.h>
#include <MsSysUtility.h>
#include <MsSystem.h>
#include <drvPM.h>
#include <common.h>
#include <CusPM.h>
#include <MsApiPM.h>
#include <MsMmap.h>
#include <MsEnvironment.h>
#include <drvBDMA.h>
#include <miu/MsDrvMiu.h>

#if defined(CONFIG_ARMV7_NONSEC) || defined(CONFIG_ARMV7_VIRT)
#include <asm/armv7.h>
#endif

#if CONFIG_MSTAR_STR_ENABLE
MS_BOOL Get_LX_MemCRC(int LxMemID, MS_U16 *LxMemCrc)
{
    U32 lx_mem_addr = 0;
    U32 lx_mem_size = 0;
    U32 CRCresult=0;
    if(LxMemID==E_LX) // LX1
    {
        Get_LX_MemInfo(LxMemID, &lx_mem_addr, &lx_mem_size);
        CRCresult=MDrv_BDMA_CRC32(lx_mem_addr+ 0x10000, lx_mem_size- 0x10000,0,0,E_BDMA_SRCDEV_MIU0,0);
    }
    else if(LxMemID==E_LX2 || LxMemID==E_LX3) // LX2 or LX3
    {
        Get_LX_MemInfo(LxMemID, &lx_mem_addr, &lx_mem_size);
        if(lx_mem_addr>=CONFIG_SYS_MIU2_INTERVAL)
        {
            UBOOT_DEBUG("LX%d At MIU 2 \n",LxMemID);
            #if (ENABLE_MSTAR_MONACO == 1)
            CRCresult=MDrv_BDMA_CRC32(lx_mem_addr, lx_mem_size,0,0,E_BDMA_SRCDEV_MIU2,0);
            #else
            UBOOT_DEBUG("Do not support MIU2\n");
            #endif
        }
        else if(lx_mem_addr>=CONFIG_SYS_MIU_INTERVAL)
        {
            UBOOT_DEBUG("LX%d At MIU 1 \n",LxMemID);
            CRCresult=MDrv_BDMA_CRC32(lx_mem_addr, lx_mem_size,0,0,E_BDMA_SRCDEV_MIU1,0);

        }
        else
        {
            UBOOT_DEBUG("LX%d At MIU 0 \n",LxMemID);
            CRCresult=MDrv_BDMA_CRC32(lx_mem_addr+ 0x10000, lx_mem_size- 0x10000,0,0,E_BDMA_SRCDEV_MIU0,0);
        }
    }
    else
    {
        UBOOT_ERROR("wrong LXMeMID:%d\n",LxMemID);
        return FALSE;
    }
    UBOOT_DEBUG("CRC result(32 bit): 0x%x\n",CRCresult);
    CRCresult = (CRCresult>>16) + CRCresult;
    *LxMemCrc = (CRCresult<<16)>>16 ;
    UBOOT_DEBUG("CRC result(16 bit): 0x%x\n",*LxMemCrc);
    return TRUE;
}

void CRCCheck(void)
{

#ifdef CONFIG_MSTAR_MONACO
    UBOOT_TRACE("IN\n");

    char *pEnv=NULL;
    unsigned char crclevel=0;
    MS_U16 uboot_crc=0;
    MS_U16 uboot_crc_sum=0;
    pEnv = getenv("str_crc");
    if(pEnv==NULL)
    {
        setenv("str_crc","3");
        saveenv();
    }
    crclevel=simple_strtol(pEnv, NULL, 16);
    if(pEnv==NULL||crclevel==3)
    {

        if(Get_LX_MemCRC(E_LX,&uboot_crc))
        {
            UBOOT_DEBUG("LX CRC Checking.....\n");
            uboot_crc_sum=uboot_crc;
        }
        if(Get_LX_MemCRC(E_LX2,&uboot_crc))
        {
            UBOOT_DEBUG("LX2 CRC Checking.....\n");
            uboot_crc_sum=uboot_crc_sum+uboot_crc;
        }
        if(Get_LX_MemCRC(E_LX3,&uboot_crc))
        {
            UBOOT_DEBUG("LX3 CRC Checking.....\n");
            uboot_crc_sum=uboot_crc_sum+uboot_crc;
        }
        if(MDrv_PM_STR_CRC_Check(uboot_crc_sum)==E_PM_FAIL)
        {
            UBOOT_ERROR("CRC UBOOT[0x%x]!!!!\n",uboot_crc_sum);
            jump_to_console();
        }
    }

    UBOOT_TRACE("OK\n");
#endif

}

#define MSTAR_SLEEP_MAGIC         0x4D535452

#if ENABLE_MSTAR_EAGLE
#define PMU_WAKEUP_ADDR_REGL     0x1F001CE0
#define PMU_WAKEUP_ADDR_REGH     0x1F001CE4
#elif ENABLE_MSTAR_KENYA
#define PMU_WAKEUP_ADDR_REGL     0xBF001CF0
#define PMU_WAKEUP_ADDR_REGH     0xBF001CF0
#elif ENABLE_MSTAR_KERES
#define PMU_WAKEUP_ADDR_REGL     0xBF001CF0
#define PMU_WAKEUP_ADDR_REGH     0xBF001CF0
#elif ENABLE_MSTAR_KIRIN
#define PMU_WAKEUP_ADDR_REGL     0xBF001CF0
#define PMU_WAKEUP_ADDR_REGH     0xBF001CF0
#elif ENABLE_MSTAR_KRIS
#define PMU_WAKEUP_ADDR_REGL     0xBF001CF0
#define PMU_WAKEUP_ADDR_REGH     0xBF001CF0
#elif ENABLE_MSTAR_KAYLA
#define PMU_WAKEUP_ADDR_REGL     0xBF001CF0
#define PMU_WAKEUP_ADDR_REGH     0xBF001CF0
#elif ENABLE_MSTAR_KIWI
#define PMU_WAKEUP_ADDR_REGL     0xBF001CF0
#define PMU_WAKEUP_ADDR_REGH     0xBF001CF0
#else
#define PMU_WAKEUP_ADDR_REGL     0x1F001D48
#define PMU_WAKEUP_ADDR_REGH     0x1F001D48
#endif
#define PMU_WAKEUP_ADDR_LMASK    0x000000FF
#define PMU_WAKEUP_ADDR_HMASK    0x0000FF00


#define WAKEUP_ADDR_MASK          0x0000FFF0
#define WAKEUP_FLAG_MASK          0x0000000F
#define WAKEUP_ADDR_SHIFT         16
#define WAKEUP_FLAG_INVALID       0
#define WAKEUP_FLAG_SLEPT         1
#define WAKEUP_FLAG_WKUP          2

#if CONFIG_MCU_MIPS32
#define MIU0_VIRTBASE1   0x80000000
#define MIU1_BASE_ADDR1  0x60000000
#define MIU1_BASE_ADDR2  0x70000000
#define MIU1_VIRTBASE1   0x00000000
#define MIU1_VIRTBASE2   0x20000000
#define _PHY_TO_VIRT(x) ((x<MIU1_BASE_ADDR1)?(x+MIU0_VIRTBASE1):    \
                          ((x<MIU1_BASE_ADDR2)?(x-MIU1_BASE_ADDR1+MIU1_VIRTBASE1):(x-MIU1_BASE_ADDR2+MIU1_VIRTBASE2)) )
#elif CONFIG_MCU_ARM
#define M512M (512*1024*1024)
#define _PHY_TO_VIRT(x) ( (((x)>=(CONFIG_MIU0_BUSADDR)&&(x)<(CONFIG_MIU0_BUSADDR+M512M)) \
                            ||((x)>=(CONFIG_MIU1_BUSADDR)&&(x)<(CONFIG_MIU1_BUSADDR+M512M)))? \
                            (x):(0) )
#else
#define _PHY_TO_VIRT(x) (x)
#endif
#define PHY_TO_VIRT(x) _PHY_TO_VIRT((unsigned long)(x))

unsigned long read_wkup_pmu(void)
{
    volatile unsigned long *preg=0;
    unsigned long val=0;

    preg=(volatile unsigned long *)PMU_WAKEUP_ADDR_REGL;
    val = ((*preg)&PMU_WAKEUP_ADDR_LMASK);
    preg=(volatile unsigned long *)PMU_WAKEUP_ADDR_REGH;
    val |= ((*preg)&PMU_WAKEUP_ADDR_HMASK);

    return val;
}
void write_wkup_pmu(unsigned long val)
{
    volatile unsigned long *preg=0;
    unsigned long oldval;

    preg=(volatile unsigned long *)PMU_WAKEUP_ADDR_REGL;
    oldval = ((*preg) & ~PMU_WAKEUP_ADDR_LMASK);
    oldval |= (val&PMU_WAKEUP_ADDR_LMASK);
    (*preg) = oldval;

    preg=(volatile unsigned long *)PMU_WAKEUP_ADDR_REGH;
    oldval = ((*preg) & ~PMU_WAKEUP_ADDR_HMASK);
    oldval |= (val&PMU_WAKEUP_ADDR_HMASK);
    (*preg) = oldval;

}

int _is_str_resume(void)
{
    unsigned long pmu_str_reg=0,*pwkupsave=0;
    pmu_str_reg=read_wkup_pmu();
    pwkupsave = (unsigned long*)((pmu_str_reg&WAKEUP_ADDR_MASK)<<WAKEUP_ADDR_SHIFT);
#if CONFIG_MCU_MIPS32
    pwkupsave = (unsigned long*)PHY_TO_VIRT(pwkupsave);
#endif
    if((pmu_str_reg & WAKEUP_FLAG_MASK) == WAKEUP_FLAG_SLEPT && pwkupsave
    	&& pwkupsave[0]==MSTAR_SLEEP_MAGIC && pwkupsave[1])
    {
		return 1;
    }
    return 0;
}

int check_str_resume(void)
{

    if(!(MDrv_PM_PowerOnMode() == E_PM_POWERON_ACON) &&
        (_is_str_resume()==1))
    {
        CRCCheck();
        return 1;
    }

    return 0;
}

#if defined(CONFIG_ARMV7_NONSEC) || defined(CONFIG_ARMV7_VIRT)
static void do_nonsec_virt_switch(void)
{
#if defined(CONFIG_ARMV7_NONSEC) || defined(CONFIG_ARMV7_VIRT)
	if (armv7_switch_nonsec() == 0)
#ifdef CONFIG_ARMV7_VIRT
	if (armv7_switch_hyp() == 0)
		debug("entered HYP mode\n");
#else
	debug("entered non-secure state\n");
#endif
#endif
}
#endif

int do_check_str_resume(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

    unsigned long pmu_str_reg=0,*pwkupsave=0;
    char* s = NULL;

    check_str_resume();

    pmu_str_reg=read_wkup_pmu();
    pwkupsave = (unsigned long*)((pmu_str_reg&WAKEUP_ADDR_MASK)<<16);

#if CONFIG_MCU_MIPS32
    pwkupsave = (unsigned long*)PHY_TO_VIRT(pwkupsave);
#endif
    if(_is_str_resume())
    {
        s = getenv("KERNEL_PROTECT");
        if (s != NULL)
        {
            MsApi_kernelProtect();
        }

        write_wkup_pmu((pmu_str_reg&~WAKEUP_FLAG_MASK)|WAKEUP_FLAG_WKUP);
        printf("[AT][MB][jump to kernel][%lu]\n", MsSystemGetBootTime());
#if (ENABLE_MSTAR_MASERATI == 1) || (ENABLE_MSTAR_KANO == 1) || (ENABLE_MSTAR_KASTOR == 1) || (ENABLE_MSTAR_MAXIM == 1) || (ENABLE_MSTAR_CURRY == 1)
        __asm__ volatile (
        	"ldr     r3, =0x16001080\n\t"
        	"ldr     r2, [r3]\n\t"
        	"ldr     r4, =(0x3 << 30)\n\t"
        	"orr     r2, r2, r4\n\t"
        	"str     r2, [r3]\n\t"
        	"ldr     r3, =0x16001000\n\t"
        	"mov     r2, #3\n\t"
        	"str     r2, [r3]\n\t"
        	"ldr     r3, =0x16002000\n\t"
        	"mov     r2, #3\n\t"
        	"str     r2, [r3]\n\t"
        	"ldr     r3, =0x16002004\n\t"
        	"mov     r2, #0xf8\n\t"
        	"str     r2, [r3]\n\t"
        );
#endif
#if CONFIG_MCU_MIPS32
        {
        	  void (*resume)(void);
        	  resume = (void *)PHY_TO_VIRT(pwkupsave[1]);
        	  if(resume)
        	      resume();
        }
#elif CONFIG_MCU_ARM

#ifdef CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE
	__asm__ __volatile__(
	"ldr  r0, =0x8400000F\n\t"
	"ldr  r1, =0x0\n\t"
	"ldr  r2, =0x0\n\t"
	"ldr  r3, =0x0\n\t"
	"ldr  r4, =0x0\n\t"
	"smc #0         \n\t"
	::
	: "r0","r3","r4","r2"
	);
#endif

#if CONFIG_ARMv8_64BIT_KERNEL

#ifdef CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE
	printf("\033[0;32;31m [Ian] %s %d ARMv8 Trustzone STR flow\033[m\n",__func__,__LINE__);
	cleanup_before_linux();
	__asm__ __volatile__(
	"ldr  r0, =0x8400000F\n\t"
	"ldr  r1, =0x0\n\t"
	"ldr  r2, =0x0\n\t"
	"ldr  r3, =0x0\n\t"
	"ldr  r4, =0x0\n\t"
	"smc #0         \n\t"
	::
	: "r0","r3","r4","r2"
	);
#else
	__asm__ __volatile__(
		"ldr r0, [%0]\n"
		"smc #0\n"
		::"r"(&pwkupsave[1]):"r0");
#endif //CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE

#else

#if defined(CONFIG_ARMV7_NONSEC) || defined(CONFIG_ARMV7_VIRT)
		cleanup_before_linux();
		do_nonsec_virt_switch();
#endif
        __asm__ volatile (
            "ldr r1, %0\n"
            "ldr r1, [r1, #4]\n"
            "cpsid if, #0x13\n"
            //; Disable MMU
            "MRC   p15, 0, r0, c1, c0, 0\n"
            "BIC   r0, r0, #0x1\n"
            "MCR   p15, 0, r0, c1, c0, 0\n"
            //; jump to wakeup address
            "mov pc, r1\n"
            ::"m"(pwkupsave):"r0","r1","memory","cc");
#endif
#endif
    }
    else
    {
        write_wkup_pmu(0);
    }
    return 0;
}





#endif
