#include <linux/fs.h>

/**
 * @brief 获取系统参数
 * @details 读取setup.s提前保存在0x90000基址处的一系列参数
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
void parse_system_config(){
#define ORIG_ROOT_DEV (*(unsigned short *)0x901FC)
    ROOT_DEV = ORIG_ROOT_DEV;
}

/**
 * @note 在jmp到main符号前,
 */
void main(void){
    int x = 2;
    int y = x*4;
    parse_system_config();

    int z = 23;
    int n = z*z;
}