#include	"proc.h"
#include	"protect.h"
#include	"type.h"
#include 	"global.h"
#include	"syscall.h"
#include	"debug.h"

PRIVATE	u8	dpl[4] = {DA_DPL0,DA_DPL1,DA_DPL2,DA_DPL3};
PRIVATE	u8	rpl[4] = {SA_RPL0,SA_RPL1,SA_RPL2,SA_RPL3};

PUBLIC	void	proc_schedule()
{
	//disp_str("#");
	if(++p_proc_ready == proc_table + proc_id)
		p_proc_ready = proc_table;
}

PUBLIC	void	pcb_init(PROC proc_handle,int privilege,int tty_no)
{
	PROCESS *	p_pcb;

	p_pcb = proc_table + proc_id;
	
	p_pcb->tty = tty_no;
	p_pcb->type = privilege;

	desc_init(&gdt[INDEX_FIRST_LDT + proc_id],
		(u32)proc_table[proc_id].ldt,
		0xfffff,DA_LDT);
	
	p_pcb->ldt_sel = (INDEX_FIRST_LDT + proc_id) << 3;
	desc_init(&p_pcb->ldt[0],0,0xfffff,
		DA_C|DA_32|DA_LIMIT_4K|dpl[privilege]);
	desc_init(&p_pcb->ldt[1],0,0xfffff,
		DA_DRW|DA_32|DA_LIMIT_4K|dpl[privilege]);
	p_pcb->pid = proc_id + 1;
	p_pcb->regs.cs = 0 | SA_TIL|rpl[privilege];
	p_pcb->regs.ds = 8 | SA_TIL|rpl[privilege];
	p_pcb->regs.es = 8 | SA_TIL|rpl[privilege];
	p_pcb->regs.fs = 8 | SA_TIL|rpl[privilege];
	p_pcb->regs.ss = 8 | SA_TIL|rpl[privilege];
	p_pcb->regs.gs = SELECTOR_GS;
	p_pcb->regs.eip = (u32)proc_handle;
	p_pcb->regs.eflags = 0x1202;
	p_pcb->regs.esp = 0x200000 + 0x1000 * proc_id;
	
	proc_id++;
}

PUBLIC	void	TestA()
{
	while(1)
	{
		printf("A");
		delay(1000);
	}
}

PUBLIC	void	TestB()
{
//	assert(0);
	while(1)
	{
		printf("B");
		delay(2000);
	}
}

PUBLIC	void	TestC()
{
	while(1)
	{
		printf("<Ticks:%x>",get_ticks());
		delay(4000);
	}
}
