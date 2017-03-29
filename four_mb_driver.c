#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#define MAJOR_NUMBER 62

#define MEM_SIZE 4096

/* forward declaration */
int four_open(struct inode *inode, struct file *filep);
int four_release(struct inode *inode, struct file *filep);
ssize_t four_read(struct file *filep, char *buf, size_t
count, loff_t *f_pos);
ssize_t four_write(struct file *filep, const char *buf,
size_t count, loff_t *f_pos);
static void four_exit(void);

/* definition of file_operation structure */
struct file_operations onebyte_fops = {
     read:     four_read,
     write:    four_write,
     open:     four_open,
     release: four_release
};

char *onebyte_data = NULL;
char *fourmb_data = NULL;

int four_open(struct inode *inode, struct file *filep)
{
     return 0; // always successful
}

int four_release(struct inode *inode, struct file *filep)
{
     return 0; // always successful
}

ssize_t four_read(struct file *filep, char *buf, size_t count, loff_t *f_pos)
{    
    int result = 0;
    result = copy_to_user(buf, onebyte_data, 1);
    if (result != 0) {
        printk(KERN_ALERT "onebyte device read failed!\n");
	return 0;
    }
    if (*f_pos == 0) {
	*f_pos+=1;
	return 1;
    } else {
	return 0;
    }
}

ssize_t four_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos)
{
   int result = 0;
   int return_value = 0;
   if (count >= 1) {
	result = copy_from_user(onebyte_data, buf, 1);	
	if (result == 0) {
	    *f_pos += 1;
	    return_value = 1;
	} else {
	    return_value = 1;
        }
   }
   if (count > 1) {
	return_value = -ENOSPC;
   }
   return return_value;
}

static int four_init(void)
{
    int result;
     // register the device
     result = register_chrdev(MAJOR_NUMBER, "fourmb", &onebyte_fops);
     if (result < 0) {
         return result;
     }
     // allocate one byte of memory for storage
     // kmalloc is just like malloc, the second parameter is

   // the type of memory to be allocated.
     // To release the memory allocated by kmalloc, use kfree.
     fourmb_data = kmalloc(MEM_SIZE * sizeof(char), GFP_KERNEL);

     if (!fourmb_data) {
          four_exit();
          // cannot allocate memory
          // return no memory error, negative signify a failure
         return -ENOMEM;
	}

     // initialize the value to be X
     //*onebyte_data = 'X';
     printk(KERN_ALERT "This is a onebyte device module\n");
     return 0;
}

static void four_exit(void)
{
     // if the pointer is pointing to something
     if (onebyte_data) {
          // free the memory and assign the pointer to NULL
          kfree(fourmb_data);
         fourmb_data = NULL;
    }
     // unregister the device
     unregister_chrdev(MAJOR_NUMBER, "fourmb");
     printk(KERN_ALERT "Onebyte device module is unloaded\n");
}

MODULE_LICENSE("GPL");
module_init(four_init);
module_exit(four_exit);
