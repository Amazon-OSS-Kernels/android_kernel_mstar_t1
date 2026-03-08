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
/// file    mdrv_iic.c
/// @brief  IIC Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
//#include "MsCommon.h"
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
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/of_i2c.h>
#include <linux/of_gpio.h>

#if defined(CONFIG_COMPAT)
#include <linux/compat.h>
#endif

#include "mst_devid.h"
#include "mdrv_iic_io.h"
#include "mhal_iic_reg.h"
#include "mdrv_iic.h"
#include "mhal_iic.h"


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define IIC_MUTEX_WAIT_TIME         3000

#define IIC_DBG_ENABLE              0

#if IIC_DBG_ENABLE
#define IIC_DBG(_f)                 (_f)
#else
#define IIC_DBG(_f)
#endif

#if 0
#define LINE_DBG()                  printf("IIC %d\n", __LINE__)
#else
#define LINE_DBG()
#endif

#define IIC_PRINT(fmt, args...)		pr_debug("[IIC][%05d] " fmt, __LINE__, ## args)

typedef struct
{
    S32                         s32MajorIIC;
    S32                         s32MinorIIC;
    struct cdev                 cDevice;
    struct file_operations      IICFop;
    struct fasync_struct        *async_queue; /* asynchronous readers */
} IIC_ModHandle_t;


#define MOD_IIC_DEVICE_COUNT         1
#define MOD_IIC_NAME                 "ModIIC"


struct mstar_i2c_dev {
	u16 bus_index;
	u16 retries;
	u16 speed_khz;
	u16 sda_pad;
	u16 scl_pad;
	u16 def_delay;
	struct platform_device* pdev;
	struct i2c_adapter      adapter;
};

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------
// Forward declaration
//--------------------------------------------------------------------------------------------------
static int                      _MDrv_IIC_Open (struct inode *inode, struct file *filp);
static int                      _MDrv_IIC_Release(struct inode *inode, struct file *filp);
static ssize_t                  _MDrv_IIC_Read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
static ssize_t                  _MDrv_IIC_Write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
static unsigned int             _MDrv_IIC_Poll(struct file *filp, poll_table *wait);
#ifdef HAVE_UNLOCKED_IOCTL
static long _MDrv_IIC_Ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
#else
static int _MDrv_IIC_Ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);
#endif
#if defined(CONFIG_COMPAT)
static long Compat_MDrv_IIC_Ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
#endif

static int                      _MDrv_IIC_Fasync(int fd, struct file *filp, int mode);

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static IIC_ModHandle_t IICDev=
{
    .s32MajorIIC = MDRV_MAJOR_IIC,
    .s32MinorIIC = MDRV_MINOR_IIC,
    .cDevice =
    {
        .kobj = {.name= MOD_IIC_NAME, },
        .owner = THIS_MODULE,
    },
    .IICFop =
    {
        .open =     _MDrv_IIC_Open,
        .release =  _MDrv_IIC_Release,
        .read =     _MDrv_IIC_Read,
        .write =    _MDrv_IIC_Write,
        .poll =     _MDrv_IIC_Poll,
        #ifdef HAVE_UNLOCKED_IOCTL
        .unlocked_ioctl = _MDrv_IIC_Ioctl,
        #else        
        .ioctl = _MDrv_IIC_Ioctl,
        #endif
		#if defined(CONFIG_COMPAT)
		.compat_ioctl = Compat_MDrv_IIC_Ioctl,
		#endif
        .fasync =   _MDrv_IIC_Fasync,
    },
};
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
#if defined(CONFIG_COMPAT)
int mdrv_iic_compat_get_ion_allaction_data(IIC_Param_Compat __user *data32, IIC_Param __user *data)
{
	IIC_Param_Compat temp;
	U8 *ptr_msg; 
	int err;



	err = get_user(temp.u8IdIIC, &data32->u8IdIIC);
	err = get_user(temp.u8ClockIIC, &data32->u8ClockIIC);
	err = get_user(temp.u8SlaveIdIIC, &data32->u8SlaveIdIIC);
	err = get_user(temp.u8AddrSizeIIC, &data32->u8AddrSizeIIC);
	
	err = get_user(temp.u8AddrIIC[0], &data32->u8AddrIIC[0]);
	err = get_user(temp.u8AddrIIC[1], &data32->u8AddrIIC[1]);
	err = get_user(temp.u8AddrIIC[2], &data32->u8AddrIIC[2]);
	err = get_user(temp.u8AddrIIC[3], &data32->u8AddrIIC[3]);
	err = get_user(ptr_msg, &data32->u8pbufIIC); 
	temp.u8pbufIIC = compat_ptr(ptr_msg); //(1) pointer transmit for 32bits to 64bits
	err = get_user(temp.u32DataSizeIIC, &data32->u32DataSizeIIC);

	err = put_user(temp.u8IdIIC, &data->u8IdIIC);
	err = put_user(temp.u8ClockIIC, &data->u8ClockIIC);
	err = put_user(temp.u8SlaveIdIIC, &data->u8SlaveIdIIC);
	err = put_user(temp.u8AddrSizeIIC, &data->u8AddrSizeIIC); //(2) lost : u8AddrSizeIIC convert
	err = put_user(temp.u8AddrIIC[0], &data->u8AddrIIC[0]);
	err = put_user(temp.u8AddrIIC[1], &data->u8AddrIIC[1]);
	err = put_user(temp.u8AddrIIC[2], &data->u8AddrIIC[2]);
	err = put_user(temp.u8AddrIIC[3], &data->u8AddrIIC[3]);
	err = put_user(temp.u8pbufIIC, &data->u8pbufIIC);
	err = put_user(temp.u32DataSizeIIC, &data->u32DataSizeIIC);

	return err;
}

