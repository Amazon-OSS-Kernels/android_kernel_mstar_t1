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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drv_system_io.c
/// @brief  System Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
//#include <linux/undefconf.h>  not used header file
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
#include <linux/time.h>  //added
#include <linux/timer.h> //added
#include <linux/device.h>
#include <linux/version.h>
#include <linux/compat.h>
#include <linux/seq_file.h>
#include <linux/dcache.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>

#include <asm/io.h>
#include <asm/types.h>
#if defined(CONFIG_MIPS)
#include <asm/mips-boards/prom.h>
#elif defined(CONFIG_ARM) || defined(CONFIG_ARM64)
#include <prom.h>
#endif

#include "mst_devid.h"
#include "mdrv_types.h"
#include "mst_platform.h"
#include "mdrv_system_io.h"
#include "mdrv_system_st.h"
#include "mdrv_system.h"
#include "mdrv_system_io_miu1.h"
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MOD_SYS_DEVICE_COUNT    1
#define MOD_SYS_NAME            "system"
#define SYS_WARNING(fmt, args...)       printk(KERN_WARNING "[SYSMOD][%06d] " fmt, __LINE__, ## args)
#if 0
#define SYS_PRINT(fmt, args...)         printk("[SYSMOD][%06d]     " fmt, __LINE__, ## args)
#else
#define SYS_PRINT(fmt, args...)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
typedef struct
{
    int                         s32SysMajor;
    int                         s32SysMinor;
    struct cdev                 cDevice;
    struct file_operations      SysFop;
} SysModHandle;


//-------------------------------------------------------------------------------------------------
//  Forward declaration
//-------------------------------------------------------------------------------------------------
MSYSTEM_STATIC int __init   _mod_sys_init(void);
MSYSTEM_STATIC void __exit  _mod_sys_exit(void);

static int _mod_sys_open (struct inode *inode, struct file *filp);
static int _mod_sys_release(struct inode *inode, struct file *filp);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
static long _mod_sys_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
#else
static int _mod_sys_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);
#endif

#if defined (CONFIG_COMPAT)
static long Compat_mod_sys_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
#endif

static ssize_t _mod_sys_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static SysModHandle SysDev=
{
    .s32SysMajor=               MDRV_MAJOR_SYSTEM,
    .s32SysMinor=               MDRV_MINOR_SYSTEM,
    .cDevice=
    {
        .kobj=                  {.name= MDRV_NAME_SYSTEM, },
        .owner  =               THIS_MODULE,
    },
    .SysFop=
    {
        .open=                  _mod_sys_open,
        .release=               _mod_sys_release,
        #if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
        .unlocked_ioctl=          _mod_sys_ioctl,
	#else
        .ioctl=                 _mod_sys_ioctl,
	#endif
        .read =                 _mod_sys_read,
	#if defined(CONFIG_COMPAT)
        .compat_ioctl= Compat_mod_sys_ioctl,
	#endif

    },
};

static struct class *system_class;

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
extern unsigned char datapool[];
extern int dataidx;
extern int uidx;
extern void enable_MUDI(void);
extern void disable_MUDI(void);
extern struct
{
    wait_queue_head_t wq;
    struct semaphore sem;
}MUDI_dev;


