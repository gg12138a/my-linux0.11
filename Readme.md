# Get help

- generate `compile_commands.json`:
  ```shell
  bear -- make
  ```

# Q&A

- how to check the address of symbol `_main`is correct?

  > 反汇编得到system.bin.disasm，和head.o.disasm进行比较 
  
- 多个.o文件链接为一个.o文件，可以参见kernel/Makefile中的例子


# TODO

- 完善tty_init().
