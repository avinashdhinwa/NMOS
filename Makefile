C_SOURCES = $(wildcard kernel/*.cpp headers/*.cpp)
HEADERS = $(wildcard kernel/*.h headers/*.h)
OBJ = ${C_SOURCES:.cpp=.o}

all: nmos.flp

run: all
	qemu-system-i386 nmos.flp

nmos.flp: boot/bootsect2.bin kernel.bin
	qemu-img create nmos.flp 1200K
	cat $^ > tmp
	dd if=tmp of=nmos.flp conv=notrunc
	rm -rf tmp

kernel.bin: kernel/kernel_entry.o ${OBJ}
	i686-elf-ld -o $@ -Ttext 0x2000 $^ --oformat binary

%.o: %.cpp ${HEADERS}
	i686-elf-g++ -ffreestanding -Iheaders/ -Wall -Wextra -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -I ../../16bit/ -o $@

%.bin: %.asm
	nasm $< -f bin -I ../../16bit/ -o $@

clean:
	rm -fr *.bin *.dis *.o nmos.flp
	rm -fr kernel/*.o boot/*.bin headers/*.o

nmos.iso: boot/bootsect3.asm kernel.bin
	qemu-img create tmp.flp 1200K
	cat $^ > tmp
	dd if=tmp of=tmp.flp conv=notrunc
	rm -rf tmp
	rm -rf iso
	mkdir iso
	mkdir iso/boot
	cp tmp.flp iso/boot/boot
	xorriso -as mkisofs -R -J -c boot/bootcat \
                    -b boot/boot \
                    -o nmos.iso iso
	rm -rf tmp.flp
iso: nmos.iso

run-iso: nmos.iso
	qemu-system-x86_64 -boot d -cdrom nmos.iso -m 512