int mdrv_iic_compat_put_ion_allaction_data(IIC_Param_Compat __user *data32, IIC_Param  __user *data, int rw_flag)
{
	IIC_Param temp;
	U8 *ptr_msg; 
	int err;
	int i;

	if (rw_flag > 0) // read iic
	{
		err = get_user(ptr_msg, &data->u8pbufIIC); 
		err = put_user(ptr_msg, &data32->u8pbufIIC);
	}
	return err;
}
#endif

static int _MDrv_IIC_Open (struct inode *inode, struct file *filp)
{
	IIC_ModHandle_t *dev;

    IIC_PRINT("%s is invoked\n", __FUNCTION__);

    dev = container_of(inode->i_cdev, IIC_ModHandle_t, cDevice);
	filp->private_data = dev;

    return 0;
}

static int _MDrv_IIC_Release(struct inode *inode, struct file *filp)
{
    IIC_PRINT("%s is invoked\n", __FUNCTION__);
    return 0;
}

static ssize_t _MDrv_IIC_Read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	S32     u32RetCountIIC = 0;
	IIC_Param_t IIC_ReadParam;
	U8        	pRdBuffer[IIC_RD_BUF_SIZE];

#if defined(CONFIG_COMPAT)
	IIC_Param_Compat __user *data32;
#endif
	IIC_Param __user *data;
	int ret = 0;

#if defined(CONFIG_COMPAT)
	if(is_compat_task())
        { //32bit
	
		data32 = compat_ptr(buf);
		data = compat_alloc_user_space(sizeof(*data));
		if (data == NULL)
			return -EFAULT;

		mdrv_iic_compat_get_ion_allaction_data(data32, data);
		if( copy_from_user(&IIC_ReadParam, (IIC_Param_t *)data, sizeof(IIC_Param_t)) ) //(3) "'count'" is old size with 32bits stuct , need using 64bits struct size 
		{
			return -1;
		}

                 udelay(1000); //delay ok


		if( IIC_ReadParam.u32DataSizeIIC > IIC_RD_BUF_SIZE )
			IIC_ReadParam.u8pbufIIC = kmalloc(IIC_ReadParam.u32DataSizeIIC, GFP_KERNEL);
		else
			IIC_ReadParam.u8pbufIIC = pRdBuffer;

		if( IIC_ReadParam.u8pbufIIC == NULL )
		{
			return -1;
		}

		if( IIC_ReadParam.u8IdIIC >= IIC_NUM_OF_MAX )
		{
			IIC_PRINT("_MDrv_IIC_Read():ERROR: invalid IIC ID\n");
			if( IIC_ReadParam.u8pbufIIC != pRdBuffer )	kfree(IIC_ReadParam.u8pbufIIC);
			return(-1);
		}

		if( IIC_ReadParam.u8IdIIC <= IIC_NUM_OF_HW )
		{
			u32RetCountIIC = MDrv_HW_IIC_ReadBytes(IIC_ReadParam.u8SlaveIdIIC,IIC_ReadParam.u8AddrSizeIIC, IIC_ReadParam.u8AddrIIC, IIC_ReadParam.u32DataSizeIIC,IIC_ReadParam.u8pbufIIC);
		}
		else
		{
			u32RetCountIIC = MDrv_SW_IIC_ReadBytes(IIC_ReadParam.u8IdIIC,IIC_ReadParam.u8SlaveIdIIC,IIC_ReadParam.u8AddrSizeIIC, IIC_ReadParam.u8AddrIIC, IIC_ReadParam.u32DataSizeIIC,IIC_ReadParam.u8pbufIIC);
		}

		if ((0 == u32RetCountIIC) && (IIC_ReadParam.u32DataSizeIIC > 0))
			u32RetCountIIC = IIC_ReadParam.u32DataSizeIIC;
			IIC_DBG(IIC_PRINT("_MDrv_IIC_Read() -> u32RetCountIIC=%d\n", u32RetCountIIC));

	if (u32RetCountIIC > 0)
		{
		int err;
		if( copy_to_user(data, &IIC_ReadParam, sizeof(IIC_Param_t)) )
                {
			IIC_PRINT("\033[0;32;31m [Ian] %s %d Error\033[m\n", __func__, __LINE__);
			}

		if (copy_to_user(data32->u8pbufIIC, IIC_ReadParam.u8pbufIIC, sizeof(U8)*u32RetCountIIC))
                {
				IIC_PRINT("\033[0;32;31m [Ian] %s %d \033[m\n", __func__, __LINE__);
				return (-1);
			}
			IIC_PRINT("\033[0;32;31m [Ian] %s %d \033[m\n", __func__, __LINE__);
		} else {
			u32RetCountIIC = 0;
		}
		if( IIC_ReadParam.u8pbufIIC != pRdBuffer )	kfree(IIC_ReadParam.u8pbufIIC);

		return u32RetCountIIC;
	}
	else
