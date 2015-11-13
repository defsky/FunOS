#ifndef	_PROC_H_
#define	_PROC_H_

#include	"type.h"
#include	"protect.h"

#define	PROC_NAME_LEN	32
#define	MAX_TASK_NUM	5
#define	MAX_PROC_NUM	5

#define	TASK_PROC		1
#define	USER_PROC		3

typedef	void 	(*PROC)();

typedef	struct	p_stackframe
	{
		u32	gs;
		u32	fs;
		u32	es;
		u32	ds;
		u32	edi;
		u32	esi;
		u32	ebp;
		u32	task_esp;
		u32	ebx;
		u32	edx;
		u32	ecx;
		u32	eax;
		u32	retaddr;
		u32	eip;
		u32	cs;
		u32	eflags;
		u32	esp;
		u32	ss;
	}STACKFRAME;

typedef	struct	p_proc
	{
		STACKFRAME	regs;
		u16		ldt_sel;
		DESCRIPTOR	ldt[LDT_SIZE];
		int		pid;
		int		type;
		int		tty;
		char		name[PROC_NAME_LEN];
	}PROCESS;

PUBLIC	void	proc_schedule();
PUBLIC	void	proc_start();
PUBLIC	void	pcb_init(PROC proc_handle,int privilege,int tty_no);

PUBLIC	void	TestA();
PUBLIC	void	TestB();
PUBLIC	void	TestC();

#endif

