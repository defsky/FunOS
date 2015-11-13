#include	"type.h"
#include	"protect.h"
#include	"global.h"

PUBLIC	void		cstart()
{
	//disp_str("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	disp_str("-------------\"cstart\" begin------------------\n");
	MemCpy(
		(void *)&gdt,						//dst
		(void *)(*((u32 *)&gdt_ptr[2])),	//src
		(*(u16 *)&gdt_ptr[0])+1				//len
		);
		
	//initialize new gdt_ptr
	*(u16 *)&gdt_ptr[0] = sizeof(DESCRIPTOR) * GDT_SIZE - 1;
	*(u32 *)&gdt_ptr[2] = (u32)&gdt;
	
	//initialize idt	
	*(u16 *)&idt_ptr[0] = sizeof(GATE) * IDT_SIZE - 1;
	*(u32 *)&idt_ptr[2] = (u32)&idt;
	
	init_prot();
	
	disp_str("-------------\"cstart\"   end------------------\n");
	//while(1){}
}

