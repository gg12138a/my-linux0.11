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

head.o.bin: head.o
	ld -Ttext 0x0 -e startup_32 -m elf_i386 -nostdlib --oformat binary -o $@ $<


.PHONY: disasm
disasm: bootsect.bin.disasm	setup.bin.disasm head.o.disasm head.bin.disasm

bootsect.bin.disasm: bootsect.bin
	ndisasm -b 16 $< > $@

setup.bin.disasm: setup.bin
	ndisasm -b 16 $< > $@

head.o.disasm: head.o
	objdump -d  $< > $@

head.bin.disasm: head.o.bin
	objdump -m i386  -b binary $^ -EL -D > $@


.PHONY: clean
clean:
	rm -f *.bin *.o *.disasm