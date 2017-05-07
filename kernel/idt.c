#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct idt_entry {
	uint16_t offset_1; // Offset bits 0..15
	uint16_t selector; // A code segment selector in GDT or LDT
	uint8_t zero;      // Unused, set to 0
	uint8_t type_attr; // Type and attributes
	uint16_t offset_2; // Offset bits 16..31
};

struct idt_ptr {
	uint16_t idt_length;
	uint32_t idt_addr;
};

struct idt_entry idt[256];
struct idt_ptr idtp;

extern void idt_load();

void idt_set(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags){
    idt[num].offset_1 = (base & 0xFFFF);
    idt[num].offset_2 = (base >> 16) & 0xFFFF;

    idt[num].selector = sel;
    idt[num].zero = 0;
    idt[num].type_attr = flags;
}

void idt_init(){

    idtp.idt_length = (sizeof (struct idt_entry) * 256) - 1;
    idtp.idt_addr = &idt;

    idt_load();
}
