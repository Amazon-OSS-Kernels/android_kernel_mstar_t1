#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#ifdef CONFIG_CONSOLE_OFF
#include <linux/idme.h>
#include <linux/string.h>
#include <linux/printk.h>
static char *cmdline;
#endif

static int cmdline_proc_show(struct seq_file *m, void *v)
{
#ifdef CONFIG_CONSOLE_OFF
	seq_printf(m, "%s\n", cmdline);
#else
	seq_printf(m, "%s\n", saved_command_line);
#endif
	return 0;
}

static int cmdline_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, cmdline_proc_show, NULL);
}

static const struct file_operations cmdline_proc_fops = {
	.open		= cmdline_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int __init proc_cmdline_init(void)
{
	proc_create("cmdline", 0, NULL, &cmdline_proc_fops);
/* console is configured to off */
#ifdef CONFIG_CONSOLE_OFF
	cmdline = kstrdup(saved_command_line, GFP_KERNEL);
	if (!cmdline) {
		cmdline = saved_command_line;
		goto out;
	}
	/* remove android console from kernel cmdline if fos flag is not set*/
	if (!(idme_get_fos_flags_value() & FOS_FLAGS_CONSOLE_ON)) {
		char *console_cmdline;
		size_t console_cmdline_len;
		size_t remain_len;

		console_cmdline = strstr(cmdline, CONFIG_ANDROID_CONSOLE_CMDLINE);
		/* check whether CONFIG_ANDROID_CONSOLE_CMDLINE is in kernel cmdline */
		if (!console_cmdline)
			goto out;

		console_cmdline_len = strlen(CONFIG_ANDROID_CONSOLE_CMDLINE) + 1;
		remain_len = strlen(console_cmdline) - console_cmdline_len;
		memmove((void *)console_cmdline, (const void *)(console_cmdline + console_cmdline_len), remain_len);
		console_cmdline[remain_len + 1] = '\0';
	}
out:
#endif
	return 0;
}
module_init(proc_cmdline_init);
