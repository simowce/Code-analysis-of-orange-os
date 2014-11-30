/* ./lib/kliba.asm*/
PUBLIC void out_byte(u16 port, u8 value);
PUBLIC u8 in_byte(u16 port);
PUBLIC void disp_str(char *str);
PUBLIC void disp_color_str(char *str, int color);

/* ./kernel/i8258A.c*/
PUBLIC void init_8259A();
PUBLIC void put_irq_handler(int irq, irq_handler handler);
PUBLIC void spurious_irq();

/* kernel/clock.c */
PUBLIC void clock_handler(int irq);
PUBLIC void milli_delay(int milli_sec);
PUBLIC void init_clock();

/* ./kernel/protect.c*/
PUBLIC void init_prot();

/* ./kernel/main.c */
void test_a();
void test_b();
void test_c();

/* ./lib/klib.c */
PUBLIC void delay(int times);

/* kernel/kernel.asm */
void restart();
PUBLIC void sys_call();
PUBLIC int get_ticks();

/* kernel/protect.c */
PUBLIC u32 seg2phys(u16 seg);

/* ./lib/string.asm*/

/* kernel/proc.c */
PUBLIC int sys_get_ticks();

/* kernel/keyboard.c */
PUBLIC void init_keyboard();
PUBLIC void keyboard_handler();
PUBLIC void keyboard_read(TTY *p_tty);

/* kernel/tty.c */
PUBLIC void task_tty();
PUBLIC void in_process(TTY *p_tty, u32 key);

/* kernel/console.c*/
PUBLIC void out_char(CONSOLE *p_con, char ch);
PUBLIC void init_console(TTY *p_tty);
PUBLIC void select_console(int nr_console);
PUBLIC void scroll_screen(CONSOLE *p_con, int direction);
