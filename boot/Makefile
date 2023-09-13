#AS86	=as86 -0 -a		# 输出16位目标文件，并输出调试信息
LD86	=ld86 -0

AS 		=as --32 -march=i386+387 -mtune=i386 -nostdlib

bootsect.o: bootsect.asm
	nasm -o $@ -g -f elf32 -F stabs $<

setup.o: setup.asm
	nasm -o $@ -g -f elf32 -F stabs $<

head.o:	head.s
	$(AS)  --generate-missing-build-notes=no -o $@ $<

bootsect.bin: bootsect.asm
	nasm -o $@ $<

setup.bin: setup.asm
	nasm -o $@ $<

head.bin: head.o
	ld -Ttext 0x0  -e startup_32 -m elf_i386  -nostdlib --print-output-format  --oformat binary -o $@ $<


.PHONY: disasm
disasm: bootsect.disasm	setup.disasm head.disasm head.bin.disasm

bootsect.disasm: bootsect.bin
	ndisasm -b 16 $< > $@

setup.disasm: setup.bin
	ndisasm -b 16 $< > $@

head.disasm: head.o
	objdump -d  $< > $@

head.bin.disasm: head.bin
	objdump -m i386  -b binary $^ -EL -D > $@


.PHONY: clean
clean:
	rm -f \
 		bootsect.bin setup.bin head.bin\
 		bootsect.o setup.o head.o \
		bootsect.disasm	setup.disasm head.disasm head.bin.disasm