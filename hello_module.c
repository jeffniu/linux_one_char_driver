#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("GPL");

static char *who = "xxx";

module_param(who, charp, S_IRUGO);
MODULE_PARM_DESC(who, "this is your name.");

static int hello_init(void)
{
    printk(KERN_ALERT "Hello, %s\n", who);
    return 0;
}

static void hello_exit(void)
{
    printk(KERN_ALERT "Goodbye, %s\n",who);
}

module_init(hello_init);
module_exit(hello_exit);
