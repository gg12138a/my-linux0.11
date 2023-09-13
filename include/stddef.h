#ifndef _STDDEF_H
#define _STDDEF_H

#define NULL ((void *)0)

#define DBG_ANCHOR()                                                           \
    {                                                                          \
        int magic = 0x1234;                                                    \
        __asm__ volatile("movl %0, %%ebx" : : "r"(magic) : "%ebx");            \
    }

#endif // !_STDDEF_H
