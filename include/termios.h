#ifndef _TERMIOS_H
#define _TERMIOS_H

/**
 * termios结构输入模式字段c_iflag各种标志的符号常数
 */
enum TERMIO_I_FLAG : unsigned short {
    IGNBRK = 0000001,  // 输入时忽略BREAK 条件
    BRKINT = 0000002,  // 在BREAK 时产生SIGINT 信号
    IGNPAR = 0000004,  // 忽略奇偶校验出错的字符
    PARMRK = 0000010,  // 标记奇偶校验错
    INPCK = 0000020,   // 允许输入奇偶校验
    ISTRIP = 0000040,  // 屏蔽字符第8 位
    INLCR = 0000100,   // 输入时将换行符NL 映射成回车符CR
    IGNCR = 0000200,   // 忽略回车符CR
    ICRNL = 0000400,   // 在输入时将回车符CR 映射成换行符NL
    IUCLC = 0001000,   // 在输入时将大写字符转换成小写字符
    IXON = 0002000,    // 允许开始/停止（XON/XOFF）输出控制
    IXANY = 0004000,   // 允许任何字符重启输出
    IXOFF = 0010000,   // 允许开始/停止（XON/XOFF）输入控制
    IMAXBEL = 0020000, // 输入队列满时响铃
};

/**
 * termios结构输入模式字段c_oflag各种标志的符号常数
 */
enum TERMIO_O_FLAG : unsigned short {
    OPOST = 0000001, // 执行输出处理
    OLCUC = 0000002, // 在输出时将小写字符转换成大写字符
    ONLCR = 0000004, // 在输出时将换行符NL 映射成回车-换行符CR-NL
    OCRNL = 0000010,  // 在输出时将回车符CR 映射成换行符NL
    ONOCR = 0000020,  // 在0 列不输出回车符CR
    ONLRET = 0000040, // 换行符NL 执行回车符的功能
    OFILL = 0000100,  // 延迟时使用填充字符而不使用时间延迟
    OFDEL = 0000200, // 填充字符是ASCII 码DEL。如果未设置，则使用ASCII NULL
    NLDLY = 0000400,  // 选择换行延迟
    NL0 = 0000000,    // 换行延迟类型0
    NL1 = 0000400,    // 换行延迟类型1
    CRDLY = 0003000,  // 选择回车延迟
    CR0 = 0000000,    // 回车延迟类型0
    CR1 = 0001000,    // 回车延迟类型1
    CR2 = 0002000,    // 回车延迟类型2
    CR3 = 0003000,    // 回车延迟类型3
    TABDLY = 0014000, // 选择水平制表延迟
    TAB0 = 0000000,   // 水平制表延迟类型0
    TAB1 = 0004000,   // 水平制表延迟类型1
    TAB2 = 0010000,   // 水平制表延迟类型2
    TAB3 = 0014000,   // 水平制表延迟类型3
    XTABS = 0014000,  // 将制表符TAB 换成空格，该值表示空格数
    BSDLY = 0020000,  // 选择退格延迟
    BS0 = 0000000,    // 退格延迟类型0
    BS1 = 0020000,    // 退格延迟类型1
    VTDLY = 0040000,  // 纵向制表延迟
    VT0 = 0000000,    // 纵向制表延迟类型0
    VT1 = 0040000,    // 纵向制表延迟类型1
    FFDLY = 0040000,  // 选择换页延迟
    FF0 = 0000000,    // 换页延迟类型0
    FF1 = 0040000,    // 换页延迟类型1
};

/**
 * termios结构输入模式字段c_oflag各种标志的符号常数
 */
enum TERMIO_C_FLAG : unsigned short {
    CBAUD = 0000017, // 传输速率位屏蔽码
    B0 = 0000000,
    /* hang up */     /* 挂断线路 */
    B50 = 0000001,    // 波特率 50
    B75 = 0000002,    // 波特率 75
    B110 = 0000003,   // 波特率 110
    B134 = 0000004,   // 波特率 134
    B150 = 0000005,   // 波特率 150
    B200 = 0000006,   // 波特率 200
    B300 = 0000007,   // 波特率 300
    B600 = 0000010,   // 波特率 600
    B1200 = 0000011,  // 波特率 1200
    B1800 = 0000012,  // 波特率 1800
    B2400 = 0000013,  // 波特率 2400
    B4800 = 0000014,  // 波特率 4800
    B9600 = 0000015,  // 波特率 9600
    B19200 = 0000016, // 波特率 19200
    B38400 = 0000017, // 波特率 38400
    EXTA = B19200,    // 扩展波特率A
    EXTB = B38400,    // 扩展波特率B
    CSIZE = 0000060,  // 字符位宽度屏蔽码
    CS5 = 0000000,    // 每字符5 比特位
    CS6 = 0000020,    // 每字符6 比特位
    CS7 = 0000040,    // 每字符7 比特位
    CS8 = 0000060,    // 每字符8 比特位
    CSTOPB = 0000100, // 设置两个停止位，而不是1 个
    CREAD = 0000200,  // 允许接收
    CPARENB = 0000400, // 开启输出时产生奇偶位、输入时进行奇偶校验
    CPARODD = 0001000, // 输入/输入校验是奇校验
    HUPCL = 0002000,   // 最后进程关闭后挂断
    CLOCAL = 0004000,  // 忽略调制解调器(modem)控制线路
    CIBAUD = (unsigned short)03600000,
    /* input baud rate (not used) */ /* 输入波特率(未使用) */
    CRTSCTS = (unsigned short)020000000000,
    /* flow control */ /* 流控制 */
    PARENB = CPARENB, // 开启输出时产生奇偶位、输入时进行奇偶校验。
    PARODD = CPARODD, // 输入/输入校验是奇校验。

};

