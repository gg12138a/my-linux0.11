#ifndef _IO_H
#define _IO_H

/**
 * 无延迟的硬件端口字节输出函数
 * @param value 欲输出字节
 * @param port 端口
 */
#define outb(value, port) _outb((unsigned char)(value), (unsigned short)(port))

/**
 * 无延迟的硬件端口字节输出函数
 * @param value 欲输出字节
 * @param port 端口
 */
inline void _outb(unsigned char value, unsigned short port) {
    __asm__("out %0, %1" : : "a"(value), "d"(port));
}

/**
 * 无延迟的硬件端口字节输入函数
 * @param port 端口
 * @return 返回读取的字节
 */
#define inb(port) _inb((unsigned short)(port))

/**
 * 无延迟的硬件端口字节输入函数
 * @param port 端口
 * @return 返回读取的字节
 */
inline unsigned char _inb(unsigned short port) {
    unsigned char _v;
    __asm__ volatile("inb %1, %0" : "=a"(_v) : "d"(port));

    return _v;
}

/**
 * 带延迟的硬件端口字节输出函数
 * @param value 欲输出字节
 * @param port 端口
 */
#define outb_p(value, port)                                                    \
    _outb_p((unsigned char)(value), (unsigned short)(port))

/**
 * 带延迟的硬件端口字节输出函数
 * @param value 欲输出字节
 * @param port 端口
 */
inline void _outb_p(unsigned char value, unsigned short port) {
    __asm__("out %0, %1\n"
            "jmp 1f\n"
            "1: jmp 2f\n"
            "2:"
            :
            : "a"(value), "d"(port));
}

/**
 * 带延迟的硬件端口字节输入函数
 * @param port 端口
 * @return 返回读取的字节
 */
#define inb_p(port) _inb_p((unsigned short)(port))

/**
 * 带延迟的硬件端口字节输入函数
 * @param port 端口
 * @return 返回读取的字节
 */
inline unsigned char _inb_p(unsigned short port) {
    unsigned char _v;
    __asm__ volatile("inb %1, %0\n"
                     "jmp 1f\n"
                     "1: jmp 2f\n"
                     "2:"
                     : "=a"(_v)
                     : "d"(port));

    return _v;
}

#endif
