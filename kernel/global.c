#define	GLOBAL_VARIBLE_HERE

#include	"global.h"
#include	"clock.h"
#include	"syscall.h"

system_call	sys_call_table[SYS_CALL_NUM] = {sys_get_ticks,sys_write,sys_printx};
PUBLIC	TTY	tty_table[TTY_NUM] = {0};
PUBLIC	CONSOLE	con_table[CONSOLE_NUM] = {0};

