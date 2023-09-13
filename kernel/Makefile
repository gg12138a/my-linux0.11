LDFLAGS	=-s -x -m elf_i386 -nostdlib -O1
CFLAGS=-c -O1 -finline-functions \
  	-fno-omit-frame-pointer -nostartfiles -fno-pie -fno-stack-protector \
	-m32 -march=i386 -mtune=i386 -fno-asynchronous-unwind-tables \
	-fno-builtin -ffreestanding -nostdlib -nostdinc -nodefaultlibs -nostartfiles

INCLUDE_DIRS=-I../include/

kernel.o: chr_drv/chr_drv.o blk_drv/blk_drv.o \
		trap.o 
	ld $(LDFLAGS) -r -o $@ $^

chr_drv/chr_drv.o:
	(cd chr_drv; make chr_drv.o)

blk_drv/blk_drv.o:
	(cd blk_drv; make blk_drv.o)

trap.o: trap.c
	$(CC) $(CFLAGS) ${INCLUDE_DIRS} -o $@ $<


.PHONY: clean
clean:
	rm -f *.o
	(cd blk_drv; make clean;)
	(cd chr_drv; make clean;)
