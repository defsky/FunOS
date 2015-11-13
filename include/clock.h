#ifndef	_CLOCK_H_
#define	_CLOCK_H_

#include	"type.h"
#include	"i8259.h"

#define	IRQ_CLOCK	HW_IRQ0

PUBLIC	void	init_clock();
PUBLIC	void	clock_handle(int irq);
PUBLIC	void	delay(int milliseconds);

#endif

