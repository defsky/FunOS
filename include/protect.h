#ifndef	_PROTECT_H_
#define	_PROTECT_H_

#include "type.h"
#include "i8259.h"

#define	INDEX_KERNEL_CS	1
#define	INDEX_KERNEL_DS	2
#define	INDEX_KERNEL_GS	3
#define	INDEX_TSS	4
#define	INDEX_FIRST_LDT	5

#define	SELECTOR_TSS	INDEX_TSS << 3
#define	KERNEL_CS	INDEX_KERNEL_CS << 3
#define KERNEL_SS	INDEX_KERNEL_DS << 3
#define	SELECTOR_GS	0x1b

#define	DA_32		0x4000
#define	DA_LIMIT_4K	0x8000
#define	DA_DR		0x90
#define	DA_DRW		0x92
#define	DA_DRWA		0x93
#define	DA_C		0x98
#define	DA_CR		0x9a
#define	DA_CCO		0x9c
#define	DA_CCOR		0x9e

#define	DA_DPL0		0x0
#define	DA_DPL1		0x20
#define	DA_DPL2		0x40
#define	DA_DPL3		0x60

#define	DA_LDT		0x82
#define	DA_TASKGATE	0x85
#define	DA_386TSS	0x89
#define	DA_386CGATE	0x8c
#define	DA_386IGATE	0x8e
#define	DA_386TGATE	0x8f

#define	SA_TIL_MASK	0xfffb
#define	SA_RPL_MASK	0xfffc
#define	SA_TIL		0x4
#define	SA_RPL0		0x0
#define	SA_RPL1		0x1
#define	SA_RPL2		0x2
#define	SA_RPL3		0x3

#define	PG_P		0x1
#define	PG_USU		0x2
#define	PG_RWW		0x4

#define	GDT_SIZE	128
#define	LDT_SIZE	2
#define IDT_SIZE	256

typedef	struct	s_descriptor
	{
		u16	limit1;
		u16	base1;
		u8	base2;
		u8	attr1;
		u8	limit_attr2;
		u8	base3;
	}DESCRIPTOR;
	
typedef struct	s_gate
	{
		u16	offset_low;
		u16	selector;
		u8	pcount;
		u8	attr;
		u16	offset_high;
	}GATE;

typedef	struct	s_tss
	{
		u32	next;
		u32	esp0;
		u32	ss0;
		u32	esp1;
		u32	ss1;
		u32	esp2;
		u32	ss2;
		u32	cr3;
		u32	eip;
		u32	eflags;
		u32	eax;
		u32	ecx;
		u32	edx;
		u32	dbx;
		u32	esp;
		u32	ebp;
		u32	esi;
		u32	edi;
		u32	es;
		u32	cs;
		u32	ss;
		u32	ds;
		u32	fs;
		u32	gs;
		u32	ldtr;
		u16	trap;
		u16	iomap;
	}TSS;

//exception entry
PUBLIC	void	divide_error();
PUBLIC	void	single_step();
PUBLIC	void	nmi();
PUBLIC	void	breakpoint();
PUBLIC	void	overflow();
PUBLIC	void	bounds_check();
PUBLIC	void	inval_opcode();
PUBLIC	void	copr_not_available();
PUBLIC	void	double_fault();
PUBLIC	void	copr_seg_overrun();
PUBLIC	void	inval_tss();
PUBLIC	void	segment_not_present();
PUBLIC	void	stack_exception();
PUBLIC	void	general_protection();
PUBLIC	void	page_fault();
PUBLIC	void	copr_error();

PUBLIC	void	exception_handler(u8 vector,u32 errno,
			u32 eip,u32 cs,u32 eflags);
PUBLIC	void	desc_init(DESCRIPTOR *p,u32 base,u32 limit,u16 attr);
PUBLIC	void	int_desc_init(u8 vector,int_handle handle,
			u8 privilege,u8 type);
PUBLIC	void	init_prot();

#endif
