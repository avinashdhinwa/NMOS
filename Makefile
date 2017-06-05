C_SOURCES = $(wildcard kernel/*.c headers/*.c)
HEADERS = $(wildcard kernel/*.h headers/*.h)
OBJ = ${C_SOURCES:.c=.o}

all: nmos.flp

run: all
	qemu-system-i386 nmos.flp

nmos.flp: boot/bootsect2.bin kernel.bin
	cat $^ > tmp.flp
	qemu-img create nmos.flp 1200K
	dd conv=notrunc if=tmp.flp of=nmos.flp
	rm -rf tmp.flp

kernel.bin: kernel/kernel_entry.o ${OBJ}
	i686-elf-ld -o $@ -Ttext 0x2000 $^ --oformat binary

%.o: %.c ${HEADERS}
	i686-elf-gcc -ffreestanding -Iheaders/ -Wall -Wextra -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -I ../../16bit/ -o $@

%.bin: %.asm
	nasm $< -f bin -I ../../16bit/ -o $@

clean:
	rm -fr *.bin *.dis *.o *.flp *.iso
	rm -fr kernel/*.o boot/*.bin headers/*.o

nmos.iso: nmos.flp
	rm -rf iso
	mkdir iso
	mkdir iso/boot
	cp nmos.flp iso/boot/boot
	xorriso -as mkisofs -R -J -c boot/bootcat \
						-b boot/boot \
						-o nmos.iso iso

iso: nmos.iso

run-iso: nmos.iso
	qemu-system-x86_64 -boot d -cdrom nmos.iso -m 512
