#include <common.h>
#include <command.h>
#include <fdt.h>
#include <libfdt.h>
#include <asm/global_data.h>
#include <idme.h>
#include "MsSysUtility.h"
#include "image.h"
#include <drvGPIO.h>
#include "CusConfig.h"

void ft_board_setup(void *blob, bd_t *bd)
{
	struct fdt_header *fdt_ptr = (struct fdt_header *)blob;

	unsigned int newsize = fdt_totalsize(fdt_ptr)
				+ CONFIG_IDME_SIZE;
	fdt_open_into(fdt_ptr, fdt_ptr, newsize);
	idme_device_tree_initialize(fdt_ptr);
	printf("IDME inserted into FDT\n");
}


void ft_uboot_log_setup(void *blob)
{
	struct fdt_header *fdt_ptr = (struct fdt_header *)blob;
	int ret=0, offset=0, err=0;

	char *temp = NULL;
	temp = (char*)(malloc(AMZN_LOG_SIZE+1));
	if (!temp) {
		printf("%s: memory allocation error\n", __FUNCTION__);
		return;
	}
	temp[0]=0;
	amzn_get_log(temp);
	int logsize = strlen(temp);
	if (logsize <= 0)  /* buffer is empty */
		goto done;

        unsigned int newsize = fdt_totalsize(fdt_ptr)
                                + logsize + 32;

	err = fdt_open_into(fdt_ptr, fdt_ptr, newsize);
	if (err != 0) {
		printf ("libfdt fdt_open_into(): %s\n",
			fdt_strerror(err));
	}

	ret = fdt_path_offset(fdt_ptr, "/ubootlog");
	if (ret & FDT_ERR_NOTFOUND) {
		/* Create the ubootlog root node in FDT */
		if ((ret = fdt_path_offset(fdt_ptr, "/")) < 0) {
			printf("%s: unable to find root offset\n", __FUNCTION__);
			goto done;
		}

		if((ret = fdt_add_subnode(fdt_ptr, ret, "ubootlog")) < 0) {
			printf("%s: unable to add ubootlog root node\n", __FUNCTION__);
			goto done;
		}

		/* Get the offset of the new ubootlog root node */
		if ((ret = fdt_path_offset(fdt_ptr, "/ubootlog")) < 0) {
			printf("%s: unable to find ubootlog root node offset\n", __FUNCTION__);
			goto done;
		}
		offset = ret;

		ret = fdt_setprop_string(fdt_ptr, offset, "value", temp);
		if (ret < 0) {
			printf("%s: unable to set ubootlog (value)\n", __FUNCTION__);
			goto done;
		}
	}

done:
	if (temp) {
		free(temp);
	}
	return;
}

int amzn_boot(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
        void* bootimg_addr = 0x25000000;
        void* dtb_dst     = 0x23000000;
	ulong dtb_src;
	ulong dtb_len;

	if (argc < 2) {
		printf("Invalid arguments\n");
		return -1;
	}

	if (!strncmp(argv[1], "recovery", 7)) {
		if (run_command("mmc read.p 0x25000000 recovery 0x01100000", 0)
				< 0) {
			printf("Failed to load recovery image\n");
			return -1;
		}
	} else if (!strncmp(argv[1],"diag", 4)) {
		if (run_command("mmc read.p 0x25000000 dkernel 0x01100000", 0) < 0) {
			printf("Failed to load dkernel image\n");
			return -1;
		}

	} else if (!strncmp(argv[1], "transition", 9)) {
		run_command("mmc erase.p dkernel", 0);
		run_command("mmc erase.p dfs", 0);
		run_command("mmc erase.p diag_userdata", 0);

		run_command("mmc remove dkernel", 0);
		run_command("mmc remove dfs", 0);
		run_command("mmc remove diag_userdata", 0);

		run_command("mmc remove userdata", 0);
		run_command("mmc create userdata MAX", 0);

		char cmd[128]="\0";
		// load env from /system/etc/set_env
		snprintf(cmd,sizeof(cmd)-1,"loadenv %s %s", SET_ENV_PATITION, SET_ENV_FILE);
		run_command(cmd,0);

		// set_env has a reset command, system will reset here, if not return -1
		return -1;

	} else if (run_command("mmc read.p 0x25000000 boot 0x01100000", 0) < 0) {
		printf("Failed to load boot image\n");
		return -1;
	}

#if BOARD_KERNEL_SEPARATED_DT
	/* copy DTB from boot image. DTB is appended after kernel and ramdisk sections */
	if (-1 == android_image_get_dtb((struct andr_img_hdr *)bootimg_addr, &dtb_src, &dtb_len)) {
		printf("Failed to load DTB\n");
	} else {
		memcpy(dtb_dst, (void*)dtb_src, dtb_len);
	}
#else
	/* read DTB from partition */
	if (run_command("mmc read.p 0x23000000 dtb 0x0100000", 0) < 0) {
		printf("Failed to load DTB\n");
		return -1;
	}
#endif

	if (run_command("fdt addr 0x23000000", 0) < 0) {
		printf("Failed to initialize DTB\n");
		return -1;
	}

	if (run_command("fdt boardsetup", 0) < 0) {
		printf("Failed to setup DTB\n");
		return -1;
	}

	if (run_command("bootm 0x25000000", 0) < 0) {
		printf("Failed to boot image\n");
		return -1;
	}

	return 0;
}

