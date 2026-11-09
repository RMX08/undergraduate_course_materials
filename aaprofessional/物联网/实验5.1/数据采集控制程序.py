import socket
import time

TCP_IP = '192.168.0.80'
TCP_PORT = 10123
request_command = bytes.fromhex("01 03 00 00 00 02 C4 0B")

def parse_sensor_data(response):
    if len(response) < 7:
        raise ValueError("返回数据长度异常")

    humidity_raw = response[3] << 8 | response[4]
    temperature_raw = response[5] << 8 | response[6]

    if temperature_raw > 0x7FFF:
        temperature_raw -= 0x10000

    temperature = temperature_raw / 10.0
    humidity = humidity_raw / 10.0

    return temperature, humidity

def main():
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.settimeout(5)
            s.connect((TCP_IP, TCP_PORT))
            print(f"连接成功: {TCP_IP}:{TCP_PORT}")

            count = 0
            max_count = 20

            while count < max_count:
                try:
                    s.sendall(request_command)
                    time.sleep(0.2)

                    response = s.recv(1024)
                    print(f"[第 {count + 1} 次] 接收到原始数据: {response.hex()}")

                    temperature, humidity = parse_sensor_data(response)
                    print(f"温度: {temperature:.1f}°C | 湿度: {humidity:.1f}%RH\n")

                except Exception as e:
                    print(f"[第 {count + 1} 次] 采集失败: {e}\n")

                count += 1
                time.sleep(2)

    except KeyboardInterrupt:
        print("已手动终止程序。")
    except Exception as e:
        print(f"连接失败或发生其他错误: {e}")

if __name__ == "__main__":
    main()
