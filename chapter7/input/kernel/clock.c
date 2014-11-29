#include "type.h"
#include "const.h"
#include "proto.h"
#include "protect.h"
#include "proc.h"
#include "global.h"

PUBLIC void clock_handler(int irq)
{
	ticks++;
	p_proc_ready->ticks --;

	if (k_reenter != 0) {
		return ;
	}

	if (p_proc_ready->ticks > 0) {
		return ;
	}

	schedule();
}

PUBLIC void milli_delay(int milli_sec)
{
	int t = get_ticks();

	while (((get_ticks() - t) * 1000 / HZ) < milli_sec) {}
}

PUBLIC void init_clock()
{
	out_byte(TIMER_MODE, RATE_GENERATOR);
	out_byte(TIMER0, (TIMER_FREQ / HZ));
	out_byte(TIMER0, ((TIMER_FREQ / HZ) >> 8));

	put_irq_handler(CLOCK_IRQ, clock_handler);
	enable_irq(CLOCK_IRQ);
}
