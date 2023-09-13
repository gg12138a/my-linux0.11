AS 		=/usr/bin/as
LD		=/usr/bin/ld

ASFLAGS =--32 -march=i386+387 -mtune=i386 -nostdlib

# -M 生成链接映射文件（Linker Map File）
# --print-output-format
LDFLAGS	=-s -x -m elf_i386 -nostdlib -O1
CFLAGS	=-c -O1 -finline-functions -nostartfiles -fno-pie \
	-m32 -march=i386 -mtune=i386 \
	-fno-builtin -ffreestanding -nostdlib -nostdinc -nodefaultlibs -nostartfiles

.PHONY: all
all: disk.img

# 512B的MBR
boot/bootsect.bin:
	(cd boot; make bootsect.bin)

# 第二个磁盘块
boot/setup.bin:
	(cd boot; make setup.bin)


system.o: 	init/to_compile.o \
			boot/head.o init/main.o mm/memory.o \
			kernel/kernel.o
	ld $(LDFLAGS) -Tlinker.ld -m elf_i386 -nostdlib  -r -o $@ $^

# 后续磁盘块
system.bin: system.o
	ld  -Tlinker.ld -m elf_i386 -nostdlib --print-output-format --oformat binary -o $@ $<


# 整个磁盘文件
disk.img: system.bin boot/bootsect.bin boot/setup.bin
	qemu-img create -f raw disk.img 1440K
	python3 ./combine.py
	dd if=tmp.img of=disk.img conv=notrunc
	rm -f tmp.img

init/to_compile.o:
	(cd init; make to_compile.o)

boot/head.o:
	(cd boot; make head.o)

init/main.o:
	(cd init; make main.o)

mm/memory.o:
	(cd mm; make memory.o)

kernel/kernel.o:
	(cd kernel; make kernel.o)



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

system.o.disasm: system.o
	objdump -d  $< > $@

system.bin.disasm: system.bin
	objdump -m i386  -b binary $^ -EL -D > $@


build-dev-env:
	pacman -S bin86
	paru -S nasm

.PHONY: clean
clean:
	rm -f *.o system.bin disk.img \
 		bochs.log bochsdbg.log bx_enh_dbg.ini system.sym

	(cd boot; make clean;)
	(cd init; make clean;)
	(cd mm; make clean;)
	(cd kernel; make clean;)

