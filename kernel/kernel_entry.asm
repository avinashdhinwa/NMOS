[bits 32]
[extern main]

	call main
	jmp $
	
global idt_load
extern idtp
idt_load:
    lidt [idtp]
    ret
	