MSYSTEM_STATIC int __init _mod_sys_init(void)
{
    int         s32Ret;
    dev_t       dev;
#ifdef CONFIG_MSTAR_UDEV_NODE
    system_class = class_create(THIS_MODULE,MDRV_NAME_SYSTEM);
    if (IS_ERR(system_class))
    {
        return PTR_ERR(system_class);
    }
#endif
    SYS_PRINT("%s is invoked\n", __FUNCTION__);

    if (SysDev.s32SysMajor)
    {
        dev=                    MKDEV(SysDev.s32SysMajor, SysDev.s32SysMinor);
        s32Ret=                 register_chrdev_region(dev, MOD_SYS_DEVICE_COUNT, MDRV_NAME_SYSTEM);
    }
    else
    {
        s32Ret=                 alloc_chrdev_region(&dev, SysDev.s32SysMinor, MOD_SYS_DEVICE_COUNT, MDRV_NAME_SYSTEM);
        SysDev.s32SysMajor=     MAJOR(dev);
    }
    if (0> s32Ret)
    {
        SYS_WARNING("Unable to get major %d\n", SysDev.s32SysMajor);
        return s32Ret;
    }

    cdev_init(&SysDev.cDevice, &SysDev.SysFop);
    if (0!= (s32Ret= cdev_add(&SysDev.cDevice, dev, MOD_SYS_DEVICE_COUNT)))
    {
        SYS_WARNING("Unable add a character device\n");
        unregister_chrdev_region(dev, MOD_SYS_DEVICE_COUNT);
        return s32Ret;
    }
#ifdef CONFIG_MSTAR_UDEV_NODE
    device_create(system_class, NULL, dev, NULL,MDRV_NAME_SYSTEM);
#endif
    #if 0
    // @FIXME: for GE bug. Slow down MCU clock to 123MHz
    TOP_REG(REG_TOP_MCU) = (TOP_REG(REG_TOP_MCU) & ~(TOP_CKG_MCU_MASK)) | TOP_CKG_MCU_SRC_123;

    MDrv_MIU_Init();
    #endif

    return 0;
}

MSYSTEM_STATIC void __exit _mod_sys_exit(void)
{
    SYS_PRINT("%s is invoked\n", __FUNCTION__);

    cdev_del(&SysDev.cDevice);
    unregister_chrdev_region(MKDEV(SysDev.s32SysMajor, SysDev.s32SysMinor), MOD_SYS_DEVICE_COUNT);
}

static int _mod_sys_open (struct inode *inode, struct file *filp)
{
    SYS_PRINT("%s is invoked\n", __FUNCTION__);
    return 0;
}

static int _mod_sys_release(struct inode *inode, struct file *filp)
{
    SYS_PRINT("%s is invoked\n", __FUNCTION__);
    return 0;
}

extern void MDrv_SYS_StopUARTClock(void);
extern void MDrv_SYS_ResumeUARTClock(void);
extern void MDrv_SYS_HoldKernel(void);

#if defined (CONFIG_COMPAT)
static int compat_get_pcmcia_allocation_data(
			COMPAT_PCMCIA_Map_Info_t __user *data32,
			PCMCIA_Map_Info_t __user *data)
{
    int err;
    U8 u8;
    U16 u16;
    compat_uptr_t ptr;

	err = get_user(u16, &data32->u16Addr);
	err |= put_user(u16, &data->u16Addr);
	err |= get_user(u8, &data32->u8Value);
	err |= put_user(u8, &data->u8Value);
	err |= get_user(u8, &data32->u8Type);
	err |= put_user(u8, &data->u8Type);
	err |= get_user(u16, &data32->u16DataLen);
	err |= put_user(u16, &data->u16DataLen);
	err |= get_user(ptr, &data32->u8pReadBuffer);
	err |= put_user(compat_ptr(ptr), &data->u8pReadBuffer);
	err |= get_user(ptr, &data32->u8pWriteBuffer);
	err |= put_user(compat_ptr(ptr), &data->u8pWriteBuffer);

	return err;

}

static int compat_put_pcmcia_allocation_data(
			COMPAT_PCMCIA_Map_Info_t __user *data32,
			PCMCIA_Map_Info_t __user *data)
{
    int err;
    U8 u8;
    U16 u16;
    compat_uptr_t ptr;
	err = get_user(u16, &data->u16Addr);
	err |= put_user(u16, &data32->u16Addr);
	err |= get_user(u8, &data->u8Value);
	err |= put_user(u8, &data32->u8Value);
	err |= get_user(u8, &data->u8Type);
	err |= put_user(u8, &data32->u8Type);
	err |= get_user(u16, &data->u16DataLen);
	err |= put_user(u16, &data32->u16DataLen);
	err |= get_user(ptr, &data->u8pReadBuffer);
	err |= put_user(ptr, &data32->u8pReadBuffer);
	err |= get_user(ptr, &data->u8pWriteBuffer);
	err |= put_user(ptr, &data32->u8pWriteBuffer);

	return err;
}