#endif
        {
		if( copy_from_user(&IIC_ReadParam, (IIC_Param_t *)buf, sizeof(IIC_Param_t)) ) //(3) "'count'" is old size with 32bits stuct , need using 64bits struct size 
		{
			return -1;
		}


		udelay(1000);

		if( IIC_ReadParam.u32DataSizeIIC > IIC_RD_BUF_SIZE )
			IIC_ReadParam.u8pbufIIC = kmalloc(IIC_ReadParam.u32DataSizeIIC, GFP_KERNEL);
		else
			IIC_ReadParam.u8pbufIIC = pRdBuffer;

		if( IIC_ReadParam.u8pbufIIC == NULL )
		{
			return -1;
		}

		if( IIC_ReadParam.u8IdIIC >= IIC_NUM_OF_MAX )
		{
			IIC_PRINT("_MDrv_IIC_Read():ERROR: invalid IIC ID\n");
			if( IIC_ReadParam.u8pbufIIC != pRdBuffer )	kfree(IIC_ReadParam.u8pbufIIC);
			return(-1);
		}

		if( IIC_ReadParam.u8IdIIC <= IIC_NUM_OF_HW )
		{
			u32RetCountIIC = MDrv_HW_IIC_ReadBytes(IIC_ReadParam.u8SlaveIdIIC,IIC_ReadParam.u8AddrSizeIIC, IIC_ReadParam.u8AddrIIC, IIC_ReadParam.u32DataSizeIIC,IIC_ReadParam.u8pbufIIC);
		}
		else
		{
			u32RetCountIIC = MDrv_SW_IIC_ReadBytes(IIC_ReadParam.u8IdIIC,IIC_ReadParam.u8SlaveIdIIC,IIC_ReadParam.u8AddrSizeIIC, IIC_ReadParam.u8AddrIIC, IIC_ReadParam.u32DataSizeIIC,IIC_ReadParam.u8pbufIIC);
		}

		if (0 == u32RetCountIIC) {
			u32RetCountIIC = IIC_ReadParam.u32DataSizeIIC;
		}
		if (u32RetCountIIC)
		{

	        if( copy_to_user(((IIC_Param_t *)buf)->u8pbufIIC, IIC_ReadParam.u8pbufIIC, sizeof(U8)*u32RetCountIIC) )
                {
				IIC_PRINT("\033[0;32;31m [Ian] %s %d \033[m\n", __func__, __LINE__);
				return (-1);
			}
		}
		if( IIC_ReadParam.u8pbufIIC != pRdBuffer )	kfree(IIC_ReadParam.u8pbufIIC);

		return u32RetCountIIC;
	}
}

static ssize_t _MDrv_IIC_Write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    U32     u32RetCountIIC = 0;
	IIC_Param_t IIC_WriteParam;
	U8        	pWrBuffer[IIC_WR_BUF_SIZE];

#if defined(CONFIG_COMPAT)
	if(is_compat_task())
        {
		IIC_Param_Compat __user *data32;
		IIC_Param __user *data;
		int ret = 0;

		data32 = compat_ptr(buf);
		data = compat_alloc_user_space(sizeof(*data));
		if (data == NULL)
			return -EFAULT;

		mdrv_iic_compat_get_ion_allaction_data(data32, data);

		if( copy_from_user(&IIC_WriteParam, (IIC_Param_t *)data, sizeof(IIC_Param_t)) )
		{
			return -1;
		}
		if( IIC_WriteParam.u32DataSizeIIC > IIC_WR_BUF_SIZE )
			IIC_WriteParam.u8pbufIIC = kmalloc(IIC_WriteParam.u32DataSizeIIC, GFP_KERNEL);
		else
			IIC_WriteParam.u8pbufIIC = pWrBuffer;

		if( IIC_WriteParam.u8pbufIIC == NULL )
		{
			return -1;
		}

		if( copy_from_user(IIC_WriteParam.u8pbufIIC, ((IIC_Param_Compat *)buf)->u8pbufIIC, IIC_WriteParam.u32DataSizeIIC) )
		{
			if( IIC_WriteParam.u8pbufIIC != pWrBuffer )	kfree(IIC_WriteParam.u8pbufIIC);
			return -1;
		}

		if( IIC_WriteParam.u8IdIIC >= IIC_NUM_OF_MAX )
		{
			if( IIC_WriteParam.u8pbufIIC != pWrBuffer )	kfree(IIC_WriteParam.u8pbufIIC);
			return(-1);
		}

		if( IIC_WriteParam.u8IdIIC <= IIC_NUM_OF_HW )
		{
			u32RetCountIIC = MDrv_HW_IIC_WriteBytes(IIC_WriteParam.u8SlaveIdIIC, IIC_WriteParam.u8AddrSizeIIC, IIC_WriteParam.u8AddrIIC, IIC_WriteParam.u32DataSizeIIC, IIC_WriteParam.u8pbufIIC);
		}
		else
		{
			u32RetCountIIC = MDrv_SW_IIC_WriteBytes(IIC_WriteParam.u8IdIIC, IIC_WriteParam.u8SlaveIdIIC, IIC_WriteParam.u8AddrSizeIIC, IIC_WriteParam.u8AddrIIC, IIC_WriteParam.u32DataSizeIIC, IIC_WriteParam.u8pbufIIC);
		}
		if( IIC_WriteParam.u8pbufIIC != pWrBuffer )	kfree(IIC_WriteParam.u8pbufIIC);

		if (0 == u32RetCountIIC)
			u32RetCountIIC = IIC_WriteParam.u32DataSizeIIC;
		return u32RetCountIIC;
	}
	else
