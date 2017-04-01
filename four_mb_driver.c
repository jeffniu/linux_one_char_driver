#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/ioctl.h>


#define SCULL_IOC_MAGIC 'k'
#define SCULL_HELLO _IO(SCULL_IOC_MAGIC, 1)
#define SCULL_READ _IOR(SCULL_IOC_MAGIC, 2, char *)
#define SCULL_WRITE _IOW(SCULL_IOC_MAGIC, 3, char *)
#define SCULL_RW _IOWR(SCULL_IOC_MAGIC, 4, char *)

#define SCULL_IOC_MAXNR 10

#define MAJOR_NUMBER 62

#define MEM_SIZE 4000000

/* forward declaration */
int four_open(struct inode *inode, struct file *filep);
int four_release(struct inode *inode, struct file *filep);
ssize_t four_read(struct file *filep, char *buf, size_t
count, loff_t *f_pos);
ssize_t four_write(struct file *filep, const char *buf,
size_t count, loff_t *f_pos);
loff_t four_llseek(struct file *filp, loff_t off, int whence);
static void four_exit(void);

long ioctl_example(struct file *filp, unsigned int cmd, unsigned long arg);

/* definition of file_operation structure */
struct file_operations onebyte_fops = {
     read:     four_read,
     write:    four_write,
     open:     four_open,
     llseek:   four_llseek,
     unlocked_ioctl: ioctl_example,
     release: four_release
};

char *fourmb_data = NULL;
int cur_size = 0; 

char dev_msg[256];

long ioctl_example(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int err = 0, tmp;
	int retval = 0;
	char buff[256];
	int index = 0;
	if (_IOC_TYPE(cmd) != SCULL_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > SCULL_IOC_MAXNR) return -ENOTTY;
    
	if (_IOC_DIR(cmd) & _IOC_READ) {
		err = !access_ok(VERIFY_WRITE, (void __user*)arg, _IOC_SIZE(cmd));
	} else if (_IOC_DIR(cmd) & _IOC_WRITE) {
		err = !access_ok(VERIFY_READ, (void __user*)arg, _IOC_SIZE(cmd));
	}
	if (err) return -EFAULT;
	
	switch(cmd) {
		case SCULL_HELLO:
			printk(KERN_WARNING "hello\n");
			break;
		case SCULL_READ:
			copy_to_user((char *)arg, dev_msg, 256);
			break;
		case SCULL_WRITE:
			copy_from_user(dev_msg, (char *)arg , 256);
			printk(KERN_ALERT "SCULL_WRITE dev_msg: %s", dev_msg);
			break;
		case SCULL_RW:
			copy_from_user(buff, (char *)arg , 256);
			copy_to_user((char *)arg, dev_msg, 256);
			for (index = 0; index < 256; index++) {
				dev_msg[index] = buff[index];
			}
			printk(KERN_ALERT "SCULL_RW dev_msg: %s", dev_msg);
			break;
		default:
			return -ENOTTY;
	}
	return retval;
}


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
            retval = result;
	} else {
	    retval = copyCount-result;
	    if( *f_pos + retval > cur_size) {
              cur_size = *f_pos + retval;
	    }
	    printk(KERN_ALERT "four_write: byte copied: %u, cur_size: %u\n", retval, cur_size);
	    *f_pos += retval;
        }
   } else {
      retval = -ENOSPC;
   }
   //printk(KERN_ALERT "return retval: %u", retval);
   return retval;
}

loff_t four_llseek(struct file *filp, loff_t off, int whence) {
    loff_t newpos;
    switch(whence) {
	case 0: /*SEEK_SET*/
  	    newpos = off;
	    break;
	case 1: /*SEEK_CUR*/
	    newpos = filp->f_pos + off;
	    break;
	case 2: /*SEEK_END*/
	    newpos = cur_size + off;
	    break;
	default:
	    return off-1;
    }
    if (newpos < 0) return off-1;
    filp->f_pos = newpos;
    return newpos;
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
