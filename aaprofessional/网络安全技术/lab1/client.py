import socket

target_ip = 'lab'
s = socket.socket()
s.connect((target_ip, 9999))
print("成功连接到虚拟机！输入 'exit' 退出连接。")

while True:
    cmd = input("请输入要执行的命令 > ")
    if not cmd: continue

    s.send(cmd.encode('utf-8'))

    if cmd.lower() == 'exit':
        break

    result = s.recv(10240).decode('utf-8')  # 接收结果（增加缓冲区大小）
    print(result)

s.close()