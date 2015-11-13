%include "pm.inc"

org 0100h
jmp label_begin

%include "fat12hdr.inc"

;GDT							Base			Limit					Attr
label_gdt:			Descriptor	0,				0,						0
label_desc_flatC:	Descriptor	0,				0xfffff,				DA_C|DA_32|DA_LIMIT_4K
label_desc_flatRW:	Descriptor	0,				0xfffff,				DA_DRW|DA_32|DA_LIMIT_4K
label_desc_video:	Descriptor	0xb8000,		0xffff,					DA_DRW + DA_DPL3

GdtLen	equ	$-label_gdt
gdtptr:	dw	GdtLen-1	;GDT limit
		dd	BaseOfLoaderPhyAddr + label_gdt		;GDT base addr
		
SelectorVideo		equ label_desc_video 	- label_gdt	+	SA_RPL3
SelectorFlatRW		equ label_desc_flatRW	- label_gdt
SelectorFlatC		equ	label_desc_flatC	- label_gdt

_kernelName			db	"KERNEL  BIN"
_bootMsg			db	"         "
					db	"         "
					db	"Loading  "
					db	"Kernel OK"
					db	"No Kernel"

BITS 16
label_begin:
	mov ax,cs
	mov ds,ax
	mov es,ax
	mov ss,ax
	mov sp,OffsetOfLoader
	
	mov ax,BaseOfKernel	
	mov es,ax
	
label_root_dir_search:
	cmp word [_wRootDirSecs],0
	jz label_no_kernel
	dec word [_wRootDirSecs]
	
	mov ax,word [_wSecNum]
	mov cx,1
	mov bx,OffsetOfKernel			;es:bx->buffer
	call _readSector
	inc word [_wSecNum]				;next root dir sector
	mov dx,10h
label_filename_cmp:
	cmp dx,0
	jz label_root_dir_search
	dec dx
	
	mov si,_kernelName				;ds:si->filename
	mov cx,11
label_char_cmp:
	cmp cx,0
	jz label_file_found
	dec cx
	
	lodsb
	cmp al,byte [es:bx]
	jnz label_next
	inc bx
	jmp label_char_cmp
label_next:
	and bx,0ffe0h
	add bx,20h
	jmp label_filename_cmp

label_no_kernel:
	mov dh,4
	call _dispBootMsg
	jmp $
label_file_found:
	push es
	push bx
	mov dh,2
	call _dispBootMsg
	pop bx
	pop es
	
	add bx,0fh
	mov ax,word [es:bx]				;save file 1st sector No.	
	push ax
	add ax,DataStartSecNo
	mov bx,OffsetOfKernel			;es:bx->buffer
label_load_file:
	push ax
	push bx
	mov ah,0eh
	mov al,'.'
	mov bx,0fh
	int 10h
	pop bx
	pop ax
	
	mov cx,1
	call _readSector
	
	pop ax
	call _getFatEntry
	cmp ax,0fffh
	jz label_file_loaded
	push ax
	add ax,DataStartSecNo
	add bx,200h
	jmp label_load_file
	
label_file_loaded:
	mov dh,3
	call _dispBootMsg
	call _killMotor
	
	;jmp $
	
	;check memory info
	xor ebx,ebx
	xor edx,edx
	mov di,_memChkBuf
.1:	mov eax,0E820h
	mov ecx,20
	mov edx,SMAP
	int 15h
	jc	.err
	inc byte [_memStrNum]
	cmp ebx,0
	jz	.memok
	add di,20
	jmp .1
.err:
	mov byte [_memStrNum],0
.memok:

	lgdt [gdtptr]
	
	cli
	
	in al,92h
	or al,02h
	out 92h,al
	
	mov eax,cr0
	or eax,1
	mov cr0,eax
;=========================Protect Mode Start==================================
	jmp dword SelectorFlatC:(BaseOfLoaderPhyAddr + label_pm_start)
;=============================================================================
_killMotor:
	push dx
	mov dx,03f2h
	mov al,0
	out dx,al
	pop dx
	ret
;_killMotor End

%include "load.inc"

BITS 32
label_pm_start:
	mov ax,SelectorVideo
	mov gs,ax
	mov ax,SelectorFlatRW
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov ss,ax
	mov esp,TopOfStack
	
	push PbootMsg1
	call DispStr
	add esp,4
	
	push RamMapCap
	call DispStr
	add esp,4
	
	call DispMemInfo
	call DispReturn
	
	call SetupPaging
	
	push PbootMsg2
	call DispStr
	add esp,4
	
	call InitKernel
	
	;jmp $
	mov eax,dword [DisPos]
	mov cl,byte [DispColor]
;===============================Kernel Start==================================
	jmp SelectorFlatC:KernelStartPoint
;=============================================================================

