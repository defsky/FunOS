org 0x7c00

jmp short label_start
nop

%include "fat12hdr.inc"

_loaderName			db	"LOADER  BIN"
_bootMsg			db	"Booting  "
					db	"Loader OK"
					db	"No Loader"
					
BaseOfStack			equ	7c00h

label_start:
	mov ax,cs
	mov es,ax
	mov ds,ax
	mov ss,ax
	mov sp,BaseOfStack

	call _cls
	
	mov dh,0
	call _dispBootMsg
	
	;reset flopy
	mov ah,0
	mov dl,0
	int 13h

	mov ax,BaseOfLoader	
	mov es,ax
	
label_loader_search_begin:
	cmp word [_wRootDirSecs],0
	jz label_no_loader
	dec word [_wRootDirSecs]

	mov ax,word [_wSecNum]
	mov cx,1
	mov bx,OffsetOfLoader			;es:bx->buffer
	call _readSector
	inc word [_wSecNum]		;next root dir sector
	
	mov di,OffsetOfLoader			;es:di->root dir buffer
	mov dx,10h
label_file_entry_cmp:	
	cmp dx,0
	jz label_loader_search_begin
	dec dx
	
	mov si,_loaderName				;ds:si->loader file name string
	mov cl,11
label_char_cmp:
	lodsb
	cmp byte [es:di],al
	jnz label_next_entry
	inc di
	loop label_char_cmp
	jmp label_file_found
label_next_entry:
	and di,0ffe0h
	add di,20h
	jmp label_file_entry_cmp
	
label_no_loader:
	mov dh,2
	call _dispBootMsg
	
	jmp $
	
label_file_found:
	add di,15
	mov ax,word [es:di]
	
	mov bx,OffsetOfLoader
label_read_file_begin:
	cmp ax,0ff8h
	jnb label_read_file_over
	cmp ax,02h
	jl label_read_file_over
	
	push ax
	add ax,DataStartSecNo			;ax+rootdirsectors+19-2
	mov cl,1
	call _readSector
	add bx,200h
	push bx
	
	mov ah,0eh
	mov al,'.'
	mov bx,0fh
	int 10h
	
	pop bx
	pop ax
	call _getFatEntry
	jmp label_read_file_begin
label_read_file_over:
	mov dh,1
	call _dispBootMsg
	
	;jmp $
;============================Loader Start=====================================
	jmp BaseOfLoader:OffsetOfLoader
;=============================================================================
%include "load.inc"
	
	times 510-($-$$) db 0;padding 0s
	dw 0xaa55;boot sector flag
