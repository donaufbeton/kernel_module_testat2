/*
 * Simple example to show Linux Kernel Module Programming
 *
 * Author: Rainer Keller, HS-Esslingen
 */
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

__init int init_module(void)
{
	printk (KERN_INFO "Hello world from linux_module\n");
	return 0;
}

__exit void cleanup_module(void)
{
}

