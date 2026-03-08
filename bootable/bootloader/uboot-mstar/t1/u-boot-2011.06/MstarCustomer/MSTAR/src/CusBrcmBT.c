/*
Copyright 2017-2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.

SPDX-License-Identifier: GPL-2.0
*/

#include <command.h>
#include <common.h>
#include <ShareType.h>
#include <CusConfig.h>
#include <MsVfs.h>
#include <MsDebug.h>
#include <usb.h>
#include <MsSystem.h>
#include <stdio.h>
#include <malloc.h>
#include <MsOS.h>

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#define DBG 0

#define DUMP(p, size, tag) \
if(DBG){ \
    int i; \
    printf("[BTUSB] "); printf(tag); printf(": \n"); \
    for(i=0; i< size; i++) { \
        printf("%02X ", p[i]); \
        if((i+1) % 16 == 0) printf("\n"); \
    } \
    printf("\n"); \
}

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define HCI_RESET                               0x0C03
#define HCI_READ_LOCAL_NAME                     0x0C14
#define HCI_VSC_DOWNLOAD_MINIDRV                0xFC2E
#define HCI_VSC_LAUNCH_RAM                      0xFC4E
#define HCI_VSC_BLE_META                        0xFCE9
#define HCI_VSC_BLE_SCAN_PARAMS                 0x200B
#define HCI_VSC_BLE_SCAN_ENABLE                 0x200C
#define HCI_VSC_WAKE_ON_BLE_ENABLE              0xFD60

#define HCI_RESET_SIZE                         3
#define HCI_CMD_PREAMBLE_SIZE                  3
#define HCD_REC_PAYLOAD_LEN_BYTE               2
#define HCI_CMD_MAX_LEN                        258
#define HCI_COMMAND_COMPLETE_EVENT             0x0E
#define BLE_ADV_PCF_MAN_DATA_SIZE              6
#define BLE_ADV_PCF_BROADCASTER_SIZE           9
#define BLE_ADV_PCF_FEATURE_SEL_SIZE           14
#define BLE_ADV_PCF_CUST_FEATURE_SIZE          5
#define BLE_SCAN_PARAMS_SIZE                   7
#define BLE_SCAN_ENABLE_SIZE                   2
#define WAKE_ON_BLE_ENALBE_SIZE                1

#define UINT8_TO_STREAM(p, u8_data)   {*(p)++ = (u8)(u8_data);}
#define UINT16_TO_STREAM(p, u16_data) {*(p)++ = (u8)(u16_data); *(p)++ = (u8)((u16_data) >> 8);}

#define HCI_CMD_TIMEOUT                         500
#define HCI_EVT_MAX_RETRY                       2

#define WAKE_ON_BLE_CONF_FILE "/misc/bluedroid/wake_on_ble.conf"

#define CHECK_VALUE(value) \
{ \
    if ((value) <= 0) { \
        printf("Failed to send/receive, %d\n", __LINE__); \
            ret = -1; \
            goto exit0; \
    } \
}

typedef struct
{
    struct usb_device udev;
    struct usb_endpoint_descriptor *ep;
    unsigned long pipe;
    u8 state;
    unsigned char *fwimg;
    u32 codeLen;
    u32 r_cur;
    u8 cmd[HCI_CMD_MAX_LEN];
    u8 num_devlist;
    u32 vidpid[32];
    u8 num_vidpid;
} bt_hw_cfg_cb_t;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static bt_hw_cfg_cb_t hw_cfg_cb;

static uint8_t ble_adv_pcf_broadcaster[1024];

static uint8_t ble_adv_pcf_feature_sel[] =
{
    0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x21, 0x00, 0x00, 0x00, 0x00
};

static uint8_t ble_adv_pcf_feature_sel_data_only[] =
{
    0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00
};

static uint8_t ble_adv_pcf_cust_feature[] =
{
    0x01, 0x02, 0x00, 0x00, 0x00
};

static uint8_t ble_scan_params[] =
{
    0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00
};

static uint8_t ble_scan_enable[] =
{
    0x01, 0x00
};

//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------
static BOOLEAN load_code_from_bin(unsigned char **image, char *bin_name, u32 *code_len);
static int readFW(unsigned char *p, int size);
static BOOLEAN btusb_read_hci_event();
static int btusb_send_hci_cmd(u8 *data, int size);
static BOOLEAN brcm_btusb_Download();
static BOOLEAN getWakeOnBleDevlist();
static BOOLEAN brcm_btusb_SetWoble();
static int checkUsbDevicePort(struct usb_device* udev, u16 vendorID, u16 productID, u8 port);
static int findUsbDevice(struct usb_device* udev, u16 vendorID, u16 productID);

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