U_BOOT_CMD(
	amzn_boot, 2, 0, amzn_boot,
	"Amazon customized-boot",
		  "normal	- Normal boot (default)\n"
	"amzn_boot recovery	- Boot into recovery mode\n"
	"amzn_boot diag		- Boot into diagnostics mode\n"
);

int target_is_production()
{
#if BOARD_AMAZON_KERNEL_SIGNING
#include "amzn_secure_boot.h"
	if (AMZN_PRODUCTION_DEVICE == amzn_target_device_type()) {
		return 1;
	}
#endif

	return 0;
}

static MS_U8 read_board_cid(void)
{
	/* read Customer ID */
	MS_U32 addr = 0x3806;
	MS_U8 val = ReadByte(addr);
	return val;
}

int is_secure_cpu()
{
	MS_U8 val = read_board_cid();

	/* secure CPU if customer ID is 0x41 */
	if (val == 0x41)
		return 1;
	else
		return 0;
}

#define SBOOT_VER_MARK   "YAMA"
#define VER_MARK_LEN     4

#define MARGO_PROD_ID    "0036"
#define PROD_ID_LEN      4

#define MARGO_HVT        "00"
#define MARGO_EVT1       "01"
#define MARGO_EVT2       "21"
#define MARGO_EVT3       "22"
#define MARGO_EVT31      "23"
#define MARGO_DVT        "30"
#define MARGO_PVT        "40"
#define REV_ID_LEN       2
#define REV_ID_OFF       7

#define DDR3_NUM         3
#define DDR4_NUM         4
#define INVALID_DDR      0

#define DDR_CONF_GPIO1   83
#define DDR_CONF_GPIO2   84

#define MAX_SBOOT_SIZE 102400 //100K
#define HASH0_SIZE     14336  //0x3800

int is_14key_keypad(void)
{
        int retVal = 1;
        char board_id[18] = {0};

        idme_get_var_external("board_id", board_id, sizeof(board_id)-1);

        if (strncmp(board_id, MARGO_PROD_ID, PROD_ID_LEN)) {
                return retVal;
        }

        if ( (!strncmp(board_id+REV_ID_OFF, MARGO_HVT, REV_ID_LEN)) ||
             (!strncmp(board_id+REV_ID_OFF, MARGO_EVT1, REV_ID_LEN)) ||
             (!strncmp(board_id+REV_ID_OFF, MARGO_EVT2, REV_ID_LEN)) ||
             (!strncmp(board_id+REV_ID_OFF, MARGO_EVT3, REV_ID_LEN)) ||
             (!strncmp(board_id+REV_ID_OFF, MARGO_EVT31, REV_ID_LEN))) {
                retVal = 0;
        }
        return retVal;
}

static MS_U8 read_ddr_board_id(void)
{
	char board_id[18] = {0};

	idme_get_var_external("board_id", board_id, sizeof(board_id)-1);
	printf("board id: %s\n", board_id);

	if (strncmp(board_id, MARGO_PROD_ID, PROD_ID_LEN)) {
		return INVALID_DDR;
	}
	if ( (!strncmp(board_id+REV_ID_OFF, MARGO_HVT, REV_ID_LEN)) ||
	     (!strncmp(board_id+REV_ID_OFF, MARGO_EVT1, REV_ID_LEN)) ||
	     (!strncmp(board_id+REV_ID_OFF, MARGO_EVT2, REV_ID_LEN)) ||
	     (!strncmp(board_id+REV_ID_OFF, MARGO_EVT3, REV_ID_LEN)) ) {
		return DDR4_NUM;
	} else if ( (!strncmp(board_id+REV_ID_OFF, MARGO_EVT31, REV_ID_LEN)) ||
		    (!strncmp(board_id+REV_ID_OFF, MARGO_DVT, REV_ID_LEN)) ||
		    (!strncmp(board_id+REV_ID_OFF, MARGO_PVT, REV_ID_LEN)) ) {
		return DDR3_NUM;
	} else {
		return INVALID_DDR;
	}
}

