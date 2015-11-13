%include	"sconst.inc"

extern	DisPos
extern	DispColor

global	disp_str
global	out_byte
global	in_byte
global	enable_irq
global	enable_int
global	disable_int

;#################################################################
;	void	disp_str(unsigned char* str)
;#################################################################
disp_str:
;Description:display string in stack
;Usage:
;	push string
;	call DispStr
;	add esp,4

	push esi
	mov esi,[esp+8]
	push eax
	push edi
	
	mov edi,dword [DisPos]
	mov ah,byte [DispColor]
	
.1:	lodsb
	test al,al
	jz .3
	cmp al,0Ah
	jnz .4
	push eax
	mov eax,edi
	mov bl,160
	div bl
	and eax,0FFh
	add eax,1
	mov bl,160
	mul bl
	mov edi,eax
	pop eax
	jmp .1
	
.4:	cmp al,08h
	jnz .2
	mov al,' '
	sub edi,2
	mov word [gs:edi],ax
	jmp .1
.2:	mov word [gs:edi],ax
	add edi,2
	jmp .1
.3:	
	mov dword [DisPos],edi
	;call set_cursor
	;call screen_scroll
	pop edi
	pop eax
	pop esi
	ret
;DispStr End

;##################################################################
;	void	out_byte(u8 port,u8 data)
;##################################################################
out_byte:
	push ebp
	mov ebp,esp
	push edx
	push eax
	mov edx,dword [ebp + 8]
	mov eax,dword [ebp + 12]
	out dx,al
	pop eax
	pop edx
	pop ebp
	nop
	nop
	ret
;	u8	in_byte(u8 port)
;##################################################################
in_byte:
	mov edx,dword [esp + 4]
	in al,dx
	nop
	nop
	ret

;#########################################################
;	void	enable_irq(u8 irq);
;#########################################################
enable_irq:
	pushf
	
	mov ecx,[esp + 8]
	sub ecx,HW_IRQ0
	mov ah,~1
	rol ah,cl
	;xor ah,0xff
	cmp ecx,8
	jae .slave
	
	cli
	in al,INT_M_MASK
	and al,ah
	out INT_M_MASK,al
	jmp .end
	
.slave:
	cli
	in al,INT_S_MASK
	and al,ah
	out INT_S_MASK,al
.end:
	;sti
	popf
	ret

;#######################################################
;	void	enable_int();
;#######################################################
enable_int:
	sti
	ret

;#######################################################
;	void	disable_int();
;#######################################################
disable_int:
	cli
	ret

