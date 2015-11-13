#include	"vga.h"
#include	"tty.h"
#include 	"keyboard.h"
#include	"global.h"

#define	CONSOLE_FIRST	con_table
#define	CONSOLE_END	con_table + CONSOLE_NUM

PUBLIC	void	set_cursor(int pos)
{
	disable_int();
	out_byte(VGA_CRTC_AR,CRTC_DR_CURSOR_H);
	out_byte(VGA_CRTC_DR,(pos >> 8) & 0xff);
	out_byte(VGA_CRTC_AR,CRTC_DR_CURSOR_L);
	out_byte(VGA_CRTC_DR,pos & 0xff);
	enable_int();
}

PUBLIC	void	set_screen(int start_pos)
{
	disable_int();
	out_byte(VGA_CRTC_AR,CRTC_DR_START_H);
	out_byte(VGA_CRTC_DR,(start_pos >> 8) & 0xff);
	out_byte(VGA_CRTC_AR,CRTC_DR_START_L);
	out_byte(VGA_CRTC_DR,(start_pos) & 0xff);
	enable_int();
}

PUBLIC	void	scroll_screen(CONSOLE * p_con,int direction)
{
	if(direction == UP)
	{
		if(p_con->v_mem_start < p_con->v_mem_size - SCREEN_WIDTH * SCREEN_HEIGHT)
		{
			p_con->v_mem_start += SCREEN_WIDTH;
		}
	}
	else if(direction == DOWN)
	{
		if(p_con->v_mem_start > 0)
		{
			p_con->v_mem_start -= SCREEN_WIDTH;
		}
	}
//	set_screen(p_con->v_mem_base + p_con->v_mem_start);
}

