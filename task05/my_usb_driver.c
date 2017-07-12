#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/hid.h>

static struct usb_device_id eudy_usb_id_table[] = {
	/* Interfaceclass = 3, Boot interface subclass = 1, Keyboard = 1 */
	{USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID,
			    USB_INTERFACE_SUBCLASS_BOOT,
			    USB_INTERFACE_PROTOCOL_KEYBOARD)},

	/*Terminating  entry */
	{}
};
MODULE_DEVICE_TABLE(usb, eudy_usb_id_table);

static int __init usb_driver_init(void)
{
	pr_debug("\nHello World Module: USB keyboard is plugged\n");
	return 0;
}

static void __exit usb_driver_exit(void)
{
	pr_debug("\nHello World Module: Good bye !\n");
}

module_init(usb_driver_init);
module_exit(usb_driver_exit);
MODULE_AUTHOR("Sandesh K A");
MODULE_LICENSE("GPL");
