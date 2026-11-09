import serial
import time
from datetime import datetime

def main():
    # 串口配置
    port = 'COM13'
    baudrate = 9600
    timeout = 1

    # 初始化串口连接
    try:
        ser = serial.Serial(port, baudrate, timeout=timeout)
        if not ser.is_open:
            ser.open()
        print(f"成功打开串口 {port}")
    except Exception as e:
        print(f"打开串口失败: {e}")
        return

    # 导出文件设置
    filename = "test5.4.txt"
    with open(filename, 'w', encoding='utf-8') as f:
        pass  # 创建或清空文件
    try:
        for i in range(10):  # 循环10次采集数据
            # 发送数据获取命令
            command = "NUMeric:NORMal:VALue?\r\n"
            send_time = time.time()
            ser.write(command.encode())
            while time.time() - send_time < 2:  # 2秒发送超时
                pass

            # 读取返回数据
            receive_start_time = time.time()
            data = ''
            while time.time() - receive_start_time < 2:  # 2秒接收超时
                if ser.in_waiting > 0:
                    data = ser.readline().decode().strip()
                    break

            if data:
                # 解析数据
                parameters = data.split(',')
                if len(parameters) >= 6:
                    voltage = float(parameters[0])
                    current = float(parameters[1])
                    power = float(parameters[2])
                    frequency = float(parameters[3])
                    energy = float(parameters[4])
                    power_factor = float(parameters[5])

                    # 获取当前时间
                    current_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')

                    # 构造数据行
                    data_line = f"{current_time};{voltage};{current};{power};{frequency};{energy};{power_factor}\n"
                    # 将数据写入文件
                    with open(filename, 'a', encoding='utf-8') as f:
                        f.write(data_line)
                    print(f"已采集第 {i+1} 次数据: {data_line.strip()}")
                else:
                    print(f"第 {i+1} 次数据格式不正确")
            else:
                print(f"第 {i+1} 次未接收到数据")

            # 等待一段时间再进行下一次采集
            time.sleep(1)

    except KeyboardInterrupt:
        print("\n程序被手动终止")
    finally:
        ser.close()
        print("串口已关闭")

if __name__ == "__main__":
    main()