static BOOLEAN load_code_from_bin(unsigned char **image, char *bin_name, u32 *code_len)
{
    // path: /system/etc/bluetooth/bcm43569a2.hcd
    if(vfs_mount("system") != 0)
    {
        UBOOT_ERROR("vfs_mount fail\n");
        return FALSE;
    }
    char brcm_patch_bin_patch[128] = "\0";
    snprintf(brcm_patch_bin_patch, sizeof(brcm_patch_bin_patch), "%s/%s", "/etc/bluetooth", bin_name);
    UBOOT_DEBUG("[BTUSB] brcm_patch_bin_patch: %s\n", brcm_patch_bin_patch);
    *code_len = vfs_getsize(brcm_patch_bin_patch);

    // malloc buffer to store bt patch file data
    *image = malloc(*code_len);
    if(vfs_read(*image, brcm_patch_bin_patch, 0, *code_len) != 0)
    {
        UBOOT_ERROR("vfs_read fail\n");
        return FALSE;
    }
    UBOOT_DEBUG("[BTUSB] load bt patch ok(size= %d)\n", *code_len);
    UBOOT_DEBUG("load bt patch ok\n");
    UBOOT_DUMP((unsigned int)*image, 0x200);
    return TRUE;
}

static int readFW(unsigned char *p, int size)
{
    if(hw_cfg_cb.r_cur >= hw_cfg_cb.codeLen)
    {
        return 0;
    }
    memcpy(p, hw_cfg_cb.fwimg + hw_cfg_cb.r_cur, size);

    hw_cfg_cb.r_cur = hw_cfg_cb.r_cur + size;
    return size;
}

static BOOLEAN btusb_read_hci_event()
{
    int realsize;
    int rtn, retry = 0;
    uint8_t event[256];
    memset(&event[0], 0, 256);
    rtn = usb_submit_int_msg(&hw_cfg_cb.udev, usb_rcvintpipe(&hw_cfg_cb.udev, hw_cfg_cb.ep->bEndpointAddress),
                       &event[0], 256, &realsize, HCI_CMD_TIMEOUT);
    UBOOT_DEBUG("[BTUSB] WaitIntEV rtn: %d\n", rtn);
    DUMP(event, 256, "WaitIntEV");

  // retry
    while(retry < HCI_EVT_MAX_RETRY && (event[0] != HCI_COMMAND_COMPLETE_EVENT))
    {
        udelay(100000);
        rtn = usb_submit_int_msg(&hw_cfg_cb.udev, usb_rcvintpipe(&hw_cfg_cb.udev, hw_cfg_cb.ep->bEndpointAddress),
                   &event[0], 256, &realsize, HCI_CMD_TIMEOUT);
        UBOOT_DEBUG("[BTUSB] Retry WaitIntEV rtn: %d\n", rtn);
        DUMP(event, 256, "WaitIntEV");
        retry++;
    }

    return (event[0]==HCI_COMMAND_COMPLETE_EVENT?TRUE:FALSE);
}

static int btusb_send_hci_cmd(u8 *data, int size)
{
    DUMP(data, size, "Sent HCI command");
    return usb_control_msg(&hw_cfg_cb.udev, usb_sndctrlpipe(&hw_cfg_cb.udev, 0), 0,
                                USB_TYPE_CLASS, 0, 0, data, size, HCI_CMD_TIMEOUT);
}

