#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <asm/param.h>

MODULE_LICENSE("Dual BSD/GPL");

unsigned long initJiffies, exitJiffies;
unsigned long initTime, exitTime;
unsigned long millisecondUnit = 1000 ; //it is 1000 in milliseconds, it is 1 in seconds

static int hello_init(void){
 	printk(KERN_ALERT "Hello, world\n");  //print hello world
 	printk(KERN_ALERT "The value of the tick time is %lu milliseconds\n", millisecondUnit / HZ ); //print tick time in milliseconds 
 	initTime = ktime_get_boottime();
 	initJiffies = jiffies;
 	return 0;
}

static void hello_exit(void){
 	exitTime = ktime_get_boottime();
 	exitJiffies = jiffies;
 	printk(KERN_ALERT "Goodbye, cruel world\n"); //print goodbye world
 	printk(KERN_ALERT "aMethod: The time difference obtained by using the jiffies is %lu senconds\n", (exitJiffies - initJiffies) / HZ); //print the time between the insertion and removal of the module in seconds with jiffies
 	printk(KERN_ALERT "bMethod: The time difference obtained by reading the timer is %lu nanoseconds\n", (exitTime - initTime)); //print the time between the insertion and removal of the module in seconds with timer
}

module_init(hello_init);
module_exit(hello_exit);