/**
 * termios结构输入模式字段c_lflag各种标志的符号常数
 */
enum TERMIO_L_FLAG : unsigned short {
    ISIG = 0000001, // 当收到字符INTR、QUIT、SUSP 或DSUSP，产生相应的信号
    ICANON = 0000002, // 开启规范模式（熟模式）
    XCASE = 0000004,  // 若设置了ICANON，则终端是大写字符的
    ECHO = 0000010,   // 回显输入字符
    ECHOE = 0000020, // 若设置了ICANON，则ERASE/WERASE 将擦除前一字符/单词
    ECHOK = 0000040, // 若设置了ICANON，则KILL 字符将擦除当前行
    ECHONL = 0000100, // 如设置了ICANON，则即使ECHO 没有开启也回显NL 字符
    NOFLSH =
        0000200, // 当生成SIGINT 和SIGQUIT
                 // 信号时不刷新输入输出队列，生成SIGSUSP信号时，刷新输入队列
    TOSTOP = 0000400, // 发送SIGTTOU
                      // 信号到后台进程的进程组，该后台进程试图自己的控制终端
    ECHOCTL =
        0001000, // 若设置了ECHO，则除TAB、NL、START 和STOP
                 // 以外的ASCI控制信号将被回显成象=^,X 式样，X 值是控制符+0x40
    ECHOPRT = 0002000, // 若设置了ICANON 和IECHO，则字符在擦除时将显示
    ECHOKE = 0004000, // 若设置了ICANON，则KILL 通过擦除行上的所有字符被回显
    FLUSHO = 0010000, // 输出被刷新。通过键入DISCARD 字符，该标志被翻转
    PENDIN = 0040000, // 当下一个字符是读时，输入队列中的所有字符将被重显
    IEXTEN = 0100000, // 开启实现时定义的输入处理
};

/**
 * modem 线路信号符号常数
 */
enum TERMIO_LINE : unsigned char {
    TIOCM_LE = 0x001,                 // 线路允许(Line Enable)
    TIOCM_DTR = 0x002,                // 数据终端就绪(Data Terminal Ready)
    TIOCM_RTS = 0x004,                // 请求发送(Request to Send)
    TIOCM_ST = 0x008,                 // 串行数据发送(Serial Transfer)
    TIOCM_SR = 0x010,                 // 串行数据接收(Serial Receive)
    TIOCM_CTS = 0x020,                // 清除发送(Clear To Send)
    TIOCM_CAR = 0x040,                // 载波监测(Carrier Detect)
    TIOCM_RNG = 0x080,                // 响铃指示(Ring indicate)
    TIOCM_DSR = (unsigned char)0x100, // 数据设备就绪(Data Set Ready)
    TIOCM_CD = TIOCM_CAR,
    TIOCM_RI = TIOCM_RNG,
};

/**
 * AT&T System V 下的termios结构
 */
#define NCC 8

struct termio {
    /**
     * input mode flag,输入模式标志
     */
    unsigned short c_iflag;

    /**
     * onput mode flag,输出模式标志
     */
    unsigned short c_oflag;

    /**
     * control mode flag,控制模式标志
     */
    unsigned short c_cflag;

    /**
     * local mode flag,本地模式标志
     */
    unsigned short c_lflag;

    /**
     * line discipline 线路规程(速率)
     */
    unsigned char c_line;

    /**
     * control characters 控制字符数组
     */
    unsigned char c_cc[NCC];
};

/**
 * POSIX 下的termios结构
 */
#define NCCS 17

struct termios {
    /**
     * input mode flag,输入模式标志
     */
    unsigned short c_iflag;

    /**
     * onput mode flag,输出模式标志
     */
    unsigned short c_oflag;

    /**
     * control mode flag,控制模式标志
     */
    unsigned short c_cflag;

    /**
     * local mode flag,本地模式标志
     */
    unsigned short c_lflag;

    /**
     * line discipline 线路规程(速率)
     */
    unsigned char c_line;

    /**
     * control characters 控制字符数组
     */
    unsigned char c_cc[NCCS];
};

#endif
