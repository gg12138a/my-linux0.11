#include <linux/fs.h>

int ROOT_DEV = 0;

int _main;
int _start;
int _printk;

#define PAGE_SIZE 4096
long user_stack [ PAGE_SIZE>>2 ] ;
struct {
    long * a;
    short b;
} _stack_start = { & user_stack [PAGE_SIZE>>2] , 0x10 };

