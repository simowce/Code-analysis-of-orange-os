#include "type.h"
#include "const.h"
#include "tty.h"
#include "console.h"
#include "protect.h"
#include "proc.h"
#include "global.h"

PUBLIC int sys_get_ticks()
{
	return ticks;
}

PUBLIC void schedule()
{
	PROCESS *p;
	int greatest_ticks = 0;

	while (!greatest_ticks) {
		for (p = proc_table; p < proc_table + NR_TASKS + NR_PROCS; p++) {
			if (p->ticks > greatest_ticks) {
				greatest_ticks = p->ticks;
				p_proc_ready = p;
			}
		}

		if (!greatest_ticks) {
			for (p = proc_table; p < proc_table + NR_TASKS + NR_PROCS; p++) {
				p->ticks = p->priority;
			}
		}
	}

}

PUBLIC int ldt_seg_linear(PROCESS *p, int idx)
{
	DESCRIPTOR *d = &p->ldts[idx];

	return d->base_high << 24 | d->base_mid << 16 | d->base_low;
}

PUBLIC void *va2la(int pid, void *va)
{
	PROCESS *p = &proc_table[pid];
	u32 seg_base = ldt_seg_linear(p, INDEX_LDT_RW);
	u32 la = seg_base + (u32)va;

	return (void *)la;
}
