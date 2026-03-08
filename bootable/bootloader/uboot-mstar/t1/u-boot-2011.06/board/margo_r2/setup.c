#include <common.h>
#include <command.h>
#include <fdt.h>
#include <libfdt.h>
#include <asm/global_data.h>
#include <idme.h>
#include "MsSysUtility.h"

void ft_board_setup(void *blob, bd_t *bd)
{
	struct fdt_header *fdt_ptr = (struct fdt_header *)blob;

	unsigned int newsize = fdt_totalsize(fdt_ptr)
				+ CONFIG_IDME_SIZE;
	fdt_open_into(fdt_ptr, fdt_ptr, newsize);
	idme_device_tree_initialize(fdt_ptr);
	printf("IDME inserted into FDT\n");
}

int amzn_boot(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	if (argc < 2) {
		printf("Invalid arguments\n");
		return -1;
	}

	if (run_command("mmc read.p 0x23000000 dtb 0x0100000", 0) < 0) {
		printf("Failed to load DTB\n");
		return -1;
	}

	if (run_command("fdt addr 0x23000000", 0) < 0) {
		printf("Failed to initialize DTB\n");
		return -1;
	}

	if (run_command("fdt boardsetup", 0) < 0) {
		printf("Failed to setup DTB\n");
		return -1;
	}

	if (!strncmp(argv[2], "recovery", 7)) {
		if (run_command("mmc read.p 0x25000000 recovery 0x01100000", 0)
				< 0) {
			printf("Failed to load recovery image\n");
			return -1;
		}
	} else if (run_command("mmc read.p 0x25000000 boot 0x01100000", 0) < 0) {
		printf("Failed to load image\n");
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

int is_secure_cpu()
{
	/* read Customer ID */
	MS_U32 addr = 0x201f;
	MS_U8 val = ReadByte(addr);

	/* secure CPU if customer ID is 0x41 */
	if (val == 0x41)
		return 1;
	else
		return 0;
}
