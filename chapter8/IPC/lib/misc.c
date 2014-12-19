#include "type.h"
#include "const.h"
#include "tty.h"
#include "console.h"
#include "protect.h"
#include "proc.h"
#include "proto.h"
#include "global.h"

PUBLIC char *i2a(int num, int base, char **pos)
{
	int remain = num % base;
	int result = num / base;

	if (result) {
		i2a(result, base, pos);
	}

	*((*pos)++) = remain >= 10 ? (remain - 10 + 'A') : (remain + '0');

	return *pos;
}

PUBLIC void spin(char *func_name)
{
	printf("\n\nspining in %s%s", func_name);

	while (1) {}
}

PUBLIC void assert_failure(char *exp, char *file, char *base_file, int line)
{
	printf("%cassert(%s) failed: file: %s, base_file: %s, line: %d",
	       MAG_CH_ASSERT,
	       exp, file, base_file, line);
	/*printf("%c%s\n", MAG_CH_ASSERT, exp);*/
	/*printf("\n\n\n\n\n%d\n", __LINE__);*/

	spin("assert_failure()");	
	/*__asm__ __volatile__("ud2");*/
}

PUBLIC void reset_message(MESSAGE *m)
{
	memset(m, 0, sizeof(m));
}

PRIVATE void block(PROCESS *p)
{
	assert(p->e_flags);
	schedule();
}

PRIVATE void unblock(PROCESS *p)
{
	assert(p->e_flags == 0);
}

PRIVATE int msg_send(PROCESS *current, int src, MESSAGE *m)
{
	PROCESS *p_src = &(proc_table[src]);
	PROCESS *sender;

	if (p_src->e_flags == RECEIVING &&
	    (p_src->p_recvfrom == proc2pid(current) ||
	     p_src->p_recvfrom == ANY)) { //接收目标表示它正处于接收状态，而且接收目标的对象即使当前进程或者是任意进程
		memcpy(p_src->p_msg,
		       sender->p_msg,
		       sizeof(MESSAGE)); //传递消息体
		p_src->e_flags &= ~(RECEIVING); //消除接收目标的接收状态
		p_src->p_msg = 0; //清空接收目标的消息指针
		p_src->p_recvfrom = NO_TASK; //接收目标的接收过程完成，清除
		unblock(p_src);
	} else {
		sender->e_flags &= SENDING;
		sender->p_sendto = src;
		sender->p_msg = m;

		if (p_src->q_sending) {
			PROCESS *p = p_src->q_sending;
			while (p->next_sending) {
				p = p->next_sending;
			}
			p->next_sending = sender;
		}
		block(sender);
	}
}

PRIVATE int msg_receive(PROCESS *current, int src, MESSAGE *m)
{
	PROCESS *p_who_wanna_receive = current;
	PROCESS *prev;
	PROCESS *p_from;
	int copyok = 0;
	
	if (src == ANY) {
		if (p_who_wanna_receive->q_sending) {
			copyok = 1;
			p_from = p_who_wanna_receive->q_sending;
		}
	} else if (src >= 0 && src < NR_TASKS + NR_PROCS) {
		p_from = &(proc_table[src]);

		if ((p_from->e_flags & SENDING) &&
		    (p_from->p_sendto == proc2pid(p_who_wanna_receive))) {
			PROCESS *p = p_who_wanna_receive->q_sending;

			while (p_from != p) {
				prev = p;
				p = p->next_sending;
			}
			copyok = 1;
		}
	}

	if (copyok) {
		if (p_from == p_who_wanna_receive->q_sending) {
			p_who_wanna_receive->q_sending = p_from->next_sending;
			p_from->next_sending = 0;
		} else {
			prev->next_sending = p_from->next_sending;
			p_from->next_sending = 0;
		}
		memcpy(p_from->p_msg, p_who_wanna_receive->p_msg, sizeof(MESSAGE));
		p_from->e_flags &= ~(SENDING);
		p_from->p_sendto = NO_TASK;
		p_from->p_msg = 0;
	} else {
		p_who_wanna_receive->e_flags |= RECEIVING;
		p_who_wanna_receive->p_recvfrom = src;
		p_who_wanna_receive->p_msg = m;

		block(p_who_wanna_receive);
	}
}
