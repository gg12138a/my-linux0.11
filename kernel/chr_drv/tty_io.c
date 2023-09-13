#include <linux/tty.h>
#include <termios.h>

struct tty_struct tty_table[] = {
    // 控制台
    {
        .termios =
            {
                .c_iflag = ICRNL,
                .c_oflag = OPOST | ONLCR,
                .c_cflag = 0,
                .c_lflag = ISIG | ICANON | ECHO | ECHOCTL | ECHOKE,
                .c_line = 0,
                .c_cc = INIT_C_CC,
            },
        // .pgrp = 0,
        // .stopped = 0,
        .write = con_write,
        .read_q = {0, 0, 0, /* 0, */ ""},
        .write_q = {0, 0, 0, /* 0, */ ""},
        .secondary = {0, 0, 0, /* 0, */ ""},
    },

    // 串口终端1
    {
        .termios =
            {
                .c_iflag = 0,
                .c_oflag = 0,
                .c_cflag = B2400 | CS8,
                .c_lflag = 0,
                .c_cc = INIT_C_CC,
            },
        // .pgrp = 0,
        // .stopped = 0,
        .write = rs_write,
        .read_q = {0x3f8, 0, 0, /* 0, */ ""},
        .write_q = {0x3f8, 0, 0, /* 0, */ ""},
        .secondary = {0, 0, 0, /* 0, */ ""},
    },

    // 串口终端2
    {
        .termios =
            {
                .c_iflag = 0,
                .c_oflag = 0,
                .c_cflag = B2400 | CS8,
                .c_lflag = 0,
                .c_cc = INIT_C_CC,
            },
        // .pgrp = 0,
        // .stopped = 0,
        .write = rs_write,
        .read_q = {0x2f8, 0, 0, /* 0, */ ""},
        .write_q = {0x2f8, 0, 0, /* 0, */ ""},
        .secondary = {0, 0, 0, /* 0, */ ""},
    },

};

void chr_dev_init(void) {}

void tty_init(void) {
    rs_init();
    con_init();
}
