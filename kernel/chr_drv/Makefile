LDFLAGS	=-s -x -m elf_i386 -nostdlib -O1
CFLAGS=-c -O1 -finline-functions \
  	-fno-omit-frame-pointer -nostartfiles -fno-pie -fno-stack-protector  \
	-m32 -march=i386 -mtune=i386 -fno-asynchronous-unwind-tables  \
	-fno-builtin -ffreestanding -nostdlib -nostdinc -nodefaultlibs -nostartfiles

INCLUDE_DIRS=-I../../include/

chr_drv.o: rs_io.o serial.o tty_io.o console.o
	ld $(LDFLAGS) -r -o $@ $^

rs_io.o: rs_io.c
	$(CC) $(CFLAGS) ${INCLUDE_DIRS} -o $@ $<

serial.o: serial.c
	$(CC) $(CFLAGS) ${INCLUDE_DIRS} -o $@ $<

tty_io.o: tty_io.c
	$(CC) $(CFLAGS) ${INCLUDE_DIRS} -o $@ $<

console.c: console.o
	$(CC) $(CFLAGS) ${INCLUDE_DIRS} -o $@ $<

.PHONY: clean
clean:
	rm -f *.o
