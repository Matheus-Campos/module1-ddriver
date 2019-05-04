#include <linux/module.h>    // required in any kernel module
#include <linux/kernel.h>    // required in any kernel module
#include <linux/fs.h>        // file operations structure, which of course allows use to open/close,read/write to device
#include <linux/cdev.h>      // this is a char driver; makes cdev available
#include <linux/semaphore.h> // used to access semaphores/ synchronization behaviors
#include <asm/uaccess.h>     // copy_to_use;copy_from_use

// create a structure for our fake device
struct fake_device
{
        char data[100];
        struct semaphore sem;
} virtual_device;

// to later register our device we need a cdev object and some other variables
struct cdev *mcdev;
int major_number; // will store our major number
int ret;          // will be used to hold return values of functions, because the kernel stack is very small

dev_t dev_num; // will hold major number that kernel gives us

#define DEVICE_NAME "module1device"

// called on device_file open
// inode reference to the file on disk and contains information about that file
// struct file represents an abstract open file
int device_open(struct inode *inode, struct file *filp)
{
        // only allow one process to open this device by using a semaphore as mutex
        if (down_interruptible(&virtual_device.sem) != 0)
        {
                printk(KERN_ALERT "module1: could not lock device during open\n");
                return -1;
        }

        printk(KERN_INFO, "module1: opened device\n");
        return 0;
}

// called when user wants to get information from the device
ssize_t device_read(struct file *filp, char *bufStoreData, size_t bufCount, loff_t *curOffset)
{
        // take data from kernel space (device) to user space (process)
        printk(KERN_INFO "module1: reading from device\n");
        ret = raw_copy_to_user(bufStoreData, virtual_device.data, bufCount);
        return ret;
}

// called when user wants to send information to the device
ssize_t device_write(struct file *filp, const char *bufSourceData, size_t bufCount, loff_t *curOffset)
{
        // send data from user to kernel
        printk(KERN_INFO "module1: writing to device\n");
        ret = raw_copy_from_user(virtual_device.data, bufSourceData, bufCount);
        return ret;
}

// called upon user close
int device_close(struct inode *inode, struct file *filp)
{
        // by calling up, which is the opposite of down for semaphore, we release the mutex we obtained at device open
        // this has the effect of allowing other process to use the device now
        up(&virtual_device.sem);
        printk(KERN_INFO "module1: closed device\n");
        return 0;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_close,
    .write = device_write,
    .read = device_read,
};

static int driver_entry(void)
{
        // register our device with the system: a two step process
        // step 1: dynamic allocation to assign our device
        ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
        if (ret < 0)
        {
                printk(KERN_ALERT "module1: failed to allocate a major number\n");
                return ret; // propagate error
        }

        major_number = MAJOR(dev_num);
        printk(KERN_INFO "module1: major number is %d\n", major_number);
        printk(KERN_INFO "\tuse 'mknod /dev/%s c %d 0' for device file\n", DEVICE_NAME, major_number); // dmesg

        // step 2: create and initialize our cdev structure
        mcdev = cdev_alloc();
        mcdev->ops = &fops; // struct file_operations
        mcdev->owner = THIS_MODULE;

        // now that we created cdev, we have to add it to the kernel
        ret = cdev_add(mcdev, dev_num, 1);
        if (ret < 0)
        {
                printk(KERN_ALERT "module1: unable to add cdev to kernel\n");
                return ret;
        }

        // initialize our semaphore
        sema_init(&virtual_device.sem, 1); // initial value of one

        return 0;
}

static void driver_exit(void)
{
        // unregister everything in reverse order
        cdev_del(mcdev);
        unregister_chrdev_region(dev_num, 1);
        printk(KERN_ALERT "module1: unloaded module\n");
}

// inform the kernel where to start and stop with our module/driver
module_init(driver_entry);
module_exit(driver_exit);
