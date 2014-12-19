
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            const.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef	_ORANGES_CONST_H_
#define	_ORANGES_CONST_H_


/* EXTERN */
#define	EXTERN	extern	/* EXTERN is defined as extern except in global.c */

/* 函数类型 */
#define	PUBLIC		/* PUBLIC is the opposite of PRIVATE */
#define	PRIVATE	static	/* PRIVATE x limits the scope of x */

/* Boolean */
#define	TRUE	1
#define	FALSE	0

/* GDT 和 IDT 中描述符的个数 */
#define	GDT_SIZE	128
#define	IDT_SIZE	256

/* 权限 */
#define	PRIVILEGE_KRNL	0
#define	PRIVILEGE_TASK	1
#define	PRIVILEGE_USER	3
/* RPL */
#define	RPL_KRNL	SA_RPL0
#define	RPL_TASK	SA_RPL1
#define	RPL_USER	SA_RPL3

/* 8259A interrupt controller ports. */
#define	INT_M_CTL	0x20	/* I/O port for interrupt controller         <Master> */
#define	INT_M_CTLMASK	0x21	/* setting bits in this port disables ints   <Master> */
#define	INT_S_CTL	0xA0	/* I/O port for second interrupt controller  <Slave>  */
#define	INT_S_CTLMASK	0xA1	/* setting bits in this port disables ints   <Slave>  */

#define NR_IRQ		16
#define	CLOCK_IRQ	0
#define	KEYBOARD_IRQ	1
#define	CASCADE_IRQ	2	/* cascade enable for 2nd AT controller */
#define	ETHER_IRQ	3	/* default ethernet interrupt vector */
#define	SECONDARY_IRQ	3	/* RS232 interrupt vector for port 2 */
#define	RS232_IRQ	4	/* RS232 interrupt vector for port 1 */
#define	XT_WINI_IRQ	5	/* xt winchester */
#define	FLOPPY_IRQ	6	/* floppy disk */
#define	PRINTER_IRQ	7
#define	AT_WINI_IRQ	14	/* at winchester */

#define NR_SYS_CALL	2

#define TIMER0		0x40
#define TIMER_MODE	0x43
#define RATE_GENERATOR	0x34

#define TIMER_FREQ	1193182L
#define HZ		100

#define FALSE		0
#define TRUE		1

#define CRTC_ADDR_REG	0x3D4
#define CRTC_DATA_REG	0x3D5
#define START_ADDR_H	0xC
#define START_ADDR_L	0xD
#define CURSOR_H	0xE
#define CURSOR_L	0xF
#define V_MEM_BASE	0xB8000
#define V_MEM_SIZE	0x8000

#define NR_CONSOLES	3

#define LED_CODE	0xED
#define KB_ACK		0xFA

#define STR_DEFAULT_LEN	1024

#define BLACK		0x0
#define WHITE		0x7
#define RED		0x4
#define GREEN		0x2
#define BLUE		0x1
#define FLASH		0x80
#define BRIGHT		0x08
#define MAKE_COLOR(x,y) ((x<<4) | y)

#define ASSERT
#ifdef ASSERT
void assert_failure(char *exp, char *file, char *base_file, int line);
#define assert(exp)	if (exp) ;\
	else assert_failure(#exp, __FILE__, __BASE_FILE__, __LINE__)
#else
#define assert(exp)
#endif

#define MAG_CH_ASSERT	'\003'
#define MAG_CH_PANIC	'\002'

#define proc2pid(x) (x - proc_table)

#define INDEX_LDT_RW	1

#define ANY		(NR_TASKS + NR_PROCS + 10)
#define NO_TASK		(NR_TASKS + NR_PROCS + 20)
#define SEND		1
#define RECEIVE		2
#define BOTH		3

#define INTERRUPT	-10

#define SENDING		0x02
#define RECEIVING	0x04

enum {
	HARD_INT = 1,
	GET_TICKS,
}

#define	RETVAL		u.m3.m3i1

#define TASK_SYS	1
#endif /* _ORANGES_CONST_H_ */
