#ifndef	_VGA_H_
#define	_VGA_H_

#include	"type.h"

#define	V_MEM_BASE	0xB8000
#define	V_MEM_SIZE	KB(32)
#define	SCREEN_WIDTH	80
#define	SCREEN_HEIGHT	25

#define	VGA_CRTC_AR		0x3d4
#define	VGA_CRTC_DR		0x3d5
#define	CRTC_DR_START_H		0xc
#define	CRTC_DR_START_L		0xd
#define	CRTC_DR_CURSOR_H	0xe
#define	CRTC_DR_CURSOR_L	0xf

PUBLIC	void	set_cursor(int pos);
PUBLIC	void	screen_scroll();

#endif

