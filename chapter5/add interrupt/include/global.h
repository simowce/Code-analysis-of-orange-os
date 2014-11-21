#ifdef	GLOBAL_VARIABLES_HERE
#undef	EXTERN
#define EXTERN
#endif

EXTERN	int		disp_pos;
EXTERN	u8		gdt_ptr[6];
EXTERN	DESCRIPTOR	gdt[GDT_SIZE];
EXTERN	u8		idt_ptr[6];
EXTERN	GATE		idt[IDT_SIZE];
