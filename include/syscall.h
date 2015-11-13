#ifndef	_SYSCALL_H_
#define	_SYSCALL_H_

#include	"type.h"
#include	"proc.h"

#define	SYS_CALL_NUM	3

typedef	u32 *	arg_list;

typedef void*	system_call;
//system call entry : int 80h
PUBLIC	void		sys_call_entry();

//system call process
PUBLIC	unsigned int	sys_get_ticks();
PUBLIC	void			sys_write(char* buf,int strlen,PROCESS* p_proc);
PUBLIC	void			sys_printx(char* buf,int strlen,PROCESS* p_proc);

//system call interface
PUBLIC	unsigned int	get_ticks();
PUBLIC	void			write(char * buf,int strlen);
PUBLIC	void			printx(char * buf,int strlen);

PUBLIC	int				printf(const char* fmt,...);
PUBLIC	int				printl(const char* fmt,...);

#endif
