#include	"i8259.h"
#include	"global.h"

PUBLIC	void	init8259()
{
	//ICW1
	out_byte(INT_M_CTL,0x11);
	out_byte(INT_S_CTL,0x11);
	
	//ICW2
	out_byte(INT_M_MASK,HW_IRQ0);
	out_byte(INT_S_MASK,HW_IRQ0 + 8);

	//ICW3
	out_byte(INT_M_MASK,0x4);
	out_byte(INT_S_MASK,0x2);

	//ICW4
	out_byte(INT_M_MASK,0x1);
	out_byte(INT_S_MASK,0x1);

	//OCW1
	out_byte(INT_M_MASK,0xff);
	out_byte(INT_S_MASK,0xff);
}

PUBLIC	void	irq_handle_init(u8 irq,IRQHandle handle)
{
	irq_table[irq - HW_IRQ0] = handle;
}

PUBLIC	void	SuperviousHandle(int irq)
{
	disp_str("IRQ:");
	disp_int(irq);
	disp_str("\n");
}