#endif
        {
		if( copy_from_user(&IIC_WriteParam, (IIC_Param_t *)buf, count) )
		{
			return -1;
		}

		if( IIC_WriteParam.u32DataSizeIIC > IIC_WR_BUF_SIZE )
			IIC_WriteParam.u8pbufIIC = kmalloc(IIC_WriteParam.u32DataSizeIIC, GFP_KERNEL);
		else
			IIC_WriteParam.u8pbufIIC = pWrBuffer;

		if( IIC_WriteParam.u8pbufIIC == NULL )
		{
			return -1;
		}

		if( copy_from_user(IIC_WriteParam.u8pbufIIC, ((IIC_Param_t *)buf)->u8pbufIIC, IIC_WriteParam.u32DataSizeIIC) )
		{
			if( IIC_WriteParam.u8pbufIIC != pWrBuffer )	kfree(IIC_WriteParam.u8pbufIIC);
			return -1;
		}

		if( IIC_WriteParam.u8IdIIC >= IIC_NUM_OF_MAX )
		{
			IIC_PRINT("_MDrv_IIC_Write():ERROR: invalid IIC ID\n");
			if( IIC_WriteParam.u8pbufIIC != pWrBuffer )	kfree(IIC_WriteParam.u8pbufIIC);
			return(-1);
		}

		if( IIC_WriteParam.u8IdIIC <= IIC_NUM_OF_HW )
		{
			u32RetCountIIC = MDrv_HW_IIC_WriteBytes(IIC_WriteParam.u8SlaveIdIIC, IIC_WriteParam.u8AddrSizeIIC, IIC_WriteParam.u8AddrIIC, IIC_WriteParam.u32DataSizeIIC, IIC_WriteParam.u8pbufIIC);
		}
		else
		{
			u32RetCountIIC = MDrv_SW_IIC_WriteBytes(IIC_WriteParam.u8IdIIC, IIC_WriteParam.u8SlaveIdIIC, IIC_WriteParam.u8AddrSizeIIC, IIC_WriteParam.u8AddrIIC, IIC_WriteParam.u32DataSizeIIC, IIC_WriteParam.u8pbufIIC);
		}
		if( IIC_WriteParam.u8pbufIIC != pWrBuffer )	kfree(IIC_WriteParam.u8pbufIIC);

		if (0 == u32RetCountIIC)
			u32RetCountIIC = IIC_WriteParam.u32DataSizeIIC;
		return u32RetCountIIC;

	}

}

static unsigned int _MDrv_IIC_Poll(struct file *filp, poll_table *wait)
{
    IIC_PRINT("%s is invoked\n", __FUNCTION__);
    return 0;
}


static int _MDrv_IIC_Fasync(int fd, struct file *filp, int mode)
{
    IIC_PRINT("%s is invoked\n", __FUNCTION__);

	return 0;//fasync_helper(fd, filp, mode, &IICDev.async_queue);
}

#if defined(CONFIG_COMPAT)
static long Compat_MDrv_IIC_Ioctl(struct file *filp, unsigned int cmd, unsigned long arg){
	int	err= 0;
	switch(cmd)
	{
        case Compat_MDRV_IIC_INIT:
        case Compat_MDRV_IIC_BUSCFG:
		{
			return filp->f_op->unlocked_ioctl(filp, cmd,(unsigned long)compat_ptr(arg));
		}
        case Compat_MDRV_IIC_CLOCK:
		{
			IIC_Param_Compat __user *data32;
			IIC_Param __user *data;

			data32 = compat_ptr(arg);
			data = compat_alloc_user_space(sizeof(*data));
			if (data == NULL)
				return -EFAULT;
			mdrv_iic_compat_get_ion_allaction_data(data32, data);
			return filp->f_op->unlocked_ioctl(filp, MDRV_IIC_CLOCK,(unsigned long)data);
		}
		case Compat_MDRV_IIC_ENABLE:
		{
			IIC_Param_Compat __user *data32;
			IIC_Param __user *data;

			data32 = compat_ptr(arg);
			data = compat_alloc_user_space(sizeof(*data));
			if (data == NULL)
				return -EFAULT;
			mdrv_iic_compat_get_ion_allaction_data(data32, data);

	
			return filp->f_op->unlocked_ioctl(filp, MDRV_IIC_ENABLE,(unsigned long)data);
		}
		default:
			IIC_PRINT("\033[0;32;31m %s %d Ioctl cmd %d undefined \033[m\n", __func__, __LINE__, cmd);
			return -ENOIOCTLCMD;
	}

}
#endif

