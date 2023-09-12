#ifndef _HEAD_H
#define _HEAD_H

/**
 * 定义了段描述符的数据结构。该结构仅说明每个描述
 * 符是由8个字节构成，每个描述符表共有256项
 */
typedef struct desc_struct {
    unsigned long a, b;
} desc_table[256];

// #define pg_dir ((unsigned long*)0)
/**
 * 内存页目录数组。每个目录项为4字节,从物理地址0开始
 */
extern unsigned long pg_dir[1024];

/**
 * 中断描述符表
 */
extern desc_table idt;

/**
 * 全局描述符表
 */
extern desc_table gdt;

#endif
