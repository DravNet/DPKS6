#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/rculist.h>
#include <linux/slab.h>

// MODULE DATA FOR `modinfo`
MODULE_LICENSE("Dual BSD/GPL");
// HELLO PARAMETER
static uint count = 1;
module_param(count, uint, S_IRUGO); 
MODULE_PARM_DESC(count, "Here is parameter description");
// STRUCT
static LIST_HEAD(list_node_head);
struct time_list{
	struct list_head node_head;
	int event_num;
	ktime_t event_time;
	ktime_t kill_time;
};



static int __init hello_init(void)
{
	if(count == 0 || (count  >= 5 && count <= 10))
		printk(KERN_WARNING "Warning - parameter = %u\n", count);
	
	else if(count > 10) {
		printk(KERN_ERR "Error - parameter > 10\n");
		return -EINVAL;
	}

	else {
		// declaring variables here, since code isn't as pretty as it used to be
		int i = 0; 
		ktime_t start, end;
		struct time_list* alloc_list; // if doesn't work, move kmalloc here

		for(i = 0; i < count; i++){
			BUG_ON(count > 10);

			start = ktime_get();

			printk(KERN_EMERG "Hello, world!\n");
			
			end = ktime_get();
			
			if(i != 3)
			{
				alloc_list = (struct time_list*) kmalloc(sizeof(struct time_list), GFP_KERNEL);
			}
			else
			{
				alloc_list = (struct time_list*) 0;
			}
			
			*alloc_list = (struct time_list){
				.event_num = i,
				.event_time = start,
				.kill_time = end
			};

			list_add_tail(&alloc_list->node_head, &list_node_head);
		}
	}
	return 0;
}

static void __exit hello_exit(void)
{
	struct time_list *md, *tmp;

	printk(KERN_EMERG "Freeing memory");

	list_for_each_entry_safe(md, tmp, &list_node_head, node_head) {
		printk(KERN_EMERG "Freeing memory from event %d", md->event_num);
		pr_info("%lld ns.\n", (long long int) (md->kill_time - md->event_time));
		list_del(&md->node_head);
		kfree(md);
	}

	BUG_ON(!list_empty(&list_node_head));
		
	printk(KERN_EMERG "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