#ifdef HAVE_UNLOCKED_IOCTL
static long _MDrv_IIC_Ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
#else
static int _MDrv_IIC_Ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
#endif
{
    S32 s32Err= 0;
	IIC_Param_t IIC_param;
    #if (defined(CONFIG_MSTAR_TITANIA)||defined(CONFIG_MSTAR_TITANIA2))
    #else
	IIC_BusCfg_t IIC_BusObj;
    #endif

    IIC_PRINT("%s is invoked\n", __FUNCTION__);

    /*
     * extract the type and number bitfields, and don't decode
     * wrong cmds: return ENOTTY (inappropriate ioctl) before access_ok()
     */
    if ((IIC_IOC_MAGIC!= _IOC_TYPE(cmd)) || (_IOC_NR(cmd)> IIC_IOC_MAXNR))
    {
        return -ENOTTY;
    }

    /*
     * the direction is a bitmask, and VERIFY_WRITE catches R/W
     * transfers. `Type' is user-oriented, while
     * access_ok is kernel-oriented, so the concept of "read" and
     * "write" is reversed
     */
    if (_IOC_DIR(cmd) & _IOC_READ)
    {
        s32Err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
    }
    else if (_IOC_DIR(cmd) & _IOC_WRITE)
    {
        s32Err =  !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
    }
    if (s32Err)
    {
        return -EFAULT;
    }


    //PROBE_IO_ENTRY(MDRV_MAJOR_IIC, _IOC_NR(cmd));

    switch(cmd)
    {
        case MDRV_IIC_INIT:
            IIC_PRINT("ioctl: MDRV_IIC_INIT\n");
            MDrv_IIC_Init();
            break;
        case MDRV_IIC_CLOCK:
            IIC_PRINT("ioctl: MDRV_IIC_CLOCK\n");
            if (copy_from_user(&IIC_param, (IIC_Param_t __user *)arg, sizeof(IIC_Param_t)))
            {
                //PROBE_IO_EXIT(MDRV_MAJOR_IIC, _IOC_NR(cmd));
 	            return -EFAULT;
            }

			if( IIC_param.u8IdIIC >= IIC_NUM_OF_MAX )
			{
			    //PROBE_IO_EXIT(MDRV_MAJOR_IIC, _IOC_NR(cmd));
				return -EFAULT;
			}

            if( IIC_param.u8IdIIC <= IIC_NUM_OF_HW )
			{
				MDrv_HW_IIC_Clock_Select(IIC_param.u8ClockIIC);
			}
			else
			{
				MDrv_SW_IIC_SetSpeed(IIC_param.u8IdIIC, IIC_param.u8ClockIIC);
			}
            break;

    #if (defined(CONFIG_MSTAR_TITANIA)||defined(CONFIG_MSTAR_TITANIA2))
    #else
		case MDRV_IIC_ENABLE:
			IIC_PRINT("ioctl: MDRV_IIC_ENABLE\n");
			if (copy_from_user(&IIC_param, (IIC_Param_t __user *)arg, sizeof(IIC_Param_t)))
			{
			    //PROBE_IO_EXIT(MDRV_MAJOR_IIC, _IOC_NR(cmd));
				return -EFAULT;
            }
			if(( IIC_param.u8IdIIC <= IIC_NUM_OF_HW )||( IIC_param.u8IdIIC >= IIC_NUM_OF_MAX ))
			{
                IIC_PRINT("SW I2C Channel is BTW (%d) ~ %d\n",IIC_NUM_OF_HW+1,IIC_NUM_OF_MAX-1);
			    //PROBE_IO_EXIT(MDRV_MAJOR_IIC, _IOC_NR(cmd));
				return -EFAULT;
			}
            IIC_PRINT("MDRV_IIC_ENABLE: SW I2C Channel = %d\n",IIC_param.u8IdIIC);
            IIC_PRINT("MDRV_IIC_ENABLE: enable = %d\n",IIC_param.u8ClockIIC);
            MDrv_SW_IIC_Enable( IIC_param.u8IdIIC, ((IIC_param.u8ClockIIC) ? TRUE : FALSE) );
			break;

            case MDRV_IIC_BUSCFG:
            IIC_PRINT("ioctl: MDRV_IIC_BUSCFG\n");
            if (copy_from_user(&IIC_BusObj, (IIC_BusCfg_t __user *)arg, sizeof(IIC_BusCfg_t)))
            {
                //PROBE_IO_EXIT(MDRV_MAJOR_IIC, _IOC_NR(cmd));
 	            return -EFAULT;
            }
            IIC_PRINT("SW I2C u8ChIdx --> is %d\n",IIC_BusObj.u8ChIdx);
			if(( IIC_BusObj.u8ChIdx <= IIC_NUM_OF_HW )||( IIC_BusObj.u8ChIdx >= IIC_NUM_OF_MAX ))
			{
                IIC_PRINT("SW I2C Channel is BTW (%d) ~ %d\n",IIC_NUM_OF_HW+1,IIC_NUM_OF_MAX-1);
			    //PROBE_IO_EXIT(MDRV_MAJOR_IIC, _IOC_NR(cmd));
				return -EFAULT;
			}
            MDrv_SW_IIC_Init_Bus((IIC_BusCfg_t*)&IIC_BusObj, IIC_BusObj.u8ChIdx);
            break;
    #endif
    
        default:
            IIC_PRINT("ioctl: unknown command\n");
            //PROBE_IO_EXIT(MDRV_MAJOR_IIC, _IOC_NR(cmd));
            return -ENOTTY;
    }
    //PROBE_IO_EXIT(MDRV_MAJOR_IIC, _IOC_NR(cmd));
    return 0;
}


