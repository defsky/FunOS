#ifndef	_KLIB_H_
#define	_KLIB_H_

#include	"type.h"

PUBLIC	u8		in_byte(u16 port);
PUBLIC	void	out_byte(u16 port,u8 data);
PUBLIC	void	itoa(unsigned char * p,u32 n);
PUBLIC	void	dec2a(char * buf,int n);

PUBLIC	void	disp_int(u32 n);
PUBLIC	void	disp_str(void *str);
PUBLIC	void	MemCpy(void *dst,void *src,int len);

#endif

