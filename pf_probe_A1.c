#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>

static int processId=1;

/* For each probe you need to allocate a kprobe structure */
static struct kprobe kp = {
	.symbol_name	= "handle_mm_fault",
};

/* kprobe pre_handler: called just before the probed instruction is executed */
static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
	if(current->pid==processId){ // check if the processId which is given by is exactly same than it prints
		printk("Page fault for process %d at address %lx \n",current->pid,regs->si);
	}
		return 0;
}
	/* A dump_stack() here will give a stack backtrace */


	static int handler_fault(struct kprobe *p, struct pt_regs *regs, int trapnr)
	{
		pr_info("fault_handler: p->addr = 0x%p, trap #%dn", p->addr, trapnr);
		/* Return 0 because we don't handle the fault. */
		return 0;
	}

static int __init kprobe_init(void)
{
	int ret;
	kp.pre_handler = handler_pre;
//	kp.post_handler = handler_post;
	kp.fault_handler = handler_fault;

	ret = register_kprobe(&kp);
	if (ret < 0) {
		pr_err("register_kprobe failed, returned %d\n", ret);
		return ret;
	}
	pr_info("Planted kprobe at %p\n", kp.addr);
	return 0;
}

static void __exit kprobe_exit(void)
{
	unregister_kprobe(&kp);
	pr_info("kprobe at %p unregistered\n", kp.addr);
}
module_param(processId,int,0);
module_init(kprobe_init);
module_exit(kprobe_exit);
MODULE_LICENSE("GPL");
