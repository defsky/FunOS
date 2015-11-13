#include	"keyboard.h"
#include	"i8259.h"
#include	"type.h"
#include	"keymap.h"
#include	"global.h"
#include 	"vga.h"

PRIVATE	u8	flag_e0;
PRIVATE	u8	l_shift;
PRIVATE	u8	r_shift;
PRIVATE	u8	l_ctrl;
PRIVATE	u8	r_ctrl;
PRIVATE	u8	l_alt;
PRIVATE	u8	r_alt;
PRIVATE	u8	caps_lock;
PRIVATE	u8	num_lock;
PRIVATE	u8	scroll_lock;

PRIVATE	KB_INPUT	kb_in;

PUBLIC	void	init_keyboard()
{
	kb_in.p_head = kb_in.p_tail = kb_in.buf;
	kb_in.count = 0;
	
	flag_e0 = 0;
	l_shift	= r_shift = 0;
	l_ctrl	= r_ctrl = 0;
	l_alt	= r_alt = 0;
	caps_lock = scroll_lock = 0;
	num_lock = 1;

	kb_setleds();
	
	irq_handle_init(IRQ_KEYBOARD,keyboard_handle);
	enable_irq(IRQ_KEYBOARD);
}

PUBLIC	void	keyboard_handle(int irq)
{
	u8 c = in_byte(KB_DATA_PORT);//read_key();
/*
disp_int(c);
*/
	if(kb_in.count < KB_BUF_LEN)
	{
		*kb_in.p_tail++ = c;
		if(kb_in.p_tail == kb_in.buf + KB_BUF_LEN)
		{
			kb_in.p_tail = kb_in.buf;
		}
		kb_in.count++;
	}
}

PUBLIC	void	keyboard_read(TTY * p_tty)
{
	u8 c = 0;
	u32 key = 0;
	u8 flag_make = 0;
	u32 * key_row = 0;
	
	if(kb_in.count > 0)
	{
		c = get_byte_from_kbuf();	
		
		if(c == 0xE1)
		{
			int i;
			u8 pause[] = {0x1D,0x45,0xE1,0x9D,0xC5};
			
			key = PAUSE;
			for(i = 0;i < 5;i++)
			{
				c = get_byte_from_kbuf();
				if(pause[i] != c)
				{
					key = 0;
					c = 0;
					break;
				}
			}
		}
		else if(c == 0xE0)
		{
			c = get_byte_from_kbuf();
			if(c == 0x2A)
			{
				if(0xE0 == get_byte_from_kbuf())
					if(0x37 == get_byte_from_kbuf())
						key = PRTSCR;
			}
			else if(c == 0xB7)
			{
				if(0xE0 == get_byte_from_kbuf())
					if(0xAA == get_byte_from_kbuf())
						key = PRTSCR;
			}
			if(key == 0)
				flag_e0 = 1;
		}
		
		if((key != PAUSE) && (key != PRTSCR))
		{
			int column = 0;
			int caps = 0;

			flag_make = c & 0x80?0:1;
			
			key_row = &key_map[(c & 0x7F) * KEY_MAP_COLS];
			
			caps = l_shift || r_shift;
			if(caps_lock)
			{
				if(*key_row >= 'a' && *key_row <= 'z')
				{
					caps = !caps;
				}
			}
			
			if(caps)
			{
				column = 1;
			}

			if(flag_e0)
			{
				column = 2;
				flag_e0 = 0;
			}
			
			key = *(key_row + column);
			switch(key)
			{
				case	LSHIFT:
					l_shift = flag_make;
					break;
				case	RSHIFT:
					r_shift = flag_make;
					break;
				case	LALT:
					l_alt = flag_make;
					break;
				case	RALT:
					r_alt = flag_make;
					break;
				case	LCTRL:
					l_ctrl = flag_make;
					break;
				case	RCTRL:
					r_ctrl = flag_make;
					break;
				default:
					break;
			}
		}
		if(flag_make)
		{
			switch(key)
			{
				case	CAPS:
					caps_lock = !caps_lock;
					kb_setleds();
					break;
				case	NUMLOCK:
					num_lock = !num_lock;
					kb_setleds();
					break;
				case	SCROLL:
					scroll_lock = !scroll_lock;
					kb_setleds();
					break;
			}
			
			key = key | (l_shift	?	FLAG_LSHIFT	:0);
			key = key | (r_shift	?	FLAG_RSHIFT	:0);
			key = key | (l_alt		?	FLAG_LALT	:0);
			key = key | (r_alt		?	FLAG_RALT	:0);
			key = key | (l_ctrl		?	FLAG_LCTRL	:0);
			key = key | (r_ctrl		?	FLAG_RCTRL	:0);
			
			in_process(p_tty,key);
		}
	}
}

PUBLIC	void	in_process(TTY * p_tty,u32 key)
{
	//u8 out_buf[2] = {'\0','\0'};
	u32 raw_code;
	
	if(!(key & FLAG_EXT))
	{
		//out_buf[0] = key_code & 0xFF;
		put_char(p_tty,key);
	}
	else
	{
		raw_code = key & KEY_CODE_MASK;
		switch(raw_code)
		{
			case	F1:
			case	F2:
			case	F3:
				//disp_int(key);
				if((key & FLAG_LALT) || (key & FLAG_RALT))
				{
					switch_console((raw_code - F1) >> 16);
				}
				break;
			default:
				put_char(p_tty,raw_code);
		}
	}
}

PUBLIC	u8	get_byte_from_kbuf()
{
	u8 c;
	
	while(kb_in.count <= 0){}
	
	disable_int();
	c = *kb_in.p_head++;
	if(kb_in.p_head == kb_in.buf + KB_BUF_LEN)
	{
		kb_in.p_head = kb_in.buf;
	}
	kb_in.count--;
	enable_int();
	
	return c;
}

PUBLIC	void	put_char(TTY * p_tty,u32 key)
{
	if(p_tty->count < IN_BUF_LEN)
	{
		*p_tty->p_tail++ = key & 0xFF;//disp_str(out_buf);
		if(p_tty->p_tail >= p_tty->buf + IN_BUF_LEN)
		{
			p_tty->p_tail = p_tty->buf;
		}
		p_tty->count++;
	}
}

PUBLIC	void	kb_wait_buf()
{
	u8 kb_buf;
	do
	{
		kb_buf = in_byte(KB_CMD_PORT);
	}while(kb_buf & 0x02);
}

PUBLIC	void	kb_wait_ack()
{
	u8 kb_data;
	do
	{
		kb_data = in_byte(KB_DATA_PORT);
	}while(kb_data != KB_ACK);
}

PUBLIC	void	kb_setleds()
{
	u8 leds = caps_lock << 2 | num_lock << 1 | scroll_lock;

	kb_wait_buf();
	out_byte(KB_DATA_PORT,KB_LED);
	kb_wait_ack();
	kb_wait_buf();
	out_byte(KB_DATA_PORT,leds);
	kb_wait_ack();
}

