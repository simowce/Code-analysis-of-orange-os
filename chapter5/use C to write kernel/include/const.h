#ifndef _ORANGES_CONST_H_
#define _ORANGES_CONST_H_

#define PUBLIC
#define PRIVATE static

#define GDT_SIZE 128
#define IDT_SIZE 256

#define INT_M_CTL 0x20
#define INT_M_CTLMASK 0x21
#define INT_S_CTL 0xA0
#define INT_S_CTLMASK 0xA1

#define PRIVILEGE_KRNL	0
#define PRIVILEGE_TASK	1
#define PRIVILEGE_USER	3

#endif
