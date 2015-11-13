#include	"protect.h"
#include	"type.h"
#include	"i8259.h"
#include	"i8253.h"
#include	"syscall.h"
#include	"global.h"

PUBLIC	void	desc_init(DESCRIPTOR *p,u32 base,u32 limit,u16 attr)
{
	p->limit1 = limit & 0xffff;
	p->base1 = base & 0xffff;
	p->base2 = (base >> 16) & 0xff;
	p->attr1 = attr & 0xff;
	p->limit_attr2 = ((limit >> 16) & 0xf) | ((attr >> 8) & 0xf0);
	p->base3 = base >> 24;
}

PUBLIC	void	int_desc_init(u8 vector,int_handle handle,
			u8 privilege,u8 type)
{
	GATE *p = &idt[vector];
	
	p->offset_low = (u32)handle & 0xffff;
	p->selector = KERNEL_CS;
	p->pcount = 0;
	p->attr = type | (privilege << 5);
	p->offset_high = ((u32)handle >> 16) & 0xffff; 
}

PUBLIC	void	init_prot()
{
	int i;
	init8259();
	init8253();
	
	int_desc_init(0,divide_error,0,DA_386IGATE);
	int_desc_init(1,single_step,0,DA_386IGATE);
	int_desc_init(2,nmi,0,DA_386IGATE);
	int_desc_init(3,breakpoint,0,DA_386IGATE);
	int_desc_init(4,overflow,0,DA_386IGATE);
	int_desc_init(5,bounds_check,0,DA_386IGATE);
	int_desc_init(6,inval_opcode,0,DA_386IGATE);
	int_desc_init(7,copr_not_available,0,DA_386IGATE);
	int_desc_init(8,double_fault,0,DA_386IGATE);
	int_desc_init(9,copr_seg_overrun,0,DA_386IGATE);
	int_desc_init(10,inval_tss,0,DA_386IGATE);
	int_desc_init(11,segment_not_present,0,DA_386IGATE);
	int_desc_init(12,stack_exception,0,DA_386IGATE);
	int_desc_init(13,general_protection,0,DA_386IGATE);
	int_desc_init(14,page_fault,0,DA_386IGATE);
	int_desc_init(16,copr_error,0,DA_386IGATE);
	
	int_desc_init(HW_IRQ0 + 0,hwint00,0,DA_386IGATE);
	int_desc_init(HW_IRQ0 + 1,hwint01,0,DA_386IGATE);
	int_desc_init(HW_IRQ0 + 2,hwint02,0,DA_386IGATE);
	int_desc_init(HW_IRQ0 + 3,hwint03,0,DA_386IGATE);
	int_desc_init(HW_IRQ0 + 4,hwint04,0,DA_386IGATE);
	int_desc_init(HW_IRQ0 + 5,hwint05,0,DA_386IGATE);
	int_desc_init(HW_IRQ0 + 6,hwint06,0,DA_386IGATE);
	int_desc_init(HW_IRQ0 + 7,hwint07,0,DA_386IGATE);
	int_desc_init(HW_IRQ0 + 8,hwint08,0,DA_386IGATE);
	int_desc_init(HW_IRQ0 + 9,hwint09,0,DA_386IGATE);
	int_desc_init(HW_IRQ0 + 10,hwint10,0,DA_386IGATE);
	int_desc_init(HW_IRQ0 + 11,hwint11,0,DA_386IGATE);
	int_desc_init(HW_IRQ0 + 12,hwint12,0,DA_386IGATE);
	int_desc_init(HW_IRQ0 + 13,hwint13,0,DA_386IGATE);
	int_desc_init(HW_IRQ0 + 14,hwint14,0,DA_386IGATE);
	int_desc_init(HW_IRQ0 + 15,hwint15,0,DA_386IGATE);
	
	int_desc_init(0x80,sys_call_entry,3,DA_386IGATE);
	
	for(i=0;i<16;i++)
		irq_handle_init(HW_IRQ0 + i,SuperviousHandle);
	
	desc_init(&gdt[INDEX_TSS],(u32)&tss,sizeof(TSS)-1,DA_386TSS);
	tss.ss0 = KERNEL_SS;
}


PUBLIC	void	exception_handler(u8 vector,u32 errno,u32 eip,u32 cs,u32 eflags)
{
	unsigned char *msg[] = {
			"Fault:0 Divide error\n",
			"Fault/Trap:Single Step error\n",
			"---NMI---\n",
			"Trap:Break Point\n",
			"Trap:Overflow\n",
			"Fault:Bounds Check error\n",
			"Fault:Invalid Operation Code\n",
			"Fault:Copr Not Available\n",
			"Abort:Double Fault\n",
			"Abort:Copr Segment Overrun\n",
			"Fault:Invalid TSS\n",
			"Fault:Segment Not Present\n",
			"Fault:Stack Exception\n",
			"Fault:General Protection error\n",
			"Fault:Page Fault\n",
			"---Intel Reserved---\n",
			"Fault:Copr Error\n"
		};
	DispColor = 0x0c;
	disp_str("--------------------------------------------------\n");
	disp_str(msg[vector]);
	disp_str("CS:EIP->");
	disp_int(cs);
	disp_str(":");
	disp_int(eip);
	disp_str("\nEFLAGS:");
	disp_int(eflags);
	disp_str("\n");
	disp_str("Error Code:");
	if(errno != 0xffffffff)
		disp_int(errno);
	else
		disp_str("N/A");
	DispColor = 0x0f;
}
