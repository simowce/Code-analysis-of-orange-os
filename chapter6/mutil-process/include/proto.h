/* ./lib/kliba.asm*/
PUBLIC void out_byte(u16 port, u8 value);
PUBLIC void in_byte(u16 port);
PUBLIC void disp_str(char *str);
PUBLIC void disp_color_str(char *str, int color);

/* ./kernel/i8258A.c*/
PUBLIC void init_8259A();
PUBLIC void put_irq_handler(int irq, irq_handler handler);
PUBLIC void spurious_irq();

/* kernel/clock.c */
PUBLIC void clock_handler(int irq);

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

/* kernel/protect.c */
PUBLIC u32 seg2phys(u16 seg);

/* ./lib/string.asm*/

