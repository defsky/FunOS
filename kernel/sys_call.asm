extern	ticks

[section .text]
global	get_ticks
global	write
global	printx

get_ticks:
	mov eax,0
	int 80h
	ret

write:
	mov	eax,1
	mov ecx,[esp + 8]
	mov edx,[esp + 4]
	int	80h
	ret

printx:
	mov	eax,2
	mov ecx,[esp + 8]
	mov edx,[esp + 4]
	int	80h
	ret
