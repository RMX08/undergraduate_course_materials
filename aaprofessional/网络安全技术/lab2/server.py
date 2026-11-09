import socket
import os
from cryptography.hazmat.primitives.ciphers.aead import AESGCM

# 1. 预共享密钥 (必须与接收端一致)
SHARED_KEY = b"this_is_a_secret_key_32_char_!!!"

def start_sender(vm_ip):
    # 要发送的文件
    file_path = "test_file.txt"
    if not os.path.exists(file_path):
        with open(file_path, "w") as f: f.write("Hello, this is a secret file!")

    # 2. 读取文件并加密
    with open(file_path, "rb") as f:
        file_data = f.read()

    aesgcm = AESGCM(SHARED_KEY)
    nonce = os.urandom(12) # 生成随机的 12 字节 Nonce
    ciphertext = aesgcm.encrypt(nonce, file_data, None)

    # 3. 组合数据并发送 (Nonce + Ciphertext)
    payload = nonce + ciphertext

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        client_socket.connect((vm_ip, 9999))
        client_socket.sendall(payload)
        print("加密文件已成功发送！")
    finally:
        client_socket.close()

if __name__ == "__main__":
    # 请在此处替换为你虚拟机的实际 IP 地址
    VM_IP = "192.168.126.128"
    start_sender(VM_IP)