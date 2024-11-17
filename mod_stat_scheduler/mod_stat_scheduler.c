#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/latencytop.h>



static int delay_ms = 0;
static bool running = true;
module_param(delay_ms, int, S_IRUGO);
MODULE_PARM_DESC(delay_ms, "Der Parameter besagt nach wie vielen Millisekunder der Work-Handler seine 'Statistik' ausgibt");

/*
static void latency_records(void) {
    pid_t pid = task_pid_vnr(current);

    printk(KERN_INFO "Latency Records für Kworker (PID: %d):\n", pid);

    
}

*/



// sudo insmod mod_stat_scheduler.ko delay_ms=42


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Peschke Julian, Luncz Dominik");
MODULE_DESCRIPTION("führt periodisch die Methode Work-Handler aus");

static void work_handler(struct work_struct *w);

/*
 * Data declarations
 */

static struct workqueue_struct * wq = NULL;
static DECLARE_DELAYED_WORK(dwork, work_handler);


/*
struct latency_record {
	unsigned long	backtrace[LT_BACKTRACEDEPTH];
	unsigned int	count;
	unsigned long	time;
	unsigned long	max;
    }



*/



static void work_handler(struct work_struct *w) {
    /*
    if (!latencytop_enabled) {
        return 0;
    }
    */  


    //int latency_record[current->latency_record_count];

    //current->latency_record_count
    //(void*)current->latency_record[0].backtrace[0]
    pid_t pid = task_pid_vnr(current);
    static int times = 0;
    printk(KERN_DEBUG "work_handler runs w:%pX the %d. time ..... (PID: %d)\n", w, times++, pid);

    int length = current->latency_record_count;

    for (int i = 0; i < length; i++) {
        printk(KERN_DEBUG "Backtrace an 0: %p\n", (void*) current->latency_record[i].backtrace[0]);
    }

   
    
    /*
    
    
    
    printk(KERN_DEBUG "Count: %u\n", current->latency_record_count);
    printk(KERN_DEBUG "Time: %lu\n", current->latency_record[0].time);
    printk(KERN_DEBUG "Max: %lu\n", current->latency_record[0].max);
    printk(KERN_DEBUG "latency_record an 0: %lu\n", current->latency_record[0]);
    
    */

    if (running) {
        queue_delayed_work(wq, &dwork, msecs_to_jiffies(delay_ms));
    }


}





__init int init_module(void)
{
    printk (KERN_INFO   "Module mit Parameter %d ms geladen \n", delay_ms);


    wq = alloc_workqueue("test", WQ_UNBOUND, 1);
    if (NULL == wq) {
        pr_err("Cannot allocate workqueue");
        return -1;
    }


    queue_delayed_work(wq, &dwork, msecs_to_jiffies(delay_ms));



    return 0;
}

__exit void cleanup_module(void)
{
    printk(KERN_INFO "Module Stat_Scheduler wird beendet");
    running = false;

    cancel_delayed_work_sync(&dwork);
    destroy_workqueue(wq);

}