#include "io.h"

void playSound(uint32_t nFrequence) {
  uint32_t afreq;
  uint8_t tmp;

  afreq = 1193180 / nFrequence; // set PIT to correct frequncy
  outb(0x43, 0xb6);
  outb(0x42, (uint8_t)afreq);
  outb(0x42, (uint8_t)(afreq >> 8));

  tmp = inb(0x61);
  if(tmp != (tmp | 3)) {
    outb(0x61, tmp | 3);        // play sound
  }
}


void stopSound() {
  uint8_t tmp = inb(0x61) & 0xFC;
  outb(0x61, tmp);
}
