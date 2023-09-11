; `.s` file, AT&T grammer, compiled by as86 or gas.

; bootsect.s is loaded at 0x7c00 by the bios-startup routines, and moves
; iself out of the way to address 0x90000, and jumps there.
;
; It then loads 'setup' directly after itself (0x90200), and the system
; at 0x10000, using BIOS interrupts.

.globl begtext, begdata, begbss, endtext, enddata, endbss
.text
begtext:
.data
begdata:
.bss
begbss:
.text

SYS_SIZE  = 0x3000              ; the number of the kernel size. 0x3000 is 0x30000 bytes = 196kB.
BOOT_SEG  = 0x07c0              ; to init ds reg when boot.
INIT_SEG  = 0x9000              ; the addr of this boot sector copied to.
SETUP_LEN = 4                   ; number of setup-sectors
SETUP_SEG = 0x9200              ; addr of setup sectors
SYS_SEG   = 0x1000              ; 240 sectors of os will be loaded to 0x10000
END_SEG   = SYS_SEG + SYS_SIZE  ; where to stop kernel loading.

ROOT_DEV = 0x306                ; 指定根文件系统设备是第2个硬盘的第1个分区

entry start
start:
  ; BIOS will load this boot sector to memory address `0x7c00`.
  ; then execute instruction jmp 0:0x7c00.
  ; so, cs register will be 0, ip reg will be 0x7c00.
  mov ax, #BOOT_SEG
  mov ds, ax

  ; copy this boot sector(512B), to `0x90000`.
  mov ax, #INIT_SEG
  mov es, ax
  mov cx, #256
  sub si, si
  sub di, di
  rep
  movw                  ; copy `ds:si` to `es:di`.

  ; jump to 'INIT_SEG:go'(0x9000:go), so cs=0x9000, ip=go.
  jmpi go, INIT_SEG

go:
  ; init memory layout.
  mov ax, cs
  mov ds, ax
  mov es, ax
  mov ss, ax
  mov sp, #0xFF00       ; arbitrary value >>512. so stack top was put to `0x9FF00`

load_setup:
  ; load 4 sectors to addr `0x90200`,
  ; from the second sector in drive 0, head 0, track 0, sector 2.
	mov	dx, #0x0000		          ; drive 0, head 0
	mov	cx, #0x0002		          ; sector 2, track 0
	mov	bx, #0x0200		          ; address = 512, in INIT_SEG
	mov	ax, #0x0200+SETUP_LEN	  ; service 2(read disk to memory), number of sectors
	int	0x13			              ; read it
	jnc	ok_load_setup		        ; ok - continue
	; 如果读出错，则复位驱动器，并重试
	mov	dx, #0x0000
	mov	ax, #0x0000
	int	0x13                    ; reset the diskette
	j	load_setup

ok_load_setup:
  ; get diskdrive params
  mov dl, #0x00
  mov ax, #0x0800             ; service 8
  int 0x13
  mov	ch, #0x00
  ; copy returned params
  seg cs                    ; 段超越，即设置下一条指令的基址寄存器
  mov sectors, cx          ; save sectors_per_track
  mov ax, #INIT_SEG
  mov es, ax                  ; reset es(updated when interrupt return)

  ; print some msg
  mov ah, #0x03
  xor bh, bh
  int 0x10                    ; read cursor pos

  mov cx, #24
  mov bx, #0x0007             ; page 0, attribute 7 (normal)
  mov bp, #msg1
  mov ax, #0x1301
  int 0x10                    ; write string, move cursor

  ; load 240 sectors of os to 0x10000
  mov ax, #SYS_SEG
  mov es, ax
  call read_it
  call kill_motor


  ; check which root-device to use.
  ; if the device is defined (!= 0), nothing is done and the given device is used.
  ; Otherwise, either /dev/PS0 (2,28) or /dev/at0 (2,8), depending on the number of sectors that the BIOS reports currently.

    seg cs
	mov	ax, root_dev
	cmp	ax, #0
	jne	root_defined  ; 如果 ax != 0, 转到root_defined
	seg cs
	mov	bx, sectors
  ; 取上面保存的每磁道扇区数;
  ; 如果sectors=15, 则说明是1.2Mb的驱动器;
  ; 如果sectors=18, 则说明是1.44Mb软驱;
  ; 因为是可引导的驱动器, 所以肯定是A驱
	mov	ax,#0x0208		; /dev/ps0 - 1.2Mb
	cmp	bx,#15        ; 判断每磁道扇区数是否=15. 如果等于, 则ax中就是引导驱动器的设备号
	je	root_defined
	mov	ax,#0x021c		; /dev/PS0 - 1.44Mb
	cmp	bx,#18
	je	root_defined
undef_root:
	jmp undef_root    ; 如果都不一样，则死循环（死机）
root_defined:
	seg cs
	mov	root_dev, ax  ; update root_dev

  ; after that (everyting loaded), we jump to
  ; the setup-routine loaded directly after
  ; the bootblock:

	jmpi	0, SETUP_SEG


; This routine loads the system at address 0x10000, making sure
; no 64kB boundaries are crossed. We try to load it as fast as
; possible, loading whole tracks whenever we can.
;
; in:	es - starting address segment (normally 0x1000)
;
sread:  .word 1+SETUP_LEN   ; sectors read in current track
head:   .word 0             ; current head
track:  .word 0             ; current track

