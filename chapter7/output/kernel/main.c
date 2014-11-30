#include "type.h"
#include "const.h"
#include "protect.h"
#include "tty.h"
#include "console.h"
#include "proto.h"
#include "proc.h"
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

PUBLIC int kernel_main()
{
	disp_str("~~~~~~~~~~~ kernel_main ~~~~~~~~~~~~\n");

	TASK *p_task = task_table;
	PROCESS *p_proc = proc_table;
	char *p_task_stack = task_stack + STACK_SIZE_TOTAL;
	u16 selector_ldt = SELECTOR_LDT_FIRST;
	int i;

	for (i = 0; i < NR_TASKS; i++) {
		strcpy(p_proc->p_name, p_task->name);
		p_proc->pid = i;
		p_proc->ldt_sel = selector_ldt;
		memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3], sizeof(DESCRIPTOR));
		p_proc->ldts[0].attr1 = DA_C | PRIVILEGE_TASK << 5;
		memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(DESCRIPTOR));
		p_proc->ldts[1].attr1 = DA_DRW | PRIVILEGE_TASK << 5;
		p_proc->regs.cs = ((8 * 0) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.ds = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.es = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.ss = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.fs = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.gs = (SELECTOR_KERNEL_GS & SA_RPL_MASK) | RPL_TASK;

		p_proc->regs.eip = (u32)p_task->initial_eip;
		p_proc->regs.esp = (u32)p_task_stack;
		p_proc->regs.eflags = 0x1202;

		p_task_stack -= p_task->stacksize;
		p_proc ++;
		p_task ++;
		selector_ldt += 1 << 3;
	}

	proc_table[0].ticks = proc_table[0].priority = 15;
	proc_table[1].ticks = proc_table[1].priority = 5;
	proc_table[2].ticks = proc_table[2].priority = 3;

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
		//disp_str("A.");
		milli_delay(10);
	}
}

void test_b()
{
	int i = 256;

	while (1) {
		//disp_str("B.");
		milli_delay(10);
	}
}

void test_c()
{
	int i = 512;

	while (1) {
		//disp_str("C.");
		milli_delay(10);
	}
}
