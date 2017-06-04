#ifndef _VGA_H
#define _VGA_H

#include "io.h"
#include "screen.h"

#define	VGA_AC_INDEX		    0x3C0
#define	VGA_AC_WRITE        0x3C0
#define	VGA_MISC_WRITE	    0x3C2
#define VGA_SEQ_INDEX		    0x3C4
#define VGA_SEQ_DATA		    0x3C5
#define	VGA_MISC_READ		    0x3CC
#define VGA_GC_INDEX 		    0x3CE
#define VGA_GC_DATA 		    0x3CF
#define VGA_CRTC_INDEX	    0x3D4
#define VGA_CRTC_DATA		    0x3D5
#define	VGA_INSTAT_READ	  	0x3DA
#define	VGA_DAC_READ_INDEX	0x3C7
#define	VGA_DAC_WRITE_INDEX	0x3C8
#define	VGA_DAC_DATA		    0x3C9

unsigned char g_320x200x256[] = {
/* MISC */
	0x63,
/* SEQ */
	0x03, 0x01, 0x0F, 0x00, 0x0E,
/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
	0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x41, 0x00, 0x0F, 0x00,	0x00
};


void _VGA_CRTC_writeReg(uint8_t reg, uint16_t val) {
  outb(VGA_CRTC_INDEX, reg);
  outb(VGA_CRTC_DATA, val);
}

uint8_t _VGA_CRTC_readReg(uint8_t reg) {
  outb(VGA_CRTC_INDEX, reg);
  return inb(VGA_CRTC_DATA);
}

void _VGA_GC_writeReg(uint8_t reg, uint16_t val) {
  outb(VGA_GC_INDEX, reg);
  outb(VGA_GC_DATA, val);
}

void _VGA_AC_writeReg(uint8_t reg, uint16_t val) {
  outb(VGA_AC_INDEX, reg);
  outb(VGA_AC_WRITE, val);
}

void _VGA_MISC_writeReg(uint16_t val) {
  outb(VGA_MISC_WRITE, val);
}

void _VGA_SEQ_writeReg(uint8_t reg, uint16_t val) {
  outb(VGA_SEQ_INDEX, reg);
  outb(VGA_SEQ_DATA, val);
}

void _VGA_writeColor(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
  outb(VGA_DAC_WRITE_INDEX, index);
  outb(VGA_DAC_DATA, r);
  outb(VGA_DAC_DATA, g);
  outb(VGA_DAC_DATA, b);
}

void _VGA_intantRead() {
  (void) inb(VGA_INSTAT_READ);
}

static int _VGA_getAddr() {
	int seg;

	outb(VGA_GC_INDEX, 6);
	seg = inb(VGA_GC_DATA);
	seg >>= 2;
	seg &= 3;
	switch(seg) {
	case 0:
	case 1:
		seg = 0xA000;
		break;
	case 2:
		seg = 0xB000;
		break;
	case 3:
		seg = 0xB800;
		break;
	}
	return seg;
}

void _VGA_drawPixel(int x, int y, char c) {
  int offset = y*320+x;
  char* vidmem = (char*) _VGA_getAddr();
  vidmem[offset] = c;
}

void _VGA_clearScreen() {
  int y;
  int x;
  for(y = 0; y < 200; y++) {
		for(x = 0; x < 320; x++) {
			_VGA_drawPixel(x, y, 1);
    }
  }
}

void _VGA_switchMode() {
  /*int i;

  _VGA_MISC_writeReg(*mode);      // miscellaneous output register
  mode++;

  for(i = 0; i < 5; i++) {
    _VGA_SEQ_writeReg(i, *mode);
		mode++;
	}

  _VGA_CRTC_writeReg(0x03, inb(VGA_CRTC_DATA) | 0x80);   // unlock crtc regs
  _VGA_CRTC_writeReg(0x11, inb(VGA_CRTC_DATA) & ~0x80);
  //mode[0x03] |= 0x80;
	//mode[0x11] &= ~0x80;

  for(i = 0; i < 25; i++) {
    _VGA_CRTC_writeReg(i, *mode);
		mode++;
	}

  for(i = 0; i < 9; i++) {
    _VGA_GC_writeReg(i, *mode);
		mode++;
	}

	for(i = 0; i < 21; i++) {
		_VGA_intantRead();
    _VGA_AC_writeReg(i, *mode);
		mode++;
	}

	_VGA_intantRead();
	outb(VGA_AC_INDEX, 0x20);*/

  _VGA_SEQ_writeReg(0x01, 0x01); // clock mode register
  _VGA_SEQ_writeReg(0x03, 0x00); // character select
  _VGA_SEQ_writeReg(0x04, 0x0e); // memory mode register

  _VGA_CRTC_writeReg(0x03, _VGA_CRTC_readReg(0x03) | 0x80);   // unlock crtc regs
  _VGA_CRTC_writeReg(0x11, _VGA_CRTC_readReg(0x03) & 0x7F);

  _VGA_AC_writeReg(0x10, 0x41);  // mode control
  _VGA_AC_writeReg(0x11, 0x00);  // overscan register
  _VGA_AC_writeReg(0x12, 0x0f);  // color plane enable
  _VGA_AC_writeReg(0x13, 0x00);  // horizontal panning
  _VGA_AC_writeReg(0x14, 0x00);  // color select

  _VGA_GC_writeReg(0x05, 0x40);  // mode register
  _VGA_GC_writeReg(0x06, 0x05);  // miscellaneous register

  _VGA_CRTC_writeReg(0x00, 0x5f); // horizontal total
  _VGA_CRTC_writeReg(0x01, 0x4f); // horizontal display enable end
  _VGA_CRTC_writeReg(0x02, 0x50); // horizontal blank start
  _VGA_CRTC_writeReg(0x03, 0x82); // horizontal blank end
  _VGA_CRTC_writeReg(0x04, 0x54); // horizontal retrace start
  _VGA_CRTC_writeReg(0x05, 0x80); // horizontal retrace end
  _VGA_CRTC_writeReg(0x06, 0xbf); // vertical total
  _VGA_CRTC_writeReg(0x07, 0x1f); // overflow register
  _VGA_CRTC_writeReg(0x08, 0x00); // preset row scan
  _VGA_CRTC_writeReg(0x09, 0x41); // maximum scan line
  _VGA_CRTC_writeReg(0x10, 0x9c); // vertical retrace start
  _VGA_CRTC_writeReg(0x11, 0x8e); // vertical retrace end
  _VGA_CRTC_writeReg(0x12, 0x8f); // vertical display enable end
  _VGA_CRTC_writeReg(0x13, 0x28); // logical width
  _VGA_CRTC_writeReg(0x14, 0x40); // underline location
  _VGA_CRTC_writeReg(0x15, 0x96); // vertical blank start
  _VGA_CRTC_writeReg(0x16, 0xb9); // vertical blank end
  _VGA_CRTC_writeReg(0x17, 0xa3); // mode control
}

#endif
