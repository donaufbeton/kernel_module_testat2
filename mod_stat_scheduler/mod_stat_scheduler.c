#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
#include <linux/sched.h>




static int delay_ms = 0;
static bool running = true;
module_param(delay_ms, int, S_IRUGO);
MODULE_PARM_DESC(delay_ms, "Der Parameter besagt nach wie vielen Millisekunder der Work-Handler seine 'Statistik' ausgibt");

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

static void work_handler(struct work_struct *w) {
    pid_t pid = task_pid_vnr(current);
    static int times = 0;
    printk(KERN_DEBUG "work_handler runs w:%pX the %d. time ..... (PID: %d)\n", w, times++, pid);

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