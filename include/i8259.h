#ifndef	_I8259_H_
#define	_I8259_H_

#include	"type.h"

#define INT_M_CTL	0x20
#define INT_M_MASK	0x21
#define INT_S_CTL	0xA0
#define INT_S_MASK	0xA1
#define	EOI		0x20
#define	HW_IRQ0		0x20

typedef void 	(*int_handle)();
typedef	void	(*IRQHandle)(int irq);

PUBLIC	void	hwint00();
PUBLIC	void	hwint01();
PUBLIC	void	hwint00();
PUBLIC	void	hwint01();
PUBLIC	void	hwint02();
PUBLIC	void	hwint03();
PUBLIC	void	hwint04();
PUBLIC	void	hwint05();
PUBLIC	void	hwint06();
PUBLIC	void	hwint07();
PUBLIC	void	hwint08();
PUBLIC	void	hwint09();
PUBLIC	void	hwint10();
PUBLIC	void	hwint11();
PUBLIC	void	hwint12();
PUBLIC	void	hwint13();
PUBLIC	void	hwint14();
PUBLIC	void	hwint15();

PUBLIC	void	enable_int();
PUBLIC	void	disable_int();

PUBLIC	void	enable_irq(u8 irq);

PUBLIC	void	init8259();
PUBLIC	void	SuperviousHandle(int irq);
PUBLIC	void	irq_handle_init(u8 irq,IRQHandle handle);

#endif

