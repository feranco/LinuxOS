#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/list.h>

void listingTasksDfs (struct task_struct* task) {
  struct task_struct *taskChild;
  struct list_head *list;
  printk("%s[%d]\n", task->comm, task->pid);
  //https://stackoverflow.com/questions/31273450/invalid-type-argument-of
  list_for_each(list, &task->children) {
    //https://stackoverflow.com/questions/5550404/list-entry-in-linux	
    //https://0xax.gitbooks.io/linux-insides/content/DataStructures/linux-datastructures-1.html
    taskChild = list_entry(list, struct task_struct, sibling);//punta al task contenente la list_head
    listingTasksDfs(taskChild);
  }
}

/* This function is called when the module is loaded. */
int listingTasks_init(void)
{
  struct task_struct *task;

  printk(KERN_INFO "Loading listingTask Module\n");

  printk(KERN_INFO "Listing tasks sorted by PID\n");
  for_each_process(task) {
    printk("%s[%d]\n", task->comm, task->pid);
  }

  printk(KERN_INFO "Listing tasks using DFS\n");
  listingTasksDfs(&init_task);
  return 0;
}

/* This function is called when the module is removed. */
void listingTasks_exit(void) {
	printk(KERN_INFO "Removing listingTasks Module\n");
}

/* Macros for registering module entry and exit points. */
module_init( listingTasks_init );
module_exit( listingTasks_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("listingTask Module");
MODULE_AUTHOR("SGG");

