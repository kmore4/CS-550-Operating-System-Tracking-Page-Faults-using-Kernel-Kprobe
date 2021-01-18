#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/time.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/math64.h>
MODULE_LICENSE("GPL");

static int print_line(void);

int processID;
module_param(processID,int,0);

/* For each probe you need to allocate a kprobe structure */
static struct kprobe kp = {
	.symbol_name	= "handle_mm_fault",
};


static int index1 = 0;

typedef struct process_data{
	unsigned long virtual_address;
	long nsec_time;
}process_data;

struct process_data p_data[2000];

//long unsigned int virtual_address_buffer[2000];
//long int nsec_time_buffer[2000];

/* kprobe pre_handler: called just before the probed instruction is executed */
static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
	// struct timespec ts;
	ktime_t ts;
	if(current->pid == processID)
	{
		// k_(&ts);
		ts = ktime_get();
		if(index1>=2000)	index1 = 0;
		p_data[index1].virtual_address = regs->si;
		// p_data[index1].nsec_time = ts.tv_nsec;
		p_data[index1].nsec_time = (long)ktime_to_ns(ts);
		//virtual_address_buffer[index1] = regs->si;
		//nsec_time_buffer[index1] = ts.tv_nsec;
		index1++;

		 //printk(KERN_INFO "Page handler_pre for process %d at address %lx and time %ld\n",processID, regs->si, (long)ktime_to_ns(ts));
	}

	return 0;
}

static int handler_fault(struct kprobe *p, struct pt_regs *regs, int trapnr)
{
	pr_info("fault_handler: p->addr = 0x%p, trap #%dn", p->addr, trapnr);
	/* Return 0 because we don't handle the fault. */
	return 0;
}

static int print_line(void)
{
	int i =0;
	int loopv;
	unsigned long vm_max;
	long time_max;
	unsigned long vm_min;
	long time_min;
	unsigned long vm_range;
	long time_range;
	long address_in_each;
	long time_in_each;
	long vm_distance;
	long time_distance;
	char graph[30][71];
	int l;
	int m;
	int loopv2;
	int x_coordinate;
	int y_coordinate;
	//long virtual_address_buffer[30];
	//long nsec_time_buffer[70];
	//int buffer_count1;
	//int buffer_count2;
	int print_x;
	int print_y;
	int count;
	count = 0;
	address_in_each = 0;
	time_in_each = 0;
	x_coordinate = 0;
	y_coordinate = 0;

	for(i=0;i<2000;i++)
	{
		if(p_data[i].virtual_address != 0)
		{
			count += 1;
			//printk(KERN_INFO "Page fault print_line for process %d at address %lx and time %ld\n",processID, p_data[i].virtual_address,p_data[i].nsec_time);
		}
	}

	for(l=0;l<30;l++)
	{
		for(m=0;m<70;m++)
		{
			graph[l][m] = ' ';
		}
		graph[l][70] = '\0';
	}

	printk(KERN_INFO "hi grapgh print\n");

	vm_max = p_data[0].virtual_address;
	vm_min = p_data[0].virtual_address;

	time_max = p_data[0].nsec_time;
	time_min = p_data[0].nsec_time;

	for(loopv=0;loopv<2000;loopv++)
	{
		if(p_data[loopv].virtual_address > vm_max)
		{
			vm_max = p_data[loopv].virtual_address;
		}
		if(p_data[loopv].virtual_address < vm_min)
		{
			vm_min = p_data[loopv].virtual_address;
		}

		if(p_data[loopv].nsec_time < time_min)
		{
			time_min = p_data[loopv].nsec_time;
		}
		if(p_data[loopv].nsec_time > time_max)
		{
			time_max = p_data[loopv].nsec_time;
		}
	}

	vm_range = vm_max - vm_min;
	time_range = time_max - time_min;

	printk(KERN_INFO "max address %lx and  max time %ld\n",vm_max,time_max);

	if(vm_max != 0)
	{
		address_in_each = div64_long(vm_range, 30);
	}
	if(time_max != 0)
	{
		time_in_each = div64_long(time_range, 70);
	}

	printk(KERN_INFO "address in each %lx and time in each %ld\n",address_in_each,time_in_each);
/*	virtual_address_buffer[0] = vm_max;

	for(buffer_count1=1;buffer_count1<30;buffer_count1++)
	{
		virtual_address_buffer[buffer_count1] = virtual_address_buffer[buffer_count1-1] - address_in_each;
		printk(KERN_INFO "address  %lx \n",virtual_address_buffer[buffer_count1]);
	}

	nsec_time_buffer[0] = 0;

	for(buffer_count2=1;buffer_count2<70;buffer_count2++)
	{
		nsec_time_buffer[buffer_count2] = nsec_time_buffer[buffer_count2-1] + time_in_each;
		printk(KERN_INFO "time  %ld \n",nsec_time_buffer[buffer_count2]);
	}

	for(loopv2=0;loopv2<2000;loopv2++)
	{
		buffer_count1=0;
		buffer_count2=0;
		while(virtual_address_buffer[buffer_count1]>=p_data[loopv2].virtual_address && buffer_count1<30)
		{
			buffer_count1 += 1;
		}
		x_coordinate = buffer_count1;
		while(nsec_time_buffer[buffer_count2]<=p_data[loopv2].nsec_time && buffer_count2<70)
		{
			buffer_count2 += 1;
		}
		y_coordinate = buffer_count2;
		printk(KERN_INFO "Coordinates = %d %d\n",x_coordinate,y_coordinate);
		graph[x_coordinate][y_coordinate] = '*';
	}*/

	for(loopv2=0;loopv2<2000;loopv2++)
	{
		vm_distance = (p_data[loopv2].virtual_address) - vm_min;
		time_distance = (p_data[loopv2].nsec_time) - time_min;
		if(time_in_each != 0)
		{
			x_coordinate = div64_long(vm_distance,address_in_each);
		}
		if(address_in_each != 0)
		{
			y_coordinate = div64_long(time_distance,time_in_each);
		}

		if(x_coordinate>0)
		{
			x_coordinate -= 1;
		}
		if(y_coordinate>0)
		{
			y_coordinate -= 1;
		}
		if(x_coordinate != 0 || y_coordinate != 0)
		{
			printk(KERN_INFO "Coordinates = %d %d\n",x_coordinate,y_coordinate);
		}
		graph[x_coordinate][y_coordinate] = '*';
	}

	for(print_x=29;print_x>=0;print_x--)
	{
		pr_cont("%d |",print_x);
		for(print_y=0;print_y<70;print_y++)
		{
			pr_cont(KERN_INFO "%c",graph[print_x][print_y]);
		}
		printk(KERN_INFO " \n");
	}
	printk(KERN_INFO "\t--------------------------------------------------------\n");
	printk(KERN_INFO "\t\t\tTime Interval\n");
	return 0;
}

static int __init kprobe_init(void)
{
	int ret;
	kp.pre_handler = handler_pre;
	//kp.post_handler = handler_post;
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
	int x;
	printk(KERN_INFO "In exit function\n");
	x = print_line();
	if(x==0){
		printk(KERN_INFO "Successful\n");
	}

	else {
		printk(KERN_INFO "Unsuccessful\n");
	}
	unregister_kprobe(&kp);
	pr_info("kprobe at %p unregistered\n", kp.addr);
}

module_init(kprobe_init);
module_exit(kprobe_exit);
