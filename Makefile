C_SOURCES = $(wildcard kernel/*.c headers/*.c)
HEADERS = $(wildcard kernel/*.h headers/*.h)
OBJ = ${C_SOURCES:.c=.o}

all: os-image

run: all
	qemu-system-i386 os-image

os-image: boot/bootsect2.bin kernel.bin
	cat $^ > os-image

kernel.bin: kernel/kernel_entry.o ${OBJ}
	i686-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

%.o: %.c ${HEADERS}
	i686-elf-gcc -ffreestanding -Iheaders/ -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -I ’../../16bit/’ -o $@

%.bin: %.asm
	nasm $< -f bin -I ’../../16bit/’ -o $@

clean:
	rm -fr *.bin *.dis *.o os-image
	rm -fr kernel/*.o boot/*.bin headers/*.o

os-image.iso: os-image
	mkdir iso
	mkdir iso/boot
	cp os-image iso/boot/boot
	xorriso -as mkisofs -R -J -c boot/bootcat \
						-b boot/boot -no-emul-boot -boot-load-size 4 \
						-o os-image.iso iso

iso: os-image.iso

run-iso: os-image.iso
	qemu-system-x86_64 -boot d -cdrom os-image.iso -m 512
