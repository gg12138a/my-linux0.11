OUTPUT_ARCH(i386)
ENTRY(startup_32)
BASE_ADDRESS = 0x0;

SECTIONS
{
    . = BASE_ADDRESS;

    .text : {
        *(.text.startup_32)
        . = ALIGN(4K);
        *(.text .text.*)
    }

    . = ALIGN(4K);
    .rodata : {
        *(.rodata .rodata.*)
    }

    . = ALIGN(4K);
    .data : {
        *(.data .data.*)
    }

    . = ALIGN(4K);
    .bss : {
        *(.bss .bss.*)
    }

    /DISCARD/ : {
        *(.eh_frame)
    }
}