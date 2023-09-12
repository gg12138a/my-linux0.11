#ifndef _MEMORY_H
#define _MEMORY_H

/**
 * 设置内存申请情况
 * @param main_start_addr start addr of main memory
 * @param end_addr end addr of main memory, 也许不是4K对齐的
 * @note 0 - 1Mb 内存空间用于内核系统（其实是0-640Kb）
 */
extern void mem_init(long main_start_addr, long end);

#endif
