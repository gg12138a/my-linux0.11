#ifndef _TTY_H
#define _TTY_H

#include <termios.h>

// 控制字符对应的ASCII 码值。[8 进制]
#define INIT_C_CC "\003\034\177\025\004\0\1\0\021\023\032\0\022\017\027\026\0"

/**
 * tty缓冲区大小
 */
#define TTY_BUF_SIZE 1024

/**
 * tty等待队列
 */
struct tty_queue {
    /**
     * 等待队列缓冲区中，当前数据指针字符数
     */
    unsigned long data;

    /**
     * 缓冲区中数据头指针
     */
    unsigned long head;

    /**
     * 缓冲区中数据尾指针
     */
    unsigned long tail;

    /**
     * 等待进程列表
     */
    // struct task_struct *proc_list;

    /**
     * 队列缓冲区
     */
    char buf[TTY_BUF_SIZE];
};

struct tty_struct {

    /**
     * 终端io属性和控制字符数据结构
     */
    struct termios termios;

    /**
     * 所属进程组
     */
    // int pgrp;

    // int stopped;

    /**
     * 向该tty设备write
     */
    void (*write)(struct tty_struct *this_tty);

    struct tty_queue read_q;
    struct tty_queue write_q;
    struct tty_queue secondary;
};

extern struct tty_struct tty_table[];

/**
 * 字符设备初始化
 */
void chr_dev_init(void);

/**
 * 初始化串口终端和控制台
 */
void tty_init(void);

/**
 * 异步串口通信初始化
 */
void rs_init(void);

/**
 * 串口1的中断处理程序
 */
void rs1_interrupt(void);

/**
 * 串口2的中断处理程序
 */
void rs2_interrupt(void);

void con_init(void);

void rs_write(struct tty_struct *this_tty);
void con_write(struct tty_struct *this_tty);

#endif // !_TTY_H
