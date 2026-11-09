import socket
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import x25519


def start_dh_client(server_ip):
    #  1. 生成客户端自己的密钥对
    client_private_key = x25519.X25519PrivateKey.generate()
    client_public_key = client_private_key.public_key()

    client_pub_bytes = client_public_key.public_bytes(
        encoding=serialization.Encoding.Raw,
        format=serialization.PublicFormat.Raw
    )

    # 2. 连接服务端
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        s.connect((server_ip, 65432))
        print(f"成功连接到服务端: {server_ip}")

        # --- 3. 交换公钥 ---
        # 先发送自己的公钥
        s.sendall(client_pub_bytes)

        # 接收服务端的公钥
        server_pub_bytes = s.recv(32)
        server_public_key = x25519.X25519PublicKey.from_public_bytes(server_pub_bytes)

        # --- 4. 计算共享密钥 ---
        # 核心步骤：使用“自己的私钥” + “对方的公钥”
        shared_key = client_private_key.exchange(server_public_key)

        print("\n[结果] 密钥协商成功！")
        print(f"协商出的共享密钥 (Hex): {shared_key.hex()}")

    except Exception as e:
        print(f"连接失败: {e}")
    finally:
        s.close()


if __name__ == "__main__":
    # 在这里输入你虚拟机的真实 IP 地址
    target_ip = '192.168.126.128'
    start_dh_client(target_ip)