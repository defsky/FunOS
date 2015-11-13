#include	"tty.h"
#include	"keyboard.h"
#include	"clock.h"
#include 	"global.h"
#include	"debug.h"

#define	TTY_FIRST	tty_table
#define	TTY_END		tty_table + CONSOLE_NUM

PUBLIC	void	init_tty(TTY * p_tty)
{
	int tty_seq = p_tty - TTY_FIRST;
	
	p_tty->count = 0;
	p_tty->p_head = p_tty->p_tail = p_tty->buf;
	
	init_console(p_tty);
}

PUBLIC	void	init_console(TTY * p_tty)
{
	int con_seq = p_tty - TTY_FIRST;
	int console_vmem_size = V_MEM_SIZE >> 1;	//in word
	console_vmem_size = console_vmem_size / CONSOLE_NUM;
	console_vmem_size = console_vmem_size / SCREEN_WIDTH;
	console_vmem_size = console_vmem_size * SCREEN_WIDTH;
	
	p_tty->p_console = &con_table[con_seq];
	p_tty->p_console->v_mem_base = console_vmem_size * con_seq;
	p_tty->p_console->v_mem_size = console_vmem_size;
	p_tty->p_console->v_mem_start = 0;
	p_tty->p_console->cursor = 0;
	
	if(con_seq == 0)
	{
		p_tty->p_console->cursor = DisPos / 2;
		DisPos = 0;
	}
	out_char(p_tty->p_console,'t');
	out_char(p_tty->p_console,'t');
	out_char(p_tty->p_console,'y');
	out_char(p_tty->p_console,con_seq + '0');
	out_char(p_tty->p_console,'#');
}

PUBLIC	void	task_tty()
{
	TTY * p;
	
	init_keyboard();
	
	for(p = TTY_FIRST;p < TTY_END;p++)
	{
		init_tty(p);
	}
	
	switch_console(0);
	
	init_clock();

	while(1)
	{
		for(p = TTY_FIRST;p < TTY_END;p++)
		{
			tty_do_read(p);
			tty_do_write(p);
		}
			//assert(p != TTY_END);
	}
}

PUBLIC	void	tty_do_read(TTY * p_tty)
{
	if(is_current(p_tty->p_console))
	{
		keyboard_read(p_tty);
	}
}

PUBLIC	void	tty_do_write(TTY * p_tty)
{
	while(p_tty->count > 0)
	{
		out_char(p_tty->p_console,*p_tty->p_head++);
		if(p_tty->p_head >= p_tty->buf + IN_BUF_LEN)
		{
			p_tty->p_head = p_tty->buf;
		}
		p_tty->count--;	
	}
}

PUBLIC	void	out_char(CONSOLE * p_con,char c)
{
	u8 *	p_vmem = (u8 *)(V_MEM_BASE + p_con->v_mem_base * 2);
	
	switch(c)
	{
		case '\n':
			p_con->cursor = p_con->cursor / SCREEN_WIDTH;
			p_con->cursor = (p_con->cursor + 1) * SCREEN_WIDTH;
			c = 0;
			break;
		case '\b':
			p_con->cursor -= 1;
			p_vmem += p_con->cursor * 2;
			c = ' ';
			*p_vmem++ = c;
			*p_vmem = DispColor;
			c = 0;
			break;
		default:
			break;
	}
	
	if(c)
	{
		p_vmem += p_con->cursor * 2;
		*p_vmem++ = c;
		*p_vmem = DispColor;
		p_con->cursor++;
	}
	
	p_con->cursor = (p_con->cursor >= p_con->v_mem_size ? 0 : p_con->cursor);
	
	while(p_con->cursor - p_con->v_mem_start >= SCREEN_WIDTH * SCREEN_HEIGHT)
	{
		scroll_screen(p_con,UP);
	}
	if(is_current(p_con))
	{
		flush(p_con);
	}
}

PUBLIC	int	is_current(CONSOLE * p_con)
{
	return (p_con == &con_table[current_console]?1:0);
}

PUBLIC	void	switch_console(int con_seq)
{
	if(con_seq < 0 || con_seq >= CONSOLE_NUM)
	{
		return;
	}
	
	current_console = con_seq;
	
	set_cursor(con_table[current_console].v_mem_base + con_table[current_console].cursor);
	set_screen(con_table[current_console].v_mem_base + con_table[current_console].v_mem_start);
}

PUBLIC	void	flush(CONSOLE * p_con)
{
		set_cursor(p_con->v_mem_base + p_con->cursor);
		set_screen(p_con->v_mem_base + p_con->v_mem_start);
}
