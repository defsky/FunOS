;constant
Kernel_CS	equ	1 << 3
SELECTOR_TSS	equ	4 << 3

EAXREG		equ	4 * 11
P_LDT_SEL	equ	4 * 18
P_STACK_TOP	equ	4 * 18
RETADDR		equ	4 * 12
TSS_ESP0	equ	4

INT_M_CTL	equ	20h
INT_M_MASK	equ	21h
INT_S_CTL	equ	0a0h
INT_S_MASK	equ	0a1h
EOI		equ	20h
HW_IRQ0		equ	20h

;extern function
extern	cstart
extern	exception_handler
extern	SuperviousHandle
extern	kernel_main
extern	proc_schedule

;extern global varible
extern	gdt_ptr
extern	idt_ptr
extern	k_reenter
extern	DisPos
extern	DispColor
extern	tss
extern	p_proc_ready
extern	irq_table
extern	sys_call_table

