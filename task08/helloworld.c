#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/printk.h>
#include <linux/jiffies.h>

#define myassignID "a89cef62eae7"
#define idLen 13

static struct dentry *my_dir;
static struct dentry *my_id_file;

static ssize_t my_misc_read(struct file *file, char __user *buffer,
			    size_t count, loff_t *ppos)
{
	return simple_read_from_buffer(buffer, count, ppos,
				       myassignID, sizeof(myassignID));
}

static ssize_t my_misc_write(struct file *file, const char __user *buffer,
			     size_t count, loff_t *ppos)
{
	char wbuffer[idLen];
	ssize_t retval;

	if (count != idLen)
		return -EINVAL;

	retval = simple_write_to_buffer(wbuffer, sizeof(myassignID),
					ppos, buffer, count - 1);

	if (retval < 0)
		return retval;

	if (strncmp(myassignID, wbuffer, (idLen - 1)))
		return -EINVAL;

	return count;
}

static const struct file_operations my_misc_fops = {
	.owner = THIS_MODULE,
	.read = my_misc_read,
	.write = my_misc_write,
};

static int __init module_startup(void)
{
	my_dir = debugfs_create_dir("eudyptula", NULL);
	if (!my_dir) {
		pr_info("Failed to create the directory under /debugfs\n");
		return -ENODEV;
	}

	my_id_file = debugfs_create_file("id", 0666, my_dir, NULL,
					 &my_misc_fops);
	if (!my_id_file) {
		pr_info("Failed to create a file under /debugfs/eudyptula\n");
		return -ENODEV;
	}

	my_id_file = debugfs_create_u32("jiffies", 0444, my_dir,
					     (u32*)&jiffies);
	if (!my_id_file) {
		pr_info("Failed to create a jiffies file under /debugfs/eudyptula\n");
		return -ENODEV;
	}

	return 0;
}

static void __exit module_cleanup(void)
{
	debugfs_remove_recursive(my_dir);
}

module_init(module_startup);
module_exit(module_cleanup);
MODULE_AUTHOR("Sandesh K A");
MODULE_LICENSE("GPL v2");

