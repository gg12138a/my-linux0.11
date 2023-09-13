#include <asm/io.h>
#include <asm/system.h>
#include <linux/tty.h>

/**
 * @p port 端口号. port1=0x3F8, port2=0x2F8
 */
static void init_serial(int port) {

    /* set DLAB of line control reg,设置线路控制寄存器的DLAB位(位7) */
    outb_p(0x80, port + 3);
    /* LS of divisor (48->2400bps),波特率因子的低字节 */
    outb_p(0x30, port);
    /* MS of divisor (0x00),波特率因子的高字节 */
    outb_p(0x00, port + 1);
    /* reset DLAB, 复位DLAB位，数据位8位 */
    outb_p(0x03, port + 3);
    /* set DTR,RTS,OUT_2, 设置DTR,RTS,辅助用户输出2 */
    outb_p(0x0b, port + 4);
    /* enable all intrs but writes, 除了写以外，允许所有中断源中断 */
    outb_p(0x0d, port + 1);
    /* 读数据口,以进行复位操作*/
    inb(port);
}

void rs_init(void) {
    // 对应硬件IRQ4信号
    set_intr_gate(0x24, rs1_interrupt);
    // 对应硬件IRQ3信号
    set_intr_gate(0x23, rs1_interrupt);

    init_serial(tty_table[1].read_q.data);
    init_serial(tty_table[2].read_q.data);

    // 允许主8259A 芯片的IRQ3，IRQ4 中断信号请求
    outb(inb_p(0x21) & 0xE7, 0x21);
}

void rs_write(struct tty_struct *this_tty) {}
