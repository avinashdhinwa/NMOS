#ifndef _IDT_H
#define _IDT_H

#include "pic.h"
#include "screen.h"
#include "config.h"
#include "ports.h"

void irq0func();

void* irq0handler(void)
{
	volatile void* address;
	asm goto("jmp %l[ISREnd]" ::: "memory" : ISREnd);																	// jump to end of ISR
	asm volatile(".align 16" ::: "memory");
	ISRStart:
	asm volatile(
		"pushal							\n\t"																												// save regs
		"pushl %%ebp				\n\t"
		"movl %%esp, %%ebp	\n\t"
		"cld										"																												// direction flag is used by gcc
		::: "memory");
	asm volatile(
		"pushl %%es       	\n\t"																												// save segment regs
		"pushl %%ds       	\n\t"
		"movw $16, %%cx   	\n\t"																												// set segment regs for kernel
		"movw %%cx, %%ds  	\n\t"
		"movw %%cx, %%es  	\n\t"
		"addl $4, (%%esp) 	\n\t"																												// add 4 to make it point to PUSHAD struct
		"call *%%eax      	\n\t"																												// call IRQ func.
		:: "a"(irq0func): "memory");																										// eax is the IRQ 0 function
	asm volatile(
		"popl %%es					\n\t"																												// restore everything
		"popl %%ds					\n\t"
		"leave							\n\t"
		"popal							\n\t"
		"iret										"																												// return from interrupt handler
		::: "memory");
	ISREnd:
	asm goto(
		".intel_syntax noprefix        \n\t"
		"mov eax, offset %l[ISRStart]	 \n\t"																						// get address of ISR start
		"mov [ebx], eax                \n\t"																						// set ebx to adderss
		".att_syntax                       "
		:: "b"(&address) : "eax", "memory" : ISRStart);																	// ebx to be set to adderss
	return (void*) address;																														// return adderss of ISR
}

void irq0func() {
	// do something
	_PIC_sendEOI(0);
}

void fillidt(void* idt, int select, int offset, int type, int perm) {
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

  struct {
    uint16_t offset_1;                    // offset bits 0..15
    uint16_t selector;                    // a code segment selector in the GDT or LDT
    uint8_t zero;                         // unused
    uint8_t type_attr;										// type and attributes
    uint16_t offset_2;                    // offset bits 16..31
  } __attribute__ ((packed)) idt_desc = {offset & 0xffff, select, 0, type_attr, (offset & 0xffff0000) >> 16};
  memcpy(&idt_desc, idt, idt_ent_size);		// copy the descriptor
}

static inline void lidt(void* base, uint16_t size) {
    struct {
        uint16_t length;
        void*    base;
    } __attribute__((packed)) IDTR = {size, base};

    asm ("lidt %0" : : "m"(IDTR));
}

void init_idt() {
  void* idt = (void*) 0x00000;

	fillidt(idt+(32*8), 8, irq0handler(), 0xe, 0);

	lidt(idt, 128);

	_PIC_remap(32, 40);													// remap the PIC

	//asm ("sti");												// re-enable interrupts
}

#endif
