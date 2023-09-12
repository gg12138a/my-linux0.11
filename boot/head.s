/*
 *  linux/boot/head.s
 *
 *  (C) 1991  Linus Torvalds
 */

/*
 *  head.s contains the 32-bit startup code.
 *
 * NOTE!!! Startup happens at absolute address 0x00000000, which is also where
 * the page directory will exist. The startup code will be overwritten by
 * the page directory.
 */
.text
.globl _idt,_gdt,_pg_dir,_tmp_floppy_area,startup_32
_pg_dir:
startup_32:
	movl $0x10,%eax
	mov %ax,%ds
	mov %ax,%es
	mov %ax,%fs
	mov %ax,%gs
	lss _stack_start,%esp	# 在_stack_start处读6B;
							#之后cs=0x10, esp=sched.c中的user_stack数组的最高元素的地址
	call setup_idt
	call setup_gdt
	movl $0x10,%eax		# reload all the segment registers
	mov %ax,%ds		# after changing gdt. CS was already
	mov %ax,%es		# reloaded in 'setup_gdt'
	mov %ax,%fs
	mov %ax,%gs
	lss _stack_start,%esp

	# 用于测试A20地址线是否开启.
	# 测试方法为, 随意向0x000000写入一个数值,测试0x100000处是否和这个值相同.
	# 若相同, 则表示A20地址线未开启. 则死循环.
	xorl %eax,%eax
l1:	
	incl %eax		# check that A20 really IS enabled
	movl %eax,0x000000	# loop forever if it isn't
	cmpl %eax,0x100000
	je l1

/*
 * NOTE! 486 should set bit 16, to check for write-protect in supervisor
 * mode. Then it would be unnecessary with the "verify_area()"-calls.
 * 486 users probably want to set the NE (#5) bit also, so as to use
 * int 16 for math errors.
 */

/*
 * 	注意! 在下面这段程序中，486 应该将位16 置位，以检查在超级用户模式下的写保护,
 * 	此后"verify_area()"调用中就不需要了。486 的用户通常也会想将NE(;//5)置位，以便
 * 	对数学协处理器的出错使用int 16。
 *
 *  下面这段程序用于检查数学协处理器芯片是否存在。方法是修改控制寄存器CR0,在假设
 *  存在协处理器的情况下执行一个协处理器指令，如果出错的话则说明协处理器芯片不存
 *  在,需要设置CR0 中的协处理器仿真位EM(位2),并复位协处理器存在标志MP(位1)。
 */

	movl %cr0,%eax		# check math chip
	andl $0x80000011,%eax	# Save PG,PE,ET
	/* "orl $0x10020,%eax" here for 486 might be good */
	orl $2,%eax		# set MP
	movl %eax,%cr0
	call check_x87
	jmp after_page_tables

/*
 * We depend on ET to be correct. This checks for 287/387.
 */
check_x87:
	fninit
	fstsw %ax
	cmpb $0,%al
	je l2			/* no coprocessor: have to set bits */
	movl %cr0,%eax
	xorl $6,%eax		/* reset MP, set EM */
	movl %eax,%cr0
	ret
.p2align 2
l2:	
	.byte 0xDB,0xE4		/* 287协处理器码 */ /* fsetpm for 287, ignored by 387 */
	ret

/*
 *  setup_idt
 *
 *  sets up a idt with 256 entries pointing to
 *  ignore_int, interrupt gates. It then loads
 *  idt. Everything that wants to install itself
 *  in the idt-table may do so themselves. Interrupts
 *  are enabled elsewhere, when we can be relatively
 *  sure everything is ok. This routine will be over-
 *  written by the page tables.
 */

/*
 * 设置256个中断描述符,并都指向ignore_int中断门.
 * 即每一个中断描述符,都有着相同的段选择子0x0008,相同的段内偏移地址ingore_int.
 */
setup_idt:
	lea ignore_int,%edx		# edx寄存器中,存的是ignore_int标号对应的地址
	movl $0x00080000,%eax
	movw %dx,%ax		# eax的高16位=0x0008(段选择子), 低16位=ignore_int.
						# 即eax此时是中断描述符的其中32位.
	movw $0x8E00,%dx	/* interrupt gate - dpl=0, present */
						# 即此时edx是中断描述符的另32位.
	lea _idt,%edi
	mov $256,%ecx
rp_sidt:
	movl %eax,(%edi)	# 将eax指向的值,复制到edi指向的内存地址
	movl %edx,4(%edi)	# 将edx指向的值,复制到edi+4指向的内存地址
	addl $8,%edi
	dec %ecx
	jne rp_sidt
	lidt idt_descr
	ret

/*
 *  setup_gdt
 *
 *  This routines sets up a new gdt and loads it.
 *  Only two entries are currently built, the same
 *  ones that were built in init.s. The routine
 *  is VERY complicated at two whole lines, so this
 *  rather long comment is certainly needed :-).
 *  This routine will beoverwritten by the page tables.
 */
