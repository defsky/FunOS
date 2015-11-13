#ifndef	_KEYMAP_H_
#define	_KEYMAT_H_

#include	"keyboard.h"
#include	"type.h"

PRIVATE	u32	key_map[] = {
/*scancode	key		!shift	shift	E0 XX */
/*0x0		0*/		0,	0,	0,
/*0x1		ESC*/		ESC,	ESC,	0,
/*0x2		1 !*/		'1',	'!',	0,
/*0x3		2 @*/		'2',	'@',	0,
/*0x4		3 #*/		'3',	'#',	0,
/*0x5		4 $*/		'4',	'$',	0,
/*0x6		5 %*/		'5',	'%',	0,
/*0x7		6 ^*/		'6',	'^',	0,
/*0x8		7 &*/		'7',	'&',	0,
/*0x9		8 **/		'8',	'*',	0,
/*0xA		9 (*/		'9',	'(',	0,
/*0xB		0 )*/		'0',	')',	0,
/*0xC		- _*/		'-',	'_',	0,
/*0xD		= +*/		'=',	'+',	0,
/*0xE		BackSpace*/	BS,	BS,	0,
/*0xF		TAB*/		TAB,	TAB,	0,
/*0x10		q*/		'q',	'Q',	0,
/*0x11		w*/		'w',	'W',	0,
/*0x12		e*/		'e',	'E',	0,
/*0x13		r*/		'r',	'R',	0,
/*0x14		t*/		't',	'T',	0,
/*0x15		y*/		'y',	'Y',	0,
/*0x16		u*/		'u',	'U',	0,
/*0x17		i*/		'i',	'I',	0,
/*0x18		o*/		'o',	'O',	0,
/*0x19		p*/		'p',	'P',	0,
/*0x1A		[ {*/		'[',	'{',	0,
/*0x1B		] }*/		']',	'}',	0,
/*0x1C		Enter*/		ENTER,	ENTER,	NUMENTER,
/*0x1D		Left Ctrl*/	LCTRL,	LCTRL,	RCTRL,
/*0x1E		a*/		'a',	'A',	0,
/*0x1F		s*/		's',	'S',	0,
/*0x20		d*/		'd',	'D',	0,
/*0x21		f*/		'f',	'F',	0,
/*0x22		g*/		'g',	'G',	0,
/*0x23		h*/		'h',	'H',	0,
/*0x24		j*/		'j',	'J',	0,
/*0x25		k*/		'k',	'K',	0,
/*0x26		l*/		'l',	'L',	0,
/*0x27		; :*/		';',	':',	0,
/*0x28		' "*/		'\'',	'"',	0,
/*0x29		` ~*/		'`',	'~',	0,
/*0x2A		Left Shift*/	LSHIFT,	LSHIFT,	0,
/*0x2B		\ |*/		'\\',	'|',	0,
/*0x2C		z*/		'z',	'Z',	0,
/*0x2D		x*/		'x',	'X',	0,
/*0x2E		c*/		'c',	'C',	0,
/*0x2F		v*/		'v',	'V',	0,
/*0x30		b*/		'b',	'B',	0,
/*0x31		n*/		'n',	'N',	0,
/*0x32		m*/		'm',	'M',	0,
/*0x33		, <*/		',',	'<',	0,
/*0x34		. >*/		'.',	'>',	0,
/*0x35		/ ?*/		'/',	'?',	NUMSLASH,
/*0x36		RightShift*/	RSHIFT,	RSHIFT,	0,
/*0x37		**/		'*',	'*',	0,
/*0x38	Left Alt*/		LALT,	LALT,	RALT,
/*0x39	Space*/			' ',	' ',	0,
/*0x3A	CapsLock*/		CAPS,	CAPS,	0,
/*0x3B	F1*/			F1,	F1,	0,
/*0x3C	F2*/			F2,	F2,	0,
/*0x3D	F3*/			F3,	F3,	0,
/*0x3E	F4*/			F4,	F4,	0,
/*0x3F	F5*/			F5,	F5,	0,
/*0x40	F6*/			F6,	F6,	0,
/*0x41	F7*/			F7,	F7,	0,
/*0x42	F8*/			F8,	F8,	0,
/*0x43	F9*/			F9,	F9,	0,
/*0x44	F10*/			F10,	F10,	0,
/*0x45	NumLock*/		NUMLOCK,NUMLOCK,0,
/*0x46	ScrollLock*/	SCROLL,	SCROLL,	0,
/*0x47	7 Home*/		'7',	NUM7,	HOME,	/*NUM7*/
/*0x48	8 Up*/			'8',	NUM8,	UP,	/*NUM8*/
/*0x49	9 PgUp*/		'9',	NUM9,	PGUP,	/*NUM9*/
/*0x4A	NUM -*/			'-',	NUMMINUS,0,	/*NUMMINUS*/
/*0x4B	4 Left*/		'4',	NUM4,	LEFT,	/*NUM4*/
/*0x4C	5*/				'5',	0,		0,	/*NUM5*/
/*0x4D	6 Right*/		'6',	NUM6,	RIGHT,	/*NUM6*/
/*0x4E	+*/				'+',	'+',	0,
/*0x4F	1 End*/			'1',	NUM1,	END,	/*NUM1*/
/*0x50	2 Down*/		'2',	NUM2,	DOWN,	/*NUM2*/
/*0x51	3 PgDn*/		'3',	NUM3,	PGDN,	/*NUM3*/
/*0x52	0 Ins*/			'0',	NUM0,	INSERT,	/*NUM0*/
/*0x53	. Del*/			'.',	NUMDOT,	DELETE,	/*NUMDOT*/
/*0x57	F11*/			F11,	F11,	0,
/*0x58	F12*/			F12,	F12,	0,
/*0x5B	LeftWin*/		0,	0,	LWIN,
/*0x5C	RightWin*/		0,	0,	RWIN,
/*0x5D	Copy*/			0,	0,	COPY
};

#endif

