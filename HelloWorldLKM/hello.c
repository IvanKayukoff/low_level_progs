#include<linux/init.h>
#include<linux/module.h>

MODULE_LICENSE("MIT");
MODULE_AUTHOR("Vanchesky");
MODULE_DESCRIPTION("This is a HelloWorld LKM implementation");

static int hello_init(void) {
	printk(KERN_ALERT "Hello world!\n");
	return 0;
}

static void hello_exit(void) {
	printk(KERN_ALERT "Goodbye cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);

