CC		=gcc

CFLAGS	= -c -m32 -march=i386 -mtune=i386 \
	-nostdinc -nostdlib -nostartfiles -nodefaultlibs \
	-ffreestanding -fno-stack-protector -fno-exceptions

main.o: main.c
	$(CC) $(CFLAGS) -o $@ $<


.PHONY: clean
clean:
	rm -f main.o