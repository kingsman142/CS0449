/*
 * e_digits minimal kernel module - /dev version
 *
 * James Hahn <jrh160@pitt.edu>
 *
 */

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>

#include <asm/uaccess.h>

#include "e.h"

/*
 * e_read is the function called when a process calls read() on
 * /dev/e.  It writes the correct number digits of e to the buffer passed in the
 * read() call.
 */

static ssize_t e_read(struct file * file, char * buf, 
			  size_t count, loff_t *ppos)
{
	e(buf, count);
	int len = strlen(buf); /* Don't include the null byte. */
	/*
	 * We only support reading the whole string at once.
	 */
	if (count < len)
		return -EINVAL;
	/*
	 * If file position is non-zero, then assume the string has
	 * been read and indicate there is no more data to be read.
	 */
	if (*ppos != 0)
		return 0;
	/*
	 * Tell the user how much data we wrote.
	 */
	*ppos = len;

	return len;
}

/*
 * The only file operation we care about is read.
 */

static const struct file_operations e_fops = {
	.owner		= THIS_MODULE,
	.read		= e_read,
};

static struct miscdevice e_dev = {
	/*
	 * We don't care what minor number we end up with, so tell the
	 * kernel to just pick one.
	 */
	MISC_DYNAMIC_MINOR,
	/*
	 * Name ourselves /dev/e_driver.
	 */
	"e_driver",
	/*
	 * What functions to call when a program performs file
	 * operations on the device.
	 */
	&e_fops
};

static int __init
e_init(void)
{
	int ret;

	/*
	 * Create the "e" device in the /sys/class/misc directory.
	 * Udev will automatically create the /dev/e device using
	 * the default rules.
	 */
	ret = misc_register(&e_dev);
	if (ret)
		printk(KERN_ERR
		       "Unable to register \"e digits\" misc device\n");

	return ret;
}

module_init(e_init);

static void __exit
e_exit(void)
{
	misc_deregister(&e_dev);
}

module_exit(e_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("James Hahn <jrh160@pitt.edu>");
MODULE_DESCRIPTION("\"e digits\" minimal module");
MODULE_VERSION("dev");
