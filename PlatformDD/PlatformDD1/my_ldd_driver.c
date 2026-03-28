
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define DRIVER_NAME "my_ldd"
#define CLASS_NAME  "my_ldd_class"

static dev_t dev_number;
static struct cdev my_cdev;
static struct class *my_class;

/* ---------------- File Operations ---------------- */

static int my_open(struct inode *inode, struct file *file)
{
    printk("Device opened\n");
    return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
    printk("Device closed\n");
    return 0;
}

static ssize_t my_read(struct file *file,char __user *buf, size_t len,loff_t *off)
{
    printk("Read called\n");
    return 0;
}

static ssize_t my_write(struct file *file,const char __user *buf,size_t len,loff_t *off)
{
    printk("Write called\n");
    return len;
}

static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = my_open,
    .release = my_release,
    .read    = my_read,
    .write   = my_write,
};

/* ---------------- Probe Function ---------------- */

static int my_probe(struct platform_device *pdev)
{
    int ret;
    printk("Probe function invoked (compatible = my-ldd)\n");

    /* Allocate device number */
    ret = alloc_chrdev_region(&dev_number,0,1,DRIVER_NAME);

    if (ret < 0)
        return ret;

    /* Initialize cdev */
    cdev_init(&my_cdev, &fops);

    ret = cdev_add(&my_cdev,dev_number,1);

    if (ret < 0)
        goto unregister_region;

    /* Create class */
    my_class = class_create(THIS_MODULE, CLASS_NAME);

    if (IS_ERR(my_class))
        goto del_cdev;

    /* Create device node */
    device_create(my_class,NULL,dev_number,NULL,"my_ldd");
    printk("Character device created\n");

    return 0;

del_cdev:
    cdev_del(&my_cdev);

unregister_region:
    unregister_chrdev_region(dev_number, 1);

    return -1;
}

/* ---------------- Remove Function ---------------- */

static int my_remove(struct platform_device *pdev)
{
    printk("Remove function called\n");
    device_destroy(my_class,dev_number);
    class_destroy(my_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_number,1);
    return 0;
}

/* ---------------- Compatibility Match ---------------- */
static const struct of_device_id my_match_table[] = {

    { .compatible = "my-ldd" },
    { }
};

MODULE_DEVICE_TABLE(of, my_match_table);

/* ---------------- Platform Driver ---------------- */
static struct platform_driver my_platform_driver = {

    .probe  = my_probe,
    .remove = my_remove,

    .driver = {
        .name = "my_ldd_driver",
        .of_match_table = my_match_table,
    },
};

/* ---------------- Register Driver ---------------- */
module_platform_driver(my_platform_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LDD Skeleton");
MODULE_DESCRIPTION("Platform Driver with Character Device and DTS");