#ifndef	_GLOBAL_H_
#define	_GLOBAL_H_

#ifdef	GLOBAL_VARIBLE_HERE
#undef	EXTERN
#define	EXTERN
#endif

#include	"type.h"
#include	"protect.h"
#include 	"proc.h"
#include	"syscall.h"
#include 	"tty.h"

EXTERN	u32			DisPos;
EXTERN	u8			DispColor;
EXTERN	u32			k_reenter;
EXTERN	u8			gdt_ptr[6];
EXTERN	DESCRIPTOR	gdt[GDT_SIZE];
EXTERN	u8			idt_ptr[6];
EXTERN	GATE		idt[IDT_SIZE];

EXTERN	TSS				tss;
EXTERN	PROCESS			proc_table[MAX_TASK_NUM];
EXTERN	PROCESS			user_proc_table[MAX_PROC_NUM];
EXTERN	PROCESS	*		p_proc_ready;	
EXTERN	int				proc_id;
EXTERN	IRQHandle		irq_table[16];
EXTERN	unsigned  int	ticks;
EXTERN	system_call		sys_call_table[];
EXTERN	TTY				tty_table[];
EXTERN	CONSOLE			con_table[];
EXTERN	int				current_console;

#endif