static long Compat_mod_sys_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int err= 0;

    switch(cmd)
    {
    case IOCTL_SYS_PCMCIA_WRITE:
	case IOCTL_SYS_PCMCIA_READ:
	case IOCTL_SYS_PCMCIA_READ_DATA:
	{
		int err;
		int ret;
		COMPAT_PCMCIA_Map_Info_t __user *data32;
		PCMCIA_Map_Info_t __user *data;
		data32 = compat_ptr(arg);
		data = (PCMCIA_Map_Info_t *)compat_alloc_user_space(sizeof(PCMCIA_Map_Info_t));

		if (data == NULL)
			return -EFAULT;

		err = compat_get_pcmcia_allocation_data(data32, data);
		if (err)
			return err;

		ret = filp->f_op->unlocked_ioctl(filp, cmd, (unsigned long)data);

		err = compat_put_pcmcia_allocation_data(data32, data);
		return ret ? ret : err;
	}
    case IOCTL_SYS_INFO:
    case IOCTL_SYS_INFO_EX:
    case IOCTL_SYS_INIT:
    case IOCTL_SYS_SET_PANEL_INFO:
    case IOCTL_SYS_SET_BOARD_INFO:
    case IOCTL_SYS_GET_PANEL_RES:
    case IOCTL_SYS_READ_GEN_REGISTER:
    case IOCTL_SYS_WRITE_GEN_REGISTER:
    case IOCTL_SYS_LOAD_AEON:
    case IOCTL_SYS_RESET_AEON:
    case IOCTL_SYS_ENABLE_AEON:
    case IOCTL_SYS_DISABLE_AEON:
    case IOCTL_SYS_SWITCH_UART:
    case IOCTL_SYS_IS_AEON_ENABLE:
    case IOCTL_SYS_DUMP_AEON_MSG:
#ifdef IO_SYS_REG_OP
    case IOCTL_SYS_REG_OP:
#endif
#ifdef IO_SYS_GET_RAW_UART
    case IOCTL_SYS_GET_RAW_UART:
#endif
    case IOCTL_SYS_RELOAD_AEON:
    case IOCTL_SYS_TIMER:
    case IOCTL_SYS_HOTEL_MODE:
    case IOCTL_SYS_HOTEL_MODE_PRINTF:
    case IOCTL_SYS_CHANGE_UART:
    case IOCTL_SYS_POWER_DOWN:
    case IOCTL_SYS_SET_GFX_GOP_INDEX:
    case IOCTL_SYS_GET_GFX_GOP_INDEX:
    case IOCTL_SYS_SET_DISPLAY_CTLR_SEPT_INDEX:
    case IOCTL_SYS_IS_DISPLAY_CTLR_SEPT_INDEX:
    case IOCTL_SYS_SET_NEXUS:
    case IOCTL_SYS_HAS_NEXUS:
    case IOCTL_SYS_PRINT_MSG:
    case IOCTL_SYS_GET_GFX_GOP_PIPELINE_DELAY:
    case IOCTL_SYS_GET_PANEL_H_START:
    case IOCTL_SYS_SET_NEXUS_PID:
    case IOCTL_SYS_GET_NEXUS_PID:
    case IOCTL_SYS_GET_MBOX_SHM:
    case IOCTL_SYS_GET_MSBIN_INFO:
    case IOCTL_SYS_GET_MIU1_BUS_BASE:
    case IOCTL_SYS_GET_MIU1_BASE:
    case IOCTL_SYS_FLUSH_MEMORY:
    case IOCTL_SYS_READ_MEMORY:
    case IOCTL_SYS_HOLD_KERNEL:
    case IOCTL_SYS_STOP_UART_CLK:
    case IOCTL_SYS_RESUME_UART_CLK:
    case IOCTL_SYS_ENABLE_MUDI:
    case IOCTL_SYS_DISABLE_MUDI:
    case IOCTL_SYS_SPI_LOAD: //20100120 Terry, SPI Load Code
    case IOCTL_SYS_FORCE_OAD_UPGRADE_FROM_DRAM:
    case IOCTL_SYS_FORCE_ENV_UPGRADE_FROM_DRAM:
	{
		return filp->f_op->unlocked_ioctl(filp, cmd,(unsigned long)compat_ptr(arg));
	}

    default:
        SYS_WARNING("Unknown ioctl command %d\n", cmd);
        return -ENOTTY;
    }

    return err;
}
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
static long _mod_sys_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
#else
static int  _mod_sys_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
#endif
{
    int err= 0;

    /* extract the type and number bitfields, and don��t decode */
    // wrong cmds: return ENOTTY (inappropriate ioctl) before access_ok()
    if (_IOC_TYPE(cmd) != SYS_IOCTL_MAGIC) return -ENOTTY;
    if (_IOC_NR(cmd) > IOCTL_SYS_MAXNR) return -ENOTTY;

    // the direction is a bitmask, and VERIFY_WRITE catches R/W
    /* transfers. ��Type�� is user oriented, while */
    // access_ok is kernel oriented, so the concept of "read" and
    // "write" is reversed
    if (_IOC_DIR(cmd) & _IOC_READ)
        err = !access_ok(VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd));
    else if (_IOC_DIR(cmd) & _IOC_WRITE)
        err = !access_ok(VERIFY_READ, (void *)arg, _IOC_SIZE(cmd));
    if (err) return -EFAULT;

    switch(cmd)
    {
    case IOCTL_SYS_INFO:
        {
            IO_Sys_Info_t j;
#if defined(CONFIG_MIPS)
            get_boot_mem_info(LINUX_MEM, &(j.LX_MEM_ADDR), &(j.LX_MEM_LENGTH));
            get_boot_mem_info(LINUX_MEM2, &(j.LX_MEM2_ADDR), &(j.LX_MEM2_LENGTH));
            get_boot_mem_info(EMAC_MEM, &(j.EMAC_ADDR), &(j.EMAC_LENGTH));
            get_boot_mem_info(DRAM, &(j.DRAM_ADDR), &(j.DRAM_LENGTH));
            get_boot_mem_info(BB, &(j.BB_ADDR), &(j.BB_LENGTH));
            get_boot_mem_info(MPOOL_MEM, &(j.MPOOL_MEM_ADDR), &(j.MPOOL_MEM_LENGTH));
            get_boot_mem_info(G3D_MEM0, &(j.G3D_MEM0_ADDR), &(j.G3D_MEM0_LENGTH));
            get_boot_mem_info(G3D_MEM1, &(j.G3D_MEM1_ADDR), &(j.G3D_MEM1_LENGTH));
            get_boot_mem_info(G3D_CMDQ, &(j.G3D_CMDQ_ADDR), &(j.G3D_CMDQ_LENGTH));

			if (j.LX_MEM2_ADDR >= MIPS_MIU1_BUS_BASE)
                j.LX_MEM2_ADDR = j.LX_MEM2_ADDR - MIPS_MIU1_BUS_BASE + MIU1_OFFSET;

			if(copy_to_user( (void *)arg, &j, sizeof(j) ))
            {
                printk( "copy_to_user error\n" ) ;
            }
#elif defined(CONFIG_ARM) || defined(CONFIG_ARM64)
            get_boot_mem_info(LINUX_MEM, &(j.LX_MEM_ADDR), &(j.LX_MEM_LENGTH));
            get_boot_mem_info(LINUX_MEM2, &(j.LX_MEM2_ADDR), &(j.LX_MEM2_LENGTH));
            get_boot_mem_info(EMAC_MEM, &(j.EMAC_ADDR), &(j.EMAC_LENGTH));
            get_boot_mem_info(DRAM, &(j.DRAM_ADDR), &(j.DRAM_LENGTH));
            get_boot_mem_info(BB, &(j.BB_ADDR), &(j.BB_LENGTH));
            get_boot_mem_info(MPOOL_MEM, &(j.MPOOL_MEM_ADDR), &(j.MPOOL_MEM_LENGTH));
            get_boot_mem_info(G3D_MEM0, &(j.G3D_MEM0_ADDR), &(j.G3D_MEM0_LENGTH));
            get_boot_mem_info(G3D_MEM1, &(j.G3D_MEM1_ADDR), &(j.G3D_MEM1_LENGTH));
            get_boot_mem_info(G3D_CMDQ, &(j.G3D_CMDQ_ADDR), &(j.G3D_CMDQ_LENGTH));

			if (j.LX_MEM2_ADDR >= ARM_MIU1_BUS_BASE)
                j.LX_MEM2_ADDR = j.LX_MEM2_ADDR - ARM_MIU1_BUS_BASE + MIU1_OFFSET;

			if(copy_to_user( (void *)arg, &j, sizeof(j) ))
            {
                printk( "copy_to_user error\n" ) ;
            }
#endif
        }
        break;
    case IOCTL_SYS_INFO_EX:
        {
            IO_Sys_Info_t_EX j;
#if defined(CONFIG_MIPS)
            get_boot_mem_info(LINUX_MEM, &(j.LX_MEM_ADDR), &(j.LX_MEM_LENGTH));
            get_boot_mem_info(LINUX_MEM2, &(j.LX_MEM2_ADDR), &(j.LX_MEM2_LENGTH));
            get_boot_mem_info(LINUX_MEM3, &(j.LX_MEM3_ADDR), &(j.LX_MEM3_LENGTH));
            get_boot_mem_info(EMAC_MEM, &(j.EMAC_ADDR), &(j.EMAC_LENGTH));
            get_boot_mem_info(DRAM, &(j.DRAM_ADDR), &(j.DRAM_LENGTH));
            get_boot_mem_info(BB, &(j.BB_ADDR), &(j.BB_LENGTH));
            get_boot_mem_info(MPOOL_MEM, &(j.MPOOL_MEM_ADDR), &(j.MPOOL_MEM_LENGTH));
            get_boot_mem_info(G3D_MEM0, &(j.G3D_MEM0_ADDR), &(j.G3D_MEM0_LENGTH));
            get_boot_mem_info(G3D_MEM1, &(j.G3D_MEM1_ADDR), &(j.G3D_MEM1_LENGTH));
            get_boot_mem_info(G3D_CMDQ, &(j.G3D_CMDQ_ADDR), &(j.G3D_CMDQ_LENGTH));

		   	/*
			if (j.LX_MEM2_ADDR >= MIPS_MIU1_BUS_BASE)
                j.LX_MEM2_ADDR = j.LX_MEM2_ADDR - MIPS_MIU1_BUS_BASE + MIU1_OFFSET;
			*/

			if(copy_to_user( (void *)arg, &j, sizeof(j) ))
            {
                printk( "copy_to_user error\n" ) ;
            }
#elif defined(CONFIG_ARM) || defined(CONFIG_ARM64)
            get_boot_mem_info(LINUX_MEM, &(j.LX_MEM_ADDR), &(j.LX_MEM_LENGTH));
            get_boot_mem_info(LINUX_MEM2, &(j.LX_MEM2_ADDR), &(j.LX_MEM2_LENGTH));
            get_boot_mem_info(LINUX_MEM3, &(j.LX_MEM3_ADDR), &(j.LX_MEM3_LENGTH));
            get_boot_mem_info(EMAC_MEM, &(j.EMAC_ADDR), &(j.EMAC_LENGTH));
            get_boot_mem_info(DRAM, &(j.DRAM_ADDR), &(j.DRAM_LENGTH));
            get_boot_mem_info(BB, &(j.BB_ADDR), &(j.BB_LENGTH));
            get_boot_mem_info(MPOOL_MEM, &(j.MPOOL_MEM_ADDR), &(j.MPOOL_MEM_LENGTH));
            get_boot_mem_info(G3D_MEM0, &(j.G3D_MEM0_ADDR), &(j.G3D_MEM0_LENGTH));
            get_boot_mem_info(G3D_MEM1, &(j.G3D_MEM1_ADDR), &(j.G3D_MEM1_LENGTH));
            get_boot_mem_info(G3D_CMDQ, &(j.G3D_CMDQ_ADDR), &(j.G3D_CMDQ_LENGTH));

 			/*
			if (j.LX_MEM2_ADDR >= ARM_MIU1_BUS_BASE)
                j.LX_MEM2_ADDR = j.LX_MEM2_ADDR - ARM_MIU1_BUS_BASE + MIU1_OFFSET;
			*/

			if(copy_to_user( (void *)arg, &j, sizeof(j) ))
            {
                printk( "copy_to_user error\n" ) ;
            }
#endif
        }
        break;
    case IOCTL_SYS_INIT:
       // {
          //  printk("********Into Kernel*********\n");
           // IO_Sys_Info_t j;
           // get_boot_mem_info(LINUX_MEM,&(j.LX_MEM_ADDR),&(j.LX_MEM_LENGTH));
           // get_boot_mem_info(LINUX_MEM2,&(j.LX_MEM2_ADDR),&(j.LX_MEM2_LENGTH));
           // get_boot_mem_info(EMAC_MEM,&(j.EMAC_ADDR),&(j.EMAC_LENGTH));
           // get_boot_mem_info(EMAC_MEM,&(j.DRAM_ADDR),&(j.DRAM_LENGTH));
           // copy_to_user( (void *)arg, &j, sizeof(j) );
       // }
        break;
    case IOCTL_SYS_SET_PANEL_INFO:
        err = MDrv_SYS_SetPanelInfo(arg);
        break;

    case IOCTL_SYS_SET_BOARD_INFO:

        break;

    case IOCTL_SYS_GET_PANEL_RES:
        MDrv_SYS_GetPanelRes(arg);
        break;

    case IOCTL_SYS_READ_GEN_REGISTER:
	SYS_PRINT("ioctl command IOCTL_SYS_READ_GEN_REGISTER is not supported.\n");
	err = -ENOTTY;
        break;

    case IOCTL_SYS_WRITE_GEN_REGISTER:
	SYS_PRINT("ioctl command IOCTL_SYS_WRITE_GEN_REGISTER is not supported.\n");
	err = -ENOTTY;
        break;
//#if defined(CONFIG_Triton)
    case IOCTL_SYS_LOAD_AEON:
	SYS_PRINT("ioctl command IOCTL_SYS_LOAD_AEON is not supported.\n");
	err = -ENOTTY;
        break;

    case IOCTL_SYS_RESET_AEON:
        MDrv_SYS_ResetAeon(arg);
        break;

    case IOCTL_SYS_ENABLE_AEON:
        MDrv_SYS_EnableAeon();
        break;

    case IOCTL_SYS_DISABLE_AEON:
        MDrv_SYS_DisableAeon();
        break;

    case IOCTL_SYS_SWITCH_UART:
        MDrv_SYS_SwitchUart(arg);
        break;

    case IOCTL_SYS_IS_AEON_ENABLE:
        err = MDrv_SYS_IsAeonEnable(arg);
        break;
//#endif

    case IOCTL_SYS_DUMP_AEON_MSG:
        MDrv_SYS_DumpAeonMessage();
        break;

#ifdef IO_SYS_REG_OP
    case IOCTL_SYS_REG_OP:
	SYS_PRINT("ioctl command IOCTL_SYS_REG_OP is not supported.\n");
	err = -ENOTTY;
        break ;
#endif

#ifdef IO_SYS_GET_RAW_UART
    case IOCTL_SYS_GET_RAW_UART:
        err = MDrv_SYS_GetRawUART(arg);
        break ;
#endif

    case IOCTL_SYS_RELOAD_AEON:
        MDrv_SYS_ReloadAeon(arg) ;
        break ;

    case IOCTL_SYS_TIMER:
        err = MDrv_SYS_Timer(arg);
        break ;

    case IOCTL_SYS_HOTEL_MODE:
        err = MDrv_SYS_HotelMode(arg);
        break ;

    case IOCTL_SYS_HOTEL_MODE_PRINTF:
        err = MDrv_SYS_HotelModePrintf(arg);
        break ;

    case IOCTL_SYS_CHANGE_UART:
        MDrv_SYS_ChangeUart( arg );
        break;

    case IOCTL_SYS_POWER_DOWN:
        MDrv_SYS_PowerDown( arg );
        break;

    case IOCTL_SYS_SET_GFX_GOP_INDEX:
        MDrv_SYS_SetGFXGOPIndex(arg);
        break;

    case IOCTL_SYS_GET_GFX_GOP_INDEX:
        MDrv_SYS_GetGFXGOPIndex(arg);
        break;

    case IOCTL_SYS_SET_DISPLAY_CTLR_SEPT_INDEX:
        MDrv_SYS_SetDisplayControllerSeparated(arg);
        break;

    case IOCTL_SYS_IS_DISPLAY_CTLR_SEPT_INDEX:
        MDrv_SYS_IsDisplayControllerSeparated(arg);
    break;

    case IOCTL_SYS_SET_NEXUS:
        MDrv_SYS_SetNexus(arg);
        break;

    case IOCTL_SYS_HAS_NEXUS:
        MDrv_SYS_HasNexus(arg);
        break;

    case IOCTL_SYS_PRINT_MSG:
            MDrv_SYS_PrintMsg(arg);
        break;

    case IOCTL_SYS_GET_GFX_GOP_PIPELINE_DELAY:
        MDrv_SYS_GetGFXGOPPipelineDelay(arg);
        break;
    case IOCTL_SYS_GET_PANEL_H_START:
        MDrv_SYS_GetPanelHStart(arg);
        break;

    case IOCTL_SYS_SET_NEXUS_PID:
          MDrv_SYS_SetNexusPID(arg);
          break;

    case IOCTL_SYS_GET_NEXUS_PID:
        MDrv_SYS_GetNexusPID(arg);
        break;

    case IOCTL_SYS_PCMCIA_WRITE:
        err = MDrv_SYS_PCMCIA_WRITE(arg);
        break;

    case IOCTL_SYS_PCMCIA_READ:
        err = MDrv_SYS_PCMCIA_READ(arg);
        break;

    case IOCTL_SYS_PCMCIA_WRITE_DATA:
        //err = MDrv_SYS_PCMCIA_WRITE_DATA(arg);
        break;

    case IOCTL_SYS_PCMCIA_READ_DATA:
        err = MDrv_SYS_PCMCIA_READ_DATA(arg);
        break;

    case IOCTL_SYS_GET_MBOX_SHM:
        MDrv_SYS_GetMBoxShareMemory(arg);
        break;

    case IOCTL_SYS_GET_MSBIN_INFO:
        MDrv_SYS_GetMsBinInfo(arg);
        break;

    case IOCTL_SYS_GET_MIU1_BUS_BASE:
        MDrv_SYS_GetMIU1BusBase(arg);
        break;

    case IOCTL_SYS_GET_MIU1_BASE:
        MDrv_SYS_GetMIU1Base(arg);
        break;

    case IOCTL_SYS_FLUSH_MEMORY:
        MDrv_SYS_FlushMemory();
        break;

    case IOCTL_SYS_READ_MEMORY:
        MDrv_SYS_ReadMemory();
        break;

    case IOCTL_SYS_HOLD_KERNEL:
    	MDrv_SYS_HoldKernel();
        break;
    case IOCTL_SYS_STOP_UART_CLK:
    	MDrv_SYS_StopUARTClock();
    	break;
    case IOCTL_SYS_RESUME_UART_CLK:
    	MDrv_SYS_ResumeUARTClock();
        break;
    case IOCTL_SYS_ENABLE_MUDI:
        enable_MUDI();
        break;

    case IOCTL_SYS_DISABLE_MUDI:
        disable_MUDI();
        break;

    case IOCTL_SYS_SPI_LOAD: //20100120 Terry, SPI Load Code

	    break;
    case IOCTL_SYS_FORCE_OAD_UPGRADE_FROM_DRAM:
        MDrv_SYS_ForceUpgradeOADByDRAM(arg);
        break;
    case IOCTL_SYS_FORCE_ENV_UPGRADE_FROM_DRAM:
        MDrv_SYS_ForceUpgradeENVByDRAM(arg);
        break;

    default:
        SYS_WARNING("Unknown ioctl command %d\n", cmd);
        return -ENOTTY;
    }

    return err;
}

