# 最终版
import socket
import struct
import time
from datetime import datetime

# 定义转换器的IP地址和端口号
SERVER_IP = "192.168.0.80"
SERVER_PORT = 10123

# 数据获取命令
GET_DATA_CMD = bytes.fromhex("01 03 00 00 00 01 84 0A")


def main():
    # 创建TCP socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        # 连接到转换器
        client_socket.connect((SERVER_IP, SERVER_PORT))
        print("已连接到转换器")

        # 设置Socket超时时间为2秒
        client_socket.settimeout(2.0)

        # 数据采集循环，最多进行20次采集
        for i in range(20):
            print(f"第 {i + 1} 次数据采集")

            # 发送数据获取命令
            send_start_time = time.time()
            try:
                client_socket.send(GET_DATA_CMD)
                send_end_time = time.time()
            except socket.timeout:
                print("发送超时，跳过本次采集")
                continue

            # 接收返回数据
            receive_start_time = time.time()
            try:
                response = client_socket.recv(1024)
                print(f"收到转换器返回的数据：{response.hex()}")
                receive_end_time = time.time()
            except socket.timeout:
                print("接收超时，跳过本次采集")
                continue

            # 解析返回数据
            if len(response) >= 7 and response[0] == 0x01 and response[1] == 0x03:
                # 检查CRC校验码（实际应用中应进行完整CRC校验）
                # 这里简化处理，仅提取光照度数据
                light_value = struct.unpack('!H', response[3:5])[0]
                print(f"解析得到的光照度值：{light_value}Lux")

                # 获取当前时间
                current_time = datetime.now().strftime("%Y-%m-%d,%H:%M:%S")

                # 将数据写入文件
                file_name = r".\test5.1.txt"
                with open(file_name, "a") as file:
                    file.write(f"{current_time};光照度：{light_value}Lux\n")
            else:
                print("接收到的数据格式不正确")

            # 延时1秒进行下一次采集
            time.sleep(1)

    except Exception as e:
        print(f"发生错误：{e}")
    except KeyboardInterrupt:
        print("\n手动停止数据采集")
    finally:
        # 关闭socket连接
        client_socket.close()
        print("已断开与转换器的连接")

if __name__ == "__main__":
    main()