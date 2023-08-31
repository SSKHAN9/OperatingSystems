#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/list.h>

/* Macros for getting information from the user */
static int pid = 0;
module_param(pid, int, 0);
MODULE_PARM_DESC(pid, "Starting PID");

void print_processes(void) {

    struct task_struct* task;
    struct task_struct* parent;
    struct task_struct* child;
    struct list_head* head;

    // Loop over scheduled tasks
    for_each_process(task) {

        if(task->pid >= pid) {

            // Process Information
            printk(KERN_INFO "=======================================================\n"
                             "PROCESS NAME: %s\n"
                             "PID: %d\n"
                             "STATE: %ld\n"
                             "PRIORITY: %d\n"
                             "STATIC PRIORITY: %d\n"
                             "NORMAL PRIORITY: %d\n",
                    task->comm, task->pid, task->__state, task->prio,
                    task->static_prio, task->normal_prio);

            // Parent task check
            if(task->parent != NULL) {

                // Parent Task
                parent = task->parent;

                // Parent Information
                printk(KERN_INFO "PARENT ------------------------------------------------------\n"
                                 "PROCESS NAME: %s\n"
                                 "PID: %d\n"
                                 "STATE: %ld\n"
                                 "PRIORITY: %d\n"
                                 "STATIC PRIORITY: %d\n"
                                 "NORMAL PRIORITY: %d\n",
                        parent->comm, parent->pid, parent->__state, parent->prio,
                        parent->static_prio, parent->normal_prio);
            }

            // Loop over all children tasks
            list_for_each_entry(child, &task->children, sibling) {

                // Child Information
                printk(KERN_INFO "CHILD -------------------------------------------------------\n"
                                 "PROCESS NAME: %s\n"
                                 "PID: %d\n"
                                 "STATE: %ld\n"
                                 "PRIORITY: %d\n"
                                 "STATIC PRIORITY: %d\n"
                                 "NORMAL PRIORITY: %d\n",
                        child->comm, child->pid, child->__state, child->prio,
                        child->static_prio, child->normal_prio);
            }
        }
    }
}

/* This function is called when the module is loaded. */
int KhanLKM_init(void) {

    printk(KERN_INFO
    "LKM loaded\n"
    "Starting PID: %d\n", pid);

    print_processes();

    return 0;
}

/* This function is called when the module is removed. */
void KhanLKM_exit(void) {

    printk(KERN_INFO "Removing LKM\n");
}

/* Macros for registering module entry and exit points. */
module_init( KhanLKM_init );
module_exit( KhanLKM_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LKM assignment");
MODULE_AUTHOR("Saad Khan");

