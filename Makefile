AS 		=/usr/bin/as
LD		=/usr/bin/ld

ASFLAGS =--32 -march=i386+387 -mtune=i386 -nostdlib
LDFLAGS	=-s -x -M -m elf_i386 -nostdlib -O2
CFLAGS	=-c -O2 -fno-pic \
	-m32 -march=i386 -mtune=i386 \
	-fno-builtin -ffreestanding -nostdlib -nostdinc -nodefaultlibs -nostartfiles
#-fno-pic

.PHONY: all
all: disk.img

# 512B的MBR
boot/bootsect.bin:
	(cd boot; make bootsect.bin)

# 第二个磁盘块
boot/setup.bin:
	(cd boot; make setup.bin)

init/to_compile.o:
	(cd init; make to_compile.o)

system.o: boot/head.o init/main.o init/to_compile.o
	ld $(LDFLAGS) -r -o $@ $^

# 后续磁盘块
system.bin: system.o
	ld $(LDFLAGS)  -e startup_32 -Ttext-seg=0x0 --oformat binary -o $@ $^ > system.map

# 整个磁盘文件
disk.img: system.bin boot/bootsect.bin boot/setup.bin
	qemu-img create -f raw disk.img 1440K
	python3 ./combine.py
	dd if=tmp.img of=disk.img conv=notrunc
	rm -f tmp.img

boot/head.o:
	(cd boot; make head.o)

init/main.o:
	(cd init; make main.o)

.PHONY: run
run: disk.img
	qemu-system-i386 -fda disk.img

.PHONY: gdb-server
gdb-server: disk.img
	qemu-system-i386 -m 16m -boot a -fda disk.img -S -s

.PHONY: gdb-client
gdb-client: disk.img system.o
	gdb   -ex 'set arch i80386' -ex 'target remote localhost:1234' -ex 'file system.o'

system.sym: system.o
	nm system.o | sort | sed 's/\ A\ /\ /g' > system.sym

.PHONY: bochs-run
bochs-run: disk.img system.sym
# bochs-2.7
	bochs -f bochsrc.bxrc -q

build-dev-env:
	pacman -S bin86
	paru -S nasm

.PHONY: clean
clean:
	rm -f system.map system.o system.bin disk.img bochs.log bochsdbg.log system.sym
	(cd boot; make clean;)
	(cd init; make clean;)