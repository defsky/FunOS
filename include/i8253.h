#ifndef	_I8253_H_
#define	_I8253_H_

#include	"type.h"

/*****************Device Parameters*********************/
#define	TIMER_CTL	0x43
#define	TIMER_COUNTER0	0x40
#define	TIMER_COUNTER1	0x41
#define	TIMER_COUNTER2	0x42

#define	TIMER_FREQ	1193180
#define	IRQ0_FREQ	100

/*******************8253 control word********************
 *D7D6	counter selector
 *	00	counter0
 *	01	counter1
 *	10	counter2
 *	11	invalid value
 *
 *D5D4	IO mode
 *	00	counter lock
 *	01	R/W lower byte
 *	10	R/W higher byte
 *	11	R/W lower byter first,then higher byte
 *
 *D3D2D1	work mode		
 *	000-101	mode 0-5
 *	
 *D0	number format
 *	0	binary
 *	1	decimal
 *
*********************************************************/
#define	TIMER_MODE	0x34	//00 11 010 0b

/***********************Interfaces**********************/
PUBLIC	void	init8253();

#endif

