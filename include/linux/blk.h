#ifndef _BLK_H
#define _BLK_H

/**
 * 块设备读写队列长度
 */
#define NR_REQUEST 32

enum REQUEST_CMD : int { READ, WRITE };

struct request {
    /**
     * -1表示空闲
     */
    int dev;

    /**
     * 读/写
     */
    enum REQUEST_CMD cmd;

    /**
     * 操作时产生的错误次数
     */
    int errors;

    /**
     * 起始扇区, 1块=2扇区
     */
    unsigned long sector;

    /**
     * 读/写扇区数
     */
    unsigned long nr_sectors;

    /**
     * 数据缓冲区
     */
    char *buffer;

    /**
     * 任务等待操作执行完成的地方
     */
    // struct task_struct *waiting;

    /**
     * 缓冲区头指针
     */
    // struct buffer_head *bh;

    /**
     * 指向下一请求项
     */
    struct request *next;
};

/**
 * 块设备初始化，初始化块读写请求链表
 */
extern void blk_dev_init(void);

#endif // !_BLK_H
