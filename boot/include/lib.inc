Delay:
	nop
	nop
	nop
	nop
	ret
DispAL:
;display al as Hex
	push ebx
	push ecx
	push edi
	
	mov edi,dword [DisPos]
	mov ah,byte [DispColor]
	;mov ah,0ch
	xor ebx,ebx
	mov ecx,2
	
	mov bl,al
	shr al,4
.begin:
	and al,0x0f
	cmp al,9
	ja	.1
	add al,'0'
	jmp	.2
.1:	sub al,0xa
	add al,'A'
.2:	mov word [gs:edi],ax
	add edi,2
	mov al,bl
	loop .begin
	
	mov dword [DisPos],edi
	pop edi
	pop ecx
	pop ebx
	ret
;DumpHex End

DispInt:
;display 32bit integer in stack in hex,e.g.:00000000h
;usage:
;	push integer
;	call DispInt
;	add esp,4

	push eax
	
	mov	eax,[esp+8]
	shr	eax,24
	call	DispAL
	
	mov	eax,[esp+8]
	shr	eax,16
	call	DispAL
	
	mov	eax,[esp+8]
	shr	eax,8
	call	DispAL
	
	mov	eax,[esp+8]
	call	DispAL
	
	push edi
	mov edi,dword [DisPos]
	mov ah,07h
	mov al,'h'
	mov word [gs:edi],ax
	add edi,4
	mov dword [DisPos],edi
	pop edi
	
	pop eax
	ret
;DispInt End

DispStr:
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
	jnz	.2
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
.2:	mov word [gs:edi],ax
	add edi,2
	jmp .1
.3:	
	mov dword [DisPos],edi
	pop edi
	pop eax
	pop esi;push esi cause ss limit error
	ret
;DispStr End

DispReturn:
	push eax
	push ebx
	mov eax,dword [DisPos]
	mov bl,160
	div bl
	and eax,0FFh
	add eax,1
	mov bl,160
	mul bl
	mov dword [DisPos],eax
	pop ebx
	pop eax
	ret
;DispReturn End

MemCpy:
;Description:fs->src seg,es->dest seg
;usage:
;	push length(bytes)
;	push source
;	push destination
;	call MemCpy
;	add esp,12
	
	push ebp
	mov ebp,esp
	push eax
	push ecx
	push esi
	push edi
	
	;source fs:esi
	mov esi,[ebp + 12]
	
	;dest es:edi
	mov edi,[ebp + 8]
	
	mov ecx,dword [ebp + 16]
.1:	
	cmp ecx,0
	jz	.2
	dec ecx
	mov al,byte [fs:esi]
	mov byte [es:edi],al
	inc esi
	inc edi
	jmp .1
.2:
	pop edi
	pop esi
	pop ecx
	pop eax
	pop ebp
	ret
;MemCpy End
