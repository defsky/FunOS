;nasm -f elf -o kernel.o kernel.asm
;ld -s -o kernel.bin kernel.o

%include	"sconst.inc"

bits	32

[section .data]
;k_reenter	dd	0

[section .bss]
stack	resb	1024 * 2
TopOfStack:

[section .text]
global	_start
global	proc_start
global	sys_call_entry

global	divide_error
global	single_step
global	nmi
global	breakpoint
global	overflow
global	bounds_check
global	inval_opcode
global	copr_not_available
global	double_fault
global	copr_seg_overrun
global	inval_tss
global	segment_not_present
global	stack_exception
global	general_protection
global	page_fault
global	copr_error

global	hwint00
global	hwint01
global	hwint00
global	hwint01
global	hwint02
global	hwint03
global	hwint04
global	hwint05
global	hwint06
global	hwint07
global	hwint08
global	hwint09
global	hwint10
global	hwint11
global	hwint12
global	hwint13
global	hwint14
global	hwint15

_start:
	mov dword [DisPos],eax
	mov byte [DispColor],cl
	
	mov esp,TopOfStack
	
	sgdt	[gdt_ptr]
	call	cstart
	lgdt	[gdt_ptr]
	lidt	[idt_ptr]
	
	jmp	Kernel_CS:csinit
	
csinit:
	;int 50h
	;hlt
	mov ax,SELECTOR_TSS
	ltr ax
	
	;sti
	jmp kernel_main
proc_start:
	mov esp,dword [p_proc_ready]
	lldt word [esp + P_LDT_SEL]
	lea eax,[esp + P_STACK_TOP]
	mov dword [tss + TSS_ESP0],eax
re_enter:
	dec dword [k_reenter]
	pop gs
	pop fs
	pop es
	pop ds
	popad
	add esp,4
	
	iretd
save:
	pushad
	push ds
	push es
	push fs
	push gs
	
	mov esi,edx
	mov dx,ss
	mov ds,dx
	mov es,dx
	mov fs,dx
	mov edx,esi
	
	mov esi,esp

	inc dword [k_reenter]
	cmp dword [k_reenter],0
	jne .1
	mov esp,TopOfStack
	push proc_start
	jmp dword [esi + RETADDR]
.1:
	push re_enter
	jmp dword [esi + RETADDR]

sys_call_entry:
	call save
	push esi
	push dword [p_proc_ready]
	sti
	
	push ecx
	push edx
	call dword [sys_call_table + 4 * eax]
	add esp,12
	pop esi
	mov dword [esi + EAXREG],eax
	cli
	ret

;--------------------------------------
%macro hwint_entry 1
	call save

	in al,INT_M_MASK
	or al,(1<<%1)
	out INT_M_MASK,al
	mov al,EOI
	out INT_M_CTL,al
	sti
	
	push %1
	call dword [irq_table + 4 * %1]
	pop ecx
	
	cli
	in al,INT_M_MASK
	and al,~(1<<%1)
	out INT_M_MASK,al
	ret
%endmacro
;--------------------------------------
ALIGN	16
hwint00:
	hwint_entry 0

ALIGN	16
hwint01:
	hwint_entry 1

ALIGN	16
hwint02:
	hwint_entry 2

ALIGN	16
hwint03:
	hwint_entry 3

ALIGN	16
hwint04:
	hwint_entry 4

ALIGN	16
hwint05:
	hwint_entry 5

ALIGN	16
hwint06:
	hwint_entry 6

ALIGN	16
hwint07:
	hwint_entry 7

;--------------------------------------
%macro hwint_entry_slave 1
	call save
	
	in al,INT_S_MASK
	or al,(1<<(%1-8))
	out INT_S_MASK,al
	mov al,EOI
	out INT_M_CTL,al
	nop
	out INT_S_CTL,al
	sti
	
	push %1
	call dword [irq_table + 4 * %1]
	pop ecx
	
	cli
	in al,INT_S_MASK
	and al,~(1<<(%1-8))
	out INT_S_MASK,al
	ret
%endmacro
;--------------------------------------
ALIGN	16
hwint08:
	hwint_entry_slave 8

ALIGN	16
hwint09:
	hwint_entry_slave 9

ALIGN	16
hwint10:
	hwint_entry_slave 10

ALIGN	16
hwint11:
	hwint_entry_slave 11

ALIGN	16
hwint12:
	hwint_entry_slave 12

ALIGN	16
hwint13:
	hwint_entry_slave 13

ALIGN	16
hwint14:
	hwint_entry_slave 14

ALIGN	16
hwint15:
	hwint_entry_slave 15


divide_error:
	push 0xffffffff		;error code
	push 0			;vector No.
	jmp exception_handle
single_step:
	push 0xffffffff
	push 1
	jmp exception_handle
nmi:
	push 0xffffffff
	push 2
	jmp exception_handle
breakpoint:
	push 0xffffffff
	push 3
	jmp exception_handle
overflow:
	push 0xffffffff
	push 4
	jmp exception_handle
bounds_check:
	push 0xffffffff
	push 5
	jmp exception_handle
inval_opcode:
	push 0xffffffff
	push 6
	jmp exception_handle
copr_not_available:
	push 0xffffffff
	push 7
	jmp exception_handle
double_fault:
	;push 0xffffffff
	push 8
	jmp exception_handle
copr_seg_overrun:
	push 0xffffffff
	push 9
	jmp exception_handle
inval_tss:
	push 10
	jmp exception_handle
segment_not_present:
	push 11
	jmp exception_handle
stack_exception:
	push 12
	jmp exception_handle
general_protection:
	push 13
	jmp exception_handle
page_fault:
	push 14
	jmp exception_handle
copr_error:
	push 0xffffffff
	push 16
	jmp exception_handle
exception_handle:
	call exception_handler
	add esp,8
	hlt

