#include	"syscall.h"
#include	"klib.h"
#include	"tty.h"
#include	"global.h"
#include	"i8259.h"

PUBLIC	int	vsprintf(char *buf,const char* fmt,arg_list argv)
{
	char* p;
	char* tmp;

	for(p = buf;*fmt;fmt++)
	{
		if(*fmt != '%')
		{
			*p++ = *fmt;
			continue;
		}
		fmt++;
		switch(*fmt)
		{
			case	'd':
				dec2a(p,*(int*)argv++);
				break;
			case	'x':
			case	'X':
				itoa(p,*argv++);

				break;
			case	's':
				tmp = (char*)*argv++;
				while(*tmp)
				{
					*p++ = *tmp++;
				}
				break;
		}
		while(*p++){}
	}
	//*p = '\0';

	return (p - buf);
}

PUBLIC	unsigned int	sys_get_ticks()
{
	return ticks;
}

PUBLIC	void	sys_write(char* buf,int strlen,PROCESS* p_proc)
{
	while(strlen)
	{
		out_char(tty_table[p_proc->tty].p_console,*buf++);
		strlen--;
	}
}

PUBLIC	int		printf(const char* fmt,...)
{
	arg_list	argv = (arg_list)((char*)(&fmt) + 4);
	char		buf[256] = {0};

	int			i = vsprintf(buf,fmt,argv);

	write(buf,i);
	
	return i;
}

PUBLIC	int		printl(const char* fmt,...)
{
	arg_list	argv = (arg_list)((char*)(&fmt) + 4);
	char		buf[256] = {0};

	int			i = vsprintf(buf,fmt,argv);

	printx(buf,i);
	
	return i;
}

PUBLIC	void	sys_printx(char* buf,int strlen,PROCESS* p_proc)
{
	int i,j;
	char* p;
	char k_reenter_err[] = "k_reenter error for unknown reason\n";

	if(k_reenter == 0)		//called in ring 1-3
	{
		
	}
	else if(k_reenter > 0)	//called in ring 0
	{
	}
	else					//unknown error
	{
		disable_int();
		for(i = 0;i < TTY_NUM;i++)
		{
			p = k_reenter_err;
			while(*p)
			{
				out_char(tty_table[i].p_console,*p++);
			}
		}
	//	__asm__ __volatile("hlt");
		while(1){}
	}

	if(k_reenter > 0 || p_proc->type == 1)
	{
		disable_int();
		DispColor = 0x0C;
		for(i = 0;i < TTY_NUM;i++)
		{
			p = buf;
			j = strlen;
			while(j)
			{
				out_char(tty_table[i].p_console,*p++);
				j--;
			}
		}
		//__asm__ __volatile__("hlt");
		//__asm__ __volatile__("in $0x20,%%al");
		while(1){}
	}
	else
	{
		while(strlen)
		{
			out_char(tty_table[p_proc->tty].p_console,*buf++);
			strlen--;
		}
	}
}
