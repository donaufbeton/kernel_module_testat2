/*
 * Small example to benchmark a Tasklet (former Bottom Halve Handler)
 *
 * Author: Rainer Keller, HS Esslingen
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/time.h>

/* Local variables */
static unsigned long long int module_start = 0;

#if defined(__x86_64__) || defined(__i386__) || 0
/* Local function declarations */
static inline unsigned long long int getrdtsc(void) __attribute__((__always_inline__));

/* Local function definitions */
static inline unsigned long long int getrdtsc(void)
{
	unsigned int hi, lo;
	__asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
	return (((unsigned long long int)hi << 32) |
		((unsigned long long int)lo));
}

#  define  gettime     getrdtsc
#else // All other Architectures
static inline unsigned long long int gettime(void) {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return (unsigned long long int)ts.tv_sec*(1000ull*1000*1000) + (unsigned long long int) ts.tv_nsec;
}
#endif /* __x86_64__ or __i386__ */


/* Tasklet Function -- former Bottom Half / bh of an interrupt handler */
static void my_tasklet_func(struct tasklet_struct * unused)
{
	unsigned long long int diff;

	diff = gettime() - module_start;
	printk("My tasklet function was called %llu ticks ago\n",
		diff);
}

DECLARE_TASKLET(my_tasklet, my_tasklet_func);


/* Initialization and Cleanup functions */
static __init int mod_tasklet_timing_init(void)
{
	module_start = gettime();

	tasklet_schedule(&my_tasklet);

	return 0;
}


static __exit void mod_tasklet_timing_exit(void)
{
	/* Stop the tasklet before we exit */
	tasklet_kill( &my_tasklet );
}

module_init(mod_tasklet_timing_init);
module_exit(mod_tasklet_timing_exit);

MODULE_AUTHOR("Rainer Keller");
MODULE_DESCRIPTION("Upon module_init invoke a Tasklet, and measure the time until it's executed");
MODULE_LICENSE("GPL");

