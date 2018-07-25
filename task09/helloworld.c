#include <linux/module.h>
#include <linux/poll.h>
#include <linux/errno.h>
#include <linux/kobject.h>
#include <linux/jiffies.h>
#include <linux/semaphore.h>
#include <linux/string.h>

#define myassignID "a89cef62eae7"
#define idLen 13

static ssize_t id_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
	char *id_str = myassignID;
	sprintf(buf, id_str, idLen);
	return idLen;
}

static ssize_t id_store(struct kobject *kobj, struct kobj_attribute *attr,
			const char *buf, size_t count)
{
	char *id_str = myassignID;

	if ((count != idLen) || (strncmp(id_str, buf, idLen)))
		return -EINVAL;
	else
		return idLen;
}

static struct kobj_attribute id_attribute = __ATTR_RW(id);//, 0666, id_show, id_store);

static struct attribute *attrs[] = {
	&id_attribute.attr,
	NULL,	
};

static struct attribute_group attr_group = {
	.attrs = attrs,
};

static struct kobject *edu_kobj;

static int __init module_startup(void)
{
	int files;

	edu_kobj = kobject_create_and_add("eudyptula", kernel_kobj);
	if(!edu_kobj)
		return -ENOMEM;

	files = sysfs_create_group(edu_kobj, &attr_group);
	if(files)
		kobject_put(edu_kobj);
	
	return files;
}

static void __exit module_cleanup(void)
{
        kobject_put(edu_kobj);
}

module_init(module_startup);
module_exit(module_cleanup);
MODULE_AUTHOR("Sandesh K A");
MODULE_LICENSE("GPL v2");

