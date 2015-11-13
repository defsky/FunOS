#include	"clock.h"
#include	"syscall.h"
#include	"proc.h"
#include 	"i8259.h"
#include	"global.h"

PUBLIC	void	init_clock()
{
	irq_handle_init(IRQ_CLOCK,clock_handle);
	enable_irq(IRQ_CLOCK);
}

PUBLIC	void clock_handle(int irq)
{
	ticks++;
	proc_schedule();
}

PUBLIC	void	delay(int milliseconds)
{
	int tmp = get_ticks();
	while((get_ticks() - tmp) * 10 < milliseconds){}
}