static int mod_iic_init(void)
{
    S32         s32Ret;
    dev_t       dev;

	IIC_PRINT("%s is invoked\n", __func__);

    if (IICDev.s32MajorIIC)
    {
        dev = MKDEV(IICDev.s32MajorIIC, IICDev.s32MinorIIC);
        s32Ret = register_chrdev_region(dev, MOD_IIC_DEVICE_COUNT, MOD_IIC_NAME);
    }
    else
    {
        s32Ret = alloc_chrdev_region(&dev, IICDev.s32MinorIIC, MOD_IIC_DEVICE_COUNT, MOD_IIC_NAME);
        IICDev.s32MajorIIC = MAJOR(dev);
    }

    if ( 0 > s32Ret)
    {
        IIC_PRINT("Unable to get major %d\n", IICDev.s32MajorIIC);
        return s32Ret;
    }

    cdev_init(&IICDev.cDevice, &IICDev.IICFop);
    if (0!= (s32Ret= cdev_add(&IICDev.cDevice, dev, MOD_IIC_DEVICE_COUNT)))
    {
        IIC_PRINT("Unable add a character device\n");
        unregister_chrdev_region(dev, MOD_IIC_DEVICE_COUNT);
        return s32Ret;
    }

    return 0;
}

static void mod_iic_exit(void)
{
    IIC_PRINT("%s is invoked\n", __FUNCTION__);

    cdev_del(&IICDev.cDevice);
    unregister_chrdev_region(MKDEV(IICDev.s32MajorIIC, IICDev.s32MinorIIC), MOD_IIC_DEVICE_COUNT);
}

/* linux i2c driver framework */

static int mstar_swi2c_xfer_msg(struct i2c_adapter *adap, struct i2c_msg *msg, int stop)
{
	struct mstar_i2c_dev *i2c_dev = i2c_get_adapdata(adap);
	int ret = 0;
	u8 *pu8Buf = msg->buf;
	u8 u32BufLen = msg->len;

	dev_dbg(&i2c_dev->pdev->dev, "I2C message to: 0x%04x, len: %d,flags: 0x%04x, stop: %d\n",
			msg->addr, msg->len, msg->flags, stop);

	if (msg->len == 0) {
		dev_dbg(&i2c_dev->pdev->dev, "I2C message size is 0\n");
		return -EINVAL;
	}

	if (msg->flags & I2C_M_RD) {
		/* send read message */
		ret = MDrv_SW_IIC_ReadByteArrayDirectly(i2c_dev->bus_index, msg->addr, u32BufLen, pu8Buf, stop);
		/* stop at every memssage */
		IIC_PRINT("swi2c recieved %u bytes from slave %u\n", ret, (u32)msg->addr);
	} else {
		/* send write message */

		//TODO: setup mutex or embedded into MDrv
		ret = MDrv_SW_IIC_WriteByteArrayDirectly(i2c_dev->bus_index, msg->addr, u32BufLen, pu8Buf, stop);
		IIC_PRINT("swi2c sent %u bytes to slave %u\n", ret, (u32)msg->addr);
	}

	return ret;
}


static int mstar_swi2c_xfer(struct i2c_adapter *adap, struct i2c_msg msgs[],
	            int num)
{
	int ret = -1;
	int i;

	for (i = 0; i < num; i++)
	{
	    /* NOTICE: place stop at the end of the last msg */
	    ret = mstar_swi2c_xfer_msg(adap, &msgs[i], (i == (num - 1)));
	    if (ret == 0) {
	        break;
	    }
	}
	return i;
}

static u32 mstar_swi2c_func(struct i2c_adapter *adap)
{
	return I2C_FUNC_I2C;
}

static const struct i2c_algorithm mstar_swi2c_algo = {
	.master_xfer    = mstar_swi2c_xfer,
	.functionality  = mstar_swi2c_func,
};

