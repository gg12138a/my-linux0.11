AS86	=as86 -0 -a		# 输出16位目标文件，并输出调试信息
LD86	=ld86 -0

AS 		=as --32 -march=i386+387 -mtune=i386 -nostdlib
LD		=ld

LDFLAGS	=-s -x -M -m elf_i386 -nostdlib
CFLAGS	= -c -m32 -march=i386 -mtune=i386 \
	-nostdinc -nostdlib -nostartfiles -nodefaultlibs \
	-ffreestanding -fno-stack-protector -fno-exceptions

.PHONY: all
all: disk-img

# 512B的MBR
boot/bootsect.bin:
	(cd boot; make bootsect.bin)

# 第二个磁盘块
boot/setup.bin:
	(cd boot; make setup.bin)

# 后续磁盘块
system-bin: boot/head.o init/main.o
	$(LD) $(LDFLAGS) -o $@ $< > system.map

# 整个磁盘文件
disk-img: system-bin boot/bootsect.bin boot/setup.bin
	# 把这些拷贝到一个文件中
	dd if=/dev/urandom of=disk-img bs=1M count=1


boot/head.o:
	(cd boot; make head.o)

init/main.o:
	(cd init; make main.o)

.PHONY: clean
clean:
	rm -f system.map
	(cd boot; make clean;)
	(cd init; make clean;)

