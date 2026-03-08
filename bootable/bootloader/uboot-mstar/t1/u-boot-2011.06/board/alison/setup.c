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
#include <amzn_secure_boot.h>

#define AMZN_T12_CID 0x40
#define CID_OFFSET   0x1004

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

                // reset the bootmode to 1
                run_command("idme bootmode 1", 0);

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

/*
 * read fos_flags from idme
 */
unsigned long get_fos_flags(void)
{
	unsigned long flags = 0;

	char fos_buf[16];
	int ret = 0;
	ret = idme_get_var_external("fos_flags", fos_buf, sizeof(fos_buf));

	if (ret < 0) {
		printf("get idme fos_flags Error\n");
		return 0;
	}
	flags = simple_strtoul(fos_buf, NULL, 16);

	printf("fos_flags=%x\n", flags);
	return flags;
}

/*
 * Checks whether dm-verity is disabled
 * For locked production device , always return false
 * For unlocked/engineering device, check amazon fos_flags
 *      if bit7 is set, return true
 *      if bit7 is clear, return false
 */
/* TODO: Update code to use FOS_FLAGS_DM_VERITY_OFF inside idme.h */
#define PLATFORM_FOS_FLAGS_DM_VERITY_OFF   (1 << 7)
int amzn_dm_verity_is_off(int unlock_status)
{
	int lock_state;
	lock_state = ((unlock_status == 0)
		&& (amzn_target_device_type() != AMZN_ENGINEERING_DEVICE));

	if (lock_state) {
		/* Locked device: dm-verity is on and cannot be off */
		return 0;
	} else if (get_fos_flags() & PLATFORM_FOS_FLAGS_DM_VERITY_OFF) {
		/*
		 * Unlocked/Engineering device with bit 7 set
		 * in fos_flags, dm-verity is off
		 */
		return 1;
	} else {
		/* dm-verity is on otherwise */
		return 0;
	}
}

int target_is_production()
{
#if UFBL_FEATURE_SECURE_BOOT
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

	/* secure CPU if customer ID is AMZN_T12_CID */
	if (val == AMZN_T12_CID)
		return 1;
	else
		return 0;
}

#define SBOOT_VER_MARK   "YAMA"
#define VER_MARK_LEN     4
#define VER_LEN          2
#define MAX_SBOOT_SIZE 102400 //100K

int is_14key_keypad(void)
{
	return 0;
}

sbvc_result sboot_version_check(uchar* sboot_buf, int sboot_len)
{
	int mark_loc = 0;

	// don't check sboot for non-secure device
	if (read_board_cid() == 0)
		return SBVC_MATCH;

	if (sboot_buf == NULL)
	{
		return SBVC_INVALID_ARG;
	}

	// check sboot version mark
	while ( (mark_loc <= sboot_len - VER_MARK_LEN - VER_LEN) &&
		strncmp(sboot_buf+mark_loc, SBOOT_VER_MARK, VER_MARK_LEN) ) {
		mark_loc++;
	}

	if (mark_loc > sboot_len - VER_MARK_LEN - VER_LEN) {
		return SBVC_MARK_NOT_FOUND;
	}

	// check CID
	MS_U8 sboot_cid = sboot_buf[CID_OFFSET];
	printf("sboot cid: %d\n", sboot_cid);

	if (sboot_cid != AMZN_T12_CID) {
		return SBVC_INVALID_CID;
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

	// find the version mark
	int mark_loc_dev = 0;
	while ( (mark_loc_dev <= sboot_len_dev - VER_MARK_LEN - VER_LEN) &&
		strncmp(sboot_dev+mark_loc_dev, SBOOT_VER_MARK, VER_MARK_LEN) ) {
		mark_loc_dev++;
	}
	if (mark_loc_dev > sboot_len_dev - VER_MARK_LEN - VER_LEN) {
		free(sboot_dev);
		return SBVC_DEV_MARK_NOT_FOUND;
	}

	unsigned short sboot_ver, sboot_ver_dev;
	sboot_ver = *(unsigned short*)(sboot_buf+mark_loc+VER_MARK_LEN);
	sboot_ver_dev = *(unsigned short*)(sboot_dev+mark_loc_dev+VER_MARK_LEN);
	printf("\n%s: sboot img version: %d, dev version: %d\n",
		__FUNCTION__, sboot_ver, sboot_ver_dev);
	if (sboot_ver == sboot_ver_dev) {
		free(sboot_dev);
		return SBVC_SAME_VER;
	}
	free(sboot_dev);

	return SBVC_MATCH;
}

int amzn_get_hardware_id()
{
	int hardware_id = 0;
	int gpio_level = 0;

	gpio_level = mdrv_gpio_get_level(HW_ID_GPIO3);
	hardware_id += gpio_level << 3;
	gpio_level = mdrv_gpio_get_level(HW_ID_GPIO2);
	hardware_id += gpio_level << 2;
	gpio_level = mdrv_gpio_get_level(HW_ID_GPIO1);
	hardware_id += gpio_level << 1;
	gpio_level = mdrv_gpio_get_level(HW_ID_GPIO0);
	hardware_id += gpio_level;

	//printf("hardware_id: %d\n", hardware_id);
	return hardware_id;
}

#define KEIRA_HVT1  "0039001100110017"
#define KEIRA_HVT2  "0039001110110017"
#define KEIRA_HVT3  "0039001200000017"

int amzn_check_model_name()
{
	/* Keira HVT exception */
	char board_id[18] = {0};
	idme_get_var_external("board_id", board_id, sizeof(board_id)-1);
	if ( !strncmp(board_id, KEIRA_HVT1, strlen(KEIRA_HVT1)) ||
	     !strncmp(board_id, KEIRA_HVT2, strlen(KEIRA_HVT2)) ||
	     !strncmp(board_id, KEIRA_HVT3, strlen(KEIRA_HVT3))) {
		return 1;  /* success */
	}

	char model_name_prefix[16] = {0};
	char idme_model_name[256] = {0};
	char *model_file_name = NULL;
	idme_get_var_external("model_name", idme_model_name, sizeof(idme_model_name)-1);
	model_file_name = strrchr(idme_model_name, '/');
	if (model_file_name) {
		model_file_name ++;
		snprintf(model_name_prefix, sizeof(model_name_prefix)-1, "Customer_%x_", amzn_get_hardware_id());
		if (!strnicmp(model_file_name, model_name_prefix, strlen(model_name_prefix))) {
			return 1;  /* success */
		}
	}
	return 0; /* failure */
}
