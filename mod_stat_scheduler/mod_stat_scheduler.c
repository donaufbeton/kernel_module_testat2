#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/delay.h>




static int delay_ms = 0;  
static bool running = true;
module_param(delay_ms, int, S_IRUGO);  
MODULE_PARM_DESC(delay_ms, "Der Parameter besagt nach wie vielen Millisekunder der Work-Handler seine 'Statistik' ausgibt");

// sudo insmod mod_stat_scheduler.ko delay_ms=42


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Peschke Julian, Luncz Dominik");
MODULE_DESCRIPTION("führt periodisch die Methode Work-Handler aus");

//extern void work_handler(struct);

__init int init_module(void)
{
	printk (KERN_INFO   "Module mit Parameter %d ms geladen \n", delay_ms);
    int i = 0;
    while (running) {
        
        printk(KERN_INFO "hi was geht bruder %d \n", i);
        i++;
    }




	return 0;
}

__exit void cleanup_module(void)
{
    running = false;
    printk("Module Stat_Scheduler wird beendet");

}

