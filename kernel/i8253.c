#include	"i8253.h"

PUBLIC	void	init8253()
{
	out_byte(TIMER_CTL,TIMER_MODE);
	out_byte(TIMER_COUNTER0,(TIMER_FREQ/IRQ0_FREQ) & 0xff);
	out_byte(TIMER_COUNTER0,(TIMER_FREQ/IRQ0_FREQ)>>8 & 0xff);
}

