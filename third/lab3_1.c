#include <asm/uaccess.h>    //copy_to_user & copy_from_user
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/time.h>
#include <linux/rtc.h>


MODULE_LICENSE("Dual BSD/GPL");

int year,mon,day,hour,min,sec;
char msg[200];
int length;
size_t size;

void main_func(void) 
{ 
    struct rtc_time tr;   //tr is the avvreviation of translate
    unsigned long  gmt_time,eastus_time,time_diff;
    gmt_time=ktime_get_real();  //get gmt time
    time_diff = 18000000000000;  //time difference between gmt and east of us, unit is nanoseconds
    eastus_time=gmt_time - 18000000000000;  //get time of east of us, change time_diff if you need to change location
    tr = rtc_ktime_to_tm(eastus_time);  //translate raw nanoseconds into real time description
    sec = tr.tm_sec;
    min = tr.tm_min;
    hour = tr.tm_hour;
    year = tr.tm_year;
    day = tr.tm_mday;
    mon = tr.tm_mon;
    
    sprintf(msg, "Hello world, the current date is %02d-%02d-%02d. The current time is %02d:%02d:%02d\n",  mon +1, day, year + 1900, hour, min, sec);
    length=strlen(msg);
    size=sizeof(char)*length;
    
}

static int lab3_dev_open(struct inode *inode, struct file *file)  //This function will be called when we open the misc device file
{
    printk(KERN_INFO "Time device opened\n");
    main_func();
    return 0;
}


static ssize_t lab3_dev_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)   //This function will be called when we read the misc Device file
{
        int temp;
	int rtsize = size;
	if(size)
	 {
		temp = copy_to_user(buf,msg,size);
		size = temp;
		printk(KERN_INFO "Time device is reading\n");
	}

	return rtsize;
     
}



static int lab3_dev_release(struct inode *inodep, struct file *filp)  //This function will be called when we release the misc Device file
{
    printk(KERN_INFO "Time device released\n");
    return 0;
}


static const struct file_operations myfops =      //file operation structure 
{
	.open = lab3_dev_open,
	.read = lab3_dev_read,
	.release = lab3_dev_release,
};



struct miscdevice lab3_dev =   //misc device structure
{
	.minor = MISC_DYNAMIC_MINOR,
	.name = "lab3",
	.fops = &myfops,
};



static int misc_init(void)  // misc module init
{
    misc_register(&lab3_dev);
    printk(KERN_INFO "misc_register module init\n");
    return 0;
}


static void misc_exit(void)  // misc module exit
{
    misc_deregister(&lab3_dev);
    printk(KERN_INFO "misc_register module exit\n");
}
 
module_init(misc_init)
module_exit(misc_exit)
 
