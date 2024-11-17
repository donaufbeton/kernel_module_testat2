/*
 * Example to run a function every 1000 ms.
 *
 * Author: Rainer Keller, HS-Esslingen
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/workqueue.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rainer Keller");
MODULE_DESCRIPTION("Module to run work_handler recurrently");

#define DELAY_MS 1000

/*
 * Function definitions
 */
static void work_handler(struct work_struct *w);

/*
 * Data declarations
 */
static struct workqueue_struct * wq = NULL;
static DECLARE_DELAYED_WORK(dwork, work_handler);
static unsigned long delay;

static void work_handler(struct work_struct *w) {
    static int times = 0;
    printk(KERN_DEBUG "work_handler runs w:%pX the %d. time\n", w, times++); // rather pr_debug()
    // Do some work, ...
}



__init int init_module(void) {
    printk(KERN_DEBUG "Hello from module\n"); // rather pr_debug()
    delay = msecs_to_jiffies(DELAY_MS);
    wq = alloc_workqueue("test", WQ_UNBOUND, 1);
    if (NULL == wq) {
        pr_err("Cannot allocate workqueue");
        return -1;
    }
    queue_delayed_work(wq, &dwork, delay);
    return 0;
}

__exit void cleanup_module(void)
{
    cancel_delayed_work_sync(&dwork);
    destroy_workqueue(wq);
}