static BOOLEAN brcm_btusb_Download()
{
    int cmdLen;
    uint8_t *p;
    int ret = FALSE;

    p = hw_cfg_cb.cmd;
    UINT16_TO_STREAM(p, HCI_RESET);
    UINT8_TO_STREAM(p, 0); /*parameter length*/
    cmdLen = HCI_CMD_PREAMBLE_SIZE;
    CHECK_VALUE(btusb_send_hci_cmd(hw_cfg_cb.cmd, cmdLen));
    CHECK_VALUE(btusb_read_hci_event());

    p = hw_cfg_cb.cmd;
    UINT16_TO_STREAM(p, HCI_READ_LOCAL_NAME);
    UINT8_TO_STREAM(p, 0); /* parameter length */
    cmdLen = HCI_CMD_PREAMBLE_SIZE;
    CHECK_VALUE(btusb_send_hci_cmd(hw_cfg_cb.cmd, cmdLen));
    CHECK_VALUE(btusb_read_hci_event());

    p = hw_cfg_cb.cmd;
    UINT16_TO_STREAM(p, HCI_VSC_DOWNLOAD_MINIDRV);
    UINT8_TO_STREAM(p, 0); /* parameter length */
    cmdLen = HCI_CMD_PREAMBLE_SIZE;
    CHECK_VALUE(btusb_send_hci_cmd(hw_cfg_cb.cmd, cmdLen));
    CHECK_VALUE(btusb_read_hci_event());

    udelay(50000); //50ms
    p = hw_cfg_cb.cmd;
    cmdLen = readFW(p, HCI_CMD_PREAMBLE_SIZE);
    while(cmdLen > 0)
    {
        cmdLen += readFW(p + HCI_CMD_PREAMBLE_SIZE, *(p + HCD_REC_PAYLOAD_LEN_BYTE));
        CHECK_VALUE(btusb_send_hci_cmd(hw_cfg_cb.cmd, cmdLen));
        CHECK_VALUE(btusb_read_hci_event());
        p = hw_cfg_cb.cmd;
        cmdLen = readFW(p, HCI_CMD_PREAMBLE_SIZE);
    }
    udelay(200000); //fw settlement delay - 200ms

    p = hw_cfg_cb.cmd;
    UINT16_TO_STREAM(p, HCI_RESET);
    UINT8_TO_STREAM(p, 0); /* parameter length */
    cmdLen = HCI_CMD_PREAMBLE_SIZE;
    CHECK_VALUE(btusb_send_hci_cmd(hw_cfg_cb.cmd, cmdLen));
    CHECK_VALUE(btusb_read_hci_event());

    ret = TRUE;

exit0:
    return ret;
}

static BOOLEAN getWakeOnBleDevlist()
{
    int i, j;
    u8 *p_conf;
    u8 conf_len;
    u8 *broadcaster = &ble_adv_pcf_broadcaster[0];
    u8 len_in_hdr;

    if(vfs_mount("userdata") != 0)
    {
        UBOOT_ERROR("vfs_mount fail\n");
        return FALSE;
    }
    UBOOT_DEBUG("[BTUSB] brcm_patch_bin_patch: %s\n", WAKE_ON_BLE_CONF_FILE);
    conf_len = vfs_getsize(WAKE_ON_BLE_CONF_FILE);

    UBOOT_DEBUG("[BTUSB] brcm_patch_bin_patch: %x\n", conf_len);
    p_conf = malloc(conf_len);
    if(vfs_read(p_conf, WAKE_ON_BLE_CONF_FILE, 0, conf_len) != 0)
    {
        UBOOT_ERROR("vfs_read fail\n");
        return FALSE;
    }

    len_in_hdr = *p_conf++;
    UBOOT_ERROR("len_in_hdr %X %X\n", len_in_hdr, conf_len - 1);
    if(conf_len - 1 != len_in_hdr)
    {
        UBOOT_ERROR("%s corrupted %X\n", WAKE_ON_BLE_CONF_FILE, len_in_hdr);
        return FALSE;
    }
    hw_cfg_cb.num_devlist = *p_conf++;
    for(i = 0; i < hw_cfg_cb.num_devlist; i++)
    {
        for(j = 0; j < 6; j++) {
           *(broadcaster + (5-j)) = *p_conf;
           p_conf++;
        }
        broadcaster = broadcaster + 6;
    }

    hw_cfg_cb.num_vidpid = *p_conf++;
    memcpy(hw_cfg_cb.vidpid, p_conf, hw_cfg_cb.num_vidpid * sizeof(u32));

    return TRUE;
}

