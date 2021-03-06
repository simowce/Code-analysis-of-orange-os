#define GLOBAL_VARIABLES_HERE

#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "proc.h"
#include "global.h"

PUBLIC PROCESS proc_table[NR_TASKS];
PUBLIC char task_stack[STACK_SIZE_TOTAL];
PUBLIC TASK task_table[NR_TASKS] = {{task_tty, STACK_SIZE_TTY, "task_tty"},
				    {test_a, STACK_SIZE_TESTA, "test_a"},
				    {test_b, STACK_SIZE_TESTB, "test_b"},
				    {test_c, STACK_SIZE_TESTC, "test_c"}};
PUBLIC irq_handler irq_table[NR_IRQ];

PUBLIC system_call sys_call_table[NR_SYS_CALL] = {sys_get_ticks};
