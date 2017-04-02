#include <linux/kernel.h>

#define KERNEL_INFO "<6>"

asmlinkage long sys_hello(void){
	printk(KERNEL_INFO "Hello World!\n");
	return 0;
}
