CC=gcc

CFLAGS=-c -O2 -fno-pic -I../include \
	-m32 -march=i386 -mtune=i386 \
	-fno-builtin -ffreestanding -nostdlib -nostdinc -nodefaultlibs -nostartfiles
INCLUDE_DIRS=-I../include

main.o: main.c
	$(CC) $(CFLAGS) ${INCLUDE_DIRS} -o $@ $<

to_compile.o: to_compile.c
	$(CC) $(CFLAGS) ${INCLUDE_DIRS} -o $@ $<

.PHONY: clean
clean:
	rm -f *.o
