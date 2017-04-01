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

#define MEM_SIZE 4000000

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

char *fourmb_data = NULL;
int cur_size = 0; 

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
    int copiedByteCount = 0;
    int copyCount = count;
    int retval = 0;
    printk(KERN_ALERT "four_read: count: %u, f_pos: %d\n", count, *f_pos);
    
    if (*f_pos+count > cur_size) {
        copyCount = cur_size-(*f_pos);
    }
    result = copy_to_user(buf, fourmb_data+(*f_pos), copyCount);
    if (result < 0) {
        printk(KERN_ALERT "four mb device read failed!\n");
	retval = 0;
    } else {
        copiedByteCount = copyCount - result;
        *f_pos+=copiedByteCount;
        retval = copiedByteCount;
    }
    return retval;
}

ssize_t four_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos)
{
   int result = 0;
   int retval = 0;
   int copyCount = count;
   printk(KERN_ALERT "four_write: count: %u, f_pos: %d\n", count, *f_pos);
   if (count + *f_pos > MEM_SIZE) {
     copyCount = MEM_SIZE - *f_pos;
   }
   if (copyCount >= 1) {
	result = copy_from_user(fourmb_data+(*f_pos), buf, count);	
	if (result < 0) {
	    printk(KERN_ALERT "four_write fail");
            retval = -EFAULT;
	} else {
	    retval = copyCount-result;
	    cur_size = *f_pos + retval;
	    printk(KERN_ALERT "four_write: byte copied: %u, cur_size: %u\n", retval, cur_size);
	    *f_pos += retval;
        }
   } else {
      retval = -EFAULT;
   }
   printk(KERN_ALERT "return retval: %u", retval);
   return retval;
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
     printk(KERN_ALERT "This is a four mb device module\n");
     return 0;
}

static void four_exit(void)
{
     // if the pointer is pointing to something
     if (fourmb_data != NULL) {
          // free the memory and assign the pointer to NULL
          kfree(fourmb_data);
         fourmb_data = NULL;
    }
     // unregister the device
     unregister_chrdev(MAJOR_NUMBER, "fourmb");
     printk(KERN_ALERT "four mb device module is unloaded\n");
}

MODULE_LICENSE("GPL");
module_init(four_init);
module_exit(four_exit);
