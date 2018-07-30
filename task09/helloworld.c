#include <linux/module.h>
#include <linux/poll.h>
#include <linux/errno.h>
#include <linux/kobject.h>
#include <linux/jiffies.h>
#include <linux/semaphore.h>
#include <linux/string.h>
#include <linux/kernel.h>

#define myassignID "a89cef62eae7"
#define idLen 13

static DEFINE_SEMAPHORE(foo_semaphore);
static char my_foo_data[PAGE_SIZE];
static int my_foo_len;

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
	down(&foo_semaphore);
	sprintf(buf, my_foo_data, my_foo_len);
	up(&foo_semaphore);
	return my_foo_len;
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
			const char *buf, size_t count)
{
	if (count > PAGE_SIZE)
		return -EINVAL;

	down(&foo_semaphore);
	sprintf(my_foo_data, buf, count);
	my_foo_len = count;
	up(&foo_semaphore);

	return count;
}

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

static ssize_t jiffies_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
	return sprintf(buf, "%lu\n", jiffies);

}

static ssize_t jiffies_store(struct kobject *kobj, struct kobj_attribute *attr,
			const char *buf, size_t count)
{
	return -EINVAL;
}

static struct kobj_attribute id_attribute = __ATTR_RW(id);
static struct kobj_attribute jiffies_attribute =
__ATTR(jiffies, 0444, jiffies_show, jiffies_store);
static struct kobj_attribute my_foo_attribute =
__ATTR(foo, 0644, foo_show, foo_store);

static struct attribute *attrs[] = {
	&id_attribute.attr,
	&jiffies_attribute.attr,
	&my_foo_attribute.attr,
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
	if (!edu_kobj)
		return -ENOMEM;

	files = sysfs_create_group(edu_kobj, &attr_group);
	if (files)
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
