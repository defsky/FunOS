#ifndef	__KEYBOARD_H_
#define	__KEYBOARD_H_

#include	"type.h"
#include	"i8259.h"
#include	"tty.h"

#define	KB_DATA_PORT	0x60
#define	KB_CMD_PORT		0x64
#define	KB_ACK			0xFA
#define	KB_LED			0xED

#define	KB_BUF_LEN	64
#define	IRQ_KEYBOARD	HW_IRQ0 + 1

//key defination
#define	FLAG_NUM	0x80000000
#define	FLAG_LCTRL	0x40000000
#define	FLAG_RCTRL	0x20000000
#define	FLAG_LALT	0x10000000
#define	FLAG_RALT	0x08000000
#define	FLAG_LSHIFT	0x04000000
#define	FLAG_RSHIFT	0x02000000
#define	FLAG_EXT	0xFF000000
		
#define	KEY_CODE_MASK	0x80FFFFFF
#define	ESC	0x00010000
#define	BS	0x00020008
#define	TAB	0x00030000
#define	ENTER	0x0004000A
#define	LCTRL	0x00050000
#define	RCTRL	0x00060000
#define	LSHIFT	0x00070000
#define	RSHIFT	0x00080000
#define	LALT	0x00090000
#define	RALT	0x000A0000
#define	CAPS	0x000B0000
#define	F1	0x000C0000
#define	F2	0x000D0000
#define	F3	0x000E0000
#define	F4	0x000F0000
#define	F5	0x00100000
#define	F6	0x00110000
#define	F7	0x00120000
#define	F8	0x00130000
#define	F9	0x00140000
#define	F10	0x00150000
#define	F11	0x00160000
#define	F12	0x00170000
#define	NUMLOCK	0x00180000
#define	LWIN	0x00190000
#define	RWIN	0x001A0000
#define	COPY	0x001B0000
#define	SCROLL	0x001C0000
#define	PRTSCR	0x001D0000
#define	PAUSE	0x001E0000
#define	HOME	0x001F0000
#define	UP	0x00200000
#define	PGUP	0x00210000
#define	LEFT	0x00220000
#define	RIGHT	0x00230000
#define	END	0x00240000
#define	DOWN	0x00250000
#define	PGDN	0x00260000
#define	INSERT	0x00270000
#define	DELETE	0x00280000

#define	NUMENTER	ENTER|FLAG_NUM
#define	NUMDOT		'.'|FLAG_NUM
#define	NUM0		'0'|FLAG_NUM
#define	NUM1		'1'|FLAG_NUM
#define	NUM2		'2'|FLAG_NUM
#define	NUM3		'3'|FLAG_NUM
#define	NUM4		'4'|FLAG_NUM
#define	NUM5		'5'|FLAG_NUM
#define	NUM6		'6'|FLAG_NUM
#define	NUM7		'7'|FLAG_NUM
#define	NUM8		'8'|FLAG_NUM
#define	NUM9		'9'|FLAG_NUM
#define	NUMSLASH	'/'|FLAG_NUM
#define	NUMMINUS	'-'|FLAG_NUM

#define	KEY_MAP_COLS	3

typedef	struct	s_kb_input
	{
		u8 *	p_head;
		u8 *	p_tail;
		int	count;
		u8	buf[KB_BUF_LEN];
	}KB_INPUT;

PUBLIC	void	kb_wait_buf();
PUBLIC	void	kb_wait_ack();
PUBLIC	void	kb_setleds();
PUBLIC	void	keyboard_read(TTY * p_tty);
PUBLIC	void	keyboard_handle(int irq);
PUBLIC	void	in_process(TTY * p_tty,u32 key);
PUBLIC	u8	get_byte_from_kbuf();
PUBLIC	void	put_char(TTY * p_tty,u32 key);

#endif

