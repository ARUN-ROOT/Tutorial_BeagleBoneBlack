#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DRIVER_NAME "my_ldd"
#define CLASS_NAME  "my_ldd_class"

static dev_t dev_number;
static struct cdev my_cdev;
static struct class *my_class;

static int led_gpio;

/* File Operations */

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

static ssize_t my_write(struct file *file,  const char __user *buf, size_t len,loff_t *off)
{
    char value;

    if (copy_from_user(&value, buf, 1))
        return -EFAULT;

    if (value == '1')
    {
        gpio_set_value(led_gpio, 1);
        printk("LED ON\n");
    }
    else if (value == '0')
    {
        gpio_set_value(led_gpio, 0);
        printk("LED OFF\n");
    }

    return len;
}

static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = my_open,
    .release = my_release,
    .write   = my_write,
};

/* Probe Function */

static int my_probe(struct platform_device *pdev)
{
    int ret;

    printk("Probe called\n");

    /* Get GPIO from DTS */

    led_gpio = of_get_named_gpio(pdev->dev.of_node,"led-gpio",0);

    if (!gpio_is_valid(led_gpio))
    {
        printk("Invalid GPIO\n");
        return -ENODEV;
    }

    printk("GPIO number = %d\n", led_gpio);

    /* Request GPIO */

    ret = gpio_request(
            led_gpio,
            "led_gpio");

    if (ret)
        return ret;

    /* Set as output */

    gpio_direction_output(led_gpio,0);

    /* Character device setup */

    ret = alloc_chrdev_region( &dev_number, 0, 1, DRIVER_NAME);

    if (ret < 0)
        return ret;

    cdev_init(&my_cdev, &fops);

    ret = cdev_add(&my_cdev, dev_number, 1);

    if (ret)
        return ret;

    my_class =class_create( THIS_MODULE, CLASS_NAME);

    device_create( my_class, NULL,dev_number, NULL,"my_ldd");

    printk("Character device created\n");

    return 0;
}

/* Remove Function */

static int my_remove(struct platform_device *pdev)
{
    gpio_set_value(led_gpio, 0);

    gpio_free(led_gpio);

    device_destroy(my_class,dev_number);

    class_destroy(my_class);

    cdev_del(&my_cdev);

    unregister_chrdev_region(dev_number,1);

    printk("Driver removed\n");

    return 0;
}

/* Match Table */

static const struct of_device_id my_match[] = {

    { .compatible = "my-ldd" },

    { }
};

MODULE_DEVICE_TABLE(of, my_match);

/* Platform Driver */

static struct platform_driver my_driver = {

    .probe  = my_probe,
    .remove = my_remove,

    .driver = {
        .name = "my_ldd_driver",
        .of_match_table = my_match,
    },
};

module_platform_driver(my_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LDD GPIO");
MODULE_DESCRIPTION("Platform Driver with GPIO LED control");