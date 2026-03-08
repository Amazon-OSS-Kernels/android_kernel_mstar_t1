#include <mhal_sys.h>
#include <MsTypes.h>
#include <MsUboot.h>
#include <command.h>
#include <common.h>
#include <MsSysUtility.h>
#include <apiSWI2C.h>
#include <MsDebug.h>

#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
#include "../mmc/inc/common/eMMC.h"
#endif
void Mhal_console_init(void)
{

    //bit 12 UART Enable, bit 11 UART RX Enable
    *(volatile MS_U32*)(MS_RIU_MAP+(0x0E12<<1)) &= ~0x1800;    //don't enable uart (decided by mboot env)

    // Switch UART0 to PIU UART 0
    // Clear UART_SEL0
    *(volatile MS_U32*)(MS_RIU_MAP+(0x101EA6<<1)) &= ~0x000F;
#if (EARLY_DISABLE_UART)
#else
    // bit 11 UART RX Enable
    *(volatile MS_U32*)(MS_RIU_MAP+(0x0E12<<1)) |= 0x0800;

    // UART_SEL0 --> PIU UART0


#if defined(CONFIG_MSTAR_BD_MST095D_10AVPC_MASERATI) && defined(CONFIG_HDMITX_MSTAR_ROCKET2)
    *(volatile MS_U32*)(MS_RIU_MAP+(0x101EA6<<1)) = 0x0004; // uart_sel0 = PIU UART0
    *(volatile MS_U32*)(MS_RIU_MAP+(0x101EA8<<1)) = 0x0050; // uart_sel5 = PIU UART1
#else
    *(volatile MS_U32*)(MS_RIU_MAP+(0x101EA6<<1)) = 0x0654;
    *(volatile MS_U32*)(MS_RIU_MAP+(0x101EA8<<1)) = 0x0000;

#endif
#endif
    // <-------

    // UART mode

    //enable reg_uart_rx_enable;
    *(volatile MS_U32*)(MS_RIU_MAP+(0x101EA2<<1)) |= (1<<10);

   
}

void Mhal_Sys_WholeChipReset( void )
{

    #if defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
	eMMC_RST_L();
	eMMC_hw_timer_delay(HW_TIMER_DELAY_1ms);
	eMMC_RST_H();
    #endif
    WriteByte( 0x2E5C, 0x79 ); //0x2E for T12, J2, A5, A3
    //MDrv_WriteByte( 0xE5C, 0x79 ); //0xE for T13, A1, A6, A7
    while(1);

}

#define WARMBOOT_BY_SOC_OVER_TEMP (0x01 << 6)
#define PRADO_I2C_ADDR 0x80
#define PRADO_I2C_BUS_NUM               (1)

static SWI2C_BusCfg Prado_I2cBusCfg[PRADO_I2C_BUS_NUM]=
{
    {PAD_DDCR_CK, PAD_DDCR_DA, 400},  // FRC PRADO
};

void Mhal_Sys_AdjustCpuClock(void)
{
    MS_U16 u16Freq = 1400;
    MS_U8 pu8TargetRegAddress[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
    MS_U8 pu8TargetData[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
    MS_U32 u32CpuClock = 0;
    MS_U8 u8OverTempFlag = 0;

    u8OverTempFlag = ReadByte(0x060A) & WARMBOOT_BY_SOC_OVER_TEMP;

    UBOOT_INFO("Over-temp flag %d\n", u8OverTempFlag);

    if( 0 == u8OverTempFlag)
    {
        pu8TargetRegAddress[0] = 0x10;
        pu8TargetRegAddress[1] = (0x06 << 1);
        pu8TargetRegAddress[2] = 0x10;
        pu8TargetRegAddress[3] = 0x2B;

        MApi_SWI2C_Init(Prado_I2cBusCfg, PRADO_I2C_BUS_NUM);
        if( FALSE == MApi_SWI2C_WriteBytes(PRADO_I2C_ADDR, 4, pu8TargetRegAddress, 0, pu8TargetData))
        {
            UBOOT_INFO("Adjust Voltage I2C error\n");
        }
        else
        {
            UBOOT_INFO("Set Voltage 0x%02x\n", pu8TargetRegAddress[3]);
            udelay(200);
            u32CpuClock = ((3623878UL / u16Freq) * 1000);

            Write2Byte(0x110CA4, (MS_U16)(u32CpuClock & 0xFFFF));
            Write2Byte(0x110CA6, (MS_U16)((u32CpuClock >> 16) & 0xFFFF));
            WriteByte(0x110CB0, 0x01);
            WriteByte(0x110CAA, 0x06);
            WriteByte(0x110CAE, 0x08);
            WriteByte(0x110CB3, ReadByte(0x110CB3) |( 0x01 << 4));
            WriteByte(0x110CA8, 0x00);
            WriteByte(0x110CA8, 0x01);
            UBOOT_INFO("Set CPU Clock %d\n", u16Freq);
        }
    }
}
