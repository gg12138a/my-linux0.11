CFLAGS=-c -O1 -finline-functions \
  	-fno-omit-frame-pointer -nostartfiles -fno-pie -fno-stack-protector -fno-section-anchors  \
	-m32 -march=i386 -mtune=i386 -fno-asynchronous-unwind-tables  \
	-fno-builtin -ffreestanding -nostdlib -nostdinc -nodefaultlibs -nostartfiles

INCLUDE_DIRS=-I../include/

memory.o: memory.c
	$(CC) $(CFLAGS) ${INCLUDE_DIRS} -o $@ $<


.PHONY: clean
clean:
	rm -f *.o
