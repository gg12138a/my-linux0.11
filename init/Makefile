CC=gcc

CFLAGS=-c -O1 -finline-functions \
  	-fno-omit-frame-pointer -nostartfiles -fno-pie -fno-stack-protector   \
	-m32 -march=i386 -mtune=i386 -fno-asynchronous-unwind-tables  \
	-fno-builtin -ffreestanding -nostdlib -nostdinc -nodefaultlibs -nostartfiles

INCLUDE_DIRS=-I../include/

main.o: main.c
	$(CC) $(CFLAGS) ${INCLUDE_DIRS} -o $@ $<

to_compile.o: to_compile.c
	$(CC) $(CFLAGS) ${INCLUDE_DIRS} -o $@ $<

main.o.disasm: main.o
	objdump -d  $< > $@

.PHONY: show-elf-info
show-elf-info: main.o to_compile.o
	objdump -x main.o
	objdump -x to_compile.o


.PHONY: clean
clean:
	rm -f *.o *.disasm
