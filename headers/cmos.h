#ifndef _CMOS_H
#define _CMOS_H

void _CMOS_write(int reg, int nmi_disable, uint8_t val) {
  outb(0x70, (nmi_disable << 7) | reg); // select registor
  outb(0x71, val);
}

uint8_t _CMOS_read(int reg, int nmi_disable) {
  outb(0x70, (nmi_disable << 7) | reg); // select registor
  return inb(0x71);
}

#endif
