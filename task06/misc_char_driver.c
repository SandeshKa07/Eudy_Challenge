#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/printk.h>

#define myassignID "a89cef62eae7"
#define idLen 13

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

static struct miscdevice my_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "eudyptula",
	.fops = &my_misc_fops,
};

static int __init my_misc_init(void)
{
	return misc_register(&my_misc_device);
}

static void __exit my_misc_exit(void)
{
	misc_deregister(&my_misc_device);
}

module_init(my_misc_init);
module_exit(my_misc_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sandesh K A");