setup_gdt:
	lgdt gdt_descr
	ret

/*
 * I put the kernel page tables right after the page directory,
 * using 4 of them to span 16 Mb of physical memory. People with
 * more than 16MB will have to expand this.
 */
.org 0x1000
pg0:

.org 0x2000
pg1:

.org 0x3000
pg2:

.org 0x4000
pg3:

.org 0x5000
/*
 * tmp_floppy_area is used by the floppy-driver when DMA cannot
 * reach to a buffer-block. It needs to be aligned, so that it isn't
 * on a 64kB border.
 */
_tmp_floppy_area:
	.fill 1024,1,0

after_page_tables:
	pushl $0		# These are the parameters to main :-)
	pushl $0
	pushl $0
	pushl $L6		# return address for main, if it decides to.
	pushl $_main
	jmp setup_paging
L6:
	jmp L6			# main should never return here, but
				# just in case, we know what happens.

/* This is the default interrupt "handler" :-) */
int_msg:
	.asciz "Unknown interrupt\n\r"
.p2align 2
ignore_int:
	pushl %eax
	pushl %ecx
	pushl %edx
	push %ds
	push %es
	push %fs
	movl $0x10,%eax
	mov %ax,%ds
	mov %ax,%es
	mov %ax,%fs
	pushl $int_msg
	call _printk
	popl %eax
	pop %fs
	pop %es
	pop %ds
	popl %edx
	popl %ecx
	popl %eax
	iret


/*
 * Setup_paging
 *
 * This routine sets up paging by setting the page bit
 * in cr0. The page tables are set up, identity-mapping
 * the first 16MB. The pager assumes that no illegal
 * addresses are produced (ie >4Mb on a 4Mb machine).
 *
 * NOTE! Although all physical memory should be identity
 * mapped by this routine, only the kernel page functions
 * use the >1Mb addresses directly. All "normal" functions
 * use just the lower 1Mb, or the local data space, which
 * will be mapped to some other place - mm keeps track of
 * that.
 *
 * For those with more memory than 16 Mb - tough luck. I've
 * not got it, why should you :-) The source is here. Change
 * it. (Seriously - it shouldn't be too difficult. Mostly
 * change some constants etc. I left it at 16Mb, as my machine
 * even cannot be extended past that (ok, but it was cheap :-)
 * I've tried to show which constants to change by having
 * some kind of marker at them (search for "16Mb"), but I
 * won't guarantee that's all :-( )
 */
.p2align 2
setup_paging:
	# 清空5个4K页(1个页目录表+4个页表页)
	movl $1024*5,%ecx		/* 5 pages - pg_dir+4 page tables */
	xorl %eax,%eax
	xorl %edi,%edi			/* pg_dir is at 0x000 */
	# 设置方向标志位std, stosl指令会递减edi
	# 清除方向标志位cld, stosl指令会递增edi
	# 将32个位,从%eax保存的值,复制到%edi指向的地址。并增加edi
	cld;rep;stosl

	# 设置页目录表的4个页表项
	movl $pg0+7,_pg_dir		/* set present bit/user r/w */
	movl $pg1+7,_pg_dir+4		/*  --------- " " --------- */
	movl $pg2+7,_pg_dir+8		/*  --------- " " --------- */
	movl $pg3+7,_pg_dir+12		/*  --------- " " --------- */

	# 填充4个页表页
	# 填在哪? - 用来填写最后一页的页基址的位置, 即pg3+1023*4地址处,即pg3+4092
	movl $pg3+4092,%edi
	# 填什么? - 最后一页的物理基址=16M-4K, 再+7是页表项的控制位
	movl $0xfff007,%eax		/*  16Mb - 4096 + 7 (r/w user,p) */
	std
l3:	stosl			/* fill pages backwards - more efficient :-) */
	subl $0x1000,%eax	# 上一页的物理基地址
	jge l3				# 减少到0,即填完了

	# 设置cr3, 令其保存根目录表的地址
	xorl %eax,%eax		/* pg_dir is at 0x0000 */
	movl %eax,%cr3		/* cr3 - page directory start */

	# 启用分页
	movl %cr0,%eax
	orl $0x80000000,%eax	# 置位PG,即分页
	movl %eax,%cr0		/* set paging (PG) bit */
	ret			/* this also flushes prefetch-queue */

.p2align 2
.word 0
idt_descr:
	.word 256*8-1		# idt contains 256 entries
	.long _idt
.p2align 2
.word 0
gdt_descr:
	.word 256*8-1		# so does gdt (not that that's any
	.long _gdt		# magic number, but it works for me :^)

.p2align 3
_idt:	.fill 256,8,0		# idt is uninitialized

_gdt:	.quad 0x0000000000000000	/* NULL descriptor */
	.quad 0x00c09a0000000fff	/* 16Mb */
	.quad 0x00c0920000000fff	/* 16Mb */
	.quad 0x0000000000000000	/* TEMPORARY - don't use */
	.fill 252,8,0			/* space for LDT's and TSS's etc */