#if 0
static int test_swi2c_xfer(struct i2c_adapter *adap)
{
	int ret;

	struct i2c_msg write_msg[2];
	struct i2c_msg read_msg[2];

	u8 data[6] = {0x53, 0x45, 0x52, 0x44, 0x42};
	u8 id_buf[2] = {0xFF, 0xFF};
	// PRADO I2C debug mode
	write_msg[0].addr = 0x80;   // slave address, in this case PMIC
	write_msg[0].flags = 0x0;   // i2c bus write
	write_msg[0].len = 5;       // commands length = 5
	write_msg[0].buf = data;    // commands

	//send command to PRADO
	ret = mstar_swi2c_xfer(adap, &write_msg[0], 1);

	//PRADO use I2C_USE_CFG
	data[0] = 0x7f;
	write_msg[0].len = 1;       // data length = 1
	write_msg[0].buf = data;    // data
	ret = mstar_swi2c_xfer(adap, &write_msg[0], 1);

	//PRADO use I2C_USE_CFG
	data[0] = 0x7d;
	write_msg[0].len = 1;       // data length = 1
	write_msg[0].buf = data;    // data
	ret = mstar_swi2c_xfer(adap, &write_msg[0], 1);

	//PRADO use I2C_USE_CFG
	data[0] = 0x50;
	write_msg[0].len = 1;       // data length = 1
	write_msg[0].buf = data;    // data
	ret = mstar_swi2c_xfer(adap, &write_msg[0], 1);

	//PRADO use I2C_USE_CFG
	data[0] = 0x55;
	write_msg[0].len = 1;       // data length = 1
	write_msg[0].buf = data;    // data
	ret = mstar_swi2c_xfer(adap, &write_msg[0], 1);

	//PRADO use I2C_USE_CFG
	data[0] = 0x35;
	write_msg[0].len = 1;       // data length = 1
	write_msg[0].buf = data;    // data
	ret = mstar_swi2c_xfer(adap, &write_msg[0], 1);

	// PRADO set target read address
	data[0] = 0x10;                 // command IC to read
	data[1] = 0xC0;                 // target register 0xC0
	read_msg[0].addr = 0x80;        // slave address, in this case PRADO 0x80
	read_msg[0].flags = 0x0;        // i2c bus write
	read_msg[0].len = 2;            // address length = 2
	read_msg[0].buf = data;         // slave register address

	// PRADO read
	read_msg[1].addr = 0x80;        // slave address, in this case PRADO 0x80
	read_msg[1].flags = I2C_M_RD;   // i2c bus read
	read_msg[1].len = 2;            // length of data to read = 2
	read_msg[1].buf = id_buf;       // read to id_buf

	ret = mstar_swi2c_xfer(adap, &read_msg[0], 1);
	ret = mstar_swi2c_xfer(adap, &read_msg[1], 1);

	if(ret < 0) {
	    IIC_PRINT("test FAILED\n");
	    return 0;
	}
	if(id_buf[1] == 0x9A) {
	    IIC_PRINT("test SUCCESS!\n");
	    return ret;
	} else {
	    IIC_PRINT("test FAILED! recieved data: %x %x\n", id_buf[0], id_buf[1]);
	}

	return ret;
}
#endif

static void mstar_swi2c_init(struct mstar_i2c_dev *i2c_dev)
{

	/* init MDrv IIC backend */
	IIC_BusCfg_t bus_cfg;
	bus_cfg.u16PadSCL = i2c_dev->scl_pad;
	bus_cfg.u16PadSDA = i2c_dev->sda_pad;
	bus_cfg.u16DefDelay = i2c_dev->def_delay;
	bus_cfg.u16SpeedKHz = i2c_dev->speed_khz;
	bus_cfg.u16Retries = i2c_dev->retries;
	IIC_PRINT("[%s]i2c_dev->retries = %u i2c_dev->bus_index= %u\n", __FUNCTION__, i2c_dev->retries, 
			i2c_dev->bus_index);
	MDrv_SW_IIC_Init_Setup();
	MDrv_SW_IIC_Init_Bus(&bus_cfg, i2c_dev->bus_index);

}

static int mstar_swi2c_parse_dt(struct device_node *dn, struct mstar_i2c_dev *i2c_dev)
{
	u32 sda_pad = 0;
	u32 scl_pad = 0;
	u32 def_delay = 0;
	u32 speed_khz = 0;
	u32 retries = 0;
	u32 bus_index = 0;

	/* parse dt */
	if(   0 != of_property_read_u32(dn, "sda-gpio", &sda_pad) ||
		0 != of_property_read_u32(dn, "scl-gpio", &scl_pad) ||
		0 != of_property_read_u32(dn, "def-delay", &def_delay) ||
		0 != of_property_read_u32(dn, "retries", &retries) ||
		0 != of_property_read_u32(dn, "bus-index", &bus_index) ||
		0 != of_property_read_u32(dn, "speed-khz", &speed_khz))
	{
		IIC_PRINT(KERN_ERR "[mstar][kswi2c]Parse dts error\n");
		return -ENXIO;
	}

	if (!gpio_is_valid(sda_pad) || !gpio_is_valid(scl_pad))
	{
		IIC_PRINT(KERN_ERR "%s: invalid GPIO pins, sda=%d/scl=%d\n",
			__FUNCTION__, sda_pad, scl_pad);
		return -ENODEV;
	}

	IIC_PRINT("[SWI2C]index: %u, retries: %u, sda:%u, scl:%u, speed_khz:%u def_delay:%u\n",
				bus_index, retries, sda_pad, scl_pad, speed_khz, def_delay);

	i2c_dev->sda_pad = (u16)sda_pad;
	i2c_dev->scl_pad = (u16)scl_pad;
	i2c_dev->def_delay = (u16)def_delay;
	i2c_dev->speed_khz = (u16)speed_khz;
	i2c_dev->retries = (u16)retries;
	i2c_dev->bus_index = (u16)bus_index;
	return 0;

}
static int mstar_iic_drv_suspend(struct platform_device *dev, pm_message_t state)
{
	return 0;
}
static int mstar_iic_drv_resume(struct platform_device *dev)
{
	return 0;
}

