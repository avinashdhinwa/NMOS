[bits 16]
[org 0x7c00]
	KERNEL_OFFSET equ 0x1000
	
	mov bp, 0x9000
	mov sp, bp
	
	mov [BOOT_DRIVE], dl
	
	mov bx, boot_msg
	call print_string
	
	;mov bx, KERNEL_OFFSET
	;mov dh, 15
	mov dl, [BOOT_DRIVE]
	call disk_load
	
	jmp pm_setup
	
	jmp $
	
BOOT_DRIVE:
	db 0
	
disk_load:
	;mov ah, 0x02
	;mov al, dh
	;mov ch, 0x00
	;mov dh, 0x00
	;mov cl, 0x02
	
	mov si, dap
	mov ah, 0x42
	
	int 0x13
	
	;cmp al, 4
	;jne disk_error_132
		
	ret
	
dap:
    db 0x10				; Size of DAP
    db 0
    dw 16				; Number of sectors to read
    dw KERNEL_OFFSET	; Offset
    dw 0				; Segment
	dd 1
    dd 0

disk_error_132:
	mov bx, disk_error_132_msg
	call print_string
	
	jmp $
	
disk_error_132_msg:
	db 'Error! Error! Something is VERY wrong! (0x132)', 0

gdt_start:

gdt_null:
    dd 0x0
    dd 0x0

gdt_code:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0

gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

boot_msg:
	db 'OS is booting files... ', 0
	
done_msg:
	db 'Done! ', 0
	
%include "boot/print_string.asm"
	
pm_setup:	
	mov bx, done_msg
	call print_string
	
    mov ax, 0
    mov ss, ax
    mov sp, 0xFFFC

    mov ax, 0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    cli
    lgdt[gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:b32
	
	[bits 32]

	VIDEO_MEMORY equ 0xb8000
	WHITE_ON_BLACK equ 0x0f

	print32:
	    pusha
	    mov edx, VIDEO_MEMORY
	.loop:
	    mov al, [ebx]
	    mov ah, WHITE_ON_BLACK
	    cmp al, 0
	    je .done
	    mov [edx], ax
	    add ebx, 1
	    add edx, 2
	    jmp .loop
	.done:
	    popa
	    ret

	b32:
	    mov ax, DATA_SEG
	    mov ds, ax
	    mov es, ax
	    mov fs, ax
	    mov gs, ax
	    mov ss, ax

	    mov ebp, 0x2000
	    mov esp, ebp

	    mov ebx, pmode_msg
	    call print32
		
		call KERNEL_OFFSET

	    jmp $
		
	pmode_msg:
		db 'Protected mode enabled!', 0
	
kernel:
	mov ebx, pmode_msg
	call print32
	jmp $
	
pmode_tst:
	db 'Testing...'
	
times 510-($-$$) db 0
db 0x55
db 0xAA