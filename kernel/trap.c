#include <asm/io.h>
#include <asm/symbol.h>
#include <asm/system.h>
#include <trap.h>

void trap_init(void) {
    int i;

    set_trap_gate(0, &divide_error); // 设置除操作出错的中断向量值。以下雷同。
    set_trap_gate(1, &debug);
    set_trap_gate(2, &nmi);
    set_system_gate(3, &int3); /* int3-5 can be called from all */
    set_system_gate(4, &overflow);
    set_system_gate(5, &bounds);
    set_trap_gate(6, &invalid_op);
    set_trap_gate(7, &device_not_available);
    set_trap_gate(8, &double_fault);
    set_trap_gate(9, &coprocessor_segment_overrun);
    set_trap_gate(10, &invalid_TSS);
    set_trap_gate(11, &segment_not_present);
    set_trap_gate(12, &stack_segment);
    set_trap_gate(13, &general_protection);
    set_trap_gate(14, &page_fault);
    set_trap_gate(15, &reserved);
    set_trap_gate(16, &coprocessor_error);
    // 下面将int17-48
    // 的陷阱门先均设置为reserved，以后每个硬件初始化时会重新设置自己的陷阱门。
    for (i = 17; i < 48; i++)
        set_trap_gate(i, &reserved);
    set_trap_gate(45, &irq13);        // 设置协处理器的陷阱门。
    outb_p(inb_p(0x21) & 0xfb, 0x21); // 允许主8259A 芯片的IRQ2 中断请求。
    outb(inb_p(0xA1) & 0xdf, 0xA1); // 允许从8259A 芯片的IRQ13 中断请求。
    set_trap_gate(39, &parallel_interrupt); // 设置并行口的陷阱门。
}