static int mstar_iic_drv_probe(struct platform_device *pdev)
{
	int retval = 0;
	struct mstar_i2c_dev *i2c_dev;
	struct i2c_adapter *adap;
	struct device_node *dn;

	IIC_PRINT("[ki2c] %s\n",__FUNCTION__);
	if( !(pdev->name) || strcmp(pdev->name,"Mstar-swi2c")
		|| pdev->id!=0)
	{
		retval = -ENXIO;
	}

	/* init swi2c device struct */
	i2c_dev = devm_kzalloc(&pdev->dev, sizeof(struct mstar_i2c_dev), GFP_KERNEL);
	if (!i2c_dev)
	{
		dev_err(&pdev->dev, "Unable to allocate memory for mstar-i2c\n");
		retval = -ENOMEM;
		goto err_init_swi2c;
	}

	/* parse OF i2c info */
	dn = pdev->dev.of_node;
	if (dn)
	{
		retval = mstar_swi2c_parse_dt(dn, i2c_dev);
		if (retval < 0)
		{
			IIC_PRINT("[ki2c] unable to parse device tree\n");
			goto err_init_swi2c;
		}
	}
	IIC_PRINT("[SWI2C]index: %u, retries: %u, sda:%u, scl:%u, speed_khz:%u def_delay:%u\n",
				i2c_dev->bus_index, i2c_dev->retries, i2c_dev->sda_pad, i2c_dev->scl_pad, i2c_dev->speed_khz, i2c_dev->def_delay);

	/* get GPIO pins */
	retval = devm_gpio_request(&pdev->dev, i2c_dev->sda_pad, "sda");
	if(retval < 0) {
		dev_err(&pdev->dev, "request gpio pad %u failed\n", i2c_dev->sda_pad);
		goto err_init_swi2c;
	}
	retval = devm_gpio_request(&pdev->dev, i2c_dev->scl_pad, "scl");
	if(retval < 0) {
		dev_err(&pdev->dev, "request gpio pad %u failed\n", i2c_dev->scl_pad);
		goto err_gpio;
	}

	i2c_dev->pdev = pdev;
	platform_set_drvdata(pdev, i2c_dev);

	/* init swi2c MDrv backend */
	mstar_swi2c_init(i2c_dev);

	/* register i2c adapter and bus algorithm */
	adap = &i2c_dev->adapter;
	adap->owner = THIS_MODULE;
	adap->class = I2C_CLASS_HWMON;
	strlcpy(adap->name, "Mstar I2C adapter", sizeof(adap->name));
	adap->dev.parent = &pdev->dev;
	adap->algo = &mstar_swi2c_algo;
	adap->dev.of_node = pdev->dev.of_node;
	adap->retries = i2c_dev->retries;
	adap->timeout = 2 * HZ;
	adap->nr = i2c_dev->bus_index;

	i2c_set_adapdata(adap, i2c_dev);

	retval = i2c_add_adapter(adap);
	if (retval) {
	    dev_err(&pdev->dev, "failed to add i2c adapter");
	    goto err_gpio;
	}

	/* register i2c bus to dt */
	of_i2c_register_devices(adap);

	dev_info(&pdev->dev, "Init mstar i2c is done\n");
#if 0
	test_swi2c_xfer(adap);
#endif
	/* all set, now init the ioctl interface */
	mod_iic_init();
	return 0;

err_gpio:
	gpio_free(i2c_dev->scl_pad);
	gpio_free(i2c_dev->sda_pad);
err_init_swi2c:
	return retval;
}

static int mstar_iic_drv_remove(struct platform_device *pdev)
{
	struct mstar_i2c_dev *dev = platform_get_drvdata(pdev);

	if( !(pdev->name) || strcmp(pdev->name,"Mstar-swi2c")
		|| pdev->id!=0)
	{
		return -1;
	}

	mod_iic_exit();	
	i2c_del_adapter(&dev->adapter);
	gpio_free(dev->scl_pad);
	gpio_free(dev->sda_pad);
	pdev->dev.platform_data = NULL;
	return 0;
}


#if defined (CONFIG_ARM64)
static struct of_device_id mstariic_of_device_ids[] = {
		{.compatible = "mstar,swi2c"},
		{},
};
#endif

static struct platform_driver Mstar_iic_driver = {
	.probe 		= mstar_iic_drv_probe,
	.remove		= mstar_iic_drv_remove,
	.suspend		= mstar_iic_drv_suspend,
	.resume		= mstar_iic_drv_resume,

	.driver = {
#if defined(CONFIG_ARM64)
		.of_match_table = mstariic_of_device_ids,
#endif
		.name	= "Mstar-iic",
		.owner	= THIS_MODULE,
	}
};


static int __init mstar_iic_drv_init_module(void)
{
	int retval=0;
	IIC_PRINT("[kswi2c] %s\n", __func__);
	retval = platform_driver_register(&Mstar_iic_driver);
	return retval;
}

static void __exit mstar_iic_drv_exit_module(void)
{
	platform_driver_unregister(&Mstar_iic_driver);
}


module_init(mstar_iic_drv_init_module);
module_exit(mstar_iic_drv_exit_module);

MODULE_AUTHOR("MSTAR");
MODULE_DESCRIPTION("IIC driver");
MODULE_LICENSE("GPL");
