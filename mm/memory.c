#include <memory.h>

#define LOW_MEM 0x100000
// 最多使用16M内存，低地址的1M为内核，跳过
#define PAGING_MEMORY (15 * 1024 * 1024)
// 被管理的页数，即除去4096
#define PAGING_PAGES (PAGING_MEMORY >> 12)
// addr是第几个页
#define MAP_NR(addr) (((addr)-LOW_MEM) >> 12)
#define USED 100

// 全局变量，存放实际物理内存最高端地址
static unsigned long HIGH_MEMORY = 0;

// 内存映射字节图，每个字节代表该页内存的被引用次数，一页大小为4096B
static unsigned char mem_map[PAGING_PAGES] = {0};

void mem_init(long main_start_addr, long end_addr) {

    HIGH_MEMORY = end_addr;

    // 首先置所有页面为已占用(USED=100)状态
    for (int i = 0; i < PAGING_PAGES; i++) {
        mem_map[i] = USED;
    }

    // 可使用内存的起始地址，是第几个页
    int main_start_pg_nr = MAP_NR(main_start_addr);

    // 可分页处理的内存块大小
    end_addr -= main_start_addr;
    end_addr >>= 12; // end_addr可以不是4k对齐的

    // 将这些可用页面对应的页面映射数组清零
    while (end_addr != 0) {
        mem_map[main_start_pg_nr++] = 0;
        end_addr--;
    }
}
