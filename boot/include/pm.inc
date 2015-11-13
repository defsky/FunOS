SMAP	equ	0x534D4150;'SMAP'

; STRUC	memInfo
	; BaseAddrL	resd	1
	; BaseAddrH	resd	1
	; LengthLow	resd	1
	; LengthHig	resd	1
	; Type		resd	1
; ENDSTRUC

; ����������

DA_32		EQU	4000h ; 32 λ��
DA_LIMIT_4K	equ	8000h	;4k padding

DA_DPL0 	EQU	00h ; DPL = 0
DA_DPL1 	EQU	20h ; DPL = 1
DA_DPL2 	EQU	40h ; DPL = 2
DA_DPL3 	EQU	60h ; DPL = 3

;Selector Type
SA_TIL		EQU	 04H ; LDT selector
SA_RPL0		EQU	 00H ;RPL = 0
SA_RPL1		EQU  01H ;RPL = 1
SA_RPL2		EQU  02H ;RPL = 2
SA_RPL3		EQU  03H ;RPL = 3	

; �洢������������

DA_DR	EQU	90h ; ���ڵ�ֻ�����ݶ�����ֵ
DA_DRW	EQU 92h ; ���ڵĿɶ�д���ݶ�����ֵ
DA_DRWA	EQU 93h ; ���ڵ��ѷ��ʿɶ�д���ݶ�����ֵ
DA_C	EQU 98h ; ���ڵ�ִֻ�д��������ֵ
DA_CR	EQU 9Ah ; ���ڵĿ�ִ�пɶ����������ֵ
DA_CCO	EQU 9Ch ; ���ڵ�ִֻ��һ�´��������ֵ
DA_CCOR	EQU 9Eh ; ���ڵĿ�ִ�пɶ�һ�´��������ֵ

; ϵͳ������������

DA_LDT		EQU  82h ; �ֲ���������������ֵ
DA_TaskGate	EQU  85h ; ����������ֵ
DA_386TSS	EQU  89h ; ���� 386 ����״̬������ֵ
DA_386CGate	EQU  8Ch ; 386 ����������ֵ
DA_386IGate	EQU  8Eh ; 386 �ж�������ֵ
DA_386TGate	EQU  8Fh ; 386 ����������ֵ

;page attribute
PG_P	equ	1
PG_USU	equ	2
PG_RWW	equ	4

;=========================Macro Defination Start==============================
%macro Descriptor 3
;usage: Descriptor Base, Limit, Attr
;	Base:  dd (32 bits �λ�ַ)
;	Limit: dd (low 20 bits available,20 bits �ν���)
;	Attr:  dw (lower 4 bits of higher byte are always 0)

	dw	%2 & 0FFFFh ; �ν���1
	dw	%1 & 0FFFFh ; �λ�ַ1
	db	(%1 >> 16) & 0FFh ; �λ�ַ2
	dw	((%2 >> 8) & 0F00h) | (%3 & 0F0FFh) ; ����1 + �ν���2 + ����2
	db	(%1 >> 24) & 0FFh ; �λ�ַ3
%endmacro ; �� 8 �ֽ�

%macro Gate 4
;usage:	Gate	Selector,	Offset,	Dcount,	Attr
;	Selector:	dw
;	Offset:		dd
;	Dcount:		db
;	Attr:		db
	
	dw	%2 & 0ffffh			;offset 1
	dw	%1 & 0ffffh			;selector
	db	%3 & 01fh			;Param count
	db	%4 & 0ffh			;Attr
	dw	(%2 >> 16) & 0ffffh	;offset 2
%endmacro;8 bytes
;=========================Macro Defination End================================