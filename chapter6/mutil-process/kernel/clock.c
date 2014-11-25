#include "type.h"
#include "const.h"
#include "proto.h"
#include "protect.h"
#include "proc.h"
#include "global.h"

PUBLIC void clock_handler(int irq)
{
	disp_str("#");

	if (k_reenter != 0) {
		disp_str("!!");

		return ;
	}

	p_proc_ready++;
	if (p_proc_ready >= proc_table + NR_TASKS)
		p_proc_ready = proc_table;
}