#define UART_DATA_MASK   63
static ssize_t _mod_sys_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{

    if (down_interruptible(&MUDI_dev.sem))
    {
        return -ERESTARTSYS;
    }

    while(dataidx==uidx)
    {
        up(&MUDI_dev.sem); /* release the lock */
        if (wait_event_interruptible(MUDI_dev.wq,dataidx!=uidx))
        {
            return -ERESTARTSYS;
        }

        if (down_interruptible(&MUDI_dev.sem))
        {
            return -ERESTARTSYS;
        }
    }

    if(copy_to_user(buf,&datapool[uidx++],1))
    {
        up(&MUDI_dev.sem);
        return 0;
    }

    uidx&=UART_DATA_MASK;
    up(&MUDI_dev.sem);

    return 1;

}

static int show_is_support_dolby_vision(struct seq_file *s, void *data)
{
	bool b_is_support_dolby_vision = false;
	if (0 > MDrv_SYS_IsSupportDolbyVison(&b_is_support_dolby_vision)) {
		printk(KERN_ERR "[%s:%d]Error: MDrv_SYS_IsSupportDolbyVison fail\n", __FUNCTION__, __LINE__);
		return -EIO;
	} else {
		seq_printf(s, "%d", b_is_support_dolby_vision ? 1 : 0);
	}
	return 0;
}

