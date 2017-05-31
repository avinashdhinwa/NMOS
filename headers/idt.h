#ifndef _IDT_H
#define _IDT_H

extern "C" {  // fix annoying problem where it wouldn't link

#include "pic.h"
#include "screen.h"
#include "config.h"
#include "ports.h"
#include "keyboard.h"
#include "memory.h"

#define MAX_ISR_NUM 33   /* 33 is keyboard handler IRQ1 */
                         /* Set this to the highest ISR you use */

extern void isr_irq0(void);
extern void isr_irq1(void);

/* Define an ISR stub for IRQ0 */
__asm__(".global isr_irq0\n"
        "isr_irq0:\n\t"
        "cld\n\t"                    /* Set direction flag forward for C functions */
        "pusha\n\t"                  /* Save all the registers */
        /* Other stuff here */
        "call isr_irq0_handler\n\t"
        "popa\n\t"                   /* Restore all the registers */
        "iret");

void isr_irq0_handler(void) {

  /* End of interrupt */
  _PIC_sendEOI(0x0);
  return;
}

/* Define an ISR stub for IRQ1 */
__asm__(".global isr_irq1\n"
        "isr_irq1:\n\t"
        "cld\n\t"                    /* Set direction flag forward for C functions */
        "pusha\n\t"                  /* Save all the registers */
        "call isr_irq1_handler\n\t"
        "popa\n\t"                   /* Restore all the registers */
        "iret");

void isr_irq1_handler(void) {
  char s = inb(0x60);

  key_packet retPacket;

  if(s < 0) {
    retPacket.keyflags = 1;
    s += 0x80;                        // add 0x80 to get actual key pressed
  } else if(s == 0x3a) {              // capslock
    capsStatus ^= 1;
  }

  if(s == 0x2a || s == 0x36) {        // shift pressed or released
    capsStatus ^= 1;
  }

  if(capsStatus == 1) {
    retPacket.key = scancode[(s*2)-1];
  } else {
    retPacket.key = scancode[(s*2)-2];
  }

  curPacket = retPacket;

  //printChar(retPacket.key);

  /* End of interrupt */
	_PIC_sendEOI(0x1);
  return;
}

struct idt_entry_s {
  uint16_t offset_1;                    // offset bits 0..15
  uint16_t selector;                    // a code segment selector in the GDT or LDT
  uint8_t zero;                         // unused
  uint8_t type_attr;                    // type and attributes
  uint16_t offset_2;                    // offset bits 16..31
} __attribute__ ((packed));

typedef struct idt_entry_s idt_entry;

void fillidt(void* idt, uint16_t select, void (*offset)(void), int type, int perm) {
  size_t idt_ent_size = (size_t) 8;       // 8 bytes per entry

	uint8_t type_attr;

	int segment = 0;

	type_attr = 1;													// present
	type_attr <<= 2;												// 2 bits to set
	type_attr |= perm;											// perm is 2 bits
	type_attr <<= 1;												// 1 bit to set
	type_attr |= segment;										// segment is 1 bit
	type_attr <<= 4;												// 4 bits to set
	type_attr |= type;											// type is 4 bits

  idt_entry idt_desc = {static_cast<uint16_t>(((uintptr_t)offset) & 0xffff), select, 0, type_attr, static_cast<uint16_t>(((uintptr_t)offset & 0xffff0000) >> 16)};
  memcpy(idt, &idt_desc, idt_ent_size);		// copy the descriptor
}

static inline void lidt(void* base, uint16_t size) {
    struct {
        uint16_t length;
        void*    base;
    } __attribute__((packed)) IDTR = {size, base};

    __asm__ ("lidt %0" : : "m"(IDTR));
}

void init_idt() {
  idt_entry *idt = (idt_entry *)0x00000;

	fillidt(idt+32, 8, isr_irq0, 0xe, 0);
	fillidt(idt+33, 8, isr_irq1, 0xe, 0);

	lidt(idt, sizeof(idt_entry)*(MAX_ISR_NUM+1) - 1);

	_PIC_remap(32, 40);													// remap the PIC

	__asm__ ("sti");												// re-enable interrupts
}

}

#endif
