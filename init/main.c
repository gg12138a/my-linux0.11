#include <asm/system.h>
#include <linux/fs.h>
#include <memory.h>
#include <trap.h>

struct drive_info {
    char dummy[32];
} drive_info; // 用于存放硬盘参数表信息

static long memory_end = 0;        // 机器具有的内存（字节数）
static long buffer_memory_end = 0; // 高速缓冲区末端地址
static long main_memory_start = 0; // 主内存（将用于分页）开始的位置

/**
 * 读取setup.s提前保存在0x90000基址处的一系列参数
 * @brief 获取系统的软硬件参数
 * @note
 * | 物理内存地址 | 长度(字节) | 名称         |       \n
 * | -------- | ---------- | ------------ |         \n
 * | 0x90000  | 2          | 光标位置     |         \n
 * | 0x90002  | 2          | 扩展内存数   |          \n
 * | 0x90004  | 2          | 显示页面     |         \n
 * | 0x90006  | 1          | 显示模式     |         \n
 * | 0x90007  | 1          | 字符列数     |         \n
 * | 0x90008  | 2          | 未知         |           \n
 * | 0x9000A  | 1          | 显示内存     |         \n
 * | 0x9000B  | 1          | 显示状态     |         \n
 * | 0x9000C  | 2          | 显卡特性参数 |           \n
 * | 0x9000E  | 1          | 屏幕行数     |         \n
 * | 0x9000F  | 1          | 屏幕列数     |         \n
 * | 0x90080  | 16         | 硬盘1参数表  |          \n
 * | 0x90090  | 16         | 硬盘2参数表  |          \n
 * | 0x901FC  | 2          | 根设备号     |
 */
void load_system_config() {
// 以下数据，均有setup.s在引导时设置
#define ORIG_ROOT_DEV (*(unsigned short *)0x901FC)
// 硬盘参数表基址
#define DRIVE_INFO (*(struct drive_info *)0x90080)
// 1M 以后的扩展内存大小（KB）
#define EXT_MEM_K (*(unsigned short *)0x90002)

    ROOT_DEV = ORIG_ROOT_DEV;
    drive_info = DRIVE_INFO;
    memory_end =
        (1 << 20) + (EXT_MEM_K << 10); // 内存大小=1MB + 扩展内存(以K为单位)*KB
    memory_end &= 0xfffff000; // 忽略内存高位不到4KB（1 页）的内存数

    if (memory_end > 16 * 1024 * 1024) {
        // 如果内存超过16MB，则按16MB 计
        memory_end = 16 * 1024 * 1024;
    }

    if (memory_end > 12 * 1024 * 1024) {
        // 如果内存>12Mb，则设置缓冲区末端=4MB
        buffer_memory_end = 4 * 1024 * 1024;
    } else if (memory_end > 6 * 1024 * 1024) {
        // 否则如果内存>6MB，则设置缓冲区末端=2MB
        buffer_memory_end = 2 * 1024 * 1024;
    } else {
        // 否则则设置缓冲区末端=1MB
        buffer_memory_end = 1 * 1024 * 1024;
    }

    main_memory_start = buffer_memory_end; // 主内存起始位置=缓冲区末端；
}


//__attribute__((section(".text")))
void _main(void) {
    load_system_config();
    mem_init(main_memory_start, memory_end);
    trap_init();

    sti();
}
