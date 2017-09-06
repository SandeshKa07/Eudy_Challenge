#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/debugfs.h>

static int __init module_startup(void)
{
	my_dir = debugfs_create_dir("eudyptula", NULL);
	if(!my_dir) {
		printk("Failed to create the directory under /debugfs\n");
		return -ENODEV;
	}
	my_id_file = debugfs_create_file("id", 0666, my_dir, myassignID, &my_misc_fops);
	if(!my_id_file) {
		printk("Failed to create a file under /debugfs/eudyptula\n");
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

