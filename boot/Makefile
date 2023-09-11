#AS86	=as86 -0 -a		# 输出16位目标文件，并输出调试信息
LD86	=ld86 -0

AS 		=as --32 -march=i386+387 -mtune=i386 -nostdlib

bootsect.o: bootsect.asm
	nasm -o $@ -g -f elf32 -F stabs $<

setup.o: setup.asm
	nasm -o $@ -g -f elf32 -F stabs $<

bootsect.bin: bootsect.asm
	nasm -o $@ $<

setup.bin: setup.asm
	nasm -o $@ $<

head.o:	head.s
	$(AS) -o $@ $<

.PHONY: clean
clean:
	rm -f \
 		bootsect.bin setup.bin \
 		bootsect.o setup.o head.o \
		bootsect.disasm setup.disasm

.PHONY: disasm
disasm: bootsect.disasm	setup.disasm

bootsect.disasm: bootsect.bin
	ndisasm -b 16 $< > $@

setup.disasm: setup.bin
	ndisasm -b 16 $< > $@