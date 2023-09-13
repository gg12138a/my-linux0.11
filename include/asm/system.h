#ifndef _SYSTEM_H
#define _SYSTEM_H

#include <linux/head.h>

/**
 * 开中断
 */
#define sti() __asm__("sti")

/**
 * 关中断
 */
#define cli() __asm__("cli")

/**
 * 空操作
 */
#define nop() __asm__("nop")

/**
 * 中断返回
 */
#define iret() __asm__("iret")

/**
 * 中断描述符的TYPE值
 */
enum IDT_SEG_TYPE : unsigned short {
    /**
     * 任务门
     */
    IASK_GATE,

    /**
     * 中断门
     */
    INTERRUPT_GATE = 14,

    /**
     * 陷阱门
     */
    TRAP_GATE = 15
};

/**
 * 特权级（Descriptor Privilege Level，DPL）
 */
enum IDT_DPL : unsigned short { RING0, RING3 };

/**
 * 设置门描述符
 * @param gate_addr 门描述符放在哪个地址
 * @param type 中断描述符的Segment Type
 * @param dpl 中断描述符的特权级DPL
 * @param addr 中断处理程序相对于segment selector偏移地址
 * @return
 *
 * @note 中断描述符的段选择子固定为0x0008，即选择第一个段
 */
inline void _set_gate(unsigned long *gate_addr, enum IDT_SEG_TYPE type,
                      enum IDT_DPL dpl, unsigned long addr) {

    gate_addr[0] = 0x00080000 + (addr & 0xFFFF); // 低16位
    gate_addr[1] = (/* seg present flag */ 0x8000 + (dpl << 13) + (type << 8)) +
                   (addr & 0xffff0000);
}

/**
 * 设置中断门
 * @param n 中断号
 * @param addr 中断程序偏移地址
 */
#define set_intr_gate(n, addr)                                                 \
    _set_gate((unsigned long *)(&(idt[n])), INTERRUPT_GATE, RING0,             \
              (unsigned long)addr)

/**
 * 设置陷阱门，特权级是0
 * @p n 中断号
 * @p addr 中断处理程序相对于segment selector偏移地址
 */
#define set_trap_gate(n, addr)                                                 \
    _set_gate((unsigned long *)(&(idt[n])), TRAP_GATE, RING0,                  \
              (unsigned long)addr)

/**
 * 设置陷阱门，特权级是3. 也即系统调用.
 * @p n 中断号
 * @p addr 中断处理程序相对于segment selector偏移地址
 */
#define set_system_gate(n, addr)                                               \
    _set_gate((unsigned long *)(&(idt[n])), TRAP_GATE, RING3,                  \
              (unsigned long)addr)

#endif