static BOOLEAN brcm_btusb_SetWoble()
{
    int cmdLen, i;
    uint8_t *p;
    int ret = FALSE;

    for(i=0; i<hw_cfg_cb.num_vidpid; i++)
    {
        p = hw_cfg_cb.cmd;
        UINT16_TO_STREAM(p, HCI_VSC_BLE_META);
        UINT8_TO_STREAM(p, BLE_ADV_PCF_MAN_DATA_SIZE); /* parameter length */
        UINT8_TO_STREAM(p, 0xA);
        UINT8_TO_STREAM(p, 0x0);
        u16 vid = (u16)(hw_cfg_cb.vidpid[i] & 0x0000FFFF);
        u16 pid = (u16)((hw_cfg_cb.vidpid[i] & 0xFFFF0000) >> 16);
        pid = (pid & 0xFF00) >> 8 | (pid & 0xFF) << 8;
        UINT16_TO_STREAM(p, vid);
        UINT16_TO_STREAM(p, pid);
        cmdLen = HCI_CMD_PREAMBLE_SIZE + BLE_ADV_PCF_MAN_DATA_SIZE;
        CHECK_VALUE(btusb_send_hci_cmd(hw_cfg_cb.cmd, cmdLen));
        CHECK_VALUE(btusb_read_hci_event());
    }

    for(i=0; i<hw_cfg_cb.num_devlist; i++)
    {
        p = hw_cfg_cb.cmd;
        UINT16_TO_STREAM(p, HCI_VSC_BLE_META);
        UINT8_TO_STREAM(p, BLE_ADV_PCF_BROADCASTER_SIZE); /* parameter length */
        UINT8_TO_STREAM(p, 0x6);
        UINT8_TO_STREAM(p, 0x0); /* Add */
        memcpy(p, &ble_adv_pcf_broadcaster[i * 6], 6);
        p = p + 6;
        *p = 0;
        cmdLen = HCI_CMD_PREAMBLE_SIZE + BLE_ADV_PCF_BROADCASTER_SIZE;
        CHECK_VALUE(btusb_send_hci_cmd(hw_cfg_cb.cmd, cmdLen));
        CHECK_VALUE(btusb_read_hci_event());
    }

    p = hw_cfg_cb.cmd;
    UINT16_TO_STREAM(p, HCI_VSC_BLE_META);
    UINT8_TO_STREAM(p, BLE_ADV_PCF_FEATURE_SEL_SIZE); /* parameter length */
    cmdLen = HCI_CMD_PREAMBLE_SIZE + BLE_ADV_PCF_FEATURE_SEL_SIZE;
    if(hw_cfg_cb.num_devlist > 0)
    {
        memcpy(p, &ble_adv_pcf_feature_sel, BLE_ADV_PCF_FEATURE_SEL_SIZE);
    }
    else
    {
        memcpy(p, &ble_adv_pcf_feature_sel_data_only, BLE_ADV_PCF_FEATURE_SEL_SIZE);
    }
    CHECK_VALUE(btusb_send_hci_cmd(hw_cfg_cb.cmd, cmdLen));
    CHECK_VALUE(btusb_read_hci_event());

    p = hw_cfg_cb.cmd;
    UINT16_TO_STREAM(p, HCI_VSC_BLE_META);
    UINT8_TO_STREAM(p, BLE_ADV_PCF_CUST_FEATURE_SIZE); /* parameter length */
    memcpy(p, &ble_adv_pcf_cust_feature, BLE_ADV_PCF_CUST_FEATURE_SIZE);
    cmdLen = HCI_CMD_PREAMBLE_SIZE + BLE_ADV_PCF_CUST_FEATURE_SIZE;
    CHECK_VALUE(btusb_send_hci_cmd(hw_cfg_cb.cmd, cmdLen));
    CHECK_VALUE(btusb_read_hci_event());

    p = hw_cfg_cb.cmd;
    UINT16_TO_STREAM(p, HCI_VSC_BLE_SCAN_PARAMS);
    UINT8_TO_STREAM(p, BLE_SCAN_PARAMS_SIZE); /* parameter length */
    memcpy(p, &ble_scan_params, BLE_SCAN_PARAMS_SIZE);
    cmdLen = HCI_CMD_PREAMBLE_SIZE + BLE_SCAN_PARAMS_SIZE;
    CHECK_VALUE(btusb_send_hci_cmd(hw_cfg_cb.cmd, cmdLen));
    CHECK_VALUE(btusb_read_hci_event());

    p = hw_cfg_cb.cmd;
    UINT16_TO_STREAM(p, HCI_VSC_BLE_SCAN_ENABLE);
    UINT8_TO_STREAM(p, BLE_SCAN_ENABLE_SIZE); /* parameter length */
    memcpy(p, &ble_scan_enable, BLE_SCAN_ENABLE_SIZE);
    cmdLen = HCI_CMD_PREAMBLE_SIZE + BLE_SCAN_ENABLE_SIZE;
    CHECK_VALUE(btusb_send_hci_cmd(hw_cfg_cb.cmd, cmdLen));
    CHECK_VALUE(btusb_read_hci_event());

    p = hw_cfg_cb.cmd;
    UINT16_TO_STREAM(p, HCI_VSC_WAKE_ON_BLE_ENABLE);
    UINT8_TO_STREAM(p, WAKE_ON_BLE_ENALBE_SIZE);
    UINT8_TO_STREAM(p, 1); /* enable */
    cmdLen = HCI_CMD_PREAMBLE_SIZE + 1;
    CHECK_VALUE(btusb_send_hci_cmd(hw_cfg_cb.cmd, cmdLen));
    CHECK_VALUE(btusb_read_hci_event());
    ret = TRUE;

exit0:
    return ret;
}

