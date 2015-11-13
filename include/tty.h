#ifndef	_TTY_H_
#define	_TTY_H_

#include	"type.h"
#include 	"vga.h"

#define	TTY_0	0x0
#define	TTY_1	0x4B
#define	TTY_2	0x96
#define	IN_BUF_LEN	64
#define	TTY_NUM	3
#define	CONSOLE_NUM	3

#define	TTY0	0
#define	TTY1	1
#define	TTY2	2

typedef	struct	s_console
	{
		int	v_mem_base;
		int	v_mem_size;
		int	v_mem_start;
		int	cursor;
	}CONSOLE;

typedef	struct	s_tty
	{
		u8	buf[IN_BUF_LEN];
		u8 *	p_head;
		u8 *	p_tail;
		int	count;
		CONSOLE *	p_console;
	}TTY;

PUBLIC	void	init_tty(TTY * p_tty);
PUBLIC	void	init_console(TTY * p_tty);
PUBLIC	void	task_tty();
PUBLIC	void	tty_do_read(TTY * p_tty);
PUBLIC	void	tty_do_write(TTY * p_tty);
PUBLIC	void	out_char(CONSOLE * p_con,char c);
PUBLIC	int	is_current(CONSOLE * p_con);
PUBLIC	void	switch_console(int con_seq);
PUBLIC	void	flush(CONSOLE* p_con);

#endif

