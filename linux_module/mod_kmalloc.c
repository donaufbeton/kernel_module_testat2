/*
 * Small example to benchmark the Kernel's memory allocation
 *
 * Author: Rainer Keller, HS Esslingen
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

static char * buffer;

unsigned long long int getrdtsc(void)
{
	unsigned int hi, lo;
	__asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
	return (((unsigned long long int)hi << 32) |
		((unsigned long long int)lo));
}

static __init int mod_kmalloc_init(void)
{
	unsigned long long start;
	unsigned long long stop;

	start = getrdtsc();
	buffer = kvmalloc (4096, GFP_KERNEL);
	stop = getrdtsc();

	printk("kmalloc of 4096 Bytes took %lld cycles\n",
		stop - start);

	return 0;
}

static __exit void mod_kmalloc_exit(void)
{
	kfree(buffer);
}

module_init(mod_kmalloc_init);
module_exit(mod_kmalloc_exit);
