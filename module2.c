#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

#define AUTHOR_NAME "Matheus Campos da Silva"
#define DRIVER_DESC "Driver de registro do pen drive de Matheus"

// probe function
// called on device insertion if and only if no other driver has beat us to the punch
static int pen_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
        printk(KERN_INFO "[*] Pen drive de Matheus (%04X:%04X) plugged\n", id->idVendor, id->idProduct);
        return 0; // indicates we will manage this device
}

// disconnect
static void pen_disconnect(struct usb_interface *interface)
{
        printk(KERN_INFO "[*] Pen drive de Matheus removido\n");
}

// usb_device_id
static struct usb_device_id pen_table[] = {
    // 090c:1000
    {USB_DEVICE(0x03f0, 0x104a)}, // information is obtained using "lsusb" at the command line
    {},
};

MODULE_DEVICE_TABLE(usb, pen_table);

// usb driver
static struct usb_driver pen_driver = {
    .name = "Pen drive de Matheus",
    .id_table = pen_table, // usb_device_id
    .probe = pen_probe,
    .disconnect = pen_disconnect,
};

static int __init pen_init(void)
{
        int ret = -1;
        printk(KERN_INFO "[*] Construtor do driver do pen drive de Matheus\n");
        printk(KERN_INFO "\tRegistrando driver no kernel\n");
        ret = usb_register(&pen_driver);
        printk(KERN_INFO "\tRegistro concluído\n");
        return ret;
}

static void __exit pen_exit(void)
{
        // unregister
        printk(KERN_INFO "[*] Desconstrutor do driver do pen drive de Matheus\n");
        usb_deregister(&pen_driver);
        printk(KERN_INFO "\tdesregistro concluído\n");
}

module_init(pen_init);
module_exit(pen_exit);

MODULE_AUTHOR(AUTHOR_NAME);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
