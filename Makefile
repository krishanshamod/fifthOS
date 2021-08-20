    OBJECTS = loader.o kmain.o drivers/io.o drivers/frame_buffer.o drivers/serial_port.o segmentation/gdt.o segmentation/segments.o interrupts/keyboard.o interrupts/interrupt_handlers.o interrupts/interrupts.o interrupts/pic.o interrupts/idt.o
    CC = gcc
    CFLAGS = -m32 -nostdlib -fno-builtin -fno-stack-protector \
         -Wno-unused -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c -masm=intel
    LDFLAGS = -T link.ld -melf_i386
    AS = nasm
    ASFLAGS = -f elf

    all: kernel.elf

    kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

    os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R                              \
                    -b boot/grub/stage2_eltorito    \
                    -no-emul-boot                   \
                    -boot-load-size 4               \
                    -A os                           \
                    -input-charset utf8             \
                    -quiet                          \
                    -boot-info-table                \
                    -o os.iso                       \
                    iso

    run: os.iso
	bochs -f bochsrc.txt -q

    %.o: %.c
	$(CC) $(CFLAGS)  $< -o $@

    %.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

    clean:
	rm -rf *.o kernel.elf os.iso
