AS86	=as86 -0 -a		# 输出16位目标文件，并输出调试信息
LD86	=ld86 -0

AS 		=as --32 -march=i386+387 -mtune=i386 -nostdlib

bootsect.o: bootsect.s
	$(AS86) -o $@ $<

bootsect.bin: bootsect.o
	$(LD86) -s -o $@ $<

setup.o: setup.s
	$(AS86) -o $@ $<

setup.bin: setup.o
	$(LD86) -s -o $@ $<

head.o:	head.s
	$(AS) -o $@ $<

.PHONY: clean
clean:
	rm -f bootsect.o bootsect.bin setup.o setup.bin head.o