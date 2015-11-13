#include	"proc.h"
#include	"global.h"
#include	"clock.h"
#include	"i8259.h"
#include	"tty.h"

kernel_main()
{
	disp_str("-------------\"kernel_main\" start-----------------\n");

	proc_id = 0;
	ticks = 0;
	k_reenter = 0;

	pcb_init(task_tty,TASK_PROC,TTY0);
	pcb_init(TestA,USER_PROC,TTY1);
	pcb_init(TestB,USER_PROC,TTY1);
	pcb_init(TestC,USER_PROC,TTY2);

	//init_clock();

	p_proc_ready = proc_table;
	proc_start();

	//stop here
	while(1){}
}
