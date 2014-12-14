#include "type.h"
#include "const.h"
#include "protect.h"
#include "tty.h"
#include "console.h"
#include "proc.h"
#include "proto.h"
#include "global.h"

/*PUBLIC int kernel_main()*/
/*{*/
	/*disp_str("-------\"kernel_main\" is running-------\n");*/

	/*PROCESS *p_proc = proc_table;*/
	/*k_reenter = -1;*/

	/*p_proc->ldt_sel = SELECTOR_LDT_FIRST;*/
	/*memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS>>3], sizeof(DESCRIPTOR));*/
	/*p_proc->ldts[0].attr1 = DA_C | PRIVILEGE_TASK << 5;*/
	/*memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS>>3], sizeof(DESCRIPTOR));*/
	/*p_proc->ldts[1].attr1 = DA_DRW | PRIVILEGE_TASK << 5;*/

	/*p_proc->regs.cs = (0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;*/
	/*p_proc->regs.ds = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;*/
	/*p_proc->regs.es = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;*/
	/*p_proc->regs.fs = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;*/
	/*p_proc->regs.ss = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;*/
	/*p_proc->regs.gs = (SELECTOR_KERNEL_GS & SA_RPL_MASK) | RPL_TASK;*/

	/*p_proc->regs.eip = (u32)test_a;*/
	/*p_proc->regs.esp = (u32) task_stack + STACK_SIZE_TOTAL;*/
	/*p_proc->regs.eflags = 0x1202;*/

	/*p_proc_ready = proc_table;*/
	/*restart();*/

	/*while(1) {};*/
/*}*/

PUBLIC void panic(const char *fmt, ...)
{
	int i;
	char buf[256] = {0};

	va_list arg = (va_list)((char *)(&fmt) + 4);
	i = vsprintf(buf, fmt, arg);
	printf("%c!!PANIC!! %s\n", MAG_CH_PANIC, buf);

	__asm__ __volatile__("ud2");
}

PUBLIC int kernel_main()
{
	disp_str("~~~~~~~~~~~ kernel_main ~~~~~~~~~~~~\n");

	TASK *p_task = task_table;
	PROCESS *p_proc = proc_table;
	char *p_task_stack = task_stack + STACK_SIZE_TOTAL;
	u16 selector_ldt = SELECTOR_LDT_FIRST;
	int i;
	u8 privilege;
	u8 rpl;
	int eflags;

	for (i = 0; i < NR_TASKS + NR_PROCS; i++) {
		if (i < NR_TASKS) {
			p_task = task_table + i;
			privilege = PRIVILEGE_TASK;
			rpl = RPL_TASK;
			eflags = 0x1202;
		} else {
			p_task = user_proc_table + i;
			privilege = PRIVILEGE_USER;
			rpl = RPL_USER;
			eflags = 0x202;
		}
		strcpy(p_proc->p_name, p_task->name);
		p_proc->pid = i;
		p_proc->ldt_sel = selector_ldt;
		memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3], sizeof(DESCRIPTOR));
		p_proc->ldts[0].attr1 = DA_C | privilege << 5;
		memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(DESCRIPTOR));
		p_proc->ldts[1].attr1 = DA_DRW | privilege << 5;
		p_proc->regs.cs = ((8 * 0) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ds = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.es = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ss = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.fs = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.gs = (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;

		p_proc->regs.eip = (u32)p_task->initial_eip;
		p_proc->regs.esp = (u32)p_task_stack;
		p_proc->regs.eflags = eflags;

		p_task_stack -= p_task->stacksize;
		p_proc ++;
		p_task ++;
		selector_ldt += 1 << 3;
	}

	proc_table[0].ticks = proc_table[0].priority = 15;
	proc_table[1].ticks = proc_table[1].priority = 5;
	proc_table[2].ticks = proc_table[2].priority = 3;

	proc_table[0].nr_tty = 0;
	proc_table[1].nr_tty = 1;
	proc_table[2].nr_tty = 2;

	k_reenter = 0;
	ticks = 0;

	init_clock();

	p_proc_ready = proc_table;
	restart();

	while(1) {};
}

void test_a()
{
	int i = 0;

	while(1) {
		//printf("<Ticks:%x> ", get_ticks());
		/*printf("A.");*/
		//disp_str("A.");
		milli_delay(1000);
		/*assert(0);*/
	}
}

void test_b()
{
	int i = 1;

	while (1) {
		/*assert(0);*/
		//disp_str("B.");
		/*printf("w\n");*/
		/*printf("%d\n", __LINE__);*/
		/*printf("%s\n", __FILE__);*/
		/*printf("%s\n", __BASE_FILE__);*/
		/*printf("for %x years", i++);*/
		milli_delay(1000000);
	}
}

void test_c()
{
	int i = 150236;
	char str[] = "Hello, w~";

	while (1) {
		/*printf("%d\n\n%c\n\n%s\n\n", i++, 'w', str);*/
		/*printf("%s\n\n%d\n", __FILE__, __LINE__);*/
		//disp_str("C.");
		/*printf("C.\n");*/
		/*printf("%x\n", nr_current_console);*/
		/*printf("%c", 'w');*/
		/*printf("%cDone\n", MAG_CH_ASSERT);*/
		/*printf("%s\n", str);*/
		/*printf("%x\n", i++);*/
		/*printf("%d\n", i);*/
		assert(0);
		/*printf("%s\n", str);*/
		/*printf("%d\n", i++);*/
		milli_delay(1000);
	}
}
