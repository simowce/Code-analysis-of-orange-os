PUBLIC void out_byte(u16 port, u8 value);
PUBLIC void in_byte(u16 port);
PUBLIC void disp_str(char *str);
PUBLIC void disp_color_str(char *str, int color);
PUBLIC void init_8259A();
PUBLIC void init_prot();

/* ./kernel/main.c */
void test_a();

/* ./lib/klib.c */
PUBLIC void delay(int times);

/* kernel/kernel.asm */
void restart();

/* kernel/protect.c */
PUBLIC u32 seg2phys(u16 seg);