static MS_U8 read_ddr_gpio(void)
{
	int config1 = mdrv_gpio_get_level(DDR_CONF_GPIO1+1);
	int config2 = mdrv_gpio_get_level(DDR_CONF_GPIO2+1);

	printf("GPIO%d:%d; GPIO%d:%d\n", DDR_CONF_GPIO1, config1,
		DDR_CONF_GPIO2, config2);

	if ( (config1 == 1) && (config2 == 0) ) {
		return DDR4_NUM;
	}
	if ( (config1 == 0) && (config2 == 1) ) {
		return DDR3_NUM;
	}
	return INVALID_DDR;
}

sbvc_result sboot_version_check(uchar* sboot_buf, int sboot_len)
{
	int mark_loc = 0;

	if (sboot_buf == NULL)
	{
		return SBVC_INVALID_ARG;
	}

	// check sboot version mark
	while ( (mark_loc <= sboot_len - VER_MARK_LEN - 4) &&
		strncmp(sboot_buf+mark_loc, SBOOT_VER_MARK, VER_MARK_LEN) ) {
		mark_loc++;
	}

	if (mark_loc > sboot_len - VER_MARK_LEN - 4) {
		return SBVC_MARK_NOT_FOUND;
	}

	// check CID
	MS_U8 sboot_cid = sboot_buf[mark_loc+VER_MARK_LEN];
	printf("sboot cid: %d\n", sboot_cid);

	if ((sboot_cid != 0) && (sboot_cid != 0x41)) {
		return SBVC_INVALID_CID;
	}

	MS_U8 board_cid = read_board_cid();
	printf("board_cid: %d\n", board_cid);

	if ((board_cid != 0) && (board_cid != 0x41)) {
		return SBVC_INVALID_CID;
	}

	// check DDR
	MS_U8 board_ddr = read_ddr_board_id();
	printf("board_ddr:%d\n", board_ddr);

	// use board_ddr_gpio
	MS_U8 board_ddr_gpio = read_ddr_gpio();
	printf("board_ddr_gpio:%d\n", board_ddr_gpio);

	if (board_ddr_gpio == INVALID_DDR) {
		return SBVC_INVALID_DDR;
	}

	MS_U8 sboot_ddr = sboot_buf[mark_loc+VER_MARK_LEN+1];
	printf("sboot ddr:%d\n", sboot_ddr);

	if ( (sboot_ddr != DDR3_NUM) && (sboot_ddr != DDR4_NUM) &&
	     (sboot_ddr != DDR3_NUM + DDR4_NUM) ) {
		return SBVC_INVALID_DDR;
	}

        if (board_cid != sboot_cid) {
                return SBVC_CID_NOT_MATCH;
        }

	if ( (sboot_ddr != DDR3_NUM+DDR4_NUM) && (sboot_ddr != board_ddr_gpio) ) {
		return SBVC_DDR_NOT_MATCH;
	}

	uchar *sboot_dev = NULL;
	// allocate memory for sboot image on the device
	sboot_dev = (uchar*)calloc(1, MAX_SBOOT_SIZE);
	if (!sboot_dev) {
		return SBVC_NO_MEM;
	}

	char mmc_command[64] = {0};
	int sboot_len_dev = (sboot_len < MAX_SBOOT_SIZE) ? sboot_len : MAX_SBOOT_SIZE;
	snprintf(mmc_command, 64, "mmc read.boot 1 0x%08x, 0, 0x%08x",
		sboot_dev, sboot_len_dev);
	printf("\n%s\n", mmc_command);
	if (run_command(mmc_command, 0) < 0) {
		free(sboot_dev);
		return SBVC_READ_ERR;
	}

#ifdef SBOOT_UPDATE_CHECK_HASH0
	// check hash0
	if (memcmp(sboot_dev, sboot_buf, HASH0_SIZE) != 0) {
		free(sboot_dev);
		return SBVC_HASH0_ERR;
	}
#endif

	// find the version mark
	int mark_loc_dev = 0;
	while ( (mark_loc_dev <= sboot_len_dev - VER_MARK_LEN - 4) &&
		strncmp(sboot_dev+mark_loc_dev, SBOOT_VER_MARK, VER_MARK_LEN) ) {
		mark_loc_dev++;
	}
	if (mark_loc_dev > sboot_len_dev - VER_MARK_LEN - 4) {
		free(sboot_dev);
		return SBVC_DEV_MARK_NOT_FOUND;
	}

	unsigned short sboot_ver, sboot_ver_dev;
	sboot_ver = *(unsigned short*)(sboot_buf+mark_loc+VER_MARK_LEN+2);
	sboot_ver_dev = *(unsigned short*)(sboot_dev+mark_loc_dev+VER_MARK_LEN+2);
	printf("\n%s: sboot img version: %d, dev version: %d\n",
		__FUNCTION__, sboot_ver, sboot_ver_dev);
	if (sboot_ver == sboot_ver_dev) {
		free(sboot_dev);
		return SBVC_SAME_VER;
	}
	free(sboot_dev);

	return SBVC_MATCH;
}
