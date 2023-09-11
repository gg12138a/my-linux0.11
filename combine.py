def combine_files(output_file, bootsect_file, setup_file, system_file):
    with open(output_file, 'wb') as output:
        # 读取bootsect.bin的内容
        with open(bootsect_file, 'rb') as bootsect:
            output.write(bootsect.read())

        # 在512字节处写入setup.bin的内容
        with open(setup_file, 'rb') as setup:
            setup_data = setup.read()
            output.seek(512)  # 定位到512字节处
            output.write(setup_data)

        # 在1024字节处写入system.bin的内容
        with open(system_file, 'rb') as system:
            system_data = system.read()
            output.seek(1024)  # 定位到1024字节处
            output.write(system_data)


combine_files("disk.img", "boot/bootsect.bin", "boot/setup.bin", "system.bin")