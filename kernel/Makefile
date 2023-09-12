LDFLAGS	=-s -x -M -m elf_i386 -nostdlib -O2
CFLAGS=-c -O2 -fno-pic -I../include/ \
	-m32 -march=i386 -mtune=i386 \
	-fno-builtin -ffreestanding -nostdlib -nostdinc -nodefaultlibs -nostartfiles

kernel.o: trap.o
	ld $(LDFLAGS) -r -o $@ $^

trap.o: trap.c
	gcc $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f *.o
