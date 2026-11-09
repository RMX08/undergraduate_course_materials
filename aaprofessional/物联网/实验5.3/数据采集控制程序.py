# 实验5.3 控制模块交互编程 - 温度/光照解析完全修正版
import socket
import time

# ===================== 配置 =====================
DEVICE_IP = "192.168.0.1"
DEVICE_PORT = 1030
TEMP_FILE = "温度.txt"
LIGHT_FILE = "光照强度.txt"
# =================================================

# 控制指令
FAN_ON = bytes.fromhex("483A0170010100004544")
FAN_OFF = bytes.fromhex("483A0170010000004544")
LAMP_ON = bytes.fromhex("483A0170020100004544")
LAMP_OFF = bytes.fromhex("483A0170020000004544")

# ---------------------- 温度解析（完全正确） ----------------------
# 格式：01 03 04 02 92 00 00 64 FF
# 温度值 = 第6、7字节：00 00 → 0 → 0.0℃
def parse_temp(line):
    try:
        p = line.strip().split()
        # 第6个字段：p[5]，第7个字段：p[6]
        val = int(p[5] + p[6], 16)
        return val / 10.0  # 真实温度 = 十六进制值 ÷10
    except:
        return 0.0

# ---------------------- 光照解析 ----------------------
# 格式：01 03 02 03 E8 C2 B7
# 光照 = 第4、5字节
def parse_light(line):
    try:
        p = line.strip().split()
        return int(p[3] + p[4], 16)
    except:
        return 0

# ---------------------- 按行读取（循环滚动） ----------------------
temp_lines = []
light_lines = []
temp_idx = 0
light_idx = 0

def load_lines():
    global temp_lines, light_lines
    with open(TEMP_FILE, encoding='utf-8') as f:
        temp_lines = [l.strip() for l in f if l.strip()]
    with open(LIGHT_FILE, encoding='utf-8') as f:
        light_lines = [l.strip() for l in f if l.strip()]

def get_next_temp():
    global temp_idx
    line = temp_lines[temp_idx % len(temp_lines)]
    temp_idx += 1
    return parse_temp(line)

def get_next_light():
    global light_idx
    line = light_lines[light_idx % len(light_lines)]
    light_idx += 1
    return parse_light(line)

# ---------------------- TCP连接 ----------------------
def connect():
    try:
        c = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        c.settimeout(3)
        c.connect((DEVICE_IP, DEVICE_PORT))
        print("✅ TCP 连接成功")
        return c
    except Exception as e:
        print("❌ 连接失败：", e)
        return None

# ---------------------- 主逻辑 ----------------------
def main():
    load_lines()
    client = connect()
    if not client:
        return

    last_fan = None
    last_lamp = None

    print("\n=== 开始自动解析控制（逐行滚动）===\n")
    while True:
        temp = get_next_temp()
        light = get_next_light()

        # 风扇：>10℃ 开
        if temp > 10:
            if last_fan != "ON":
                client.send(FAN_ON)
                last_fan = "ON"
            fan_s = "开启"
        else:
            if last_fan != "OFF":
                client.send(FAN_OFF)
                last_fan = "OFF"
            fan_s = "关闭"

        # 灯：<5000开，>10000关
        if light < 5000:
            if last_lamp != "ON":
                client.send(LAMP_ON)
                last_lamp = "ON"
            lamp_s = "开启"
        elif light > 10000:
            if last_lamp != "OFF":
                client.send(LAMP_OFF)
                last_lamp = "OFF"
            lamp_s = "关闭"
        else:
            lamp_s = "保持"

        print(f"温度:{temp:5.1f}℃ | 风扇:{fan_s} | 光照:{light:6d}Lux | 灯:{lamp_s}")
        time.sleep(3)

if __name__ == "__main__":
    main()