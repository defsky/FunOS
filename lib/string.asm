global	MemCpy

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