CFLAGS=-c -O2 -fno-pic -I../include/ \
	-m32 -march=i386 -mtune=i386 \
	-fno-builtin -ffreestanding -nostdlib -nostdinc -nodefaultlibs -nostartfiles

memory.o: memory.c
	gcc $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f *.o
