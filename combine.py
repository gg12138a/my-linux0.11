def exit_fall():
    import sys
    sys.exit(-1)

def combine_files(output_file, bootsect_file, setup_file, system_file):
    with open(output_file, 'wb') as output:
        # 读取bootsect.bin的内容
        with open(bootsect_file, 'rb') as bootsect:
            bootsect_data = bootsect.read()
            if len(bootsect_data) != 512:
                print(bootsect_file, "has wrong size:", len(bootsect_data))
                exit_fall()
            output.write(bootsect_data)
            # print(len(bootsect_data))


        # 在512字节处写入setup.bin的内容
        with open(setup_file, 'rb') as setup:
            setup_data = setup.read()
            if len(setup_data) > 512:
                print(setup_file, "has wrong size:", len(setup_data))
                exit_fall()
            output.seek(512)  # 定位到512字节处
            output.write(setup_data)
            # print(len(setup_data))

        # 在1024字节处写入system.bin的内容
        with open(system_file, 'rb') as system:
            system_data = system.read()
            output.seek(1024)  # 定位到1024字节处
            output.write(system_data)
            # print(len(system_data))


output_img_name = "tmp.img"
combine_files(output_img_name, "boot/bootsect.bin", "boot/setup.bin", "system.bin")

import os
file_size = os.path.getsize(output_img_name)
print(file_size)
if file_size >=1440*1024 :
    print("total disk file", "has wrong size:", file_size)
    exit_fall()