read_it:
  mov ax, es
  test ax, #0x0fff
die_loop:
  jne die_loop    ; es must be at 64kB boundary
  xor bx, bx      ; bx is starting addr within segment
rp_read:
  ; if all read
  mov ax, es
  cmp ax, #END_SEG
  jb ok1_read
  ret
ok1_read:
  ; 计算和验证当前磁道需要读取的扇区数.放在ax寄存器中。
  ; 根据当前磁道还未读取的扇区数以及段内数据字节开始偏移位置，计算如果全部读取这些
  ; 未读扇区,所读总字节数是否会超过64KB段长度的限制。若会超过,则根据此次最多能读
  ; 入的字节数(64KB - 段内偏移位置), 反算出此次需要读取的扇区数。
  seg cs
  mov ax, sectors
  sub ax, sread           ; 减去当前磁道已读扇区数
  mov cx, ax
  shl cx, #9
  add cx, bx              ; cx = ax*512B + bx(段内当前偏移值) = 此次读操作后，段内共读入字节数
  jnc ok2_read            ; 若没有超过64KB,则跳转至ok2_read处执行
  je ok2_read
  xor ax, ax
  sub ax, bx
  shr ax, #9              ; 若加上此次将读磁道上所有未读扇区时会超过64KB,
                          ; 则计算此时最多能读入的字节数(64KB - 段内读偏移位置),
                          ; 再转换成需要读取的扇区数。

ok2_read:
	call read_track
	mov cx, ax              ; dx = 该此操作已读取的扇区数
	add ax, sread           ; 当前磁道上已经读取的扇区数
	seg cs
	cmp ax, sectors      ; 如果当前磁道上的还有扇区未读,则跳转到ok3_read处
	jne ok3_read            ; 读该磁道的下一磁头面(1号磁头)上的数据. 如果已经完成,则去读下一磁道
	mov ax, #1
	sub ax, head            ; 判断当前磁头号
	jne ok4_read            ; 如果是0磁头, 则再去读1磁头面上的扇区数据, 否则去读下一磁道
	inc track

ok4_read:
	mov head,ax			        ; 保存当前磁头号
	xor ax,ax			          ; 清当前磁道已读扇区数
ok3_read:
	mov sread,ax		        ; 保存当前磁道已读扇区数
	shl dx,cl			          ; 上次已读扇区数*512字节
	add bx,dx			          ; 调整当前段内数据开始位置。
	jnc rp_read			        ; 若小于64KB边界值, 则跳转到rp_read处, 继续读数据;
						              ; 否则调整当前段，为读下一段数据作准备
	mov ax,es
	add ax,0x1000		        ; 将段基址调整为指向下一个64KB段内存
	mov es,ax
	xor bx,bx
	jmp rp_read


; 读当前磁道上指定开始扇区和需读扇区数的数据到es:bx开始处。
; al - 需读扇区数;
; es:bx - 缓冲区开始位置。
read_track:
	push ax
	push bx
	push cx
	push dx
	mov dx, track		  ; 取当前磁道号
	mov cx, sread		  ; 取当前磁道上已读扇区数
	inc cx				    ; cl = 开始读扇区
	mov ch, dl			  ; ch = 当前磁道号
	mov dx, head			; 取当前磁头号
	mov dh, dl			  ; dh = 磁头号
	mov dl, #0			  ; dl = 驱动器号(为0表示当前驱动器)
	and dx, 0x0100		; 磁头号不大于1
	mov ah, #2			  ; ah = 2, 读磁盘扇区功能号
	int 0x13
	jc bad_rt			    ; 若出错,则跳转至bad_rt
	pop dx
	pop cx
	pop bx
	pop ax
	ret

; 执行驱动器复位操作(磁盘中断功能号0), 再跳转到read_track处重试
bad_rt:
	mov ax, #0
	mov dx, #0
	int 0x13
	pop dx
	pop cx
	pop bx
	pop ax
	jmp read_track


; 关闭软驱的马达, 从而以已知状态进入内核。
; This procedure turns off the floppy drive motor, so
; that we enter the kernel in a known state, and
; don't have to worry about it later.
;
kill_motor:
	push dx
	mov dx,#0x3f2   ; 软驱控制卡的驱动端口, 只写
	mov al,#0       ; A驱动器, 关闭FDC, 禁止DMA和中断请求, 关闭马达
	outb            ; 将al中的内容输出到dx指定的端口去
	pop dx
	ret

; sector numbers per track of current disk
sectors:
  .word 0

msg1:
  .byte 13, 10                  ; 回车, 换行的ASCII码
  .ascii "Loading system ..."
	.byte 13,10,13,10

.org 508		; 表示下面语句从地址508(1FC)开始,
          ; 所以root_dev在启动扇区的第508开始的2个字节中。
root_dev:
  dw ROOT_DEV	; 这里存放根文件系统所在的设备号(init/main.c中会用)
boot_flag:
  dw 0xAA55		; 硬盘有效标识


.text
endtext:
.data
enddata:
.bss
endbss: