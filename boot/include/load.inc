BaseOfLoader		equ	9000h
OffsetOfLoader		equ	0100h
BaseOfLoaderPhyAddr	equ	BaseOfLoader * 10h

BaseOfKernel		equ	8000h
OffsetOfKernel		equ	0
KernelStartPoint	equ	30400h
BaseOfKernelPhyAddr	equ	BaseOfKernel * 10h

BootMsgLen 			equ 9

_wSecNum			dw	RootDirStartSecNo		;sector No. to read
_wRootDirSecs		dw	RootDirTotalSecs		;for root dir search loop counter
_f_even				db	0

_cls:
	mov ax,0x0600;al=00h,clear screen
	mov bx,0x0f00;bh=init color
	mov cx,0
	mov dx,0x184f;80 cols,25 rows
	int 10h
	ret

_dispBootMsg:
;usage:
;	mov dh,(string No.)
;	call _dispBootMsg

	mov ax,BootMsgLen
	mov cx,ax			;string length
	mov bl,dh
	mul bl
	add ax,_bootMsg
	mov bp,ax
	mov ax,ds
	mov es,ax			;es:bp->string offset
	mov bx,0x000f		;bh=00h,page code;bl=02,bgcolor=black,fgcolor=green
	mov ax,0x1301		;ah=13h,al=01h,Teletype mode display string
	xor dl,dl
	int 10h
	ret
;_dispBootMsg End
	
_readSector:
; 中断			寄存器									作用
; 13h 	 		ah=00h	dl=驱动器号(0表示A盘)			复位软驱

; 				ah=02h			al=要读扇区数			从磁盘读数据入es:bx
; 				ch=柱面(磁道)	cl=起始扇区号
; 				dh=磁头号		dl=驱动器号(0表示A盘)
; 				es:bx->数据缓冲区

;                          ┌ 柱面号 = y >> 1   
;     扇区号x       ┌ 商 y ┤   
; -------------- => ┤      └ 磁头号 = y & 1   
;  每磁道扇区数     │   
;                   └ 余 z => 起始扇区号 = z + 1 
;usage:	ax = sector seq No. 
;		cx = sector count
;		bx = buffer
	push bp
	mov bp,sp
	sub esp,2
	mov word [bp-2],cx
	
	push bx
	mov bl,18			;word [BPB_SecPerTrk]
	div bl
	add ah,1
	mov cl,ah			;start sector No.
	mov ch,al
	and al,1
	mov dh,al			;head No.
	shr ch,1			;cylinder No.
	
	pop bx
	xor dl,dl			;drive A
.1:	mov ah,02h			;read
	mov al,byte [bp-2]	;sector count
	int 13h
	jc .1		;if CF=1 then retry
	
	; push bx
	; mov cl,byte [bp-2]
; .2:	mov ah,0eh
	; mov al,'.'
	; mov bx,0fh
	; int 10h
	; loop .2
	; pop bx
	
	add esp,2
	pop bp
	ret
;_readSector End

_getFatEntry:
;ax->sector No.
	push es
	push bx
	push dx
	push ax
	mov ax,BaseOfLoader
	sub ax,0100h
	mov es,ax				;es:bx->FAT buffer
	pop ax
	
	mov byte [_f_even],0
	xor dx,dx
	mov bx,3
	mul bx
	mov bx,2
	div bx
	cmp dx,0
	jz label_even
	mov byte [_f_even],1
label_even:	
	xor dx,dx
	mov bx,512
	div bx
	
	push dx
	add ax,FAT1SecNo
	mov cx,2				;read 2 sectors
	xor bx,bx				;es:bx->FAT buffer
	call _readSector
	
	pop si
	mov ax,word [es:si]
	cmp byte [_f_even],1
	jnz .1
	shr ax,4
.1:	and ax,0fffh
	
	pop dx
	pop bx
	pop es
	ret
;_getFatEntry End
