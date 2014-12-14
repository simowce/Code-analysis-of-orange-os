#include "type.h"
#include "const.h"
#include "protect.h"
#include "tty.h"
#include "console.h"
#include "proc.h"
#include "proto.h"
#include "global.h"
#include "keyboard.h"

#define TTY_FIRST (tty_table)
#define TTY_END	(tty_table + NR_CONSOLES)

PRIVATE void tty_do_read(TTY *p_tty)
{
	if (is_current_console(p_tty->p_console)) {
		keyboard_read(p_tty);
	}
}

PRIVATE void tty_do_write(TTY *p_tty)
{
	if (p_tty->inbuf_count) {
		char ch = *(p_tty->p_inbuf_tail);

		p_tty->p_inbuf_tail++;
		if (p_tty->p_inbuf_tail == p_tty->in_buf + TTY_IN_BYTES) {
			p_tty->p_inbuf_tail = p_tty->in_buf;
		}
		p_tty->inbuf_count--;

		out_char(p_tty->p_console, ch);
	}
}

PRIVATE void init_tty(TTY *p_tty) 
{
	p_tty->inbuf_count = 0;
	p_tty->p_inbuf_head = p_tty->p_inbuf_tail = p_tty->in_buf;

	init_screen(p_tty);
}

PUBLIC void task_tty()
{
	TTY *p_tty;

	init_keyboard();

	for (p_tty = tty_table; p_tty < TTY_END; p_tty++) {
		init_tty(p_tty);
	}
	select_console(0);
	while (1) {
		for (p_tty = tty_table; p_tty < TTY_END; p_tty++) {
			tty_do_read(p_tty);
			tty_do_write(p_tty);
			panic("IN TTY");
		}
	}
}

PRIVATE void put_key(TTY *p_tty, u32 key)
{
	if (p_tty->inbuf_count < TTY_IN_BYTES) {
		*(p_tty->p_inbuf_head) = key;
		p_tty->p_inbuf_head++;
		if (p_tty->p_inbuf_head == p_tty->in_buf + TTY_IN_BYTES) {
			p_tty->p_inbuf_head = p_tty->in_buf;
		}
		p_tty->inbuf_count++;
	}
}

PUBLIC void in_process(TTY *p_tty, u32 key)
{
	char output[2] = {0};

	if (!(key & FLAG_EXT)) {
		put_key(p_tty, key);
	} else {
		int raw_code = key & MASK_RAW;

		switch (raw_code) {
		case ENTER:
			put_key(p_tty, '\n');
			break;
		case BACKSPACE:
			put_key(p_tty, '\b');
			break;
		case TAB:
			put_key(p_tty, '\t');
			break;
		case UP:
			if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {
				scroll_screen(p_tty->p_console, SCR_DN);
			} else {
				
			}
			break;
		case DOWN:
			if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {
				scroll_screen(p_tty->p_console, SCR_UP);
			}
			break;
		case F1:
		case F2:
		case F3:
		case F4:
		case F5:
		case F6:
		case F7:
		case F8:
		case F9:
		case F10:
		case F11:
		case F12:
			if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {
				select_console(raw_code - F1);
			}
			break;
		default:
			break;
		}
	}
}

PUBLIC void tty_write(TTY *p_tty, char *buf, int len)
{
	char *p = buf;
	int i = len;

	while(i) {
		out_char(p_tty->p_console, *p++);
		i--;
	}
}

PUBLIC int sys_write_system(char *buf, int len, PROCESS *p_proc)
{
	tty_write(&tty_table[p_proc->nr_tty], buf, len);
	return 0;
}

PUBLIC int sys_printx(int _unused1, int _unused2, char *s, PROCESS *p_proc)
{
	char ch;
	char *p;
	char reenter_err[] = "? k_reenter is incorrect for unknown reason.";
	reenter_err[0] = MAG_CH_PANIC;

	if (k_reenter == 0)
		p = va2la(proc2pid(p_proc), s);
	else if (k_reenter > 0)
		p = s;
	else
		p = reenter_err;


	if (*p == MAG_CH_PANIC ||
	   (*p == MAG_CH_ASSERT && p_proc_ready < &proc_table[NR_TASKS])) {
		disable_int();
		char *v = (char *)(V_MEM_BASE);
		const char *q = p + 1;

		while (v < (char *)(V_MEM_BASE + V_MEM_SIZE)) {
			*v++ = *q++;
			*v++ = RED_CHAR;
			if (!*q) {
				while (((int)v - V_MEM_BASE) % (SCREEN_WIDTH * 16)) {
					v++;
					*v++ = RED_CHAR;
				}
				q = p + 1;
			}
		}

		__asm__ __volatile__ ("hlt");
	}

	while ((ch = *p++)) {
		if (ch == MAG_CH_PANIC || ch == MAG_CH_ASSERT) {
			continue;
		}
		out_char(tty_table[p_proc->nr_tty].p_console, ch);
	}

	return 0;
}