static int checkUsbDevicePort(struct usb_device* udev, u16 vendorID, u16 productID, u8 port)
{
    struct usb_device* pdev = NULL;
#if defined (CONFIG_USB_PREINIT)
    usb_stop(port);
    if (usb_post_init(port) == 0)
#else
    if (usb_init(port) == 0)
#endif
    {
        /* get device */
        pdev = usb_get_dev_index(0);
        if ((pdev != NULL) && (pdev->descriptor.idVendor == vendorID) && (pdev->descriptor.idProduct == productID))
        {
            UBOOT_TRACE("OK\n");
            memcpy(udev, pdev, sizeof(struct usb_device));
            return 0 ;
        }
    }
    return -1;
}
static int findUsbDevice(struct usb_device* udev, u16 vendorID, u16 productID)
{
    int ret = -1;
    u8 idx = 0;
    char portNumStr[10] = "\0";
    char* pBTUsbPort = NULL;
    UBOOT_TRACE("IN\n");
    if(udev == NULL)
    {
        UBOOT_ERROR("udev can not be NULL\n");
        return -1;
    }
    #define BT_USB_PORT "bt_usb_port"
    pBTUsbPort = getenv(BT_USB_PORT);
    if(pBTUsbPort != NULL)
    {
        idx = atoi(pBTUsbPort);
        ret = checkUsbDevicePort(udev, vendorID, productID, idx);
        if(ret == 0)
        {
            return 0;
        }
    }

    #if defined(ENABLE_FIFTH_EHC)
    const char u8UsbPortCount = 5;
    #elif defined(ENABLE_FOURTH_EHC)
    const char u8UsbPortCount = 4;
    #elif defined(ENABLE_THIRD_EHC)
    const char u8UsbPortCount = 3;
    #elif defined(ENABLE_SECOND_EHC)
    const char u8UsbPortCount = 2;
    #else
    const char u8UsbPortCount = 1;
    #endif
    for(idx = 0; idx < u8UsbPortCount; idx++)
    {
        ret = checkUsbDevicePort(udev, vendorID, productID, idx);
        if(ret == 0)
        {
            snprintf(portNumStr, sizeof(portNumStr), "%d", idx);
            setenv(BT_USB_PORT, portNumStr);
            saveenv();
            return 0;
        }
    }
    if(pBTUsbPort != NULL)
    {
        setenv(BT_USB_PORT, NULL);
        saveenv();
    }
    UBOOT_ERROR("Not find usb device\n");
    return -1;
}

int do_setBrcmBT( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    BOOLEAN ret;

    struct usb_config *config;
    struct usb_interface *ifdesc;
    u32 fwlen;

    memset(&hw_cfg_cb.udev, 0, sizeof(struct usb_device));
    UBOOT_TRACE("IN\n");
    if (argc < 1)
    {
        cmd_usage(cmdtp);
        return -1;
    }
    // Broadcom BCM43569A2
    #define VENDOR_ID  (0x0A5C)
    #define PRODUCT_ID (0x2045)

    ret = findUsbDevice(&hw_cfg_cb.udev, VENDOR_ID, PRODUCT_ID);
    if(ret != 0)
    {
        UBOOT_ERROR("failed to find bt usb devic\n");
        return -1;
    }

    if(load_code_from_bin(&hw_cfg_cb.fwimg, "bcm43569a2.hcd", &fwlen) == FALSE)
    {
        UBOOT_ERROR("failed to read FW\n");
        goto exit0;
    }

    hw_cfg_cb.codeLen = fwlen;
    config = &hw_cfg_cb.udev.config;
    ifdesc = &config->if_desc[0];
    hw_cfg_cb.ep = &ifdesc->ep_desc[0];
    hw_cfg_cb.pipe = usb_rcvintpipe(&hw_cfg_cb.udev, hw_cfg_cb.ep->bEndpointAddress);

    hw_cfg_cb.r_cur = 0;

    USB_Interrupt_InitEx(&hw_cfg_cb.udev, 1);

    if(getWakeOnBleDevlist() == FALSE)
    {
        UBOOT_ERROR("failed to read woble conf file\n");
        goto exit0;
    }

    ret = brcm_btusb_Download();

    if(ret == TRUE) {
        ret = brcm_btusb_SetWoble();
    }
    if(ret == FALSE)
    {
        UBOOT_ERROR("set bt usb device woble cmd failed\n");
    }

exit0:
    if(hw_cfg_cb.fwimg)
    {
        free(hw_cfg_cb.fwimg);
    }
    UBOOT_TRACE("OK\n");
    return (ret==TRUE?0:-1);
}
