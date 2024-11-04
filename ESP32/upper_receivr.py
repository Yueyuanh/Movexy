import serial
import struct
import time

# 定义结构体格式
struct_format = 'BBfffffff9f4B2B2B'
frame_head = 0xA5

# 初始化串口
ser = serial.Serial('COM9', 115200)  # 替换为你的串口号
time.sleep(2)


def receive_data():
    while True:
        if ser.read() == bytes([frame_head]):  # 等待帧头
            data_size = struct.calcsize(struct_format)
            data = ser.read(data_size - 1)  # 已经读取了帧头，还需要读取剩余的数据

            unpacked_data = struct.unpack(struct_format, bytes([frame_head]) + data)
            return unpacked_data


while True:
    data = receive_data()
    print(data)
    time.sleep(0.02)