%include "lib.inc"
InitKernel:
	xor esi,esi
	mov esi,BaseOfKernelPhyAddr
	movzx ecx,word [esi + 2ch]					;program header count
	mov esi,dword [BaseOfKernelPhyAddr + 1ch]
	add esi,BaseOfKernelPhyAddr					;program header addr
.1:	mov eax,dword [esi + 0]
	cmp eax,0
	jz .noaction
	mov eax,dword [esi + 16]
	push eax									;length
	mov eax,dword [esi + 4]
	add eax,BaseOfKernelPhyAddr					;src
	push eax
	mov eax,dword [esi + 8]						;dst
	push eax
	call MemCpy
	add esp,12
.noaction:
	add esi,20h
	dec ecx
	cmp ecx,0
	jnz .1
	ret
;InitKernel End

DispMemInfo:
	push eax
	push ebx
	push ecx
	push edx
	push esi
	push edi
	
	mov ecx,[MemStrNum]
	mov esi,MemChkBuf
.1:	mov edi,BaseAddrL;MemInfoStr
	mov edx,5	
.2:	push dword [esi]
	call DispInt
	pop	eax
	
	;stosd not work?
	mov dword [edi],eax
	add edi,4
	
	add esi,4
	dec edx
	cmp edx,0
	jnz	.2
	
	cmp dword [AddrType],2
	jz .3
	mov eax,dword [BaseAddrL]
	add eax,dword [LengthLow]
	cmp eax,dword [MemSize]
	jc	.3
	mov dword [MemSize],eax
	
.3:	call DispReturn
	loop .1
	
	call DispReturn	
	push SzRamSize
	call DispStr
	add esp,4
	
	push dword [MemSize]
	call DispInt
	;add esp,4
	pop eax
	mov ebx,400000h
	div ebx
	test edx,edx
	jz .4
	add eax,1
.4:	mov dword [PageTblNum],eax
	
	pop edi
	pop esi
	pop edx
	pop ecx
	pop ebx
	pop eax
	ret
;DispMemInfo End

SetupPaging:
	mov ax,SelectorFlatRW
	mov es,ax
	
	mov edi,PageDirBase
	mov ecx,dword [PageTblNum];1024
	mov eax,PageTblBase|PG_P|PG_USU|PG_RWW	
.1:	stosd
	add eax,4096
	loop .1
	
	mov edi,PageTblBase
	mov eax,1024
	mov ebx,dword [PageTblNum]
	mul ebx
	mov ecx,eax;1024*1024
	xor eax,eax
	mov eax,PG_P|PG_USU|PG_RWW
.2:	stosd
	add eax,4096
	loop .2
	
	mov eax,PageDirBase
	mov cr3,eax
	mov eax,cr0
	or	eax,80000000h
	mov cr0,eax
	ret
;SetupPageing End

PageDirBase		equ	100000h
PageTblBase		equ	101000h

label_data:
	_pbootmsg1	db	"Protected Mode OK",0Ah,0Ah,0
	_pbootmsg2	db	"Setup Paging OK",0Ah,0
	_szRamSize	db	"Ram size:",0
	_ramMapCap	db	"BaseAddrL BaseAddrH LengthLow LengthHig Type",0Ah,0
	_disPos		dd	640
	_dispColor	db	0Fh
	_memSize	dd	0
	_pageTblNum	dd	0
	_memStrNum	db	0
	
	_baseAddrL	dd	0
	_baseAddrH	dd	0
	_lengthLow	dd	0
	_lengthHig	dd	0
	_addrType	dd	0
	_memChkBuf	times 256 db 0

	PbootMsg1	equ	BaseOfLoaderPhyAddr	+	_pbootmsg1
	PbootMsg2	equ	BaseOfLoaderPhyAddr	+	_pbootmsg2
	DisPos		equ	BaseOfLoaderPhyAddr	+	_disPos
	DispColor	equ BaseOfLoaderPhyAddr	+	_dispColor
	MemChkBuf	equ BaseOfLoaderPhyAddr	+	_memChkBuf
	MemSize		equ	BaseOfLoaderPhyAddr	+	_memSize
	MemStrNum	equ	BaseOfLoaderPhyAddr	+	_memStrNum
	BaseAddrL	equ	BaseOfLoaderPhyAddr	+	_baseAddrL
	BaseAddrH	equ	BaseOfLoaderPhyAddr	+	_baseAddrH
	LengthLow	equ BaseOfLoaderPhyAddr	+	_lengthLow
	LengthHig	equ	BaseOfLoaderPhyAddr	+	_lengthHig
	AddrType	equ	BaseOfLoaderPhyAddr	+	_addrType
	SzRamSize	equ BaseOfLoaderPhyAddr	+	_szRamSize
	RamMapCap	equ	BaseOfLoaderPhyAddr	+	_ramMapCap
	PageTblNum	equ	BaseOfLoaderPhyAddr	+	_pageTblNum
	
label_stack:
	times 1024 db 0
TopOfStack	equ	BaseOfLoaderPhyAddr	+ $