static int open_is_support_dolby_vision(struct inode *inode, struct file *file)
{
	return single_open(file, show_is_support_dolby_vision, NULL);
}

static const struct file_operations fops_is_support_dolby_vision = {
	.open		= open_is_support_dolby_vision,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int show_is_support_memc(struct seq_file *s, void *data)
{
	bool b_is_support_memc = false;
	if (0 > MDrv_SYS_IsSupportMEMC(&b_is_support_memc)) {
		printk(KERN_ERR "[%s:%d]Error: MDrv_SYS_IsSupportMEMC fail\n", __FUNCTION__, __LINE__);
		return -EIO;
	} else {
		seq_printf(s, "%d", b_is_support_memc ? 1 : 0);
	}
	return 0;
}

static int open_is_support_memc(struct inode *inode, struct file *file)
{
	return single_open(file, show_is_support_memc, NULL);
}

static const struct file_operations fops_is_support_memc = {
	.open		= open_is_support_memc,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int show_chip_part_name(struct seq_file *s, void *data)
{
	char str_chip_part_name[64];
	memset(str_chip_part_name, 0, sizeof(str_chip_part_name));

	if (0 > MDrv_SYS_GetChipPartName(str_chip_part_name)) {
		printk(KERN_ERR "[%s:%d]Error: MDrv_SYS_GetChipPartName fail\n", __FUNCTION__, __LINE__);
		return -EIO;
	} else {
		seq_printf(s, "%s", str_chip_part_name);
	}
	return 0;
}

static int open_chip_part_name(struct inode *inode, struct file *file)
{
	return single_open(file, show_chip_part_name, NULL);
}

static const struct file_operations fops_chip_part_name = {
	.open		= open_chip_part_name,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int __init init_is_support_dolby_vision(void)
{
	struct dentry *d;
	d = debugfs_create_file("is_support_dolby_vision", 0664, NULL, NULL, &fops_is_support_dolby_vision);
	if (!d) {
		pr_err("Failed to create is_support_dolby_vision debug file.\n");
		return -ENOMEM;
	}
	return 0;
}

static int __init init_is_support_memc(void)
{
	struct dentry *d;
	d = debugfs_create_file("is_support_memc", 0664, NULL, NULL, &fops_is_support_memc);
	if (!d) {
		pr_err("Failed to create is_support_memc debug file.\n");
		return -ENOMEM;
	}
	return 0;
}

static int __init init_chip_part_name(void)
{
	struct dentry *d;
	d = debugfs_create_file("chip_part_name", 0664, NULL, NULL, &fops_chip_part_name);
	if (!d) {
		pr_err("Failed to create chip_part_name debug file.\n");
		return -ENOMEM;
	}
	return 0;
}

#if defined(CONFIG_MSTAR_MSYSTEM) || defined(CONFIG_MSTAR_MSYSTEM_MODULE)
#else//#if defined(CONFIG_MSTAR_MSYSTEM) || defined(CONFIG_MSTAR_MSYSTEM_MODULE)
module_init(_mod_sys_init);
module_exit(_mod_sys_exit);
late_initcall(init_is_support_dolby_vision);
late_initcall(init_is_support_memc);
late_initcall(init_chip_part_name);

MODULE_AUTHOR("MSTAR");
MODULE_DESCRIPTION("SYSTEM driver");
MODULE_LICENSE("MSTAR");
#endif//#if defined(CONFIG_MSTAR_MSYSTEM) || defined(CONFIG_MSTAR_MSYSTEM_MODULE)
